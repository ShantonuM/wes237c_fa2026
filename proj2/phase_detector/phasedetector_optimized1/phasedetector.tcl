open_component phasedetector.comp -reset
add_files [list phasedetector.cpp cordiccart2pol.cpp fir.cpp]
add_files -tb [list phasedetector_test.cpp input_i.dat input_q.dat out.gold.dat]
set_top phasedetector
puts "Running: set_top phasedetector"
set_part xc7z020-clg400-1
puts "Running: set_part xc7z020-clg400-1"
create_clock -period 10


csynth_design

exit