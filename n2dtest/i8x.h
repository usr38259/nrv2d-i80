#pragma once

#ifndef __I8X_H__
#define __I8X_H__

#include "emux.h"

struct i8085_state
{
	int                 cputype;        /* 0 8080, 1 8085A */
	PAIR                PC,SP,AF,BC,DE,HL,WZ;
	UINT8               HALT;
	UINT8               IM;             /* interrupt mask (8085A only) */
//	UINT8               STATUS;         /* status word */

//	UINT8               after_ei;       /* post-EI processing; starts at 2, check for ints at 0 */
//	UINT8               nmi_state;      /* raw NMI line state */
//	UINT8               irq_state[4];   /* raw IRQ line states */
//	UINT8               trap_pending;   /* TRAP interrupt latched? */
//	UINT8               trap_im_copy;   /* copy of IM register when TRAP was taken */
//	UINT8               sod_state;      /* state of the SOD line */

//	UINT8               ietemp;         /* import/export temp space */

//	legacy_cpu_device *device;
//	address_space *program;
//	direct_read_data *direct;
//	address_space *io;
	int                 icount;
};

i8080_cpu_init (struct i8085_state *cpustate);
i8085_cpu_init (struct i8085_state *cpustate);
i808x_cpu_reset (struct i8085_state *cpustate);
i808x_cpu_execute (struct i8085_state *cpustate);

#endif /* __I8X_H__ */
