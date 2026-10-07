# Helper script to run the make cmds, and then move imp files
# (HLS C++ source, header, report and tcl) to a sub-dir
#
# Usage: sh make_and_move.sh <dir_name>

make clean; make hls; make report

if [ ! -d "$1" ]; then
  mkdir $1
fi

cp fir.cpp $1; cp fir.h $1; cp fir_csynth.rpt $1; cp fir.tcl $1

