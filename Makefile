#Makfile:        Makefile for root directory
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"

export PROJ_DIR    = /home/nmohan/github/NiRobot
export PY_DIR      = /usr/include/python2.7
export BIN_DIR     = $(PROJ_DIR)/bin
export OBJ_DIR     = $(PROJ_DIR)/Obj
export INC_DIR     = $(PROJ_DIR)/inc
export LIB_DIR     = $(PROJ_DIR)/lib
export LIBPY_DIR   = $(PROJ_DIR)/lib_py
export BLD_DIR     = $(PROJ_DIR)/bld
export TST_DIR     = $(PROJ_DIR)/tst
export CFLAGS      = -g -Wall -Wextra -I $(INC_DIR) -I $(PY_DIR) \
                     -Wno-missing-field-initializers 
export SFLAGS      = -fPIC -shared
export RPATH       = -L$(OBJ_DIR) -Wl,-rpath=$(OBJ_DIR)

ACTIONS =  mkdirs \
           bld_all

all: $(ACTIONS)

bld_all:
	$(MAKE) -C $(LIB_DIR)
	$(MAKE) -C $(BIN_DIR)
	$(MAKE) -C $(TST_DIR)

mkdirs: 
	if [ ! -d "Obj" ]; then mkdir $(OBJ_DIR); fi
	if [ ! -f "Obj/__init__.py" ]; then touch $(OBJ_DIR)/__init__.py; fi
	if [ ! -d "bld" ]; then mkdir $(BLD_DIR); fi

clean:
	rm -rf $(OBJ_DIR)/*.so
	rm -rf $(OBJ_DIR)/*.o
	rm -rf $(TST_DIR)/bld/*
	rm -rf $(BLD_DIR)/*
	rm -rf $(LIBPY_DIR)/*pyc
