#Makfile:        Makefile for Test Directory
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"
 
# --- Global Variables ------#
CMOCK     = /usr/local
TBLD_DIR  = $(NMTX_TST_DIR)/bld
CFLAGS_T  = $(shell $(CMOCK)/bin/cmock-config --cflags)
LDFLAGS_T = $(shell $(CMOCK)/bin/cmock-config --libs $(GTEST_DIR))

ACTIONS   = mkdirs

GTST_LIBS = -lgtest_main \
            -lgtest \
            -lgmock \
            -lgmock_main \
            -lpthread

# ----List of Test Binairies ---- #
TSTS            = $(TBLD_DIR)/unittest_NMT_sock

unittest_NMT_sock,DEPS = -lNMT_stdlib \
                         -lNMT_sock

all: $(ACTIONS) \
     $(TSTS)
.PHONY: all

mkdirs:
	if [ ! -d "Obj" ];    then ln -s $(OBJ_DIR) $(NMTX_TST_DIR); fi
	if [ ! -d "lib_py" ]; then ln -s $(NMTX_LIBPY_DIR) $(NMTX_TST_DIR); fi
	if [ ! -d "bld" ];    then mkdir $(NMTX_TST_DIR)/bld; fi

$(OBJ_DIR)/%.o: $(NMTX_TST_DIR)/%.cc
	g++ $(CFLAGS_T) -I $(NMTX_INC_DIR) -c $^ -o $@ 

$(TBLD_DIR)/%:
	echo $*
	g++ $(LDFLAGS_T) $(RPATH) -I $(NMTX_INC_DIR) -o $@ $^ $(GTST_LIBS) $($*,DEPS)

$(TBLD_DIR)/unittest_NMT_sock: $(OBJ_DIR)/unittest_NMT_sock.o
