#Makfile:        Bin Directory Makefile
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"

#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Targets                                       #
#                                                                               #
#-------------------------------------------------------------------------------#
BINARIES  = $(BLD_DIR)/regdump

#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Dependancies                                  #
#                                                                               #
#-------------------------------------------------------------------------------#
regdump,DEPS  := -lwiringPi \
                 -lcrypt \
                 -lm \
                 -lrt

all: $(BINARIES)
.PHONY: all

#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Automatic Rules                               #
#                                                                               #
#-------------------------------------------------------------------------------#
$(OBJ_DIR)/%.o: $(NMTX_LIB_DIR)/%.c
	$(CC) -I $(NMTX_INC_DIR) -fPIC $(DEPFLAGS) -c $^ -o $@ 

$(OBJ_DIR)/%.o: $(NMTX_LIB_DIR)/%.cpp
	$(CC) -I $(NMTX_INC_DIR) -fPIC $(DEPFLAGS) -c $^ -o $@ 

$(BLD_DIR)/%:
	$(CC) $(CFLAGS) $(SFLAGS) $(RPATH) -o $@ $^ $($(*).so,DEPS)
