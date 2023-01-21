#include "blank.h"

#include "../../psx/archive.h"
#include "../../psx/mem.h"
#include "../../scenes/stage/stage.h"

//Week 1 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
} Back_Blank;

//Week 1 background functions
void Back_Blank_DrawBG(StageBack *back)
{
	Back_Blank *this = (Back_Blank*)back;
}

void Back_Blank_Free(StageBack *back)
{
	Back_Blank *this = (Back_Blank*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_Blank_New(void)
{
	//Allocate background structure
	Back_Blank *this = (Back_Blank*)Mem_Alloc(sizeof(Back_Blank));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = NULL;
	this->back.draw_md = NULL;
	this->back.draw_bg = Back_Blank_DrawBG;
	this->back.free = Back_Blank_Free;
	
	return (StageBack*)this;
}
