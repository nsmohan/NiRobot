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
              MTDR.so \
              CAM_MOTOR_CTRL.so \
              HCxSR04.so \
              ultrasonicController.so

#---------------------------------------#
#                                       #
#            Dependancies               #
#                                       #
#---------------------------------------#
NMT_stdlib_LIBS     = -lc

NMT_log_LIBS        = -lNMT_stdlib \
                      -lc

RSXA_LIBS           = -lNMT_stdlib \
                      -lNMT_log \
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

MTDR_LIBS           = -lNMT_stdlib \
                      -lNMT_log \
                      -lwiringPi \
                      -lPCA9685 \
                      -lcrypt \
                      -lm \
                      -lrt \
                      -lRSXA

CAM_MOTOR_CTRL_LIBS = -lNMT_stdlib \
                      -lNMT_log \
                      -lwiringPi \
                      -lMTDR \
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
                
ultrasonicController_LIBS 		= -lNMT_log \
								  -lNMT_stdlib \
								  -lHCxSR04 \
								  -lwiringPi \
								  -lRSXA \
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
