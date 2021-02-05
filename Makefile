#Makefile for project creation - created by talberko1

#---QUICK GUIDE---

#In order to create the required directories, use 'make start'.
#This command will generate the directories 'src' and 'include' by default.
#The default names for each generated directory can be edited below.

#In order to compile your project, use 'make' or 'make all'





#---TARGET EXECUTABLE / LIBRARY NAME---

TARGET	:=	app

#---AUTO GENERATED DIRECTORIES

SRC_DIR	:=	src
INC_DIR	:=	include
OBJ_DIR	:=	build
BIN_DIR	:=	bin

#---FILE EXTENSIONS---

OBJ_EXT	:=	o
DEP_EXT	:=	d
SRC_EXT	:=	cpp

#---COMPILER---

CC	:=	g++



#---START OF USER DATA---

DPDK_DPIFIREWALL_INCLUDES	=	-I$(INC_DIR)

include /usr/local/etc/PcapPlusPlus.mk

MONGO_INCLUDES = -I/usr/local/include/mongocxx/v_noabi -I/usr/local/include/libmongoc-1.0

BSON_INCLUDES =	-I/usr/local/include/bsoncxx/v_noabi -I/usr/local/include/libbson-1.0

MONGO_LIB	=	-lmongocxx

BSON_LIB	=	-lbsoncxx

#---END OF USER DATA---



#---START OF FLAGS---



#---WARNING FLAGS---



WFLAGS	:=	-Wall -Werror

#---COMPILER FLAGS--- (-I<include path>, Example: -I/usr/local/include)

CFLAGS	:=	$(WFLAGS)	$(PCAPPP_INCLUDES)	$(MONGO_INCLUDES)	$(BSON_INCLUDES)	$(DPDK_DPIFIREWALL_INCLUDES)

#---LINKER FLAGS--- (-L<library path>, Example: -L/usr/local/lib)

LDFLAGS	:=	$(WFLAGS)	$(PCAPPP_LIBS_DIR)

#---LIBRARIES--- (-l<lib name>, Example: -lPcap++, -lmongocxx.so)

LDLIBS	:=	$(PCAPPP_LIBS)	$(MONGO_LIB)	$(BSON_LIB)



#---END OF FLAGS---





#---START OF INTERNAL MAKEFILE---#

COMPILE	:=	-c
OUTPUT	:=	-o

recursive_wildcard=$(foreach d,$(wildcard $(1:=/*)),$(call recursive_wildcard,$d,$2,)	$(filter $(subst *,%,$2),$d))

SRCS	:=	$(call recursive_wildcard,$(SRC_DIR),*.$(SRC_EXT))
OBJS	:=	$(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%.$(OBJ_EXT),$(basename $(SRCS)))
DEPS	:=	$(patsubst $(SRC_DIR)/%,$(OBJ_DIR)/%.$(DEP_EXT),$(basename $(SRCS)))

GENERATED_DIRS	:=	$(OBJ_DIR)	$(BIN_DIR)

#---DEPENDENCY FLAGS---
DEPFLAGS	:=	-MMD	-MP

#---MAKE TARGET---
.PHONY: all
all:	$(SRCS)	$(GENERATED_DIRS)	$(TARGET)

#---MAKE INITIAL DIRECTORIES---
.PHONY: init
init:
	@echo "Building source directory: $(SRC_DIR)"
	@mkdir	$(SRC_DIR)
	@echo "Building include directory: $(INC_DIR)"
	@mkdir	$(INC_DIR)
	@echo "Created base directories!"

#---MAKE REQUIRED DIRECTORIES---
$(GENERATED_DIRS):
	@mkdir $@

#Make target application
$(TARGET):	$(OBJS)
	@echo "Building target: $(BIN_DIR)/$@"
	@$(CC)	$(LDFLAGS) $^ $(OUTPUT) $(BIN_DIR)/$@ $(LDLIBS)
	@echo "Done!"

.PRECIOUS:	$(OBJ_DIR)/.	$(OBJ_DIR)%/.

$(OBJ_DIR)/.:
	@mkdir -p $@

$(OBJ_DIR)%/.:
	@mkdir -p $@	

.SECONDEXPANSION:

#Make object files and put them in the 'obj' directory
$(OBJ_DIR)/%.$(OBJ_EXT):	$(SRC_DIR)/%.$(SRC_EXT) | $$(@D)/.
	@echo "Building file: $@"
	@$(CC)	$(DEPFLAGS)	$(CFLAGS)	$(COMPILE)	$< $(OUTPUT)	$@

#Clean target application
clean:
	@echo "Cleaning.."
	$(foreach dir,$(GENERATED_DIRS),@rm -rf $(dir))
	@echo "Done!"
-include $(DEPS)

#---END OF INTERNAL MAKEFILE---