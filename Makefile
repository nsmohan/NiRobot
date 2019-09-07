#Makfile:        Makefile for all the code
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"


BIN_DIR		= bin
OBJ_DIR		= Obj
OBJ_DIR_ARM = Obj_arm
INC_DIR		= inc
LIB_DIR		= lib
OUT_DIR		= bld
OUT_DIR_ARM = bld_arm
CFLAGS 		= -g -Wall -Wextra -I $(INC_DIR)
SFLAGS		= -fPIC -shared
RPATH       = -L$(OBJ_DIR) -Wl,-rpath=$(OBJ_DIR)

BINS		= $(OUT_DIR)/regdump \
              $(OUT_DIR)/mtdr \

OBJS		= $(OBJ_DIR)/libNMT_stdlib.so \
              $(OBJ_DIR)/libNMT_log.so \
              $(OBJ_DIR)/libPCA9685.so \

PCA9685_LIBS  = -lNMT_stdlib \
                -lNMT_log \
                -lc \
                -lwiringPi \
                -lcrypt \
                -lm \
                -lrt

REG_DUMP_LIBS = -lNMT_stdlib \
                -lwiringPi \
                -lcrypt \
                -lm \
                -lrt

MTDR_LIBS     = -lNMT_stdlib \
                -lNMT_log \
                -lPCA9685 \
                -lcrypt \
                -lm \
                -lrt

NMT_LOG_LIBS  = -lc \
                -ljson-c \
                -lNMT_stdlib \
             
NMT_STD_LIBS  = -lc

all: $(OBJS) \
     $(BINS)

$(OBJ_DIR)/libNMT_stdlib.so: $(LIB_DIR)/NMT_stdlib.c $(INC_DIR)/NMT_stdlib.h
	gcc $(CFLAGS) $(SFLAGS) -o $@ $< $(NMT_STD_LIBS)

$(OBJ_DIR)/libNMT_log.so: $(LIB_DIR)/NMT_log.c $(INC_DIR)/NMT_log.h
	gcc $(CFLAGS) $(SFLAGS) $(RPATH) -o  $@ $< $(NMT_LOG_LIBS)

$(OBJ_DIR)/libPCA9685.so: $(LIB_DIR)/PCA9685.c $(INC_DIR)/PCA9685.h
	gcc $(CFLAGS) $(SFLAGS) $(RPATH) -o  $@ $< $(PCA9685_LIBS)

$(OUT_DIR)/regdump: $(BIN_DIR)/register_dump.c
	gcc $(CFLAGS) $(RPATH) -o $@ $^ $(REG_DUMP_LIBS)

$(OUT_DIR)/mtdr: $(BIN_DIR)/mtdr.c
	gcc $(CFLAGS) $(RPATH) -o $@ $^ $(MTDR_LIBS)

clean:
	rm all $(OBJS) $(BINS)
