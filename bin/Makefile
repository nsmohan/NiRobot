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
       ads115_test \
       ultrasonic_test \
       RSDA 
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

RSDA_LIBS     = -lNMT_stdlib \
                -lNMT_log \
                -lRSXA \
                -lNMT_sock_base \
                -lNMT_sock_multi \
                -lNMT_sock_tcp \
                -ljsoncpp \
                -lHCxSR04\
                -lADS115 \
                -lSensorDataAbstraction \
                -lRSDA_lib\
                -lpthread

ultrasonic_test_LIBS = -lNMT_log \
                       -lRSXA \
                       -lSensorDataAbstraction \
                       -lHCxSR04

ads115_test_LIBS     = -lNMT_log \
                       -lRSXA \
                       -lADS115 \
                       -lSensorDataAbstraction

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

$(BLD_DIR)/%: $(BIN_DIR)/%.hpp $(BIN_DIR)/%.cpp
	g++ $(CFLAGS) $(RPATH) -o $@ $^ $($(*)_LIBS)

clean:
	rm -rf $(BIN_DIR)/*
