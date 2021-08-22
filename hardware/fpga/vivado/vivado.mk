FPGA_OBJ=synth_system.bit
FPGA_LOG=vivado.log

FPGA_SERVER=$(VIVADO_SERVER)
FPGA_USER=$(VIVADO_USER)

include ../../fpga.mk

# work-around for http://svn.clifford.at/handicraft/2016/vivadosig11
export RDI_VERBOSE = False

post-build:

board-clean: hw-clean
	rm -rf .Xil/ *.map *. *~ synth_*.mmi synth_*.bit synth_system*.v \
	*_tb table.txt tab_*/ *webtalk* *.jou xelab.* xsim[._]* xvlog.* \
	*.log uart_loader *.ltx system.v fsm_encoding.os
	if [ $(CLEANIP) ]; then rm -rf ip; fi

.PHONY: pre-build post-build board-clean
