open_component pl.comp -reset
add_files [list pl.cpp]
add_files -tb [list dsp_vs_pl_test.cpp in.dat out.gold.dat]
set_top mac
puts "Running: set_top mac"
set_part xc7z020-clg400-1
puts "Running: set_part xc7z020-clg400-1"
create_clock -period 10


csynth_design

exit