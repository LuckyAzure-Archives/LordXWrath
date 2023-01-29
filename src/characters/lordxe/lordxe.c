#include "lordxe.h"

#include "../../psx/mem.h"
#include "../../psx/archive.h"
#include "../../scenes/stage/stage.h"
#include "../../main.h"

//LordXE character structure
enum
{
	LordXE_ArcMain_idle0,
	LordXE_ArcMain_idle1,
	LordXE_ArcMain_idle2,
	LordXE_ArcMain_idle3,
	LordXE_ArcMain_idle4,
	LordXE_ArcMain_idle5,
	LordXE_ArcMain_left0,
	LordXE_ArcMain_left1,
	LordXE_ArcMain_left2,
	LordXE_ArcMain_left3,
	LordXE_ArcMain_left4,
	LordXE_ArcMain_down0,
	LordXE_ArcMain_down1,
	LordXE_ArcMain_down2,
	LordXE_ArcMain_down3,
	LordXE_ArcMain_down4,
	LordXE_ArcMain_up0,
	LordXE_ArcMain_up1,
	LordXE_ArcMain_up2,
	LordXE_ArcMain_up3,
	LordXE_ArcMain_up4,
	LordXE_ArcMain_right0,
	LordXE_ArcMain_right1,
	LordXE_ArcMain_right2,
	LordXE_ArcMain_right3,
	LordXE_ArcMain_right4,

	LordXE_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[LordXE_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_LordXE;

//LordXE character definitions
static const CharFrame char_lordxe_frame[] = {
	{LordXE_ArcMain_idle0,{0,0,120,174},{56,157}}, //0 Idle
	{LordXE_ArcMain_idle0,{121,0,120,174},{56,156}}, //1 Idle
	{LordXE_ArcMain_idle1,{0,0,122,186},{57,169}}, //2 Idle
	{LordXE_ArcMain_idle2,{0,0,132,194},{66,180}}, //3 Idle
	{LordXE_ArcMain_idle3,{0,0,134,194},{68,180}}, //4 Idle
	{LordXE_ArcMain_idle4,{0,0,134,194},{68,180}}, //5 Idle
	{LordXE_ArcMain_idle5,{0,0,132,188},{67,175}}, //6 Idle
	{LordXE_ArcMain_idle5,{0,0,132,188},{67,175}}, //7 Idle
	{LordXE_ArcMain_idle5,{0,0,132,188},{67,175}}, //8 Idle
	
	{LordXE_ArcMain_left0,{0,0,246,194},{187,182}}, //9 Left
	{LordXE_ArcMain_left1,{0,0,230,190},{171,178}}, //10 Left
	{LordXE_ArcMain_left2,{0,0,224,188},{165,176}}, //11 Left
	{LordXE_ArcMain_left3,{0,0,222,188},{162,176}}, //12 Left
	{LordXE_ArcMain_left4,{0,0,222,188},{162,176}}, //13 Left
	
	{LordXE_ArcMain_down0,{0,0,138,166},{68,151}}, //14 Down
	{LordXE_ArcMain_down1,{0,0,136,180},{67,165}}, //15 Down
	{LordXE_ArcMain_down2,{0,0,134,186},{67,171}}, //16 Down
	{LordXE_ArcMain_down3,{0,0,134,188},{67,172}}, //17 Down
	{LordXE_ArcMain_down4,{0,0,134,186},{67,170}}, //18 Down
	
	{LordXE_ArcMain_up0,{0,0,130,256},{75,236}}, //19 Up
	{LordXE_ArcMain_up1,{0,0,126,234},{71,216}}, //20 Up
	{LordXE_ArcMain_up2,{0,0,130,228},{73,210}}, //21 Up
	{LordXE_ArcMain_up3,{0,0,132,228},{73,210}}, //22 Up
	{LordXE_ArcMain_up4,{0,0,132,228},{73,210}}, //23 Up
	
	{LordXE_ArcMain_right0,{0,0,184,234},{59,219}}, //24 Right
	{LordXE_ArcMain_right1,{0,0,196,216},{88,200}}, //25 Right
	{LordXE_ArcMain_right2,{0,0,196,216},{94,201}}, //26 Right
	{LordXE_ArcMain_right3,{0,0,196,216},{96,201}}, //27 Right
	{LordXE_ArcMain_right4,{0,0,196,216},{96,201}}, //28 Right
	

};

static const Animation char_lordxe_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, 4, 5, 6, 7, 8, ASCR_BACK, 1}},		//CharAnim_Idle
	{1, (const u8[]){ 9, 10, 11, 12, 13, ASCR_BACK, 1}},		//CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_LeftAlt
	{1, (const u8[]){ 14, 15, 16, 17, 18, ASCR_BACK, 1}},		//CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_DownAlt
	{1, (const u8[]){ 19, 20, 21, 22, 23, ASCR_BACK, 1}},		//CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_UpAlt
	{1, (const u8[]){ 24, 25, 26, 27, 28, ASCR_BACK, 1}},		//CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_RightAlt

};

//LordXE character functions
void Char_LordXE_SetFrame(void *user, u8 frame)
{
	Char_LordXE *this = (Char_LordXE*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_lordxe_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_LordXE_Tick(Character *character)
{
	Char_LordXE *this = (Char_LordXE*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_LordXE_SetFrame);
	Character_Draw(character, &this->tex, &char_lordxe_frame[this->frame]);
}

void Char_LordXE_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_LordXE_Free(Character *character)
{
	Char_LordXE *this = (Char_LordXE*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_LordXE_New(fixed_t x, fixed_t y)
{
	//Allocate lordxe object
	Char_LordXE *this = Mem_Alloc(sizeof(Char_LordXE));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_LordXE_New] Failed to allocate lordxe object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_LordXE_Tick;
	this->character.set_anim = Char_LordXE_SetAnim;
	this->character.free = Char_LordXE_Free;
	
	Animatable_Init(&this->character.animatable, char_lordxe_anim);
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
	this->character.health_bar = 0xFF113333;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	this->character.size = FIXED_DEC(120,100);
	this->character.flipped = false;
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\LORDXE.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim",
		"idle1.tim",
		"idle2.tim",
		"idle3.tim",
		"idle4.tim",
		"idle5.tim",
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
