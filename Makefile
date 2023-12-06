# Project Directories
COMMONLIB_DIR := .
CORE_DIR := $(COMMONLIB_DIR)/Core
COMMS_DIR := $(COMMONLIB_DIR)/Comms

# Source and Include Directories for Core and Comms
CORE_SRC := $(CORE_DIR)/Src
CORE_INC := $(CORE_DIR)/Inc
COMMS_SRC := $(COMMS_DIR)/Src
COMMS_INC := $(COMMS_DIR)/Inc


LIB_OUT := ./lib

# Object Files Directory
OBJ_DIR := $(LIB_OUT)/obj

# Compiler
CC := g++

# Compiler Flags
CFLAGS := -I$(CORE_INC) -I$(COMMS_INC) -c

# Find Source Files
CORE_SRCS := $(wildcard $(CORE_SRC)/*.cpp)
COMMS_SRCS := $(wildcard $(COMMS_SRC)/*.cpp)

# Generate Object File Names
CORE_OBJS := $(patsubst $(CORE_SRC)/%.cpp, $(OBJ_DIR)/%.o, $(CORE_SRCS))
COMMS_OBJS := $(patsubst $(COMMS_SRC)/%.cpp, $(OBJ_DIR)/%.o, $(COMMS_SRCS))

# Library Names
CORE_LIB := $(LIB_OUT)/libcore.a
COMMS_LIB := $(LIB_OUT)/libcomms.a

# Default Target
all: $(CORE_LIB) $(COMMS_LIB)

# Rules for Core Library
$(CORE_LIB): $(CORE_OBJS)
	@mkdir -p $(LIB_OUT)
	ar rcs $@ $^

# Rules for Comms Library
$(COMMS_LIB): $(COMMS_OBJS)
	@mkdir -p $(LIB_OUT)
	ar rcs $@ $^

# Compile Core Source Files
$(OBJ_DIR)/%.o: $(CORE_SRC)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -o $@

# Compile Comms Source Files
$(OBJ_DIR)/%.o: $(COMMS_SRC)/%.cpp
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $< -o $@

# Clean Up
clean:
	rm -f $(CORE_LIB) $(COMMS_LIB)
	rm -f $(OBJ_DIR)/*.o
