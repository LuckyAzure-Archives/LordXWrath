#include "slaves.h"

#include "../../psx/mem.h"
#include "../../psx/archive.h"
#include "../../scenes/stage/stage.h"
#include "../../main.h"

Slaves slaves;

//Slaves character structure
enum
{
	Slaves_ArcMain_tails0,
	Slaves_ArcMain_tails1,
	Slaves_ArcMain_tails2,
	Slaves_ArcMain_tails3,
	Slaves_ArcMain_tails4,
	Slaves_ArcMain_tails5,
	Slaves_ArcMain_tails6,
	Slaves_ArcMain_knuck0,
	Slaves_ArcMain_knuck1,
	Slaves_ArcMain_knuck2,
	Slaves_ArcMain_knuck3,
	Slaves_ArcMain_knuck9,
	Slaves_ArcMain_knuck10,
	Slaves_ArcMain_knuck6,
	Slaves_ArcMain_knuck7,
	Slaves_ArcMain_knuck8,
	Slaves_ArcMain_knuck4,
	Slaves_ArcMain_knuck5,
	Slaves_ArcMain_eggman0,
	Slaves_ArcMain_eggman1,
	Slaves_ArcMain_eggman2,
	Slaves_ArcMain_eggman3,
	Slaves_ArcMain_eggman4,
	Slaves_ArcMain_eggman5,
	Slaves_ArcMain_eggman6,
	Slaves_ArcMain_eggman7,
	Slaves_ArcMain_eggman8,
	Slaves_ArcMain_eggman9,
	Slaves_ArcMain_eggman10,
	Slaves_ArcMain_interx0,
	Slaves_ArcMain_interx1,
	Slaves_ArcMain_interx2,
	Slaves_ArcMain_interx3,
	Slaves_ArcMain_interx4,
	Slaves_ArcMain_interx5,
	Slaves_ArcMain_interx6,
	Slaves_ArcMain_interx7,
	Slaves_ArcMain_interx8,
	Slaves_ArcMain_interx9,
	Slaves_ArcMain_interx10,
	
