// SPDX-License-Identifier: GPL-2.0
/*
* Nuvoton SignIt Tool
*
* Copyright (C) 2018 Nuvoton Technologies, All Rights Reserved
*<<<------------------------------------------------------------------------
* File Contents:
*   tool_version.h
*		Version number for SignIt tool
*  Project:
*		SignIt
*---------------------------------------------------------------------------
*/

#ifndef TOOL_VERSION_H
#define TOOL_VERSION_H


/*
signItVersion = N7N6N5N4N3N2N1N0

Where:

·         N5N4 – signIt code Major Version.

·         N3N2 – signIt code Minor Version.

·         N1N0 – signIt code Revision.
*/

#define signIt_STIRNG		"signIt - Binary Singature Generator"
#define signItVersion			0x00000003  //ver 0.0.3

#define VER_MAJ(X)			((X&0x00ff0000)>>16)
#define VER_MIN(X)			((X&0x0000ff00)>>8)
#define VER_REV(X)			((X&0x000000ff)>>0)

#endif //TOOL_VERSION_H
