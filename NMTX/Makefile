#Makfile:        NMTX Main Makefile
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"

#-------------------------------------------------------------------------------#
#                                                                               #
#                               Global Variables                                #
#                                                                               #
#-------------------------------------------------------------------------------#
export NMTX_DIR       = $(PWD)
export NMTX_LIB_DIR   = $(NMTX_DIR)/lib
export NMTX_LIBPY_DIR = $(NMTX_DIR)/lib_py
export NMTX_INC_DIR   = $(NMTX_DIR)/inc
export NMTX_TST_DIR   = $(NMTX_DIR)/tst
export NMTX_OBJ_DIR   = $(NMTX_DIR)/Obj
export CFLAGS         = -g -Wall -Wextra -I $(NMTX_INC_DIR) -Wno-missing-field-initializers
export SFLAGS         = -fPIC -shared
export RPATH          = -L$(NMTX_OBJ_DIR) -Wl,-rpath=$(NMTX_OBJ_DIR)

#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Main                                          #
#                                                                               #
#-------------------------------------------------------------------------------#
ACTIONS = mkdirs \
          bld_all

all: $(ACTIONS)

mkdirs: 
	if [ ! -d "Obj" ]; then mkdir $(NMTX_OBJ_DIR); fi

bld_all: 
	$(MAKE) -C $(NMTX_LIB_DIR)
	$(MAKE) -C $(NMTX_TST_DIR)

clean: 
	rm -rf $(NMTX_OBJ_DIR)/*
