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

Event event;

static void Slavess()
{
	switch(stage.song_step)
	{
		case 1085:
			event.shake = 100;
			slaves.type = 3;
			slaves.change = true;
			break;
		case 1601:
			event.shake = 100;
			slaves.type = 1;
			slaves.change = true;
			break;
		case 2430:
			event.shake = 100;
			slaves.type = 2;
			slaves.change = true;
			break;
		case 3233:
			event.shake = 100;
			slaves.type = 3;
			slaves.change = true;
			break;
	}
}

void Events()
{
	event.shake = lerp(event.shake,0,FIXED_DEC(2,10));
	//FntPrint("steps: %d", stage.song_step);
	if(stage.prefs.followcamera)
		FollowCharCamera();
	if (stage.stage_id == StageId_1_3)
		Slavess();
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

