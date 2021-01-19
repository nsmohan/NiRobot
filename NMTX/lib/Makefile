#Makfile:        Library Directory Makefile
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"

#-------------------------------------------------------------------------------#
#                                                                               #
#                               Global Variables                                #
#                                                                               #
#-------------------------------------------------------------------------------#
C_SOURCES    := $(shell find $(NMTX_LIB_DIR) -name '*.c')
CPP_SOURCES  := $(shell find $(NMTX_LIB_DIR) -name '*.cpp')
C_OBJFILES   := $(patsubst $(NMTX_LIB_DIR)/%.c,  $(OBJ_DIR)/%.o,$(C_SOURCES))
CPP_OBJFILES := $(patsubst $(NMTX_LIB_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(CPP_SOURCES))
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
SHARED_LIBS  = $(OBJ_DIR)/libNMT_stdlib.so \
               $(OBJ_DIR)/libNMT_sock.so

#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Dependancies                                  #
#                                                                               #
#-------------------------------------------------------------------------------#
libNMT_stdlib.so,DEPS  := -lpthread
libNMT_sock.so,DEPS    := -lNMT_stdlib

#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Main                                          #
#                                                                               #
#-------------------------------------------------------------------------------#
all: $(SHARED_LIBS)
.PHONY: all

$(OBJ_DIR)/libNMT_stdlib.so: $(OBJ_DIR)/NMT_stdlib.o \
                                  $(OBJ_DIR)/NMT_log.o

$(OBJ_DIR)/libNMT_sock.so:   $(OBJ_DIR)/NMT_sock_base.o \
                                  $(OBJ_DIR)/NMT_sock_tcp.o \
                                  $(OBJ_DIR)/NMT_sock_multi.o
#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Automatic Rules                               #
#                                                                               #
#-------------------------------------------------------------------------------#
$(OBJ_DIR)/%.o: $(NMTX_LIB_DIR)/%.c
	$(CC) -I $(NMTX_INC_DIR) -fPIC $(DEPFLAGS) -c $^ -o $@ 

$(OBJ_DIR)/%.o: $(NMTX_LIB_DIR)/%.cpp
	$(CC) -I $(NMTX_INC_DIR) -fPIC $(DEPFLAGS) -c $^ -o $@ 

$(OBJ_DIR)/%.so:
	$(CC) $(CFLAGS) $(SFLAGS) $(RPATH) -o $@ $^ $($(*).so,DEPS)


