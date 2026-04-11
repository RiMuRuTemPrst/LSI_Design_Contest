#include "xadder32.h"

XAdder32_Config XAdder32_ConfigTable[] __attribute__ ((section (".drvcfg_sec"))) = {

	{
		"xlnx,adder32-1.0", /* compatible */
		0xa0000000 /* reg */
	},
	 {
		 NULL
	}
};