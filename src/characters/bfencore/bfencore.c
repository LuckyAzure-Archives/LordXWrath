#include "bfencore.h"

#include "../../psx/mem.h"
#include "../../psx/archive.h"
#include "../../psx/random.h"
#include "../../scenes/stage/stage.h"
#include "../../main.h"

//BFEncore skull fragments
static SkullFragment char_bfencore_skull[15] = {
	{ 1 * 8, -87 * 8, -13, -13},
	{ 9 * 8, -88 * 8,   5, -22},
	{18 * 8, -87 * 8,   9, -22},
	{26 * 8, -85 * 8,  13, -13},
	
	{-3 * 8, -82 * 8, -13, -11},
	{ 8 * 8, -85 * 8,  -9, -15},
	{20 * 8, -82 * 8,   9, -15},
	{30 * 8, -79 * 8,  13, -11},
	
	{-1 * 8, -74 * 8, -13, -5},
	{ 8 * 8, -77 * 8,  -9, -9},
	{19 * 8, -75 * 8,   9, -9},
	{26 * 8, -74 * 8,  13, -5},
	
	{ 5 * 8, -73 * 8, -5, -3},
	{14 * 8, -76 * 8,  9, -6},
	{26 * 8, -67 * 8, 15, -3},
};

//BFEncore player types
enum
{
	BFEncore_ArcMain_bfe0,
	BFEncore_ArcMain_bfe1,
	BFEncore_ArcMain_bfe2,
	BFEncore_ArcMain_bfe3,
	BFEncore_ArcMain_bfe4,
	BFEncore_ArcMain_bfe5,
	BFEncore_ArcMain_bfe6,
	BFEncore_ArcMain_bfe7,
	BFEncore_ArcMain_bfe8,
	BFEncore_ArcMain_bfe9,
	BFEncore_ArcMain_bfe12,
	BFEncore_ArcMain_bfe10,
	BFEncore_ArcMain_bfe11,

	BFEncore_ArcMain_Max,
};

#define BFEncore_Arc_Max BFEncore_ArcMain_Max

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main, arc_dead;
	CdlFILE file_dead_arc; //dead.arc file position
	IO_Data arc_ptr[BFEncore_Arc_Max];
	
	Gfx_Tex tex, tex_retry;
	u8 frame, tex_id;
	
	u8 retry_bump;
	
	SkullFragment skull[COUNT_OF(char_bfencore_skull)];
	u8 skull_scale;
} Char_BFEncore;

