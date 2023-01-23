#include "bfpov.h"

#include "../../psx/mem.h"
#include "../../psx/archive.h"
#include "../../psx/random.h"
#include "../../scenes/stage/stage.h"
#include "../../main.h"

BFPOV bfpov;

//Boyfriend skull fragments
static SkullFragment char_bfpov_skull[15] = {
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

//Boyfriend player types
enum
{
	BF_ArcMain_bf0,
	BF_ArcMain_bf1,
	BF_ArcMain_bf2,
	BF_ArcMain_bf3,
	BF_ArcMain_bf4,
	BF_ArcMain_bf5,
	BF_ArcMain_bf6,
	BF_ArcMain_bf7,
	BF_ArcMain_dead0,
	BF_ArcMain_dead1,
	BF_ArcMain_dead2,
	BF_ArcMain_dead3,
	BF_ArcMain_dead4,
	BF_ArcMain_dead5,
	BF_ArcMain_dead6,
	
	BFPOV_ArcMain_bfpov0,
	BFPOV_ArcMain_bfpov1,
	BFPOV_ArcMain_bfpov2,
	BFPOV_ArcMain_bfpov3,
	BFPOV_ArcMain_bfpov5,
	BFPOV_ArcMain_bfpov4,
	
	BF_ArcMain_Max,
};

#define BF_Arc_Max BF_ArcMain_Max

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main, arc_dead;
	CdlFILE file_dead_arc; //dead.arc file position
	IO_Data arc_ptr[BF_Arc_Max];
	
	Gfx_Tex tex, tex_retry;
	u8 frame, tex_id;
	
	u8 retry_bump;
	
	SkullFragment skull[COUNT_OF(char_bfpov_skull)];
	u8 skull_scale;
} Char_BFPOV;

