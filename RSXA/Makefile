#Makfile:        RSXA Main Makefile
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"

#-------------------------------------------------------------------------------#
#                                                                               #
#                               Global Variables                                #
#                                                                               #
#-------------------------------------------------------------------------------#
export RSXA_LIB_DIR   = $(RSXA_DIR)/lib
export RSXA_INC_DIR   = $(RSXA_DIR)/inc
export RSXA_TST_DIR   = $(RSXA_DIR)/tst
export RSXA_LIBPY_DIR = $(RSXA_DIR)/lib_py
export CFLAGS         += -I $(RSXA_INC_DIR)

#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Main                                          #
#                                                                               #
#-------------------------------------------------------------------------------#
ACTIONS = bld_all \
          cpxifs

all: $(ACTIONS)

cpxifs:
	ln -fs $(RSXA_INC_DIR)/* $(XIFS_DIR)/RSXA/
	ln -fs $(RSXA_LIBPY_DIR)/* $(XIFS_DIR)/RSXA/

bld_all: 
	$(MAKE) -C $(RSXA_LIB_DIR)
	$(MAKE) -C $(RSXA_TST_DIR)
