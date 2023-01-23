#ifndef PSXF_GUARD_BFPOV_H
#define PSXF_GUARD_BFPOV_H

#include "../../scenes/stage/player.h"

//Boyfriend player functions
Character *Char_BFPOV_New(fixed_t x, fixed_t y);

typedef struct
{
	u8 type;
	boolean change;
} BFPOV;

extern BFPOV bfpov;

#endif