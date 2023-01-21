#ifndef PSXF_GUARD_SLAVES_H
#define PSXF_GUARD_SLAVES_H

#include "../../scenes/stage/character.h"

//LordX character functions
Character *Char_Slaves_New(fixed_t x, fixed_t y);

typedef struct
{
	u8 type;
	boolean change;
} Slaves;

extern Slaves slaves;

#endif