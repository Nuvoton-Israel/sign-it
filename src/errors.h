// SPDX-License-Identifier: GPL-2.0
/*
* Nuvoton SignIt Tool
*
* Copyright (C) 2018 Nuvoton Technologies, All Rights Reserved
*<<<------------------------------------------------------------------------
* File Contents:
*   errors.h
*		Error numbers and handling for all errors possible on SignIt.
*  Project:
*		SignIt
*---------------------------------------------------------------------------
*/

#ifndef ERRORS_H
#define ERRORS_H

#include <string>
#include "SignIt_types.h"




//Errors:
typedef enum _STATUS
{
	ERR_OPEN_FILE           = 0x01, // Could not open input file 
	ERR_PARSING             = 0x02, // Parsing error
	ERR_ILLEGAL_VAL         = 0x03, // Illegal Value
	ERR_OPENSSL				= 0x04, // openSSL error
	ERR_OUTPUT_FILE         = 0x05, // Could not create output file
	ERR_NOT_IMPLEMENTED     = 0x06, // Command not implemented
	ERR_BAD_IMAGE_SIZE		= 0x07, // Binary image size does not match calculated size by fields
	ERR_BAD_FIELD_SIZE      = 0x08, // Binary field image size does not match actual data size
	ERR_SAME_FIELD_TWICE	= 0x09, // Field encountered twice
	ERR_ILLEGAL_FIELD       = 0x0a, // Unknown XML Field
	ERR_UNKNOWN_ATTR		= 0x0b, // Unknown Field Attribute
	ERR_FILE_NOT_FOUND		= 0x0c,	// File not found
	ERR_FILE_ERROR			= 0x0d, // File handling error
	ERR_FIELD_OVERLAP		= 0x0e, // Fields overlap detected
	ERR_IMAGE_TOO_LARGE		= 0x0f, // Image exceeded limit size
	ERR_ECC_ERROR			= 0x10, // ECC error
	ERR_CMD_LINE_ERR		= 0x11, // Command line error
} STUS;

//Statuses:
#define STS_OK			        0x00


//Functions:
UINT32  ERR_FillErrorDetails(UINT32 err, UINT32 lineNum, std::string &line);
UINT32  ERR_PrintError(UINT32 err, const std::string msg);

#endif //ERRORS_H
