#include "lordxc.h"

#include "../../psx/mem.h"
#include "../../psx/archive.h"
#include "../../scenes/stage/stage.h"
#include "../../main.h"

//LordXC character structure
enum
{
  LordXC_ArcMain_idle0,
  LordXC_ArcMain_idle1,
  LordXC_ArcMain_idle2,
  LordXC_ArcMain_idle3,
  LordXC_ArcMain_idle4,
  LordXC_ArcMain_left0,
  LordXC_ArcMain_left1,
  LordXC_ArcMain_left2,
  LordXC_ArcMain_left3,
  LordXC_ArcMain_left4,
  LordXC_ArcMain_down0,
  LordXC_ArcMain_down1,
  LordXC_ArcMain_down2,
  LordXC_ArcMain_down3,
  LordXC_ArcMain_down4,
  LordXC_ArcMain_up0,
  LordXC_ArcMain_up1,
  LordXC_ArcMain_up2,
  LordXC_ArcMain_up3,
  LordXC_ArcMain_up4,
  LordXC_ArcMain_right0,
  LordXC_ArcMain_right1,
  LordXC_ArcMain_right2,
  LordXC_ArcMain_right3,
  LordXC_ArcMain_right4,
	
	LordXC_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[LordXC_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_LordXC;

//LordXC character definitions
static const CharFrame char_lordxc_frame[] = {
	{LordXC_ArcMain_idle0,{0,0,154,170},{62,155}},
	{LordXC_ArcMain_idle1,{0,0,152,170},{62,155}},
	{LordXC_ArcMain_idle2,{0,0,152,170},{63,155}},
	{LordXC_ArcMain_idle3,{0,0,146,172},{64,156}},
	{LordXC_ArcMain_idle4,{0,0,146,172},{64,156}},
	
	{LordXC_ArcMain_left0,{0,0,162,174},{91,162}},
	{LordXC_ArcMain_left1,{0,0,156,168},{70,156}},
	{LordXC_ArcMain_left2,{0,0,154,166},{66,155}},
	{LordXC_ArcMain_left3,{0,0,156,168},{65,155}},
	{LordXC_ArcMain_left4,{0,0,156,166},{66,155}},
	
	{LordXC_ArcMain_down0,{0,0,142,142},{68,134}},
	{LordXC_ArcMain_down1,{0,0,138,150},{65,142}},
	{LordXC_ArcMain_down2,{0,0,136,152},{64,142}},
	{LordXC_ArcMain_down3,{0,0,136,152},{64,142}},
	{LordXC_ArcMain_down4,{0,0,134,150},{63,142}},
	
	{LordXC_ArcMain_up0,{0,0,156,190},{80,173}},
	{LordXC_ArcMain_up1,{0,0,156,182},{78,166}},
	{LordXC_ArcMain_up2,{0,0,158,180},{80,163}},
	{LordXC_ArcMain_up3,{0,0,158,178},{80,162}},
	{LordXC_ArcMain_up4,{0,0,158,180},{80,162}},
	
	{LordXC_ArcMain_right0,{0,0,154,160},{65,146}},
	{LordXC_ArcMain_right1,{0,0,160,160},{71,146}},
	{LordXC_ArcMain_right2,{0,0,166,160},{73,146}},
	{LordXC_ArcMain_right3,{0,0,168,160},{75,146}},
	{LordXC_ArcMain_right4,{0,0,168,160},{75,145}},
};

static const Animation char_lordxc_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4, ASCR_BACK, 1}}, //CharAnim_Idle
	{1, (const u8[]){ 5,  6,  7,  8,  9, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{1, (const u8[]){10, 11, 12, 13, 14, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{1, (const u8[]){15, 16, 17, 18, 19, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{1, (const u8[]){20, 21, 22, 23, 24, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//LordXC character functions
void Char_LordXC_SetFrame(void *user, u8 frame)
{
	Char_LordXC *this = (Char_LordXC*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_lordxc_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_LordXC_Tick(Character *character)
{
	Char_LordXC *this = (Char_LordXC*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_LordXC_SetFrame);
	Character_Draw(character, &this->tex, &char_lordxc_frame[this->frame]);
}

void Char_LordXC_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_LordXC_Free(Character *character)
{
	Char_LordXC *this = (Char_LordXC*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_LordXC_New(fixed_t x, fixed_t y)
{
	//Allocate lordxc object
	Char_LordXC *this = Mem_Alloc(sizeof(Char_LordXC));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_LordXC_New] Failed to allocate lordxc object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_LordXC_Tick;
	this->character.set_anim = Char_LordXC_SetAnim;
	this->character.free = Char_LordXC_Free;
	
	Animatable_Init(&this->character.animatable, char_lordxc_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i[0][0] = 0;
	this->character.health_i[0][1] = 0;
	this->character.health_i[0][2] = 46;
	this->character.health_i[0][3] = 30;
	
	this->character.health_i[1][0] = 47;
	this->character.health_i[1][1] = 0;
	this->character.health_i[1][2] = 43;
	this->character.health_i[1][3] = 35;
	
	//health bar color
	this->character.health_bar = 0xFFA5004A;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	this->character.size = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\LORDXC.ARC;1");
	
	const char **pathp = (const char *[]){
  "idle0.tim",
  "idle1.tim",
  "idle2.tim",
  "idle3.tim",
  "idle4.tim",
  "left0.tim",
  "left1.tim",
  "left2.tim",
  "left3.tim",
  "left4.tim",
  "down0.tim",
  "down1.tim",
  "down2.tim",
  "down3.tim",
  "down4.tim",
  "up0.tim",
  "up1.tim",
  "up2.tim",
  "up3.tim",
  "up4.tim",
  "right0.tim",
  "right1.tim",
  "right2.tim",
  "right3.tim",
  "right4.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
