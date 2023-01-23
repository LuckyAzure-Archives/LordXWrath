#include "event.h"
#include "../scenes/stage/stage.h"

#include "../audio.h"
#include "../psx/mem.h"
#include "../psx/timer.h"
#include "../psx/pad.h"
#include "../psx/random.h"
#include "../psx/movie.h"
#include "../psx/mutil.h"

#include "../characters/slaves/slaves.h"
#include "../characters/bfpov/bfpov.h"

Event event;


static void movieview_tick()
{
	RECT_FIXED top = {
		-FIXED_DEC(SCREEN_WIDTH,2), 
		-FIXED_DEC(SCREEN_HEIGHT,2),
		FIXED_DEC(SCREEN_WIDTH,1),
		FIXED_DEC(30,1)
	};
	RECT_FIXED bottom = {
		-FIXED_DEC(SCREEN_WIDTH,2),
		-FIXED_DEC(SCREEN_HEIGHT,2) + FIXED_DEC(SCREEN_HEIGHT - 30,1),
		FIXED_DEC(SCREEN_WIDTH,1),
		FIXED_DEC(30,1)
	};
	
	RECT src = {
		0,
	    228,
		1,
		1
	};
	
	Stage_DrawTexCol(&stage.tex_hud0, &src, &top, stage.bump, 0, 0, 0);
	Stage_DrawTexCol(&stage.tex_hud0, &src, &bottom, stage.bump, 0, 0, 0);
}

static void Slavess()
{
	stage.prefs.botplay = true;
	switch(stage.song_step)
	{
		case 0:
			event.hidehud = true;
			event.fadebool = false;
			break;
		case 145:
			event.zoom = FIXED_DEC(1,1);
			event.speed = FIXED_DEC(1,100);
			break;
		case 256:
			event.hidehud = false;
			event.whiteflash = 256;
			stage.camera.zoom = FIXED_DEC(2,1);
			event.speed = FIXED_DEC(5,100);
			break;
		case 288:
			Attack();
			break;
		case 511:
			event.flash = 256;
			event.movieview = true;
			break;
		case 575:
			Attack();
			break;
		case 671:
			Attack();
			break;
		case 768:
			Attack();
			event.flash = 256;
			event.movieview = false;
			break;
		case 895:
			event.flash = 256;
			event.movieview = true;
			break;
		case 928:
			Attack();
			break;
		case 1024:
			event.movieview = false;
			event.fadebool = true;
			break;
		case 1086:
			event.flash = 256;
			event.fadebool = false;
			event.fade = 0;
			stage.opponent->flipped = true;
			stage.opponent->size = FIXED_DEC(5,3);
			stage.opponent->x = FIXED_DEC(-70,1);
			stage.opponent->y = FIXED_DEC(100,1);
			stage.opponent->focus_x = FIXED_DEC(0,1);
			stage.opponent->focus_y = FIXED_DEC(-220,1);
			stage.player->focus_x = FIXED_DEC(-70,1);
			stage.player->focus_y = FIXED_DEC(-90,1);
			stage.player->x = FIXED_DEC(110,1);
			stage.player->y = FIXED_DEC(70,1);
			stage.player->focus_zoom = FIXED_DEC(4,3);
			stage.player->size = FIXED_DEC(1,1);
			event.shake = 100;
			slaves.type = 3;
			slaves.change = true;
			bfpov.type = 1;
			bfpov.change = true;
			break;
		case 1570:
			event.fadebool = true;
			event.fadebehind = true;
			break;
		case 1600:
			event.fade = 0;
			event.fadebool = false;
			event.fadebehind = false;
			event.flash = 256;
			stage.opponent->flipped = true;
			stage.opponent->size = FIXED_DEC(1,1);
			stage.opponent->focus_x = FIXED_DEC(-65,1);
			stage.opponent->focus_y = FIXED_DEC(-80,1);
			stage.opponent->x = FIXED_DEC(90,1);
			stage.opponent->y = FIXED_DEC(110,1);
			stage.player->flipped = true;
			stage.player->focus_x = FIXED_DEC(30,1);
			stage.player->focus_y = FIXED_DEC(-80,1);
			stage.player->x = FIXED_DEC(-90,1);
			stage.player->y = FIXED_DEC(110,1);
			stage.player->focus_zoom = FIXED_DEC(1,1);
			stage.player->size = FIXED_DEC(1,1);
			event.shake = 100;
			slaves.type = 1;
			slaves.change = true;
			bfpov.type = 0;
			bfpov.change = true;
			break;
		case 1616:
			Attack();
			break;
		case 1760:
			Attack();
			break;
		case 1856:
			event.fade = 255;
			event.flash = 256;
			event.movieview = true;
			break;
		case 1887:
			Attack();
			break;
		case 2103:
			event.movieview = false;
			break;
		case 2111:
			event.flash = 256;
			event.movieview = true;
			break;
		case 2367:
			event.fadebool = true;
			break;
		case 2430:
			event.fadebool = false;
			event.fade = 0;
			event.movieview = false;
			event.flash = 256;
			stage.opponent->flipped = false;
			stage.opponent->focus_x = FIXED_DEC(65,1);
			stage.opponent->focus_y = FIXED_DEC(-80,1);
			stage.opponent->x = FIXED_DEC(-90,1);
			stage.player->flipped = false;
			stage.player->focus_x = FIXED_DEC(-30,1);
			stage.player->focus_y = FIXED_DEC(-80,1);
			stage.player->x = FIXED_DEC(90,1);
			event.shake = 100;
			slaves.type = 2;
			slaves.change = true;
			break;
		case 2464:
			Attack();
			break;
		case 2575:
			Attack();
			break;
		case 2687:
			event.fadebool = true;
			break;
		case 2719:
			event.fadebool = false;
			event.fade = 0;
			event.whiteflash = 256;
			break;
		case 2848:
			Attack();
			break;
		case 2991:
			Attack();
			break;
		case 3120:
			Attack();
			break;
		case 3184:
			Attack();
			break;
		case 3216:
			event.fadebool = true;
			event.fadebehind = true;
			break;
		case 3232:
			event.fade = 0;
			event.fadebool = false;
			event.fadebehind = false;
			event.flash = 256;
			stage.opponent->size = FIXED_DEC(5,3);
			stage.opponent->x = FIXED_DEC(70,1);
			stage.opponent->y = FIXED_DEC(100,1);
			stage.opponent->focus_x = FIXED_DEC(0,1);
			stage.opponent->focus_y = FIXED_DEC(-220,1);
			stage.player->flipped = true;
			stage.player->focus_x = FIXED_DEC(70,1);
			stage.player->focus_y = FIXED_DEC(-90,1);
			stage.player->x = FIXED_DEC(-110,1);
			stage.player->y = FIXED_DEC(70,1);
			stage.player->focus_zoom = FIXED_DEC(4,3);
			stage.player->size = FIXED_DEC(1,1);
			event.shake = 100;
			slaves.type = 3;
			slaves.change = true;
			bfpov.type = 1;
			bfpov.change = true;
			break;
		case 3487:
			event.flash = 256;
			event.movieview = true;
			break;
		case 4024:
			event.fadebool = true;
			break;
	}
}

