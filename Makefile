#Makfile:        Makefile for all the code
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"


PROJ_DIR    = /home/nmohan/github/NiRobot
BIN_DIR     = $(PROJ_DIR)/bin
OBJ_DIR     = $(PROJ_DIR)/Obj
OBJ_DIR_ARM = $(PROJ_DIR)/Obj_arm
INC_DIR     = $(PROJ_DIR)/inc
LIB_DIR     = $(PROJ_DIR)/lib
OUT_DIR     = $(PROJ_DIR)/bld
OUT_DIR_ARM = $(PROJ_DIR)/bld_arm
CFLAGS      = -g -Wall -Wextra -I $(INC_DIR)
SFLAGS      = -fPIC -shared
RPATH       = -L$(OBJ_DIR) -Wl,-rpath=$(OBJ_DIR)

BINS        = $(OUT_DIR)/regdump \
              $(OUT_DIR)/camera_control

OBJS        = $(OBJ_DIR)/libNMT_stdlib.so \
              $(OBJ_DIR)/libNMT_log.so \
              $(OBJ_DIR)/librs.so \
              $(OBJ_DIR)/libPCA9685.so \
              $(OBJ_DIR)/libmtdr.so \
              $(OBJ_DIR)/libcam_motor_ctrl.so

PCA9685_LIBS  = -lNMT_stdlib \
                -lNMT_log \
                -lc \
                -lwiringPi \
                -lcrypt \
                -lm \
                -lrt \
                -lrs

REG_DUMP_LIBS = -lNMT_stdlib \
                -lwiringPi \
                -lcrypt \
                -lm \
                -lrt

MTDR_LIBS     = -lNMT_stdlib \
                -lNMT_log \
                -lwiringPi \
                -lPCA9685 \
                -lcrypt \
                -lm \
                -lrt \
                -lrs

CAM_CTRL_LIBS = -lNMT_stdlib \
                -lNMT_log \
                -lwiringPi \
                -lmtdr \
                -lPCA9685 \
                -lcrypt \
                -lm \
                -lrt \
                -lrs

NMT_LOG_LIBS  = -lc \
                -ljson-c \
                -lNMT_stdlib \
             
NMT_STD_LIBS  = -lc

CAM_CTRL_BIN  =  -lNMT_log \
                 -lcam_motor_ctrl

RS_LIBS       = -lNMT_stdlib \
                -lNMT_log \
                -ljson-c \
                -lc 

all: $(OBJS) \
     $(BINS)

$(OBJ_DIR)/libNMT_stdlib.so: $(LIB_DIR)/NMT_stdlib.c $(INC_DIR)/NMT_stdlib.h
	gcc $(CFLAGS) $(SFLAGS) -o $@ $< $(NMT_STD_LIBS)

$(OBJ_DIR)/libNMT_log.so: $(LIB_DIR)/NMT_log.c $(INC_DIR)/NMT_log.h
	gcc $(CFLAGS) $(SFLAGS) $(RPATH) -o  $@ $< $(NMT_LOG_LIBS)

$(OBJ_DIR)/librs.so: $(LIB_DIR)/RSXA.c $(INC_DIR)/RSXA.h
	gcc $(CFLAGS) $(SFLAGS) $(RPATH) -o  $@ $< $(RS_LIBS)

$(OBJ_DIR)/libPCA9685.so: $(LIB_DIR)/PCA9685.c $(INC_DIR)/PCA9685.h
	gcc $(CFLAGS) $(SFLAGS) $(RPATH) -o  $@ $< $(PCA9685_LIBS)

$(OBJ_DIR)/libmtdr.so: $(LIB_DIR)/MTDR.c $(INC_DIR)/MTDR.h
	gcc $(CFLAGS) $(SFLAGS) $(RPATH) -o  $@ $< $(MTDR_LIBS)

$(OBJ_DIR)/libcam_motor_ctrl.so: $(LIB_DIR)/CAM_MOTOR_CTRL.cpp $(INC_DIR)/CAM_MOTOR_CTRL.hpp
	g++ $(CFLAGS) $(SFLAGS) $(RPATH) -o  $@ $< $(CAM_CTRL_LIBS)

$(OUT_DIR)/regdump: $(BIN_DIR)/register_dump.c
	gcc $(CFLAGS) $(RPATH) -o $@ $^ $(REG_DUMP_LIBS)

$(OUT_DIR)/camera_control: $(BIN_DIR)/CAMERA_CTRL.cpp
	g++ $(CFLAGS) $(RPATH) -o $@ $^ $(CAM_CTRL_BIN)

#$(OUT_DIR)/mtdr: $(BIN_DIR)/MTDR.c $(INC_DIR)/MTDR.h
#	gcc $(CFLAGS) $(RPATH) -o $@ $^ $(MTDR_LIBS)

clean:
	rm all $(OBJS) $(BINS)
