#include "main.h"

#include "psx/timer.h"
#include "psx/io.h"
#include "psx/gfx.h"
#include "audio.h"
#include "psx/pad.h"

#include "scenes/menu/menu.h"
#include "scenes/stage/stage.h"

//Game loop
GameLoop gameloop;

//Error handler
char error_msg[0x200];

void ErrorLock(void)
{
	while (1)
	{
		#ifdef PSXF_PC
			MsgPrint(error_msg);
			exit(1);
		#else
			FntPrint("A fatal error has occured\n~c700%s\n", error_msg);
			Gfx_Flip();
		#endif
	}
}

//Memory heap
//#define MEM_STAT //This will enable the Mem_GetStat function which returns information about available memory in the heap

#define MEM_IMPLEMENTATION
#include "psx/mem.h"
#undef MEM_IMPLEMENTATION

#ifndef PSXF_STDMEM
static u8 malloc_heap[0x1B0000];
#endif

//Entry point
int main(int argc, char **argv)
{
	//Remember arguments
	my_argc = argc;
	my_argv = argv;
	
	//Initialize system
	PSX_Init();
	
	Mem_Init((void*)malloc_heap, sizeof(malloc_heap));
	
	stage.pal_i = 1;
	IO_Init();
	Audio_Init();
	Gfx_Init();
	Pad_Init();
	
	Timer_Init(false, false);
	
	//Start game
	gameloop = GameLoop_Menu;
	Menu_Load(MenuPage_Opening);
	
	//Game loop
	while (PSX_Running())
	{
		//Prepare frame
		Timer_Tick();
		Audio_ProcessXA();
		Pad_Update();
		
		#ifdef MEM_STAT
			//Memory stats
			size_t mem_used, mem_size, mem_max;
			Mem_GetStat(&mem_used, &mem_size, &mem_max);
			#ifndef MEM_BAR
				FntPrint("mem: %08X/%08X (max %08X)\n", mem_used, mem_size, mem_max);
			#endif
		#endif
		
		//Set video mode
		if (stage.prefs.palmode)
		{
			if (stage.pal_i == 1)
			{
				SetVideoMode(MODE_PAL);
				SsSetTickMode(SS_TICK50);
				stage.disp[0].screen.y = stage.disp[1].screen.y = 24;
				Timer_Init(true, true);
				stage.pal_i = 2;
			}
		}
		else
		{
			if (stage.pal_i == 1)
			{
				SetVideoMode(MODE_NTSC);
				SsSetTickMode(SS_TICK60);
				stage.disp[0].screen.y = stage.disp[1].screen.y = 0;
				Timer_Init(false, false);
				stage.pal_i = 2;
			}
		}
		
		//Tick and draw game
		switch (gameloop)
		{
			case GameLoop_Menu:
				Menu_Tick();
				break;
			case GameLoop_Stage:
				Stage_Tick();
				break;
		}
		
		//Flip gfx buffers
		Gfx_Flip();
	}
	
	//Deinitialize system
	Pad_Quit();
	Gfx_Quit();
	Audio_Quit();
	IO_Quit();
	
	PSX_Quit();
	return 0;
}
