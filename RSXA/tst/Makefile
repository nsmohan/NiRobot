#Makfile:        Makefile for Test Directory
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"
 
ACTIONS   = mkdirs

# ----List of Test Binairies ---- #

all: $(ACTIONS)
.PHONY: all

mkdirs:
	if [ ! -d "Obj" ];    then ln -s $(OBJ_DIR) $(RSXA_TST_DIR); fi
	if [ ! -d "lib_py" ]; then ln -s $(RSXA_LIBPY_DIR) $(RSXA_TST_DIR); fi
	if [ ! -d "NMTX" ];   then ln -s $(XIFS_DIR)/NMTX $(RSXA_TST_DIR); fi
