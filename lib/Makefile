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
              NMT_sock.so \
              L9110.so

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

CAM_MOTOR_CTRL_LIBS = -lNMT_stdlib \
                      -lNMT_log \
                      -lwiringPi \
                      -lLD27MG \
                      -lPCA9685 \
                      -lcrypt \
                      -lm \
                      -lrt \
                      -lRSXA

HCxSR04_LIBS        = -lNMT_log \
                      -lNMT_stdlib \
                      -lRSXA \
                      -lwiringPi \
                      -lcrypt \
                      -lm \
                      -lrt

NMT_sock_LIBS       = -lNMT_stdlib \
                      -lNMT_log \
                      -lboost_python

L9110_LIBS         =  -lNMT_stdlib \
                      -lNMT_log \
                      -lRSXA \
                      -lwiringPi \
                      -lcrypt \
                      -lm \
                      -lrt

RMDR_LIBS           = -lNMT_stdlib \
                      -lNMT_log \
                      -lPCA9685 \
                      -lLD27MG \
                      -lwiringPi \
                      -lcrypt \
                      -lm \
                      -lrt

# -------Update output file name ---------#
TARGET_OBJS := $(foreach OBJ,$(OBJS),$(OBJ_DIR)/lib$(OBJ))

all: $(TARGET_OBJS)
.PHONY: all

# -----Build C Files ---------#
$(OBJ_DIR)/lib%.so: $(LIB_DIR)/%.c $(INC_DIR)/%.h
	gcc $(CFLAGS) $(SFLAGS) $(RPATH) -o  $@ $< $($(*)_LIBS)

# -----Build C++ Files ---------#
$(OBJ_DIR)/lib%.so: $(LIB_DIR)/%.cpp $(INC_DIR)/%.hpp
	g++ $(CFLAGS) $(SFLAGS) $(RPATH) -o  $@ $< $($(*)_LIBS)


clean:
	rm -rf $(OBJ_DIR)/*.so
