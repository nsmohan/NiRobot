#Makfile:        Make for rui Directory
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"
 
# --- Global Variables ------#
ACTIONS   = mkdirs

all: $(ACTIONS)
.PHONY: all

mkdirs:
	if [ ! -d "lib_py" ]; then ln -s $(LIBPY_DIR) $(RUI_DIR); fi
