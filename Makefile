#Makfile:        Makefile for all the code
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"


export PROJ_DIR    = /home/nmohan/git/NiRobot
export BIN_DIR     = $(PROJ_DIR)/bin
export OBJ_DIR     = $(PROJ_DIR)/Obj
export INC_DIR     = $(PROJ_DIR)/inc
export LIB_DIR     = $(PROJ_DIR)/lib
export OUT_DIR     = $(PROJ_DIR)/bld
export CFLAGS      = -g -Wall -Wextra -I $(INC_DIR)
export SFLAGS      = -fPIC -shared
export RPATH       = -L$(OBJ_DIR) -Wl,-rpath=$(OBJ_DIR)

all: 
	$(MAKE) -C $(LIB_DIR)

clean:
	rm -rf $(OBJ_DIR)/*.so
