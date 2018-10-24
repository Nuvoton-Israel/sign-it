#/*------------------------------------------------------------------------*/
#/*  Nuvoton Technology Corporation Confidential                           */
#/*                                                                        */
#/*  Copyright (c) 2015-2018 by Nuvoton Technology Corporation             */
#/*  All rights reserved                                                   */
#/*                                                                        */
#/*<<<---------------------------------------------------------------------*/
#/* File Contents:                                                         */
#/*   makefile                                                             */
#/*            This file contains SignIt tool                              */
#/* Project:                                                               */
#/*            Poleg BMC                                                   */
#/*------------------------------------------------------------------------*/

#----------------------------------------------------------------------------
# tools used in makefile
#----------------------------------------------------------------------------


#----------------------------------------------------------------------------
# Suffixes
#----------------------------------------------------------------------------

.SUFFIXES:
.SUFFIXES: .h .c .cpp .o

#----------------------------------------------------------------------------
# Directories
#----------------------------------------------------------------------------

SRC_DIR		= ../src
DELIV_LOC       = ../deliverables/linux/
OUTPUT_DIR      = $(DELIV_LOC)/release
OUT_NAME        = signit
#----------------------------------------------------------------------------
# Files
#----------------------------------------------------------------------------

signit_SRC    =                            \
		 $(SRC_DIR)/errors.cpp     \
		 $(SRC_DIR)/main.cpp       \
		 $(SRC_DIR)/ToolParams.cpp \
		 $(SRC_DIR)/utilities.cpp


#----------------------------------------------------------------------------
# Object files of the project
#----------------------------------------------------------------------------


#----------------------------------------------------------------------------
# C preprocessor flags
#----------------------------------------------------------------------------

#----------------------------------------------------------------------------
# C compilation flags
#----------------------------------------------------------------------------

CC		= g++
MAKEDIR		= mkdir -p
INCLUDE 	= -I $(SRC_DIR) -I ../include
TARGET  	= signit
CFLAGS  	= -std=c++0x -D__LINUX_APP__

# Google-specific compilation
#CFLAGS  	= -O3 -g -Wall -Werror -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack


signit:
	@echo Creating \"$(TARGET)\" in directory \"$(OUTPUT_DIR)\" ...
	@$(MAKEDIR)	$(OUTPUT_DIR)
	@echo $(CC) $(CFLAGS) $(INCLUDE) $(signit_SRC) -o $(OUTPUT_DIR)/signit
	@$(CC) $(CFLAGS) $(INCLUDE) $(signit_SRC) -o $(OUTPUT_DIR)/signit
	
all:
	@echo Creating \"$(TARGET)\" in directory \"$(OUTPUT_DIR)\" ...
	@$(MAKEDIR)	$(OUTPUT_DIR)
	@echo $(CC) $(CFLAGS) $(INCLUDE) $(signit_SRC) -o $(OUTPUT_DIR)/signit
	@$(CC) $(CFLAGS) $(INCLUDE) $(signit_SRC) -o $(OUTPUT_DIR)/signit


#----------------------------------------------------------------------------
# Clean
#----------------------------------------------------------------------------
RMDIR		= rm
RMFLAGS		= -rf

clean:
	@echo Removing Directory \"$(OUTPUT_DIR)\" ...
	@$(RMDIR) $(RMFLAGS) $(OUTPUT_DIR)
	


