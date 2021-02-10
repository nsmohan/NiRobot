#Makfile:        Library Directory Makefile
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"

#-------------------------------------------------------------------------------#
#                                                                               #
#                               Global Variables                                #
#                                                                               #
#-------------------------------------------------------------------------------#
C_SOURCES    := $(shell find $(RMCT_LIB_DIR) -name '*.c')
CPP_SOURCES  := $(shell find $(RMCT_LIB_DIR) -name '*.cpp')
C_OBJFILES   := $(patsubst $(RMCT_LIB_DIR)/%.c,  $(OBJ_DIR)/%.o,$(C_SOURCES))
CPP_OBJFILES := $(patsubst $(RMCT_LIB_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SOURCES))
OBJFILES     := $(C_OBJFILES) $(CPP_OBJFILES)
DEPFILES     := $(patsubst %.o,%.d,$(OBJFILES))
DEPFLAGS     := -MMD -MP
#-------------------------------------------------------------------------------#
#                                                                               #
#                               Includes                                        #
#                                                                               #
#-------------------------------------------------------------------------------#
-include $(DEPFILES)

#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Targets                                       #
#                                                                               #
#-------------------------------------------------------------------------------#
SHARED_LIBS  = $(OBJ_DIR)/libRMCT_lib.so \

#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Dependancies                                  #
#                                                                               #
#-------------------------------------------------------------------------------#
libRMCT_lib.so,DEPS  := -lNMT_stdlib \
                        -lRSXA

#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Main                                          #
#                                                                               #
#-------------------------------------------------------------------------------#
all: $(SHARED_LIBS)
.PHONY: all

$(OBJ_DIR)/libRMCT_lib.so: $(OBJ_DIR)/L9110.o \
                           $(OBJ_DIR)/LD27MG.o \
                           $(OBJ_DIR)/PCA9685.o \
                           $(OBJ_DIR)/RMCT_lib.o
#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Automatic Rules                               #
#                                                                               #
#-------------------------------------------------------------------------------#
$(OBJ_DIR)/%.o: $(RMCT_LIB_DIR)/%.c
	$(CC) $(CFLAGS) -I $(RMCT_INC_DIR) -fPIC $(DEPFLAGS) -c $^ -o $@ 

$(OBJ_DIR)/%.o: $(RMCT_LIB_DIR)/%.cpp
	$(CC) $(CFLAGS) -I $(RMCT_INC_DIR) -fPIC $(DEPFLAGS) -c $^ -o $@ 

$(OBJ_DIR)/%.so:
	$(CC) $(CFLAGS) $(SFLAGS) $(RPATH) -o $@ $^ $($(*).so,DEPS)