//Boyfriend player definitions
static const CharFrame char_bfpov_frame[] = {
	{BF_ArcMain_bf0,{0,0,85,85},{45,72}}, //0
	{BF_ArcMain_bf0,{90,0,85,86},{45,74}}, //1
	{BF_ArcMain_bf0,{0,92,85,87},{45,74}}, //2
	{BF_ArcMain_bf0,{86,90,84,90},{45,77}}, //3
	{BF_ArcMain_bf1,{0,0,84,88},{44,76}}, //4
	
	{BF_ArcMain_bf1,{90,0,87,93},{46,78}}, //5
	{BF_ArcMain_bf1,{2,96,87,91},{46,77}}, //6
	{BF_ArcMain_bf1,{93,95,86,90},{46,76}}, //7
	
	{BF_ArcMain_bf2,{4,0,84,95},{44,81}}, //8
	{BF_ArcMain_bf2,{94,0,84,93},{44,79}}, //9
	{BF_ArcMain_bf2,{0,96,84,93},{43,79}}, //10
	
	{BF_ArcMain_bf2,{87,103,88,85},{46,72}}, //11
	{BF_ArcMain_bf3,{0,3,84,87},{44,74}}, //12
	{BF_ArcMain_bf3,{86,0,83,90},{43,77}}, //13
	
	{BF_ArcMain_bf3,{1,99,88,84},{46,72}}, //14
	{BF_ArcMain_bf3,{93,95,84,87},{44,74}}, //15
	{BF_ArcMain_bf4,{2,0,83,90},{43,77}}, //16
	
	{BF_ArcMain_bf4,{87,3,78,96},{42,82}}, //17
	{BF_ArcMain_bf4,{166,2,80,93},{43,79}}, //18
	{BF_ArcMain_bf4,{0,102,82,89},{44,75}}, //19
	
	{BF_ArcMain_bf4,{87,103,79,97},{42,82}}, //20
	{BF_ArcMain_bf4,{170,102,80,93},{43,79}}, //21
	{BF_ArcMain_bf5,{0,0,82,91},{44,77}}, //22
	
	{BF_ArcMain_bf5,{87,4,86,90},{45,76}}, //23
	{BF_ArcMain_bf5,{0,98,86,91},{45,76}}, //24
	{BF_ArcMain_bf5,{91,95,86,91},{45,76}}, //25
	
	{BF_ArcMain_bf6,{1,3,84,91},{45,78}}, //26
	{BF_ArcMain_bf6,{95,0,84,93},{45,79}}, //27
	{BF_ArcMain_bf6,{0,96,84,92},{45,79}}, //28
	
	{BF_ArcMain_bf6,{87,101,107,64},{53,57}}, //29
	{BF_ArcMain_bf7,{0,3,109,65},{53,57}}, //30
	{BF_ArcMain_bf7,{112,0,110,67},{53,59}}, //31
	
	{BF_ArcMain_dead0,{0,0,238,170},{120,151}}, //32
	{BF_ArcMain_dead1,{0,0,238,170},{120,151}}, //33
	{BF_ArcMain_dead2,{0,0,238,170},{120,151}}, //34
	{BF_ArcMain_dead3,{0,0,238,170},{120,151}}, //35
	
	{BF_ArcMain_dead0,{0,0,238,170},{120,151}}, //36
	{BF_ArcMain_dead4,{0,0,238,176},{120,156}}, //37
	{BF_ArcMain_dead5,{0,0,238,176},{120,157}}, //38
	{BF_ArcMain_dead6,{0,0,238,176},{120,157}}, //39
	
	{BFPOV_ArcMain_bfpov0,{0,0,100,97},{54,79}}, //40
	{BFPOV_ArcMain_bfpov0,{102,0,99,96},{54,78}}, //41
	{BFPOV_ArcMain_bfpov0,{0,99,99,98},{52,80}}, //42
	{BFPOV_ArcMain_bfpov0,{101,99,98,98},{50,80}}, //43
	{BFPOV_ArcMain_bfpov1,{0,0,98,98},{50,80}}, //44
	{BFPOV_ArcMain_bfpov1,{100,0,98,98},{50,80}}, //45
	
	{BFPOV_ArcMain_bfpov1,{0,100,95,96},{65,79}}, //46
	{BFPOV_ArcMain_bfpov1,{97,100,97,96},{63,79}}, //47
	
	{BFPOV_ArcMain_bfpov2,{0,0,99,93},{58,76}}, //48
	{BFPOV_ArcMain_bfpov2,{100,0,101,96},{60,79}}, //49
	
	{BFPOV_ArcMain_bfpov2,{0,98,91,110},{40,92}}, //50
	{BFPOV_ArcMain_bfpov2,{93,98,92,108},{41,90}}, //51
	
	{BFPOV_ArcMain_bfpov3,{0,0,103,101},{44,84}}, //52
	{BFPOV_ArcMain_bfpov3,{105,0,99,99},{47,82}}, //53
	
	{BFPOV_ArcMain_bfpov5,{0,120,97,113},{67,96}}, //54
	{BFPOV_ArcMain_bfpov5,{100,120,97,113},{62,96}}, //55
	
	{BFPOV_ArcMain_bfpov4,{0,0,105,98},{63,82}}, //56
	{BFPOV_ArcMain_bfpov4,{107,0,99,99},{59,82}}, //57
	
	{BFPOV_ArcMain_bfpov4,{0,101,88,128},{39,109}}, //58
	{BFPOV_ArcMain_bfpov4,{90,101,91,128},{40,109}}, //59
	
	{BFPOV_ArcMain_bfpov5,{0,0,103,119},{41,102}}, //60
	{BFPOV_ArcMain_bfpov5,{105,0,103,119},{45,101}}, //61
};

