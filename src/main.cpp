// SPDX-License-Identifier: GPL-2.0
/*
* Nuvoton SignIt Tool
*
* Copyright (C) 2018 Nuvoton Technologies, All Rights Reserved
*<<<------------------------------------------------------------------------
* File Contents:
*   main.c
*		This file includes the main flow of SignIt.
*               It checks all input parameters and then pass them to OpenSSL
*  Project:
*		SignIt
*---------------------------------------------------------------------------
*/

#include <iostream>
#include <algorithm>    // std::sort
#include <vector>
#define TCLAP_SETBASE_ZERO 1

#include <string>
#include <sstream>
#include <cstdio>
#include <tclap/CmdLine.h>
#include "ToolParams.h"
#include "tool_version.h"
#include "errors.h"
#include "utilities.h"
#include "SignIt_types.h"


#define TERMINATE_APP(STS)		{cout<<endl<<"FAILED"<<endl; exit(STS);}


typedef enum _EXIT_CODE 
{
	//ES=Exit Status
	ES_CLI_PARSING_ERROR	=	0x01,
	ES_STATUS_REPORT_ERROR	=	0x03,
	ES_FILE_GEN_ERR         =	0x04,
	ES_BUILDING_ERROR		=	0x05,
	ES_GENERATING_ERROR		=	0x06
} EXIT_CODE;

using namespace std;


