#include "house.h"

#include "../../psx/archive.h"
#include "../../psx/mem.h"
#include "../../scenes/stage/stage.h"

//Week 1 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_back0, tex_back1, tex_back2;
} Back_House;

//Week 1 background functions
void Back_House_DrawBG(StageBack *back)
{
	Back_House *this = (Back_House*)back;
	
	fixed_t fx, fy;
	
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT backl_src = {0, 0, 256, 256};
	RECT_FIXED backl_dst = {
		FIXED_DEC(-256,1) - fx,
		FIXED_DEC(-100,1) - fy,
		FIXED_DEC(256,1),
		FIXED_DEC(256,1)
	};
	RECT backr_src = {0, 0, 256, 256};
	RECT_FIXED backr_dst = {
		FIXED_DEC(-2,1) - fx,
		FIXED_DEC(-100,1) - fy,
		FIXED_DEC(256,1),
		FIXED_DEC(256,1)
	};
	
	Stage_DrawTex(&this->tex_back0, &backl_src, &backl_dst, stage.camera.bzoom);
	Stage_DrawTex(&this->tex_back1, &backr_src, &backr_dst, stage.camera.bzoom);
}

void Back_House_Free(StageBack *back)
{
	Back_House *this = (Back_House*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_House_New(void)
{
	//Allocate background structure
	Back_House *this = (Back_House*)Mem_Alloc(sizeof(Back_House));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_House_DrawBG;
	this->back.free = Back_House_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\STAGES\\HOUSE.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "0.tim"), 0);
	Gfx_LoadTex(&this->tex_back1, Archive_Find(arc_back, "1.tim"), 0);
	Gfx_LoadTex(&this->tex_back2, Archive_Find(arc_back, "2.tim"), 0);
	Mem_Free(arc_back);
	
	return (StageBack*)this;
}
