// SPDX-License-Identifier: GPL-2.0
/*
* Nuvoton SignIt Tool
*
* Copyright (C) 2018 Nuvoton Technologies, All Rights Reserved
*<<<------------------------------------------------------------------------
* File Contents:
*   ToolsParams.cpp
*		The ToolParams module contains all the input parameters needed for SignIt.
*  Project:
*		SignIt
*---------------------------------------------------------------------------
*/

#include <fstream>
#include <iostream> 
#include "ToolParams.h"
#include "errors.h"

using namespace std;
ToolParams::ToolParams(void)
{
	inputFileBuff = nullptr;
	modulusKeyBuff = nullptr;
	privateKeyBuff = nullptr;
	_embedSignatureInFile = false;
}


ToolParams::~ToolParams(void)
{
	delete[] inputFileBuff;
	delete[] modulusKeyBuff;
	delete[] privateKeyBuff;
}

std::string ToolParams::KeyForm()
{
	if (_keyForm == KeyForm_DER)
	{
		return "der";
	} 
	else if (_keyForm == KeyForm_PEM)
	{
		return "pem";
	}
	else if (_keyForm == KeyForm_BIN)
	{
		return "bin";
	}
	
	return "none";
}

UINT32 ToolParams::KeyForm( std::string val )
{
	if (val == "bin" || val == "BIN")
	{
		KeyForm(KeyForm_BIN);
		return STS_OK;
	}
	else if (val == "pem" || val == "PEM")
	{
		KeyForm(KeyForm_PEM);
		return STS_OK;
	}
	else if (val == "der" || val == "DER")
	{
		KeyForm(KeyForm_DER);
		return STS_OK;
	}
	else if (val == "auto")
	{
		// parse key type according to key extension
		return KeyForm(getFileExtension(_privateKeyInputFile));
	}
	
	// value is empty, and thus invalid.
	KeyForm(KeyForm_EMPTY);
	ERR_PrintError(ERR_ILLEGAL_VAL, "bad value for key format: "+val);
	return ERR_ILLEGAL_VAL;
}

UINT32 ToolParams::loadInputFile()
{
	ifstream infile;
	
	// check that the signed area size is not larger than the file itself
	if (_startOffset + _signedAreaSize > _filesize)
	{
		string errStr = "offset or signed area size exceeds input file size\nfilename: " + _inputFilename;
		ERR_PrintError(ERR_FILE_ERROR, errStr);
		return ERR_FILE_ERROR;
	} 
	
	
	infile.open(_inputFilename.c_str(), ios::binary);
	if (!infile.is_open())
	{
		string errStr = "Filename: " + _inputFilename;
		ERR_PrintError(ERR_FILE_NOT_FOUND, errStr);
		return ERR_FILE_NOT_FOUND;
	}

	// allocate a buffer
	inputFileBuff = new UINT8[_filesize];

	// fill buffer with file content
	for (UINT32 i = 0; i <  _filesize; i++)
	{
		inputFileBuff[i] = (UINT8) infile.get();
		if (!infile.good())
		{
			string errString = "error reading from file: " + _inputFilename;
			ERR_PrintError(ERR_FILE_ERROR, errString);
			return ERR_FILE_ERROR;
		}
	}

	// close input file
	infile.close();
	return STS_OK;
}

void ToolParams::dumpFields( bool fileAlso )
{
	cout << "tool dumpFields:" << endl;
	cout << "_inputFilename		"   <<  _inputFilename		  <<  endl;
	cout << "_newFilename		"   <<  _newFilename		   <<  endl;
	cout << "_signatureFilename	"   <<  _signatureFilename	   <<  endl;
	cout << "_privateKeyInputFile	"   <<  _privateKeyInputFile	   <<  endl;
	cout << "_keyForm			"   <<  _keyForm			   <<  endl;
	cout << "_startOffset		"   <<  _startOffset		   <<  endl;
	cout << "_signedAreaSize	"   <<  _signedAreaSize	   <<  endl;
	cout << "_signatureOffset	"   <<  _signatureOffset	   <<  endl;
	cout << "_filesize	"   <<  _filesize	   <<  endl;
	cout << "_embedSignatureInFile	"   <<  _embedSignatureInFile	   <<  endl;


	if (fileAlso)
	{
		if (this->inputFileBuff != nullptr)
		{
			for (UINT32 i = 0; i < this->_filesize; ++i)
			{
				if (i %16 == 0)
				{
					cout << endl;
				}
				printf("0x%02X ",inputFileBuff[i]);

			}
			cout << endl;
		}
	}
	
}

