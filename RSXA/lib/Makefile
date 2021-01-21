#Makfile:        Library Directory Makefile
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"

#-------------------------------------------------------------------------------#
#                                                                               #
#                               Global Variables                                #
#                                                                               #
#-------------------------------------------------------------------------------#
C_SOURCES    := $(shell find $(RSXA_LIB_DIR) -name '*.c')
CPP_SOURCES  := $(shell find $(RSXA_LIB_DIR) -name '*.cpp')
C_OBJFILES   := $(patsubst $(RSXA_LIB_DIR)/%.c,  $(OBJ_DIR)/%.o,$(C_SOURCES))
CPP_OBJFILES := $(patsubst $(RSXA_LIB_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SOURCES))
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
SHARED_LIBS  = $(OBJ_DIR)/libRSXA.so

#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Dependancies                                  #
#                                                                               #
#-------------------------------------------------------------------------------#
libRSXA.so,DEPS    := -lNMT_stdlib \
                      -ljson-c \
                      -lc

#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Main                                          #
#                                                                               #
#-------------------------------------------------------------------------------#
all: $(SHARED_LIBS)
.PHONY: all

$(OBJ_DIR)/libRSXA.so: $(OBJ_DIR)/RSXA.o 

#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Automatic Rules                               #
#                                                                               #
#-------------------------------------------------------------------------------#
$(OBJ_DIR)/%.o: $(RSXA_LIB_DIR)/%.c
	$(CC) -I $(RSXA_INC_DIR) -fPIC $(DEPFLAGS) -c $^ -o $@ 

$(OBJ_DIR)/%.o: $(RSXA_LIB_DIR)/%.cpp
	$(CC) -I $(RSXA_INC_DIR) -fPIC $(DEPFLAGS) -c $^ -o $@ 

$(OBJ_DIR)/%.so:
	$(CC) $(CFLAGS) $(SFLAGS) $(RPATH) -o $@ $^ $($(*).so,DEPS)


