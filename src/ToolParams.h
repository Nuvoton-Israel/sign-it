// SPDX-License-Identifier: GPL-2.0
/*
* Nuvoton SignIt Tool
*
* Copyright (C) 2018 Nuvoton Technologies, All Rights Reserved
*<<<------------------------------------------------------------------------
* File Contents:
*   ToolsParams.h
*		The ToolParams module contains all the input parameters needed for SignIt.
*  Project:
*		SignIt
*---------------------------------------------------------------------------
*/

#ifndef TOOLPARAMS_H_
#define TOOLPARAMS_H_

#include <string>
#include "utilities.h"
#include "SignIt_types.h"

class ToolParams
{
public:
	ToolParams(void);
	~ToolParams(void);

	std::string InputFilename() const { return _inputFilename; }
	UINT32 InputFilename(std::string val);
	
	std::string NewFilename() const { return _newFilename; }
	void NewFilename(std::string val);

	std::string PrivateKeyInputFile() const { return _privateKeyInputFile; }
	UINT32 PrivateKeyInputFile(std::string val);

	UINT32 LoadPrivateKey() ;

	std::string ModuluKeyInputFile() const { return _moduluKeyInputFile; }
	UINT32 ModuluKeyInputFile(std::string val);

	UINT32 LoadModuluKey();


	KEY_FORMAT_T KeyForm_T() const { return _keyForm; }
	std::string	 KeyForm(); // get string value of the key implementation in cpp
	void KeyForm(KEY_FORMAT_T val) { _keyForm = val; }
	UINT32 KeyForm(std::string val); 

	std::string SignatureFilename() const { return _signatureFilename; }
	void SignatureFilename(std::string val) { _signatureFilename = val; }

	UINT32 StartOffset() const { return _startOffset; }
	UINT32 StartOffset(UINT32 val);

	
	UINT32 SignedAreaSize() const { return _signedAreaSize; }
	UINT32 SignedAreaSize(UINT32 val);

	UINT32 SignatureOffset() const { return _signatureOffset; }
	UINT32 SignatureOffset(UINT32 val);

	bool Is_EmbedSignatureInFile() const { return _embedSignatureInFile; }
	void Is_EmbedSignatureInFile(bool val) { _embedSignatureInFile = val; }

	UINT32 loadInputFile(); // loads the input file to inputFileBuff
	void dumpFields(bool fileAlso=false);	// dumps the class memebers
	UINT32 extractSignedAreaToFile(const std::string filename); // implementation in cpp
	// this method will construct a DER file, and will overwrite the state of _keyForm and _privateKeyInputFile
	UINT32 convertBinToDER(const std::string der_filename);
	UINT32 embedSignatureOnImage(const std::string signatureFilename);
	
	UINT32 dumpBufferToFile(const std::string filename);
	UINT32 setOutputFile(const std::string filename);
	UINT8* inputFileBuff;
	UINT8*	modulusKeyBuff;
	UINT8*  privateKeyBuff;

private:
	std::string			_inputFilename;		// has to be specified
	std::string			_newFilename;		// optional, if not specified, no file will be generated
	std::string			_signatureFilename;
	std::string			_privateKeyInputFile;
	std::string			_moduluKeyInputFile;

	KEY_FORMAT_T	_keyForm;			
	
	UINT32			_startOffset;
	UINT32			_signedAreaSize;
	UINT32			_signatureOffset;	

	UINT32			_filesize;
	bool			_embedSignatureInFile;

};

#endif // TOOLPARAMS_H_

