#!/bin/sh

# 
# Vivado(TM)
# runme.sh: a Vivado-generated Runs Script for UNIX
# Copyright 1986-2022 Xilinx, Inc. All Rights Reserved.
# Copyright 2022-2024 Advanced Micro Devices, Inc. All Rights Reserved.
# 

if [ -z "$PATH" ]; then
  PATH=/home/rimurutempest/Tool/Xilinx/Vitis/2024.2/bin:/home/rimurutempest/Tool/Xilinx/Vivado/2024.2/ids_lite/ISE/bin/lin64:/home/rimurutempest/Tool/Xilinx/Vivado/2024.2/bin
else
  PATH=/home/rimurutempest/Tool/Xilinx/Vitis/2024.2/bin:/home/rimurutempest/Tool/Xilinx/Vivado/2024.2/ids_lite/ISE/bin/lin64:/home/rimurutempest/Tool/Xilinx/Vivado/2024.2/bin:$PATH
fi
export PATH

if [ -z "$LD_LIBRARY_PATH" ]; then
  LD_LIBRARY_PATH=
else
  LD_LIBRARY_PATH=:$LD_LIBRARY_PATH
fi
export LD_LIBRARY_PATH

HD_PWD='/home/rimurutempest/Code/LSI_Design_Contest/HLS/Adder32bit/Adder32bit_Vivado/Adder32bit_Vivado.runs/Adder32bit_BD_rst_ps8_0_100M_0_synth_1'
cd "$HD_PWD"

HD_LOG=runme.log
/bin/touch $HD_LOG

ISEStep="./ISEWrap.sh"
EAStep()
{
     $ISEStep $HD_LOG "$@" >> $HD_LOG 2>&1
     if [ $? -ne 0 ]
     then
         exit
     fi
}

EAStep vivado -log Adder32bit_BD_rst_ps8_0_100M_0.vds -m64 -product Vivado -mode batch -messageDb vivado.pb -notrace -source Adder32bit_BD_rst_ps8_0_100M_0.tcl
