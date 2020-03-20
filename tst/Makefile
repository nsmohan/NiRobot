#Makfile:        Makefile for Test Directory
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"
 
CMOCK= /usr/local
GTST_LIBS = -lgtest_main \
            -lgtest \
            -lgmock \
            -lgmock_main \
            -lpthread

GMOCK = /usr/src/gmock
GTEST = /usr/src/gtest

CFLAGS_T=$(shell $(CMOCK)/bin/cmock-config --cflags)
LDFLAGS_T=$(shell $(CMOCK)/bin/cmock-config --libs $(GMOCK) $(GTEST))

SRCS=unittest_PCA9685.cpp
STUBS=$(TST_DIR)/wiringPi_stub.o

UT_PCA9685_LIBS = -lNMT_stdlib \
                  -lPCA9685 \
                  -lNMT_log

TARGET_BLDS := $(foreach TST,$(TSTS),$(TST_DIR)/$(TST))

all: $(TST_DIR)/UT_PCA9685
	if [ ! -d "Obj" ]; then ln -s $(OBJ_DIR) $(TST_DIR); fi
	if [ ! -d "lib_py" ]; then ln -s $(LIBPY_DIR) $(TST_DIR); fi
.PHONY: all

$(TST_DIR)/wiringPi_stub.o: $(TST_DIR)/wiringPi_stub.cc
	g++ $(CFLAGS_T) -c $^ -o $@ 

$(TST_DIR)/unittest_PCA9685.o: $(TST_DIR)/unittest_PCA9685.cc
	g++ $(CFLAGS_T) -I $(INC_DIR) $(UT_PCA9685) -c $^ -o $@

$(TST_DIR)/UT_PCA9685: $(TST_DIR)/wiringPi_stub.o
	g++  $(LDFLAGS_T) -H -I $(INC_DIR) -o $@ $^ $(GTST_LIBS) -lwiringPi -lcrypt -lm -lrt