UINT32 ToolParams::InputFilename( std::string val )
{
	
	_inputFilename = val;
	
	// get file size (in order to allocate buffer)
	if (getFileSize(_inputFilename.c_str(), _filesize))
	{
		return ERR_FILE_ERROR;
	}

	return STS_OK;
}

UINT32 ToolParams::PrivateKeyInputFile( std::string val )
{
	
	if (!fileExist(val))
	{
		ERR_PrintError(ERR_FILE_NOT_FOUND, val);
		return ERR_FILE_NOT_FOUND;
	}

	
	_privateKeyInputFile = val;
	return STS_OK;
}



UINT32 ToolParams::LoadPrivateKey()
{
	// if the key is raw value, load it to a buffer (in order to produce DER file later)
	if (_keyForm == KeyForm_BIN)
	{
		UINT32 keyFileSize;
		getFileSize(_privateKeyInputFile.c_str(), keyFileSize);
		if (keyFileSize != SIZE_OF_RSA_KEY)
		{
			stringstream errStr;
			errStr << _privateKeyInputFile << " is not exactly " << SIZE_OF_RSA_KEY << " bytes as it should";
			ERR_PrintError(ERR_FILE_ERROR, errStr.str());
			return ERR_FILE_ERROR;
		}

		// allocate the buffer
		privateKeyBuff = new UINT8[SIZE_OF_RSA_KEY];

		// get data from file
		ifstream infile(_privateKeyInputFile, ios_base::binary);
		infile.read((char *)privateKeyBuff, SIZE_OF_RSA_KEY);
		if (!infile.good())
		{
			string errString = "error reading from file: " + _inputFilename;
			ERR_PrintError(ERR_FILE_ERROR, errString);
			return ERR_FILE_ERROR;
		}
		infile.close();

	}

	return STS_OK;
}

UINT32 ToolParams::ModuluKeyInputFile( std::string val )
{
	_moduluKeyInputFile = val;
	return STS_OK;
}


UINT32 ToolParams::LoadModuluKey() 
{
	if (_keyForm == KeyForm_BIN)
	{
		if (_moduluKeyInputFile == "")
		{
			ERR_PrintError(ERR_PARSING, "when using unformatted binary key use the m switch to add public key bin file");
		}
		if (!fileExist(_moduluKeyInputFile))
		{

			ERR_PrintError(ERR_FILE_NOT_FOUND, _moduluKeyInputFile);
			return ERR_FILE_NOT_FOUND;
		}

		UINT32 keyFileSize;
		getFileSize(_moduluKeyInputFile.c_str(), keyFileSize);
		if (keyFileSize != SIZE_OF_RSA_KEY)
		{
			stringstream errStr;
			errStr << _moduluKeyInputFile << "is not exactly " << SIZE_OF_RSA_KEY << " bytes as it should";
			ERR_PrintError(ERR_FILE_ERROR, errStr.str());
			return ERR_FILE_ERROR;
		}

		// allocate the buffer
		modulusKeyBuff = new UINT8[SIZE_OF_RSA_KEY];

		// get data from file
		ifstream infile(_moduluKeyInputFile, ios_base::binary);
		infile.read((char *)modulusKeyBuff, SIZE_OF_RSA_KEY);
		if (!infile.good())
		{
			string errString = "error reading from file: " + _inputFilename;
			ERR_PrintError(ERR_FILE_ERROR, errString);
			return ERR_FILE_ERROR;
		}
		infile.close();

	}
	else if(_moduluKeyInputFile != "")
	{
		cout << "NOTE! using PEM/DER format, modulus key input file "<< _moduluKeyInputFile <<" will be ignored"<<endl;
	}

	return STS_OK;
}

UINT32 ToolParams::StartOffset( UINT32 val )
{
	if (val > _filesize)
	{
		ERR_PrintError(ERR_ILLEGAL_VAL, "given signed area start offset is larger than the input file\n");
		return ERR_ILLEGAL_VAL;
	}
	_startOffset = val;
	return STS_OK;
}

UINT32 ToolParams::SignedAreaSize( UINT32 val )
{
	if (val + _startOffset > _filesize)
	{
		ERR_PrintError(ERR_ILLEGAL_VAL, "given signed area is larger than the input file\n");
		return ERR_ILLEGAL_VAL;
	}

	if (val == 0) // in this case, cacluate the signed area size according to input parameters
	{
		_signedAreaSize = _filesize - _startOffset;
	}
	else
	{
		_signedAreaSize = val;
	}
	
	return STS_OK;
}