//BFEncore player definitions
static const CharFrame char_bfencore_frame[] = {
	{BFEncore_ArcMain_bfe0,{0,0,96,134},{50,121}}, //0 Idle
	{BFEncore_ArcMain_bfe0,{97,0,96,134},{50,122}}, //1 Idle
	{BFEncore_ArcMain_bfe1,{0,0,94,134},{48,125}}, //2 Idle
	{BFEncore_ArcMain_bfe1,{95,0,94,137},{48,127}}, //3 Idle
	
	{BFEncore_ArcMain_bfe2,{0,0,113,136},{70,117}}, //4 Left
	{BFEncore_ArcMain_bfe2,{114,0,112,136},{68,118}}, //5 Left
	{BFEncore_ArcMain_bfe3,{0,0,112,138},{65,118}}, //6 Left
	
	{BFEncore_ArcMain_bfe3,{114,0,94,138},{50,124}}, //7 Down
	{BFEncore_ArcMain_bfe4,{0,0,92,142},{49,128}}, //8 Down
	{BFEncore_ArcMain_bfe4,{94,0,90,142},{48,130}}, //9 Down
	
	{BFEncore_ArcMain_bfe5,{0,0,120,150},{51,134}}, //10 Up
	{BFEncore_ArcMain_bfe5,{122,0,120,150},{51,133}}, //11 Up
	{BFEncore_ArcMain_bfe6,{0,0,122,146},{52,131}}, //12 Up
	
	{BFEncore_ArcMain_bfe6,{124,0,103,146},{44,124}}, //13 Right
	{BFEncore_ArcMain_bfe7,{0,0,104,134},{48,124}}, //14 Right
	{BFEncore_ArcMain_bfe7,{106,0,105,134},{51,123}}, //15 Right
	
	{BFEncore_ArcMain_bfe8,{0,0,111,125},{69,111}}, //16 Leftmiss
	{BFEncore_ArcMain_bfe8,{113,0,111,124},{68,110}}, //17 Leftmiss
	{BFEncore_ArcMain_bfe8,{0,127,111,124},{66,110}}, //18 Leftmiss
	
	{BFEncore_ArcMain_bfe8,{113,127,88,118},{45,110}}, //19 Downmiss
	{BFEncore_ArcMain_bfe9,{0,0,87,120},{45,112}}, //20 Downmiss
	{BFEncore_ArcMain_bfe9,{89,0,84,122},{44,114}}, //21 Downmiss
	
	{BFEncore_ArcMain_bfe12,{0,0,109,138},{45,126}}, //22 Upmiss
	{BFEncore_ArcMain_bfe10,{0,0,111,138},{46,124}}, //23 Upmiss
	{BFEncore_ArcMain_bfe10,{113,0,113,138},{47,122}}, //24 Upmiss
	
	{BFEncore_ArcMain_bfe11,{0,0,97,118},{41,114}}, //25 Rightmiss
	{BFEncore_ArcMain_bfe11,{99,0,96,118},{42,114}}, //26 Rightmiss
	{BFEncore_ArcMain_bfe11,{0,120,96,118},{43,114}}, //27 Rightmiss
	

};

static const Animation char_bfencore_anim[PlayerAnim_Max] = {
	{2, (const u8[]){ 0, 1, 2, 3, ASCR_BACK, 1}},		//CharAnim_Idle
	{2, (const u8[]){ 4, 5, 6, ASCR_BACK, 1}},		//CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_LeftAlt
	{2, (const u8[]){ 7, 8, 9, ASCR_BACK, 1}},		//CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_DownAlt
	{2, (const u8[]){ 10, 11, 12, ASCR_BACK, 1}},		//CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_UpAlt
	{2, (const u8[]){ 13, 14, 15, ASCR_BACK, 1}},		//CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_RightAlt
	{2, (const u8[]){ 16, 17, 18, ASCR_BACK, 1}},		//PlayerAnim_LeftMiss
	{2, (const u8[]){ 19, 20, 21, ASCR_BACK, 1}},		//PlayerAnim_DownMiss
	{2, (const u8[]){ 22, 23, 24, ASCR_BACK, 1}},		//PlayerAnim_UpMiss
	{2, (const u8[]){ 25, 26, 27, ASCR_BACK, 1}},		//PlayerAnim_RightMiss
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//PlayerAnim_Peace
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//PlayerAnim_Sweat
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//PlayerAnim_Dead0
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//PlayerAnim_Dead1
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//PlayerAnim_Dead2
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//PlayerAnim_Dead3
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//PlayerAnim_Dead4
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//PlayerAnim_Dead5
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//PlayerAnim_Dead6
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//PlayerAnim_Dead7

};

