# This makefile was tested with Vivado HLS 2019.2
# and Vitis v2024.1

# select the HLS compiler to use (vivado_hls or vitis)
TOP = bram_vs_pl

USE_VITIS=1

ifeq ($(USE_VITIS),1)

VITIS_HLS_PATH := $(shell which vitis_hls 2>/dev/null)
ifeq ($(strip $(VITIS_HLS_PATH)),)
  $(error Could not find "vitis_hls" on PATH.)
endif
VITIS_COMPILER := $(shell dirname "$(VITIS_HLS_PATH)")

ifneq (,$(findstring 2024.2,$(VITIS_HLS_PATH)))
  VHLS := $(VITIS_COMPILER)/../../../Vitis/2024.2
else
  VHLS := $(VITIS_COMPILER)/../../../Vitis_HLS/2024.1
endif

HLS_COMPILER=vitis-run --mode hls --tcl
else
VHLS := $(shell vivado_hls -r)
HLS_COMPILER=vivado_hls -f
endif


# Two implementations of accumulate() share one testbench ($(TOP)_test.cpp):
#   bram.cpp - input array left in block RAM
#   pl.cpp   - input array completely partitioned into registers
IMPLS = bram pl
EXAMPLES_BIN = $(addsuffix _test.bin,$(IMPLS))
EXAMPLES_LOG = $(addsuffix _test.log,$(IMPLS))

CC = gcc
CXX = g++

# insert new LD path to the LD_LIBRARY_PATH if it is not already there
LD_PATH_NEW = /usr/lib/x86_64-linux-gnu
LD_LIBRARY_PATH := $(shell echo $(value LD_LIBRARY_PATH) | grep -q $(LD_PATH_NEW) || echo $(LD_PATH_NEW):)$(value LD_LIBRARY_PATH)

# Setting the scripts directory location for our repo
SCRIPTS_DIR = $(CURDIR)/../../pp4fpgas/project_files/scripts
GEN_TCL = $(SCRIPTS_DIR)/gen_hls_runner_script.py

#HLS_CONFIG_FILE = $(CURDIR)/__hls_config__.ini
HLS_CONFIG_FILE = $(CURDIR)/__hls_config__.ini

#$(warning $(EXAMPLES_LOG))

%.o: %.cpp
	$(CXX) -I$(VHLS)/include -c $< -o $@


# Build one test binary per implementation; each writes its own <impl>_test.log
$(EXAMPLES_BIN): %_test.bin: $(TOP)_test.cpp %.cpp $(TOP).h
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) $(CXX) -I$(VHLS)/include -DLOG_FILE='"$*_test.log"' $(TOP)_test.cpp $*.cpp -g -o $@

# The testbench writes the log itself, so do not redirect stdout into it.
# The logs are phony so "make test" always reruns the tests (a failed run
# leaves its log behind, which would otherwise look up to date).
$(EXAMPLES_LOG): %_test.log: %_test.bin in.dat out.gold.dat
	./$< || { s=$$?; echo "./$< failed $$s, see $@" >&2; exit $$s; }

%_test.bin: %_test.cpp %.cpp
	LD_LIBRARY_PATH=$(LD_LIBRARY_PATH) $(CXX) -I$(VHLS)/include $? -g -o $@

#  missing cos_table and sin_table definition. No testbench files available
#	dft.c \
#	dft_precompute.c \


# One HLS component per implementation: bram.comp and pl.comp
CXX_HLS_TARGET_FILES = $(addsuffix .cpp,$(IMPLS))

HLS_TARGETS = $(patsubst %.cpp,%.comp,$(CXX_HLS_TARGET_FILES))

%.comp: %.tcl
	$(HLS_COMPILER) $< || { s=$$?; echo "$(HLS_COMPILER) $< failed $$s" >&2; exit $$s; }


%.tcl: %.cpp
	$(GEN_TCL) -c $(HLS_CONFIG_FILE) -i $< -o $@

%.log: %.bin
	./$< > $@ 2>&1 || { s=$$?; echo "./$< > $@ failed $$s" >&2; exit $$s; }


hls: $(HLS_TARGETS)

test: $(EXAMPLES_LOG)

# Synthesis reports are named after the top function (accumulate), so copy each
# one out as <impl>_csynth.rpt
report: hls
	for i in $(IMPLS); do \
	  cp -f $$i.comp/hls/syn/report/accumulate_csynth.rpt $(CURDIR)/$${i}_csynth.rpt || true; \
	done


clean:
	rm -rf *.o *.log *.bin *.tcl *.comp *.rpt logs hls out.dat

.PHONY: clean hls test report $(EXAMPLES_LOG)

# Prevent make from deleting the .bin intermediate files
.PRECIOUS: %.bin %.tcl
