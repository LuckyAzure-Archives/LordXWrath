#include "slavess.h"

#include "../../psx/archive.h"
#include "../../psx/mem.h"
#include "../../scenes/stage/stage.h"

//Week 1 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_back0, tex_back1, tex_back2, tex_back3, tex_back4, tex_back5;
} Back_Slavess;

//Week 1 background functions
void Back_Slavess_DrawBG(StageBack *back)
{
	Back_Slavess *this = (Back_Slavess*)back;
	
	fixed_t fx, fy;
	
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT backl_src = {0, 0, 256, 256};
	RECT_FIXED backl_dst = {
		FIXED_DEC(-256,1) - fx,
		FIXED_DEC(-256,1) - fy,
		FIXED_DEC(256,1),
		FIXED_DEC(256,1)
	};
	RECT backr_src = {0, 0, 256, 256};
	RECT_FIXED backr_dst = {
		FIXED_DEC(-2,1) - fx,
		FIXED_DEC(-256,1) - fy,
		FIXED_DEC(256,1),
		FIXED_DEC(256,1)
	};
	
	if(stage.song_step > 3488)
	{
		Stage_DrawTex(&this->tex_back4, &backl_src, &backl_dst, stage.camera.bzoom);
		Stage_DrawTex(&this->tex_back5, &backr_src, &backr_dst, stage.camera.bzoom);
	}
	else
	{
		Stage_DrawTex(&this->tex_back0, &backl_src, &backl_dst, stage.camera.bzoom);
		Stage_DrawTex(&this->tex_back1, &backr_src, &backr_dst, stage.camera.bzoom);
	}
	
	RECT backl1_src = {0, 0, 256, 208};
	RECT_FIXED backl1_dst = {
		FIXED_DEC(-256,1) - fx,
		FIXED_DEC(-2,1) - fy,
		FIXED_DEC(256,1),
		FIXED_DEC(208,1)
	};
	RECT backr1_src = {0, 0, 256, 208};
	RECT_FIXED backr1_dst = {
		FIXED_DEC(-2,1) - fx,
		FIXED_DEC(-2,1) - fy,
		FIXED_DEC(256,1),
		FIXED_DEC(208,1)
	};
	Stage_DrawTex(&this->tex_back2, &backl1_src, &backl1_dst, stage.camera.bzoom);
	Stage_DrawTex(&this->tex_back3, &backr1_src, &backr1_dst, stage.camera.bzoom);
}

void Back_Slavess_Free(StageBack *back)
{
	Back_Slavess *this = (Back_Slavess*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Slavess_New(void)
{
	//Allocate background structure
	Back_Slavess *this = (Back_Slavess*)Mem_Alloc(sizeof(Back_Slavess));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Slavess_DrawBG;
	this->back.free = Back_Slavess_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\STAGES\\SLAVESS.ARC;1");
	Gfx_LoadTex(&this->tex_back0, Archive_Find(arc_back, "0.tim"), 0);
	Gfx_LoadTex(&this->tex_back1, Archive_Find(arc_back, "1.tim"), 0);	
	Gfx_LoadTex(&this->tex_back2, Archive_Find(arc_back, "2.tim"), 0);
	Gfx_LoadTex(&this->tex_back3, Archive_Find(arc_back, "3.tim"), 0); 
	Gfx_LoadTex(&this->tex_back4, Archive_Find(arc_back, "0a.tim"), 0);
	Gfx_LoadTex(&this->tex_back5, Archive_Find(arc_back, "1a.tim"), 0); 
	Mem_Free(arc_back);
	
	return (StageBack*)this;
}
