#Makfile:        Library Directory Makefile
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"

#---------------------------------------#
#                                       #
#              Targets                  #
#                                       #
#---------------------------------------#
OBJS        = NMT_stdlib.so \
              NMT_log.so \
              RSXA.so \
              PCA9685.so \
              LD27MG.so \
              HCxSR04.so \
              NMT_sock_base.so \
              NMT_sock_multi.so \
              NMT_sock_tcp.so \
              L9110.so \
              RMCT_lib.so \
              RSDA_lib.so \
              ADS115.so \
              SensorDataAbstraction.so \

#---------------------------------------#
#                                       #
#            Dependancies               #
#                                       #
#---------------------------------------#
NMT_stdlib_LIBS     = -lc

NMT_log_LIBS        = -lNMT_stdlib \
                      -lc

RSXA_LIBS           = -lNMT_stdlib \
                      -ljson-c \
                      -lc

PCA9685_LIBS        = -lNMT_stdlib \
                      -lNMT_log \
                      -lc \
                      -lwiringPi \
                      -lcrypt \
                      -lm \
                      -lrt \
                      -lRSXA

LD27MG_LIBS         = -lNMT_stdlib \
                      -lNMT_log \
                      -lPCA9685 \
                      -lRSXA

HCxSR04_LIBS        = -lNMT_log \
                      -lNMT_stdlib \
                      -lRSXA \
                      -lwiringPi \
                      -lcrypt \
                      -lm \
                      -lrt

NMT_sock_base_LIBS   = -lNMT_stdlib \
                       -lNMT_log

NMT_sock_multi_LIBS  = -lNMT_stdlib \
                       -lNMT_log \
                       -lNMT_sock_base

NMT_sock_tcp_LIBS    = -lNMT_stdlib \
                       -lNMT_log \
                       -lNMT_sock_base


L9110_LIBS         =  -lNMT_stdlib \
                      -lNMT_log \
                      -lRSXA \
                      -lPCA9685

RMCT_lib_LIBS      = -lNMT_stdlib \
                     -lNMT_log \
                     -lRSXA \
                     -lL9110 \
                     -lPCA9685 \
                     -lLD27MG

ADS115_LIBS       = -lNMT_log \
                    -lRSXA \
                    -lwiringPi \
                    -lcrypt \
                    -lm \
                    -lrt

SensorDataAbstraction_LIBS = -lNMT_log \
                             -lRSXA \
                             -lADS115 \
                             -lHCxSR04 \

RSDA_lib_LIBS                = -lNMT_log \
                             -lRSXA \
                             -lADS115 \
                             -lSensorDataAbstraction \
                             -lHCxSR04


# -------Update output file name ---------#
TARGET_OBJS := $(foreach OBJ,$(OBJS),$(OBJ_DIR)/lib$(OBJ))
TARGET_OBJS += $(foreach OBJ,$(PY_OBJS),$(OBJ_DIR)/$(OBJ))

all: $(TARGET_OBJS)
.PHONY: all

# -----Build C Files ---------#
$(OBJ_DIR)/lib%.so: $(LIB_DIR)/%.c $(INC_DIR)/%.h
	gcc $(CFLAGS) $(SFLAGS) $(RPATH) -o  $@ $< $($(*)_LIBS)

# -----Build C++ Files ---------#
$(OBJ_DIR)/lib%.so: $(LIB_DIR)/%.cpp $(INC_DIR)/%.hpp
	g++ $(CFLAGS) $(SFLAGS) $(RPATH) -o  $@ $< $($(*)_LIBS)

# -----Build C++ Files ---------#
$(OBJ_DIR)/%.so: $(LIB_DIR)/%_py.cpp $(INC_DIR)/%.hpp
	g++ $(CFLAGS) $(SFLAGS) $(RPATH) -o  $@ $<  $($(*)_py_LIBS)

clean:
	rm -rf $(OBJ_DIR)/*.so
