#include "<charactername>.h"

#include "../../psx/mem.h"
#include "../../psx/archive.h"
#include "../../scenes/stage/stage.h"
#include "../../main.h"

//<CharacterName> character structure
enum
{
<StructureHere>
	<CharacterName>_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[<CharacterName>_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_<CharacterName>;

//<CharacterName> character definitions
static const CharFrame char_<charactername>_frame[] = {
<FramesHere>
};

static const Animation char_<charactername>_anim[CharAnim_Max] = {
<AnimationsHere>
};

//<CharacterName> character functions
void Char_<CharacterName>_SetFrame(void *user, u8 frame)
{
	Char_<CharacterName> *this = (Char_<CharacterName>*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_<charactername>_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_<CharacterName>_Tick(Character *character)
{
	Char_<CharacterName> *this = (Char_<CharacterName>*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_<CharacterName>_SetFrame);
	Character_Draw(character, &this->tex, &char_<charactername>_frame[this->frame]);
}

void Char_<CharacterName>_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_<CharacterName>_Free(Character *character)
{
	Char_<CharacterName> *this = (Char_<CharacterName>*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_<CharacterName>_New(fixed_t x, fixed_t y)
{
	//Allocate <charactername> object
	Char_<CharacterName> *this = Mem_Alloc(sizeof(Char_<CharacterName>));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_<CharacterName>_New] Failed to allocate <charactername> object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_<CharacterName>_Tick;
	this->character.set_anim = Char_<CharacterName>_SetAnim;
	this->character.free = Char_<CharacterName>_Free;
	
	Animatable_Init(&this->character.animatable, char_<charactername>_anim);
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
	this->character.health_bar = 0xFF<HB Color>;
	
	this->character.focus_x = FIXED_DEC(65,1);
	this->character.focus_y = FIXED_DEC(-115,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	this->character.size = FIXED_DEC(<CharSize>,100);
	this->character.flipped = false;
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\<CHARACTERNAME>.ARC;1");
	
	const char **pathp = (const char *[]){
<TextureHere>
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