static const Animation char_bf_anim[PlayerAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3,  4, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 5,  6,  7, ASCR_BACK, 1}},             //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_LeftAlt
	{2, (const u8[]){11, 12, 13, ASCR_BACK, 1}},             //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_DownAlt
	{2, (const u8[]){17, 18, 19, ASCR_BACK, 1}},             //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UpAlt
	{2, (const u8[]){23, 24, 25, ASCR_BACK, 1}},             //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_RightAlt
	
	{1, (const u8[]){  8,  9, 10, ASCR_BACK, 1}},     //PlayerAnim_LeftMiss
	{1, (const u8[]){ 14, 15, 16, ASCR_BACK, 1}},     //PlayerAnim_DownMiss
	{1, (const u8[]){ 20, 21, 22, ASCR_BACK, 1}},     //PlayerAnim_UpMiss
	{1, (const u8[]){ 26, 27, 28, ASCR_BACK, 1}},     //PlayerAnim_RightMiss
	
	{2, (const u8[]){13, 14, 15, ASCR_BACK, 1}},         //PlayerAnim_Peace
	{2, (const u8[]){16, 17, 18, 19, ASCR_REPEAT}},      //PlayerAnim_Sweat
	
	{3, (const u8[]){32, 33, 34, 35, 32, 33, 34, 35, ASCR_CHGANI, PlayerAnim_Dead1}}, //PlayerAnim_Dead0
	{3, (const u8[]){32, ASCR_REPEAT}},                                                       //PlayerAnim_Dead1
	{3, (const u8[]){33, 34, 35, ASCR_CHGANI, PlayerAnim_Dead3}}, //PlayerAnim_Dead2
	{2, (const u8[]){32, ASCR_REPEAT}},                                                       //PlayerAnim_Dead3
	{3, (const u8[]){32, 33, 34, 35, ASCR_CHGANI, PlayerAnim_Dead3}},             //PlayerAnim_Dead4
	{3, (const u8[]){32, 33, 34, 35, ASCR_CHGANI, PlayerAnim_Dead3}},             //PlayerAnim_Dead5
	
	{10, (const u8[]){32, 33, 34, 35, ASCR_BACK, 1}}, //PlayerAnim_Dead4
	{ 3, (const u8[]){32, 33, 34, 35, ASCR_REPEAT}},  //PlayerAnim_Dead5
};

static const Animation char_bfpov_anim[PlayerAnim_Max] = {
	{2, (const u8[]){40, 41, 42, 43, 44, 45, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){46, 47, ASCR_BACK, 1}},             //CharAnim_Left
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_LeftAlt
	{2, (const u8[]){48, 49, ASCR_BACK, 1}},             //CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_DownAlt
	{2, (const u8[]){50, 51, ASCR_BACK, 1}},             //CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_UpAlt
	{2, (const u8[]){52, 53, ASCR_BACK, 1}},             //CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},       //CharAnim_RightAlt
	
	{1, (const u8[]){ 54, 55, ASCR_BACK, 1}},     //PlayerAnim_LeftMiss
	{1, (const u8[]){ 56, 57, ASCR_BACK, 1}},     //PlayerAnim_DownMiss
	{1, (const u8[]){ 58, 59, ASCR_BACK, 1}},     //PlayerAnim_UpMiss
	{1, (const u8[]){ 60, 61, ASCR_BACK, 1}},     //PlayerAnim_RightMiss
	
	{2, (const u8[]){13, 14, 15, ASCR_BACK, 1}},         //PlayerAnim_Peace
	{2, (const u8[]){16, 17, 18, 19, ASCR_REPEAT}},      //PlayerAnim_Sweat
	
	{3, (const u8[]){32, 33, 34, 35, 32, 33, 34, 35, ASCR_CHGANI, PlayerAnim_Dead1}}, //PlayerAnim_Dead0
	{3, (const u8[]){32, ASCR_REPEAT}},                                                       //PlayerAnim_Dead1
	{3, (const u8[]){33, 34, 35, ASCR_CHGANI, PlayerAnim_Dead3}}, //PlayerAnim_Dead2
	{2, (const u8[]){32, ASCR_REPEAT}},                                                       //PlayerAnim_Dead3
	{3, (const u8[]){32, 33, 34, 35, ASCR_CHGANI, PlayerAnim_Dead3}},             //PlayerAnim_Dead4
	{3, (const u8[]){32, 33, 34, 35, ASCR_CHGANI, PlayerAnim_Dead3}},             //PlayerAnim_Dead5
	
	{10, (const u8[]){32, 33, 34, 35, ASCR_BACK, 1}}, //PlayerAnim_Dead4
	{ 3, (const u8[]){32, 33, 34, 35, ASCR_REPEAT}},  //PlayerAnim_Dead5
};