//BFEncore player functions
void Char_BFEncore_SetFrame(void *user, u8 frame)
{
	Char_BFEncore *this = (Char_BFEncore*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_bfencore_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_BFEncore_Tick(Character *character)
{
	Char_BFEncore *this = (Char_BFEncore*)character;
	
	//Handle animation updates
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0 ||
	    (character->animatable.anim != CharAnim_Left &&
	     character->animatable.anim != CharAnim_LeftAlt &&
	     character->animatable.anim != CharAnim_Down &&
	     character->animatable.anim != CharAnim_DownAlt &&
	     character->animatable.anim != CharAnim_Up &&
	     character->animatable.anim != CharAnim_UpAlt &&
	     character->animatable.anim != CharAnim_Right &&
	     character->animatable.anim != CharAnim_RightAlt))
		Character_CheckEndSing(character);
	
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		//Perform idle dance
		if (Animatable_Ended(&character->animatable) &&
			(character->animatable.anim != CharAnim_Left &&
		     character->animatable.anim != CharAnim_LeftAlt &&
		     character->animatable.anim != PlayerAnim_LeftMiss &&
		     character->animatable.anim != CharAnim_Down &&
		     character->animatable.anim != CharAnim_DownAlt &&
		     character->animatable.anim != PlayerAnim_DownMiss &&
		     character->animatable.anim != CharAnim_Up &&
		     character->animatable.anim != CharAnim_UpAlt &&
		     character->animatable.anim != PlayerAnim_UpMiss &&
		     character->animatable.anim != CharAnim_Right &&
		     character->animatable.anim != CharAnim_RightAlt &&
		     character->animatable.anim != PlayerAnim_RightMiss) &&
			(stage.song_step & 0x7) == 0)
			character->set_anim(character, CharAnim_Idle);
		
		//Stage specific animations
		if (stage.note_scroll >= 0)
		{
			switch (stage.stage_id)
			{
				default:
					break;
			}
		}
	}
	
	//Retry screen
	if (character->animatable.anim >= PlayerAnim_Dead3)
	{
		//Tick skull fragments
		if (this->skull_scale)
		{
			SkullFragment *frag = this->skull;
			for (size_t i = 0; i < COUNT_OF_MEMBER(Char_BFEncore, skull); i++, frag++)
			{
				//Draw fragment
				RECT frag_src = {
					(i & 1) ? 112 : 96,
					(i >> 1) << 4,
					16,
					16
				};
				fixed_t skull_dim = (FIXED_DEC(16,1) * this->skull_scale) >> 6;
				fixed_t skull_rad = skull_dim >> 1;
				RECT_FIXED frag_dst = {
					character->x + (((fixed_t)frag->x << FIXED_SHIFT) >> 3) - skull_rad - stage.camera.x,
					character->y + (((fixed_t)frag->y << FIXED_SHIFT) >> 3) - skull_rad - stage.camera.y,
					skull_dim,
					skull_dim,
				};
				Stage_DrawTex(&this->tex_retry, &frag_src, &frag_dst, FIXED_MUL(stage.camera.zoom, stage.bump));
				
				//Move fragment
				frag->x += frag->xsp;
				frag->y += ++frag->ysp;
			}
			
			//Decrease scale
			this->skull_scale--;
		}
		
		//Draw input options
		u8 input_scale = 16 - this->skull_scale;
		if (input_scale > 16)
			input_scale = 0;
		
		RECT button_src = {
			 0, 96,
			16, 16
		};
		RECT_FIXED button_dst = {
			character->x - FIXED_DEC(32,1) - stage.camera.x,
			character->y - FIXED_DEC(88,1) - stage.camera.y,
			(FIXED_DEC(16,1) * input_scale) >> 4,
			FIXED_DEC(16,1),
		};
		
		//Cross - Retry
		Stage_DrawTex(&this->tex_retry, &button_src, &button_dst, FIXED_MUL(stage.camera.zoom, stage.bump));
		
		//Circle - Blueball
		button_src.x = 16;
		button_dst.y += FIXED_DEC(56,1);
		Stage_DrawTex(&this->tex_retry, &button_src, &button_dst, FIXED_MUL(stage.camera.zoom, stage.bump));
		
		//Draw 'RETRY'
		u8 retry_frame;
		
		if (character->animatable.anim == PlayerAnim_Dead6)
		{
			//Selected retry
			retry_frame = 2 - (this->retry_bump >> 3);
			if (retry_frame >= 3)
				retry_frame = 0;
			if (this->retry_bump & 2)
				retry_frame += 3;
			
			if (++this->retry_bump == 0xFF)
				this->retry_bump = 0xFD;
		}
		else
		{
			//Idle
			retry_frame = 1 +  (this->retry_bump >> 2);
			if (retry_frame >= 3)
				retry_frame = 0;
			
			if (++this->retry_bump >= 55)
				this->retry_bump = 0;
		}
		
		RECT retry_src = {
			(retry_frame & 1) ? 48 : 0,
			(retry_frame >> 1) << 5,
			48,
			32
		};
		RECT_FIXED retry_dst = {
			character->x -  FIXED_DEC(7,1) - stage.camera.x,
			character->y - FIXED_DEC(92,1) - stage.camera.y,
			FIXED_DEC(48,1),
			FIXED_DEC(32,1),
		};
		Stage_DrawTex(&this->tex_retry, &retry_src, &retry_dst, FIXED_MUL(stage.camera.zoom, stage.bump));
	}
	
	//Animate and draw character
	Animatable_Animate(&character->animatable, (void*)this, Char_BFEncore_SetFrame);
	Character_Draw(character, &this->tex, &char_bfencore_frame[this->frame]);
}

