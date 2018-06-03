transcript on
if {[file exists rtl_work]} {
	vdel -lib rtl_work -all
}
vlib rtl_work
vmap work rtl_work

vlog -vlog01compat -work work +incdir+C:/fpga_proj {C:/fpga_proj/top.v}
vlog -vlog01compat -work work +incdir+C:/fpga_proj {C:/fpga_proj/divider.v}

