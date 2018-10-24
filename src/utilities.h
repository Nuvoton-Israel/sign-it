// SPDX-License-Identifier: GPL-2.0
/*
* Nuvoton SignIt Tool
*
* Copyright (C) 2018 Nuvoton Technologies, All Rights Reserved
*<<<------------------------------------------------------------------------
* File Contents:
*   utilities.h
*		This file includes general macros and functions for file handling
*  Project:
*		SignIt
*---------------------------------------------------------------------------
*/

#ifndef UTILITIES_H
#define UTILITIES_H


#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include "SignIt_types.h"

extern UINT32 verbosLevel;

#define SIZE_OF_RSA_KEY					0x100
#define SIGNATURE_SIZE_BYTES			0x100

#define ALIGN(x, alg) (((x) + (alg) - 1) / (alg) * (alg))
typedef enum KEY_FORMAT_T
{
	KeyForm_EMPTY = 0,
	KeyForm_DER,
	KeyForm_PEM,
	KeyForm_BIN
}KEY_FORMAT_T;

const std::string interimSignedAreaFilename =	"_tempSigned.bin";
const std::string interimSignatutreFilename =	"_tempSignature.bin";
const std::string interimDERKeyFilename		=	"_tempKey.der";


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
UINT32 getFileSize(const char* filename, UINT32 &size);
std::string getFileExtension(std::string filename);

bool fileExist (const std::string& name);
UINT32 convertKeyToDER( const UINT8 *rawPublicKey, const UINT8 *rawPrivateKey, UINT8 * &binaryDERKey, UINT32 &size );
UINT32 reverseFile(const std::string filename);
#endif // UTILITIES_H
