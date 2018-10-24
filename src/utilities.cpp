// SPDX-License-Identifier: GPL-2.0
/*
* Nuvoton SignIt Tool
*
* Copyright (C) 2018 Nuvoton Technologies, All Rights Reserved
*<<<------------------------------------------------------------------------
* File Contents:
*   utilities.cpp
*		This file includes general macros and function implementation for file handling
*  Project:
*		SignIt
*---------------------------------------------------------------------------
*/

#include <iostream>
#include <cstring>
#include "utilities.h"
#include "errors.h"
#include "DER_template.h"

UINT32 verbosLevel = 0;

/*
	Utilities
*/

// String split to vector routines:
using namespace std;

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (std::getline(ss, item, delim)) {
		elems.push_back(item);
	}
	return elems;
}


std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

UINT32 getFileSize(const char* filename, UINT32 &size)
{
	UINT32 err = 0;
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	if (in.is_open() == false)
	{
		err = ERR_FILE_NOT_FOUND;
		ERR_PrintError(err, filename);
		size = 0;
		return err;
	}
	size = (UINT32) in.tellg(); 
	in.close();
	return STS_OK;
}

bool fileExist (const std::string& name) {
	ifstream f(name.c_str());
	if (f.good()) {
		f.close();
		return true;
	} else {
		f.close();
		return false;
	}   
}

UINT32 convertKeyToDER(const UINT8 *rawPublicKey, const UINT8 *rawPrivateKey, UINT8 * &binaryDERKey, UINT32 &size )
{
	binaryDERKey = new UINT8[DER_TAMPLATE_SIZE];

	memcpy(binaryDERKey, DER_template_array, DER_TAMPLATE_SIZE);
	memcpy(&binaryDERKey[DER_TAMPLATE_MODULUS_OFFSET], rawPublicKey, SIZE_OF_RSA_KEY);
	memcpy(&binaryDERKey[DER_TAMPLATE_PRIVATE_EXP_OFFSET], rawPrivateKey, SIZE_OF_RSA_KEY);

	size = DER_TAMPLATE_SIZE;

	return STS_OK;
}


std::string getFileExtension( std::string filename )
{
	UINT16 extension_location = filename.find_last_of('.');

	// handle no extension
	if (extension_location == string::npos)
	{
		return "";
	}
	
	// return the extension
	return filename.substr(extension_location+1);
}

UINT32 reverseFile(const std::string filename)
{
	UINT8 tempBuf[SIGNATURE_SIZE_BYTES];
	ifstream infile;
	ofstream  outfile;

	infile.open(filename.c_str(), ios::binary);
	if (!infile.is_open())
	{
		string errStr = "Filename: " + filename;
		ERR_PrintError(ERR_FILE_NOT_FOUND, errStr);
		return ERR_FILE_NOT_FOUND;
	}

	for (UINT32 i = 0; i < SIGNATURE_SIZE_BYTES; ++i)
	{
		tempBuf[SIGNATURE_SIZE_BYTES - i - 1] = (UINT8) infile.get();
		if (!infile.good())
		{
			string errString = "error reading from file: " + filename;
			ERR_PrintError(ERR_FILE_ERROR, errString);
			return ERR_FILE_ERROR;
		}
	}

	infile.close();

	outfile.open(filename.c_str(), ios::binary);
	outfile.write((char *)tempBuf, SIGNATURE_SIZE_BYTES);
	if (!outfile.good())
	{

		string errStr = "Error writing to file " + filename;
		ERR_PrintError(ERR_FILE_ERROR, errStr);
		return ERR_FILE_ERROR;
	}
	outfile.close();

	return STS_OK;
}
