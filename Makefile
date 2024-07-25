################################################################################
# File: 		Makefile
# Author: 		Furkan Yildirim
# Brief:
# Create Date: 	2023-07-19
# Update Date: 	2023-12-24
# Version: 		0.2
#
# Copyright (c) 2023
################################################################################

# Projenin adı
EXE_NAME = random_num_gen

MAKEFLAGS += -j12
MAKEFLAGS += --output-sync

######################################
# Application Source Directories
######################################
SOURCE_DIRS = $(SRC_DIR)
SOURCE_DIRS += $(SRC_DIR)/lib/time
SOURCE_DIRS += $(SRC_DIR)/lib/util

CC = gcc
CFLAGS 	:=	-Wall \
    		-Wextra \
    		-std=c99 \
    		-fdiagnostics-color

LDFLAGS = -lgdi32 -lwinmm

# obje dosyalarinin olusturulmak istendigi dizin
BUILD_DIR = build/objs
# exe dosyasinin olusturulmak istendigi dizin
OUT_DIR = exefile
# Projenin olusturuldugu dizin
SRC_DIR = source

######################################

# Source files
SOURCE = $(foreach dir, $(SOURCE_DIRS), $(wildcard $(dir)/*.c))
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCE))

SEPARATOR = ====================================================================

UNAME_CMD := uname
OS_DETECTED := $(shell $(UNAME_CMD))

ifeq ($(OS_DETECTED),)
    CC_SEARCH := which $(CC)
    mkdir = mkdir_windows
    clean_target = clean_windows
    env_predetection_msg = Checked environment for '$(UNAME_CMD)', not found
    env_detected_msg = Assuming Windows environment
else
    CC_SEARCH := which $(CC)
    mkdir = mkdir_linux
    clean_target = clean_linux
    env_predetection_msg = Checked environment, found: $(OS_DETECTED)
    env_detected_msg = Assuming Unix like environment
endif

## Testing to check if the toolchain is correct
CC_DETECTED := $(shell $(CC_SEARCH))

mkdir_message := Created directory for object files

# Phony
.PHONY: all clean compiler size

##  C Compiler path
compiler:
ifeq ($(CC_DETECTED),)
    $(error The compiler cannot be found in $(CC) path. Make sure Toolchain is installed)
else
	@echo compiler: $(CC_DETECTED)
endif

# Obje dosyalari olusturma kurali
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo $(SEPARATOR)
	@echo Source: $<
	@echo Compiled: $@
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Exe dosyasi olusturma kurali
$(OUT_DIR)/$(EXE_NAME).exe: $(OBJECTS)
	@mkdir -p $(OUT_DIR)
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

size: $(OUT_DIR)/$(EXE_NAME).exe
	@echo $(SEPARATOR)
	@echo "Executable Size:"
	@size $<

# Proje derleme kurali
all: compiler $(OUT_DIR)/$(EXE_NAME).exe size
	@echo "Build completed!"

# Obje dosyalari ve exe dosyasi silme
clean:
	@rm -rf $(BUILD_DIR) $(OUT_DIR) clean_dummy
	@echo "Items deleted!"
	@echo $(SEPARATOR)
