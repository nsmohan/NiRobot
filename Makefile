#Makfile:        Makefile for all the code
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"


BIN_DIR		= bin
OBJ_DIR		= Obj
INC_DIR		= inc
LIB_DIR		= lib
OUT_DIR		= output
CFLAGS 		= -g -Wall -Wextra -I $(INC_DIR)
BINS		= $(OBJ_DIR)/libNMT_stdlib.so $(OUT_DIR)/lib_test

all: $(BINS)

$(OBJ_DIR)/libNMT_stdlib.so: $(LIB_DIR)/NMT_stdlib.c $(INC_DIR)/NMT_stdlib.h
	gcc $(CFLAGS) -fPIC -shared -o  $@ $(LIB_DIR)/NMT_stdlib.c -lc

$(OUT_DIR)/lib_test: $(BIN_DIR)/testlib.c
	gcc $(CFLAGS) -L$(OBJ_DIR) -Wl,-rpath=$(OBJ_DIR) -o $@ $^  -lNMT_stdlib

clean:
	rm  $(OBJ_DIR)/*.so 