UINT32 ToolParams::SignatureOffset( UINT32 val )
{
	if (val != 0xFFFFFFFF)
	{
		Is_EmbedSignatureInFile(true);
	}

	if (_embedSignatureInFile)
	{
		if (val > _filesize)
		{
			ERR_PrintError(ERR_ILLEGAL_VAL, "given signature location is larger than input file size\n");
			return ERR_ILLEGAL_VAL;
		}
		if (val + SIGNATURE_SIZE_BYTES > _filesize)
		{
			ERR_PrintError(ERR_ILLEGAL_VAL, "given signature location overruns end of input file\n");
			return ERR_ILLEGAL_VAL;
		}
		else if ((val <= _startOffset) && ((val + SIGNATURE_SIZE_BYTES) > (_startOffset)))
		{
			ERR_PrintError(ERR_ILLEGAL_VAL, "given signature location overlaps with signed area\n");
			return ERR_ILLEGAL_VAL;
		}
		else if ((val >= _startOffset) && ((val) < (_startOffset + _signedAreaSize)))
		{
			ERR_PrintError(ERR_ILLEGAL_VAL, "given signature location is inside a signed area\n");
			return ERR_ILLEGAL_VAL;
		}
	}
	_signatureOffset = val;
	return STS_OK;
}

void ToolParams::NewFilename( std::string val )
{
	this->_newFilename = val;
	if (val != "")
	{
		_embedSignatureInFile = true;
	}
}

// this method assumes:
// _startOffset + _signedAreaSize < _filesize
// inputFileBuff is allocated and valid
UINT32 ToolParams::extractSignedAreaToFile( const std::string filename )
{
	UINT32 status;
	//open file for writing
	ofstream outFile (filename.c_str(), ofstream::binary); 
	if (!outFile.is_open())
	{
		status = ERR_FILE_ERROR;
		string errStr = "Error creating or opening file " + filename;
		ERR_PrintError(status, errStr);
		return status;
	}

	// dump the extracted area
	outFile.write((char *)&inputFileBuff[_startOffset], _signedAreaSize);
	if (!outFile.good())
	{
		status = ERR_FILE_ERROR;
		string errStr = "Error writing to file " + filename;
		ERR_PrintError(status, errStr);
	}

	outFile.close();
	return STS_OK;

}

UINT32 ToolParams::convertBinToDER( const std::string der_filename )
{
	UINT32 status;
	UINT8 *tempBuff;
	UINT32 size;

	convertKeyToDER(modulusKeyBuff, privateKeyBuff, tempBuff, size);

	ofstream outFile (der_filename.c_str(), ofstream::binary);
	if (!outFile.is_open())
	{
		status = ERR_FILE_ERROR;
		string errStr = "Error creating or opening file " + der_filename;
		ERR_PrintError(status, errStr);
		return status;
	}

	outFile.write((char *)tempBuff, size);
	if (!outFile.good())
	{
		status = ERR_FILE_ERROR;
		string errStr = "Error writing to file " + der_filename;
		ERR_PrintError(status, errStr);
		return status;
	}
	outFile.close();
	delete[] tempBuff;

	// now that the key file is complete, change the parameters object to be DER
	KeyForm(KeyForm_DER);
	PrivateKeyInputFile(der_filename);

	return STS_OK;
}

// assumptions: this operation will not override signed area of the image
UINT32 ToolParams::embedSignatureOnImage( const std::string signatureFilename )
{
	ifstream infile;

	infile.open(signatureFilename.c_str(), ios::binary);
	if (!infile.is_open())
	{
		string errStr = "Filename: " + signatureFilename;
		ERR_PrintError(ERR_FILE_NOT_FOUND, errStr);
		return ERR_FILE_NOT_FOUND;
	}

	infile.read((char *)&inputFileBuff[_signatureOffset], SIGNATURE_SIZE_BYTES);
	if (!infile.good())
	{
		string errString = "error reading from file: " + signatureFilename;
		ERR_PrintError(ERR_FILE_ERROR, errString);
		return ERR_FILE_ERROR;
	}
	
	return STS_OK;
}

UINT32 ToolParams::dumpBufferToFile( const std::string filename )
{
	UINT32 status;
	ofstream outFile (filename.c_str(), ofstream::binary);
	if (!outFile.is_open())
	{
		status = ERR_FILE_ERROR;
		string errStr = "Error creating or opening file " + filename;
		ERR_PrintError(status, errStr);
		return status;
	}

	outFile.write((char *)inputFileBuff, _filesize);
	if (!outFile.good())
	{
		status = ERR_FILE_ERROR;
		string errStr = "Error writing to file " + filename;
		ERR_PrintError(status, errStr);
		return status;
	}
	outFile.close();

	return STS_OK;

}

UINT32 ToolParams::setOutputFile( const std::string filename )
{
	if (Is_EmbedSignatureInFile())
	{
		NewFilename(filename);
		SignatureFilename("");
	}
	else
	{
		NewFilename("");
		SignatureFilename(filename);
	}

	return STS_OK;
}
