#Makfile:        Makefile for root directory
#__author__      = "Nitin Mohan"
#__copyright__   = "Copy Right 2018. NM Technologies"

export PROJ_DIR    = $(PWD)
export BIN_DIR     = $(PROJ_DIR)/bin
export OBJ_DIR     = $(PROJ_DIR)/Obj
export INC_DIR     = $(PROJ_DIR)/inc
export LIB_DIR     = $(PROJ_DIR)/lib
export LIBPY_DIR   = $(PROJ_DIR)/lib_py
export BLD_DIR     = $(PROJ_DIR)/bld
<<<<<<< HEAD
export CFLAGS      = -g -Wall -Wextra -I $(INC_DIR) -std=c++17
=======
<<<<<<< HEAD
export TST_DIR     = $(PROJ_DIR)/tst
export CFLAGS      = -g -Wall -Wextra -I $(INC_DIR) \
                     -Wno-missing-field-initializers 
=======
export CFLAGS      = -g -Wall -Wextra -I $(INC_DIR) -std=c++17
>>>>>>> ultrasonicAbstraction
>>>>>>> dev
export SFLAGS      = -fPIC -shared
export RPATH       = -L$(OBJ_DIR) -Wl,-rpath=$(OBJ_DIR)

all: 
	$(MAKE) -C $(LIB_DIR)
	$(MAKE) -C $(BIN_DIR)
	$(MAKE) -C $(TST_DIR)


clean:
	rm -rf $(OBJ_DIR)/*.so
	rm -rf $(BLD_DIR)/*
	rm -rf $(LIBPY_DIR)/*pyc