void Attack()
{
	event.flash = 256;
}

void Load_Events()
{
	if (stage.stage_id == StageId_1_3)
	{
		event.zoom = FIXED_DEC(3,2);
		event.speed = FIXED_DEC(5,100);
		event.shake = 0;
		event.flash = 0;
		event.fade = 255;
		event.fadebool = true;
		event.movieview = false;
		event.fadebehind = false;
	}
	else
	{
		event.zoom = FIXED_DEC(1,1);
		event.speed = FIXED_DEC(5,100);
		event.shake = 0;
		event.flash = 0;
		event.fade = 0;
		event.fadebool = false;
		event.movieview = false;
		event.fadebehind = false;
	}
}

void Events_Front()
{
	Flash_tick();
	Fade_tick();
}

void Events()
{
	event.shake = lerp(event.shake,0,FIXED_DEC(2,10));
	//FntPrint("steps: %d", stage.song_step);
	
	if(stage.prefs.followcamera)
		FollowCharCamera();
	if (stage.stage_id == StageId_1_3)
		Slavess();
	White_flash_tick();
	Flash_tick();
	if(!event.fadebehind)
		Fade_tick();
}

void Events_Back()
{
	if(event.movieview)
		movieview_tick();
	if(event.fadebehind)
		Fade_tick();
}

void FollowCharCamera()
{
	u8 sensitivity = 2;
	if (stage.cur_section->flag & SECTION_FLAG_OPPFOCUS)
	{
		if (stage.opponent->animatable.anim == CharAnim_Up)
			stage.camera.y -= FIXED_DEC(sensitivity,10);
		if (stage.opponent->animatable.anim == CharAnim_Down)
			stage.camera.y += FIXED_DEC(sensitivity,10);
		if (stage.opponent->animatable.anim == CharAnim_Left)
			stage.camera.x -= FIXED_DEC(sensitivity,10);
		if (stage.opponent->animatable.anim == CharAnim_Right)
			stage.camera.x += FIXED_DEC(sensitivity,10);
	}
	else
	{
		if (stage.player->animatable.anim == CharAnim_Up)
			stage.camera.y -= FIXED_DEC(sensitivity,10);
		if (stage.player->animatable.anim == CharAnim_Down)
			stage.camera.y += FIXED_DEC(sensitivity,10);
		if (stage.player->animatable.anim == CharAnim_Left)
			stage.camera.x -= FIXED_DEC(sensitivity,10);
		if (stage.player->animatable.anim == CharAnim_Right)
			stage.camera.x += FIXED_DEC(sensitivity,10);
	}
}

void NoteHitEvent(u8 type)
{
	if (type & NOTE_FLAG_BULLET)
	{
		stage.camera.zoom += FIXED_DEC(200,1000);
		stage.bump += FIXED_DEC(100,1000);
	}
}

void NoteHitEnemyEvent(u8 type)
{
	if (type & NOTE_FLAG_BULLET)
	{
		stage.bump += FIXED_DEC(100,1000);
	}
	if (false)
		event.shake = 1000;
}

void NoteMissEvent(u8 type, u8 state)
{
	PlayerState *this = &stage.player_state[state];
	if (type & NOTE_FLAG_BULLET)
		this->health = -0x7000;
}

void White_flash_tick()
{
	static const RECT flash = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	Gfx_BlendRect(&flash, event.whiteflash, event.whiteflash, event.whiteflash, 1);
	if (event.whiteflash > 0)
		event.whiteflash -= 8;
}

void Flash_tick()
{
	static const RECT flash = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	Gfx_BlendRect(&flash, event.flash, 0, 0, 1);
	if (event.flash > 0)
		event.flash	-= 8;
}

void Fade_tick()
{
	static const RECT fade = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
	Gfx_BlendRect(&fade, event.fade, event.fade, event.fade, 2);
	if (event.fadebool && event.fade < 255)
		event.fade += 5;
	else if (!event.fadebool && event.fade > 0)
		event.fade -= 5;
}
