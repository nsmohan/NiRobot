#Makfile:        Makefile for Test Directory
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"
 
# --- Global Variables ------#
CMOCK     = /usr/local
GMOCK     = /usr/src/gmock
GTEST     = /usr/src/gtest
STUB_DIR  = $(TST_DIR)/stubs
TBLD_DIR  = $(TST_DIR)/bld
CFLAGS_T  = $(shell $(CMOCK)/bin/cmock-config --cflags)
LDFLAGS_T = $(shell $(CMOCK)/bin/cmock-config --libs $(GMOCK) $(GTEST))

ACTIONS   = mkdirs

GTST_LIBS = -lgtest_main \
            -lgtest \
            -lgmock \
            -lgmock_main \
            -lpthread

# ----List of Test Binairies ---- #
TSTS            = $(TBLD_DIR)/unittest_PCA9685 \
                  $(TBLD_DIR)/unittest_LD27MG \
                  $(TBLD_DIR)/unittest_L9110 \
                  $(TBLD_DIR)/unittest_RMCT_lib \
                  $(TBLD_DIR)/unittest_NMT_sock

unittest_PCA9685_LIBS = -lwiringPi \
                        -lcrypt \
                        -lm \
                        -lrt \
                        -lNMT_log \
                        -lNMT_stdlib \
                        -lPCA9685

unittest_LD27MG_LIBS =  -lNMT_log \
                        -lNMT_stdlib \
                        -lPCA9685 \
                        -lRSXA \
                        -lLD27MG

unittest_L9110_LIBS = -lNMT_log \
                      -lRSXA \
                      -lPCA9685 \
                      -lL9110

unittest_RMCT_lib_LIBS = -lNMT_stdlib \
                         -lNMT_log \
                         -lRSXA \
                         -lL9110 \
                         -lLD27MG \
                         -lPCA9685 \
                         -lwiringPi \
                         -lcrypt \
                         -lm \
                         -lrt \
                         -lRMCT_lib

unittest_NMT_sock_LIBS = -lNMT_stdlib \
                         -lNMT_log \
                         -lNMT_sock_base \
                         -lNMT_sock_multi \
                         -lNMT_sock_tcp

all: $(ACTIONS) \
     $(TSTS)
.PHONY: all

mkdirs:
	if [ ! -d "Obj" ]; then ln -s $(OBJ_DIR) $(TST_DIR); fi
	if [ ! -d "lib_py" ]; then ln -s $(LIBPY_DIR) $(TST_DIR); fi
	if [ ! -d "bld" ]; then mkdir $(TST_DIR)/bld; fi

$(OBJ_DIR)/%.o: $(STUB_DIR)/%.cc
	g++ $(CFLAGS_T) -I $(INC_DIR) -c $^ -o $@ 

$(OBJ_DIR)/%.o: $(TST_DIR)/%.cc
	g++ $(CFLAGS_T) -I $(INC_DIR) -I $(STUB_DIR) -c $^ -o $@ 

$(TBLD_DIR)/unittest_PCA9685: $(OBJ_DIR)/wiringPi_stub.o $(OBJ_DIR)/unittest_PCA9685.o
	g++  $(LDFLAGS_T) $(RPATH) -I $(INC_DIR) -o $@ $^ $(GTST_LIBS) $(unittest_PCA9685_LIBS)

$(TBLD_DIR)/unittest_LD27MG: $(OBJ_DIR)/PCA9685_stub.o $(OBJ_DIR)/unittest_LD27MG.o
	g++  $(LDFLAGS_T) $(RPATH) -I $(INC_DIR) -o $@ $^ $(GTST_LIBS) $(unittest_LD27MG_LIBS)

$(TBLD_DIR)/unittest_L9110: $(OBJ_DIR)/PCA9685_stub.o $(OBJ_DIR)/unittest_L9110.o
	g++  $(LDFLAGS_T) $(RPATH) -I $(INC_DIR) -o $@ $^ $(GTST_LIBS) $(unittest_L9110_LIBS)

$(TBLD_DIR)/unittest_NMT_sock: $(OBJ_DIR)/unittest_NMT_sock.o
	g++  $(LDFLAGS_T) $(RPATH) -I $(INC_DIR) -o $@ $^ $(GTST_LIBS) $(unittest_NMT_sock_LIBS)

$(TBLD_DIR)/unittest_RMCT_lib: $(OBJ_DIR)/LD27MG_stub.o $(OBJ_DIR)/unittest_RMCT_lib.o $(OBJ_DIR)/PCA9685_stub.o \
                               $(OBJ_DIR)/wiringPi_stub.o
	g++  $(LDFLAGS_T) $(RPATH) -I $(INC_DIR) -o $@ $^ $(GTST_LIBS) $(unittest_RMCT_lib_LIBS)
