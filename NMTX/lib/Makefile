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
C_OBJFILES   := $(patsubst $(NMTX_LIB_DIR)/%.c,  $(NMTX_OBJ_DIR)/%.o,$(C_SOURCES))
CPP_OBJFILES := $(patsubst $(NMTX_LIB_DIR)/%.cpp,$(NMTX_OBJ_DIR)/%.o,$(CPP_SOURCES))
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
SHARED_LIBS  = $(NMTX_OBJ_DIR)/libNMT_stdlib.so \
               $(NMTX_OBJ_DIR)/libNMT_sock.so

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

$(NMTX_OBJ_DIR)/libNMT_stdlib.so: $(NMTX_OBJ_DIR)/NMT_stdlib.o \
                                  $(NMTX_OBJ_DIR)/NMT_log.o

$(NMTX_OBJ_DIR)/libNMT_sock.so:   $(NMTX_OBJ_DIR)/NMT_sock_base.o \
                                  $(NMTX_OBJ_DIR)/NMT_sock_tcp.o \
                                  $(NMTX_OBJ_DIR)/NMT_sock_multi.o
#-------------------------------------------------------------------------------#
#                                                                               #
#                                 Automatic Rules                               #
#                                                                               #
#-------------------------------------------------------------------------------#
$(NMTX_OBJ_DIR)/%.o: $(NMTX_LIB_DIR)/%.c
	$(CC) -I $(NMTX_INC_DIR) -fPIC $(DEPFLAGS) -c $^ -o $@ 

$(NMTX_OBJ_DIR)/%.o: $(NMTX_LIB_DIR)/%.cpp
	$(CC) -I $(NMTX_INC_DIR) -fPIC $(DEPFLAGS) -c $^ -o $@ 

$(NMTX_OBJ_DIR)/%.so:
	$(CC) $(CFLAGS) $(SFLAGS) $(RPATH) -o $@ $^ $($(*).so,DEPS)