void Char_BFEncore_SetAnim(Character *character, u8 anim)
{
	Char_BFEncore *this = (Char_BFEncore*)character;
	
	//Perform animation checks
	switch (anim)
	{
		case PlayerAnim_Dead0:
			character->focus_x = FIXED_DEC(0,1);
			character->focus_y = FIXED_DEC(-40,1);
			character->focus_zoom = FIXED_DEC(125,100);
			break;
		case PlayerAnim_Dead2:
			//Load retry art
			//Gfx_LoadTex(&this->tex_retry, this->arc_ptr[BFEncore_ArcDead_Retry], 0);
			break;
	}
	
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_BFEncore_Free(Character *character)
{
	Char_BFEncore *this = (Char_BFEncore*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_BFEncore_New(fixed_t x, fixed_t y)
{
	//Allocate boyfriend object
	Char_BFEncore *this = Mem_Alloc(sizeof(Char_BFEncore));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_BFEncore_New] Failed to allocate boyfriend object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_BFEncore_Tick;
	this->character.set_anim = Char_BFEncore_SetAnim;
	this->character.free = Char_BFEncore_Free;
	
	Animatable_Init(&this->character.animatable, char_bfencore_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = CHAR_SPEC_MISSANIM;
	
	this->character.health_i[0][0] = 0;
	this->character.health_i[0][1] = 0;
	this->character.health_i[0][2] = 46;
	this->character.health_i[0][3] = 30;
	
	this->character.health_i[1][0] = 47;
	this->character.health_i[1][1] = 0;
	this->character.health_i[1][2] = 43;
	this->character.health_i[1][3] = 35;
	
	//health bar color
	this->character.health_bar = 0xFF00A4E6;
	
	this->character.focus_x = FIXED_DEC(-30,1);
	this->character.focus_y = FIXED_DEC(-80,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	this->character.size = FIXED_DEC(90,100);
	this->character.flipped = false;
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\BFENCORE.ARC;1");
	this->arc_dead = NULL;
	//IO_FindFile(&this->file_dead_arc, "\\CHAR\\BFEncoreDEAD.ARC;1");
	
	const char **pathp = (const char *[]){
		"bfe0.tim",
		"bfe1.tim",
		"bfe2.tim",
		"bfe3.tim",
		"bfe4.tim",
		"bfe5.tim",
		"bfe6.tim",
		"bfe7.tim",
		"bfe8.tim",
		"bfe9.tim",
		"bfe12.tim",
		"bfe10.tim",
		"bfe11.tim",

		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	//Initialize player state
	this->retry_bump = 0;
	
	//Copy skull fragments
	memcpy(this->skull, char_bfencore_skull, sizeof(char_bfencore_skull));
	this->skull_scale = 64;
	
	SkullFragment *frag = this->skull;
	for (size_t i = 0; i < COUNT_OF_MEMBER(Char_BFEncore, skull); i++, frag++)
	{
		//Randomize trajectory
		frag->xsp += RandomRange(-4, 4);
		frag->ysp += RandomRange(-2, 2);
	}
	
	return (Character*)this;
}
