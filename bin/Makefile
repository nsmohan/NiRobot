#Makfile:        Makefile for bin directory
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"

#---------------------------------------#
#                                       #
#              Targets                  #
#                                       #
#---------------------------------------#
BLDS = regdump \
       RMCT \
	   ultrasonic_test
#---------------------------------------#
#                                       #
#              Sources/Libs             #
#                                       #
#---------------------------------------#
regdump_LIBS = -lNMT_stdlib \
               -lwiringPi \
               -lcrypt \
               -lm \
               -lrt

RMCT_LIBS     = -lNMT_stdlib \
                -lNMT_log \
                -lRSXA \
                -lNMT_sock_base \
                -lNMT_sock_tcp \
                -ljsoncpp \
                -lRMCT_lib \
                -lL9110

ultrasonic_test_LIBS = -lNMT_stdlib \
                       -lNMT_log \
                       -lRSXA \
                       -lHCxSR04

# -------Update output file name ---------#
TARGET_BLDS := $(foreach BLD,$(BLDS),$(BLD_DIR)/$(BLD))

all: $(TARGET_BLDS)
.PHONY: all

# -----Build C Files ---------#
$(BLD_DIR)/%: $(BIN_DIR)/%.c
	gcc $(CFLAGS) $(RPATH) -o $@ $^ $($(*)_LIBS)

# -----Build C++ Files ---------#
$(BLD_DIR)/%: $(BIN_DIR)/%.cpp
	g++ $(CFLAGS) $(RPATH) -o $@ $^ $($(*)_LIBS)

clean:
	rm -rf $(BIN_DIR)/*
