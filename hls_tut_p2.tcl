open_project gemm
set_top gemm
add_files gemm.cc
add_files -tb gemm_test.cc
add_files -tb tut_src/test_lib.cc
open_solution "solution0"
set_part {xc7z020clg484-1}
create_clock -period 10 -name default
csim_design -clean
csynth_design
export_design -format ip_catalog
close_project

exit

