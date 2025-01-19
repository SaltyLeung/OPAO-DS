#include <nds.h>
#include <maxmod9.h>
#include <stdio.h>

#include "soundbank.h"
#include "soundbank_bin.h"


void sound_init() {
	mmInitDefaultMem((mm_addr)soundbank_bin);
	mmLoadEffect( SFX_FLOW );
	mmLoadEffect( SFX_HIT );
	mmLoadEffect( SFX_OP );
}

void play_shoot_effect() {
    mm_sound_effect op = {
		{ SFX_FLOW } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};
	mmEffectEx(&op);
}

void play_hit_effect() {
    mm_sound_effect op = {
		{ SFX_HIT } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};
	mmEffectEx(&op);
}

void play_update_effect() {
    mm_sound_effect op = {
		{ SFX_UPDATE } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};
	mmEffectEx(&op);
}

mm_sfxhand music = 0;
void play_music() {
    if (music != 0) {
		mmEffectCancel(music);
    }
    mm_sound_effect op = {
		{ SFX_OP } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};
	music = mmEffectEx(&op);
}