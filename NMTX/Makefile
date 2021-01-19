#Makfile:        NMTX Main Makefile
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"

#-------------------------------------------------------------------------------#
#                                                                               #
#                               Global Variables                                #
#                                                                               #
#-------------------------------------------------------------------------------#
export NMTX_LIB_DIR   = $(NMTX_DIR)/lib
export NMTX_LIBPY_DIR = $(NMTX_DIR)/lib_py
export NMTX_INC_DIR   = $(NMTX_DIR)/inc
export NMTX_TST_DIR   = $(NMTX_DIR)/tst
export CFLAGS         += -I $(NMTX_INC_DIR)

#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Main                                          #
#                                                                               #
#-------------------------------------------------------------------------------#
ACTIONS = bld_all

all: $(ACTIONS)

bld_all: 
	$(MAKE) -C $(NMTX_LIB_DIR)
	$(MAKE) -C $(NMTX_TST_DIR)
