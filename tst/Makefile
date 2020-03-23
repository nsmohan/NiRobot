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
TSTS            = $(TBLD_DIR)/unittest_PCA9685

unittest_PCA9685_LIBS = $(GTST_LIBS) \
                        -lwiringPi \
                        -lcrypt \
                        -lm \
                        -lrt \
                        -lNMT_log \
                        -lNMT_stdlib \
                        -lPCA9685

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

$(TBLD_DIR)/%: $(OBJ_DIR)/wiringPi_stub.o $(OBJ_DIR)/unittest_PCA9685.o
	g++  $(LDFLAGS_T) $(RPATH) -I $(INC_DIR) -o $@ $^ $($(*)_LIBS)