int main(int argc, char *argv[])
{
	UINT32 status;
	ToolParams params;

	// build the command line
	try
	{
		stringstream ver;
		ver<<VER_MAJ(signItVersion)<<"."<<VER_MIN(signItVersion)<<"."<<VER_REV(signItVersion)<<endl; 

		TCLAP::CmdLine signIt_CLI("signIt (Binary signing tool)",' ',ver.str() );
	
		// add parameters
		TCLAP::UnlabeledValueArg<string>	inputFilename("input_file", "Input binary image to sign", true, "", "input file");
		TCLAP::ValueArg<string>		outFilename("o", "out", "A path to a binary output file. By default the output file is the signature, if ebmed_signature is selected, the output file is the input image, with the signature embedded inside of it. The default filename is 'signed.bin'", false,"signed.bin","output_filename");
		TCLAP::ValueArg<UINT32>		inputStartOffset("b", "begin_offset", "Offset (in bytes) within the input file from which the signed area starts. If omitted default is 0. If given in hex, prefix with 0x.", false, 0, "signed_begin_offset");
		TCLAP::ValueArg<UINT32>		inputSize("s", "signed_size", "Size of signed area (in bytes). If omitted, the signed area assumed to be from the start offset till the end of the file, if given in hex, prefix with 0x", false, 0, "signed_area_size");
		TCLAP::ValueArg<UINT32>		embedSignarure("e","ebmed_signature","If this flag is present the output file will be a copy of <input file> and the signature would be embedded inside it at the supplied <signature_location>",false,0xFFFFFFFF,"signature_location");
		
		// for the key type we need a closed list
		vector<string> allowedKeyFormats;
		allowedKeyFormats.push_back("pem");
		allowedKeyFormats.push_back("PEM");
		allowedKeyFormats.push_back("der");
		allowedKeyFormats.push_back("DER");
		allowedKeyFormats.push_back("bin");
		allowedKeyFormats.push_back("auto");
		TCLAP::ValuesConstraint<string> allowedVals( allowedKeyFormats );
		TCLAP::ValueArg<string>		keyFormat("f", "keyform", "The format in which the input key is supplied. If omitted, key format is identified by file extension. \"PEM\" and \"DER\" are standard unencrypted PKCS#1 RSA key formats, \"bin\" stands for unformatted binary file which contains 256 bytes of the key", false, "auto", &allowedVals);
		TCLAP::ValueArg<string>		privateKeyFilename("p", "private_key_file", "Input RSA private key to perform signature with", true, "", "private_key_file");
		TCLAP::ValueArg<string>		publicKeyFilename("m", "modulu_file", "Input RSA public key, (applicable and required only when the key is given in unformatted binary file)", false, "", "public_key_file");
		TCLAP::SwitchArg			reverseSignature("r","reverse","If this switch is on, the signature will be reversed (big endian to little endian)",false);

		signIt_CLI.add(outFilename);
		signIt_CLI.add(embedSignarure);
		signIt_CLI.add(reverseSignature);
		signIt_CLI.add(inputSize);
		signIt_CLI.add(inputStartOffset);
		signIt_CLI.add(keyFormat);
		signIt_CLI.add(publicKeyFilename);
		signIt_CLI.add(privateKeyFilename);
		signIt_CLI.add(inputFilename);

		signIt_CLI.parse( argc, argv );

		// get parameters to the tool object
		status = params.InputFilename(inputFilename.getValue());
		if (status)
		{
			TERMINATE_APP(ES_CLI_PARSING_ERROR);
		}


		status = params.PrivateKeyInputFile(privateKeyFilename.getValue());
		if (status)
		{
			TERMINATE_APP(ES_CLI_PARSING_ERROR);
		}
		status = params.ModuluKeyInputFile(publicKeyFilename.getValue());
		if (status)
		{
			TERMINATE_APP(ES_CLI_PARSING_ERROR);
		}

		status = params.KeyForm(keyFormat.getValue());
		if (status)
		{
			TERMINATE_APP(ES_CLI_PARSING_ERROR);
		}

		status = params.LoadPrivateKey();
		if (status)
		{
			TERMINATE_APP(ES_CLI_PARSING_ERROR);
		}

		status = params.LoadModuluKey();
		if (status)
		{
			TERMINATE_APP(ES_CLI_PARSING_ERROR);
		}

		
		status = params.StartOffset(inputStartOffset.getValue());
		if (status)
		{
			TERMINATE_APP(ES_CLI_PARSING_ERROR);
		}
		status = params.SignedAreaSize(inputSize.getValue());
		if (status)
		{
			TERMINATE_APP(ES_CLI_PARSING_ERROR);
		}
		status = params.SignatureOffset(embedSignarure.getValue());
		if (status)
		{
			TERMINATE_APP(ES_CLI_PARSING_ERROR);
		}

		params.setOutputFile(outFilename.getValue());
		
		
		//debug: dump fields
		//params.dumpFields(false);

		// check if openssl is present
		cout << "checking openssl version:" << endl;
		status = system("openssl version");
		if (status)
		{
			ERR_PrintError(ERR_OPENSSL, "could not initiate openSSL");
			TERMINATE_APP(ES_BUILDING_ERROR);
		}

		// load the file image
		status = params.loadInputFile();
		if (status)
		{
			TERMINATE_APP(ES_FILE_GEN_ERR);
		}

		// build a signed area file
		status = params.extractSignedAreaToFile(interimSignedAreaFilename);
		if (status)
		{
			TERMINATE_APP(ES_FILE_GEN_ERR);
		}

		// if the key is given in binary data, convert it to DER
		if (KeyForm_BIN == params.KeyForm_T())
		{
			params.convertBinToDER(interimDERKeyFilename);
			if (status)
			{
				// clean up and exit
				remove(interimSignedAreaFilename.c_str());
				TERMINATE_APP(ES_FILE_GEN_ERR);
			}
		}
		

		// build the command line for openssl
		stringstream openssl_cmd;
		string keyform = params.KeyForm();
		string keypath = params.PrivateKeyInputFile();
		openssl_cmd << "openssl dgst -sha256 -binary -keyform " << keyform << " -out " << interimSignatutreFilename << " -sign " << keypath << " " << interimSignedAreaFilename << endl;
		
		// sign using opnessl command line
		status = system(openssl_cmd.str().c_str());
		if (status)
		{
			ERR_PrintError(ERR_OPENSSL, "openssl encountered an error while generating signature");
			
			// clean up and exit

			remove(interimSignedAreaFilename.c_str());
			if (KeyForm_BIN == params.KeyForm_T())
			{
				remove(interimDERKeyFilename.c_str());
			}

			
			TERMINATE_APP(ES_FILE_GEN_ERR);
		}
		
		// if the signature should be reversed, reverse it
		if (reverseSignature.getValue())
		{
			reverseFile(interimSignatutreFilename);
		}

		// if the signature should be embedded, embed it in the already opened buffer
		if (params.Is_EmbedSignatureInFile())
		{
			status = params.embedSignatureOnImage(interimSignatutreFilename);
			if (status)
			{
				ERR_PrintError(ERR_FILE_ERROR, "could not embed signature on image");

				// clean up and exit
				remove(interimSignedAreaFilename.c_str());
				if (KeyForm_BIN == params.KeyForm_T())
				{
					remove(interimDERKeyFilename.c_str());
				}
				remove(interimSignatutreFilename.c_str());

				TERMINATE_APP(ES_FILE_GEN_ERR);
			}
			// dump the image into file
			string out_filename = params.NewFilename();
			if (out_filename == "")
			{
				out_filename = params.InputFilename();
			}

			status = params.dumpBufferToFile(out_filename);
			if (status)
			{
				ERR_PrintError(ERR_FILE_ERROR, "could not embed signature on image");

				// clean up and exit
				remove(interimSignedAreaFilename.c_str());
				if (KeyForm_BIN == params.KeyForm_T())
				{
					remove(interimDERKeyFilename.c_str());
				}
				remove(interimSignatutreFilename.c_str());

				TERMINATE_APP(ES_FILE_GEN_ERR);
			}

		}
		
		// clean up and closing
		// if the user want a copy of the signature, provide it otherwise, remove the interim file
		if (params.SignatureFilename() != "")
		{
			rename(interimSignatutreFilename.c_str(), params.SignatureFilename().c_str());
		}
		else
		{
			remove(interimSignatutreFilename.c_str());
		}

		remove(interimDERKeyFilename.c_str());
		remove(interimSignedAreaFilename.c_str());
		cout<<endl<<"SUCCESS"<<endl;
	}
	catch (TCLAP::ArgException &e)  // catch any exceptions
	{ std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl; }

	return STS_OK;
}

