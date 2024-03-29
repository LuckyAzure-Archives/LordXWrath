#ifndef PSXF_GUARD_CHARACTER_H
#define PSXF_GUARD_CHARACTER_H

#include "../../psx/io.h"
#include "../../psx/gfx.h"

#include "../../psx/fixed.h"
#include "animation.h"

//Character specs
typedef u8 CharSpec;
#define CHAR_SPEC_MISSANIM (1 << 0) //Has miss animations

//Character enums
typedef enum
{
	CharAnim_Idle,
	CharAnim_Left,  CharAnim_LeftAlt,
	CharAnim_Down,  CharAnim_DownAlt,
	CharAnim_Up,    CharAnim_UpAlt,
	CharAnim_Right, CharAnim_RightAlt,
	
	CharAnim_Max //Max standard/shared animation
} CharAnim;

//Character structures
typedef struct
{
	u8 tex;
	u16 src[4];
	s16 off[2];
} CharFrame;

typedef struct Character
{
	//Character functions
	void (*tick)(struct Character*);
	void (*set_anim)(struct Character*, u8);
	void (*free)(struct Character*);
	
	//Position
	fixed_t x, y;
	
	//Character information
	CharSpec spec;
	
	u16 health_i[2][4];
	u32 health_bar;
	fixed_t focus_x, focus_y, focus_zoom;
	
	fixed_t size;
	boolean flipped;
	
	//Animation state
	Animatable animatable;
	fixed_t sing_end;
	u16 pad_held;
} Character;

//Character functions
void Character_Free(Character *this);
void Character_Init(Character *this, fixed_t x, fixed_t y);
void Character_DrawParallax(Character *this, Gfx_Tex *tex, const CharFrame *cframe, fixed_t parallax);
void Character_Draw(Character *this, Gfx_Tex *tex, const CharFrame *cframe);

void Character_CheckStartSing(Character *this);
void Character_CheckEndSing(Character *this);
void Character_PerformIdle(Character *this);

#endif