//Boyfriend player functions
void Char_BFPOV_SetFrame(void *user, u8 frame)
{
	Char_BFPOV *this = (Char_BFPOV*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_bfpov_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_BFPOV_Tick(Character *character)
{
	Char_BFPOV *this = (Char_BFPOV*)character;
	
	if (bfpov.change)
	{
		bfpov.change = false;
		switch(bfpov.type)
		{
			case 0:
				Animatable_Init(&this->character.animatable, char_bf_anim);
				break;
			case 1:
				Animatable_Init(&this->character.animatable, char_bfpov_anim);
				break;
		}
		Animatable_SetAnim(&character->animatable, CharAnim_Idle);
	}
	
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
			for (size_t i = 0; i < COUNT_OF_MEMBER(Char_BFPOV, skull); i++, frag++)
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
	Animatable_Animate(&character->animatable, (void*)this, Char_BFPOV_SetFrame);
	Character_Draw(character, &this->tex, &char_bfpov_frame[this->frame]);
}

void Char_BFPOV_SetAnim(Character *character, u8 anim)
{
	Char_BFPOV *this = (Char_BFPOV*)character;
	
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
			//Gfx_LoadTex(&this->tex_retry, this->arc_ptr[BFPOV_ArcDead_Retry], 0);
			break;
	}
	
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_BFPOV_Free(Character *character)
{
	Char_BFPOV *this = (Char_BFPOV*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_BFPOV_New(fixed_t x, fixed_t y)
{
	//Allocate boyfriend object
	Char_BFPOV *this = Mem_Alloc(sizeof(Char_BFPOV));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_BFPOV_New] Failed to allocate boyfriend object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_BFPOV_Tick;
	this->character.set_anim = Char_BFPOV_SetAnim;
	this->character.free = Char_BFPOV_Free;
	
	Animatable_Init(&this->character.animatable, char_bf_anim);
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
	this->character.health_bar = 0xFF29B5D6;
	
	this->character.focus_x = FIXED_DEC(-30,1);
	this->character.focus_y = FIXED_DEC(-80,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	this->character.size = FIXED_DEC(1,1);
	this->character.flipped = false;
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\BFPOV.ARC;1");
	this->arc_dead = NULL;
	//IO_FindFile(&this->file_dead_arc, "\\CHAR\\BFPOVDEAD.ARC;1");
	
	const char **pathp = (const char *[]){
		"bf0.tim",
		"bf1.tim",
		"bf2.tim",
		"bf3.tim",
		"bf4.tim",
		"bf5.tim",
		"bf6.tim",
		"bf7.tim",
		"dead0.tim",
		"dead1.tim",
		"dead2.tim",
		"dead3.tim",
		"dead4.tim",
		"dead5.tim",
		"dead6.tim",
		"bfpov0.tim",
		"bfpov1.tim",
		"bfpov2.tim",
		"bfpov3.tim",
		"bfpov5.tim",
		"bfpov4.tim",
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
	memcpy(this->skull, char_bfpov_skull, sizeof(char_bfpov_skull));
	this->skull_scale = 64;
	
	SkullFragment *frag = this->skull;
	for (size_t i = 0; i < COUNT_OF_MEMBER(Char_BFPOV, skull); i++, frag++)
	{
		//Randomize trajectory
		frag->xsp += RandomRange(-4, 4);
		frag->ysp += RandomRange(-2, 2);
	}
	
	return (Character*)this;
}
