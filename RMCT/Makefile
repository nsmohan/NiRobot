#Makfile:        RMCT Main Makefile
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"

#-------------------------------------------------------------------------------#
#                                                                               #
#                               Global Variables                                #
#                                                                               #
#-------------------------------------------------------------------------------#
export RMCT_LIB_DIR   = $(RMCT_DIR)/lib
export RMCT_BIN_DIR   = $(RMCT_DIR)/bin
export RMCT_LIBPY_DIR = $(RMCT_DIR)/lib_py
export RMCT_INC_DIR   = $(RMCT_DIR)/inc
export RMCT_TST_DIR   = $(RMCT_DIR)/tst
export CFLAGS         += -I $(RMCT_INC_DIR)

#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Main                                          #
#                                                                               #
#-------------------------------------------------------------------------------#
ACTIONS = bld_all

all: $(ACTIONS)

bld_all: 
	#$(MAKE) -C $(RMCT_BIN_DIR)
	$(MAKE) -C $(RMCT_LIB_DIR)
	#$(MAKE) -C $(RMCT_TST_DIR)