	Slaves_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Slaves_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_Slaves;

//Slaves character definitions
static const CharFrame char_slaves_frame[] = {
	{Slaves_ArcMain_tails0,{0,0,98,132},{38,126}}, //0
	{Slaves_ArcMain_tails1,{0,0,96,130},{36,122}}, //1
	{Slaves_ArcMain_tails0,{100,0,96,122},{36,118}}, //2
	{Slaves_ArcMain_tails1,{0,0,96,130},{36,122}}, //3
	{Slaves_ArcMain_tails0,{0,0,98,132},{38,126}}, //4
	
	{Slaves_ArcMain_tails1,{98,0,134,130},{85,123}}, //5
	{Slaves_ArcMain_tails2,{0,0,126,134},{77,127}}, //6
	
	{Slaves_ArcMain_tails2,{129,0,80,109},{37,105}}, //7
	{Slaves_ArcMain_tails3,{0,0,81,105},{37,101}}, //8
	
	{Slaves_ArcMain_tails3,{83,0,104,152},{54,144}}, //9
	{Slaves_ArcMain_tails4,{0,0,116,142},{58,134}}, //10
	
	{Slaves_ArcMain_tails5,{0,0,142,128},{59,122}}, //11
	{Slaves_ArcMain_tails6,{0,0,134,120},{59,113}}, //12
	
	{Slaves_ArcMain_knuck0,{0,0,131,157},{50,151}}, //13
	{Slaves_ArcMain_knuck1,{0,0,156,168},{67,162}}, //14
	{Slaves_ArcMain_knuck2,{0,0,158,169},{69,163}}, //15
	{Slaves_ArcMain_knuck3,{0,0,157,169},{68,163}}, //16
	
	{Slaves_ArcMain_knuck9,{0,0,172,174},{84,158}}, //17
	{Slaves_ArcMain_knuck10,{0,0,167,170},{76,159}}, //18
	
	{Slaves_ArcMain_knuck6,{0,0,98,137},{44,131}}, //19
	{Slaves_ArcMain_knuck6,{121,0,114,137},{42,133}}, //20
	
	{Slaves_ArcMain_knuck7,{0,0,117,168},{22,183}}, //21
	{Slaves_ArcMain_knuck8,{0,0,134,172},{42,173}}, //22
	
	{Slaves_ArcMain_knuck4,{0,0,156,156},{50,151}}, //23
	{Slaves_ArcMain_knuck5,{0,0,133,150},{36,145}}, //24
	
	{Slaves_ArcMain_eggman0,{0,0,125,168},{51,161}}, //25
	{Slaves_ArcMain_eggman1,{0,0,125,173},{52,166}}, //26
	{Slaves_ArcMain_eggman2,{0,0,126,176},{52,169}}, //27
	{Slaves_ArcMain_eggman3,{0,0,123,174},{54,167}}, //28
	
	{Slaves_ArcMain_eggman4,{0,0,244,164},{173,156}}, //29
	{Slaves_ArcMain_eggman5,{0,0,224,167},{153,158}}, //30
	
	{Slaves_ArcMain_eggman6,{0,0,112,140},{43,127}}, //31
	{Slaves_ArcMain_eggman6,{113,0,111,140},{48,127}}, //32
	
	{Slaves_ArcMain_eggman7,{0,0,234,233},{140,226}}, //33
	{Slaves_ArcMain_eggman8,{0,0,256,217},{146,211}}, //34
	
	{Slaves_ArcMain_eggman9,{0,0,156,156},{60,146}}, //35
	{Slaves_ArcMain_eggman10,{0,0,152,154},{60,144}}, //36
	
	{Slaves_ArcMain_interx0,{0,0,102,209},{54,199}}, //37
	{Slaves_ArcMain_interx0,{111,0,96,209},{49,199}}, //38
	{Slaves_ArcMain_interx1,{0,0,96,215},{49,205}}, //39
	{Slaves_ArcMain_interx1,{106,0,95,215},{49,205}}, //40
	{Slaves_ArcMain_interx2,{0,0,96,213},{49,205}}, //41
	{Slaves_ArcMain_interx2,{105,2,98,211},{50,204}}, //42
	
	{Slaves_ArcMain_interx3,{0,0,232,222},{134,209}}, //43
	{Slaves_ArcMain_interx4,{0,0,236,220},{132,206}}, //44
	
	{Slaves_ArcMain_interx5,{0,0,229,148},{135,133}}, //45
	{Slaves_ArcMain_interx6,{0,0,234,146},{145,139}}, //46
	
	{Slaves_ArcMain_interx7,{0,0,238,229},{107,223}}, //47
	{Slaves_ArcMain_interx8,{0,0,253,218},{115,212}}, //48
	
	{Slaves_ArcMain_interx9,{0,0,172,235},{60,210}}, //49
	{Slaves_ArcMain_interx10,{0,0,172,240},{61,215}}, //50
};

static const Animation char_tails_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 5,  6, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){ 7,  8, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){ 9, 10, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){11, 12, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

static const Animation char_knuckles_anim[CharAnim_Max] = {
	{2, (const u8[]){13, 14,  15,  16, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){17, 18, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){19, 20, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){21, 22, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){23, 24, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

static const Animation char_eggman_anim[CharAnim_Max] = {
	{2, (const u8[]){25, 26, 27, 28, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){29, 30, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){31, 32, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){33, 34, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){35, 36, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

static const Animation char_internalx_anim[CharAnim_Max] = {
	{2, (const u8[]){37, 38, 39, 40, 41, 42, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){43, 44, ASCR_BACK, 1}},         //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_LeftAlt
	{2, (const u8[]){45, 46, ASCR_BACK, 1}},         //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_DownAlt
	{2, (const u8[]){47, 48, ASCR_BACK, 1}},         //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_UpAlt
	{2, (const u8[]){49, 50, ASCR_BACK, 1}},         //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},   //CharAnim_RightAlt
};

//Slaves character functions
void Char_Slaves_SetFrame(void *user, u8 frame)
{
	Char_Slaves *this = (Char_Slaves*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_slaves_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_Slaves_Tick(Character *character)
{
	Char_Slaves *this = (Char_Slaves*)character;
	
	if (slaves.change)
	{
		slaves.change = false;
		switch(slaves.type)
		{
			case 0:
				Animatable_Init(&this->character.animatable, char_tails_anim);
				break;
			case 1:
				Animatable_Init(&this->character.animatable, char_knuckles_anim);
				break;
			case 2:
				Animatable_Init(&this->character.animatable, char_eggman_anim);
				break;
			case 3:
				Animatable_Init(&this->character.animatable, char_internalx_anim);
				break;
		}
	}
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_Slaves_SetFrame);
	Character_Draw(character, &this->tex, &char_slaves_frame[this->frame]);
}

void Char_Slaves_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_Slaves_Free(Character *character)
{
	Char_Slaves *this = (Char_Slaves*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_Slaves_New(fixed_t x, fixed_t y)
{
	//Allocate slaves object
	Char_Slaves *this = Mem_Alloc(sizeof(Char_Slaves));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_Slaves_New] Failed to allocate slaves object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_Slaves_Tick;
	this->character.set_anim = Char_Slaves_SetAnim;
	this->character.free = Char_Slaves_Free;
	
	slaves.type = 0;
	
	Animatable_Init(&this->character.animatable, char_tails_anim);
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
	this->character.focus_y = FIXED_DEC(-80,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	this->character.size = FIXED_DEC(4,4);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\SLAVES.ARC;1");
	
	const char **pathp = (const char *[]){
		"tails0.tim",
		"tails1.tim",
		"tails2.tim",
		"tails3.tim",
		"tails4.tim",
		"tails5.tim",
		"tails6.tim",
		"knuck0.tim",
		"knuck1.tim",
		"knuck2.tim",
		"knuck3.tim",
		"knuck9.tim",
		"knuck10.tim",
		"knuck6.tim",
		"knuck7.tim",
		"knuck8.tim",
		"knuck4.tim",
		"knuck5.tim",
		"eggman0.tim",
		"eggman1.tim",
		"eggman2.tim",
		"eggman3.tim",
		"eggman4.tim",
		"eggman5.tim",
		"eggman6.tim",
		"eggman7.tim",
		"eggman8.tim",
		"eggman9.tim",
		"eggman10.tim",
		"interx0.tim",
		"interx1.tim",
		"interx2.tim",
		"interx3.tim",
		"interx4.tim",
		"interx5.tim",
		"interx6.tim",
		"interx7.tim",
		"interx8.tim",
		"interx9.tim",
		"interx10.tim",
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
