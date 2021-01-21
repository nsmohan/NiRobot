#Makfile:        Makefile for root directory
#__author__      = "Nitin Mohan
#__copyright__   = "Copy Right 2018. NM Technologies"

export PROJ_DIR     = $(PWD)
export PY_DIR       = /usr/include/python2.7
export BIN_DIR      = $(PROJ_DIR)/bin
export OBJ_DIR      = $(PROJ_DIR)/Obj
export INC_DIR      = $(PROJ_DIR)/.xifs
export LIB_DIR      = $(PROJ_DIR)/lib
export LIBPY_DIR    = $(PROJ_DIR)/lib_py
export BLD_DIR      = $(PROJ_DIR)/bld
export TST_DIR      = $(PROJ_DIR)/tst
export RUI_DIR      = $(PROJ_DIR)/rui
export XIFS_DIR     = $(PROJ_DIR)/.xifs
export TOOLS_DIR    = $(PROJ_DIR)/tools
export GTEST_OUTPUT = "xml:$(PROJ_DIR)/test_results/"
export NMTX_DIR     = $(PROJ_DIR)/NMTX
export RSXA_DIR     = $(PROJ_DIR)/RSXA
export CFLAGS       = -g -Wall -Wextra -I $(INC_DIR) -I $(PY_DIR) \
                      -Wno-missing-field-initializers
export SFLAGS       = -fPIC -shared
export RPATH        = -L$(OBJ_DIR) -Wl,-rpath=$(OBJ_DIR)

COVERAGE_RESULTS_DIR = $(PROJ_DIR)/coverage_results

ACTIONS =  mkdirs \
           bld_all

all: $(ACTIONS)

bld_all:
	$(MAKE) -C $(NMTX_DIR)
	$(MAKE) -C $(RSXA_DIR)
	#$(MAKE) -C $(LIB_DIR)
	#$(MAKE) -C $(BIN_DIR)
	#$(MAKE) -C $(TST_DIR)
	#$(MAKE) -C $(RUI_DIR)
	#$(MAKE) -C $(TOOLS_DIR)

coverage: CFLAGS += --coverage
coverage: $(ACTIONS)

coverage:
	if [ ! -d "coverage_results" ]; then mkdir $(COVERAGE_RESULTS_DIR); fi
	python3 tst/run_tests.py
	lcov --capture --directory . --output-file $(COVERAGE_RESULTS_DIR)/coverage.info
	genhtml $(COVERAGE_RESULTS_DIR)/coverage.info --output-directory $(COVERAGE_RESULTS_DIR)

mkdirs: 
	if [ ! -d "Obj" ]; then mkdir $(OBJ_DIR); fi
	if [ ! -f "Obj/__init__.py" ]; then touch $(OBJ_DIR)/__init__.py; fi
	if [ ! -d "bld" ]; then mkdir $(BLD_DIR); fi
	if [ ! -d ".xifs" ]; then mkdir $(XIFS_DIR); fi
	if [ ! -d ".xifs/NMTX" ]; then mkdir $(XIFS_DIR)/NMTX; fi
	if [ ! -d ".xifs/RSXA" ]; then mkdir $(XIFS_DIR)/RSXA; fi

clean:
	$(RM) $(OBJ_DIR)/*.so
	$(RM) $(OBJ_DIR)/*.o
	$(RM) $(OBJ_DIR)/*.d
	$(RM) $(TST_DIR)/bld/*
	$(RM) $(BLD_DIR)/*
	$(RM) $(LIBPY_DIR)/*pyc

coverage_clean:
	find -name "*.gcda" -type f -delete
	find -name "*.gcno" -type f -delete
