#include "lordx.h"

#include "../../psx/mem.h"
#include "../../psx/archive.h"
#include "../../scenes/stage/stage.h"
#include "../../main.h"

//LordX character structure
enum
{
	LordX_ArcMain_idle0,
	LordX_ArcMain_idle1,
	LordX_ArcMain_idle2,
	LordX_ArcMain_idle3,
	LordX_ArcMain_idle4,
	LordX_ArcMain_idle5,
	LordX_ArcMain_idle6,
	LordX_ArcMain_left0,
	LordX_ArcMain_left1,
	LordX_ArcMain_left2,
	LordX_ArcMain_left3,
	LordX_ArcMain_left4,
	LordX_ArcMain_down0,
	LordX_ArcMain_down1,
	LordX_ArcMain_down2,
	LordX_ArcMain_down3,
	LordX_ArcMain_down4,
	LordX_ArcMain_up0,
	LordX_ArcMain_up1,
	LordX_ArcMain_up2,
	LordX_ArcMain_up3,
	LordX_ArcMain_up4,
	LordX_ArcMain_right0,
	LordX_ArcMain_right1,
	LordX_ArcMain_right2,
	LordX_ArcMain_right3,
	LordX_ArcMain_right4,
	
	LordX_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[LordX_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_LordX;

//LordX character definitions
static const CharFrame char_lordx_frame[] = {
	{LordX_ArcMain_idle0,{0,0,121,227},{66,223}}, //0
	{LordX_ArcMain_idle1,{0,0,119,221},{60,217}}, //1
	{LordX_ArcMain_idle2,{0,0,122,220},{61,215}}, //2
	{LordX_ArcMain_idle3,{0,0,124,220},{62,215}}, //3
	{LordX_ArcMain_idle4,{0,0,115,224},{60,219}}, //4
	{LordX_ArcMain_idle5,{0,0,118,228},{63,223}}, //5
	{LordX_ArcMain_idle6,{0,0,120,228},{67,224}}, //6
	
	{LordX_ArcMain_left0,{0,0,211,218},{134,202}}, //7
	{LordX_ArcMain_left1,{0,0,214,222},{135,206}}, //8
	{LordX_ArcMain_left2,{0,0,206,212},{128,196}}, //9
	{LordX_ArcMain_left3,{0,0,214,201},{135,185}}, //10
	{LordX_ArcMain_left4,{0,0,207,204},{127,188}}, //11
	
	{LordX_ArcMain_down0,{0,0,185,194},{82,182}}, //12
	{LordX_ArcMain_down1,{0,0,182,193},{78,181}}, //13
	{LordX_ArcMain_down2,{0,0,186,194},{81,182}}, //14
	{LordX_ArcMain_down3,{0,0,202,193},{96,181}}, //15
	{LordX_ArcMain_down4,{0,0,201,194},{95,181}}, //16
	
	{LordX_ArcMain_up0,{0,0,147,236},{62,230}}, //17
	{LordX_ArcMain_up1,{0,0,154,238},{62,230}}, //18
	{LordX_ArcMain_up2,{0,0,157,234},{62,227}}, //19
	{LordX_ArcMain_up3,{0,0,161,230},{62,223}}, //20
	{LordX_ArcMain_up4,{0,0,159,231},{62,224}}, //21
	
	{LordX_ArcMain_right0,{0,0,232,196},{63,190}}, //22
	{LordX_ArcMain_right1,{0,0,232,196},{62,190}}, //23
	{LordX_ArcMain_right2,{0,0,231,195},{63,189}}, //24
	{LordX_ArcMain_right3,{0,0,226,197},{63,190}}, //25
	{LordX_ArcMain_right4,{0,0,228,196},{63,190}}, //26
};

static const Animation char_lordx_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4,  5,  6, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 7,  8,  9, 10, 11, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){12, 13, 14, 15, 16, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){17, 18, 19, 20, 21, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){22, 23, 24, 25, 26, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//LordX character functions
void Char_LordX_SetFrame(void *user, u8 frame)
{
	Char_LordX *this = (Char_LordX*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_lordx_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_LordX_Tick(Character *character)
{
	Char_LordX *this = (Char_LordX*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_LordX_SetFrame);
	Character_Draw(character, &this->tex, &char_lordx_frame[this->frame]);
}

void Char_LordX_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_LordX_Free(Character *character)
{
	Char_LordX *this = (Char_LordX*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_LordX_New(fixed_t x, fixed_t y)
{
	//Allocate lordx object
	Char_LordX *this = Mem_Alloc(sizeof(Char_LordX));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_LordX_New] Failed to allocate lordx object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_LordX_Tick;
	this->character.set_anim = Char_LordX_SetAnim;
	this->character.free = Char_LordX_Free;
	
	Animatable_Init(&this->character.animatable, char_lordx_anim);
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
	this->arc_main = IO_Read("\\CHAR\\LORDX.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim",
		"idle1.tim",
		"idle2.tim",
		"idle3.tim",
		"idle4.tim",
		"idle5.tim",
		"idle6.tim",
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
