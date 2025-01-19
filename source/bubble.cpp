#include "bubble.h"
#include "bubble_tex.h"
#include "sound_manage.h"
//#include "bubble_tex_pcx.h"

static int textureID = -1;
float bubble::BUBBLE_BLOW_ADDON = 0.0f;
bubble::bubble() {
    scale[0] = .1f;
    scale[1] = .1f;
    scale[2] = .1f;

    if (textureID == -1) {

	//sImage pcx;			// (NEW) and different from nehe.
	//loadPCX((u8*)bubble_tex_pcx, &pcx);
	//image8to16trans(&pcx, 0);

        glGenTextures(1, &textureID);
	    glBindTexture(0, textureID);
	    glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_32, 
	    	TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD, (u8*)bubble_texBitmap);

	    //glTexImage2D(0, 0, GL_RGBA, TEXTURE_SIZE_32, 
	    //	TEXTURE_SIZE_32, 0, TEXGEN_TEXCOORD, (u8*)pcx.image.data8);

            
	    //imageDestroy(&pcx);
    }
}

void bubble::update(int frame_count) {
    if (!_is_activate) {
        return;
    }
    logic_y += 1;
    //iprintf("bubble logic xy %d %d", logic_x, logic_y);
}


bool bubble::blow() {
    //iprintf("bubble blow blow\n");
    float blow_speed = (1 + BUBBLE_BLOW_ADDON) * .002f;
    scale[0] += blow_speed;
    scale[1] += blow_speed;
    scale[2] += blow_speed;

    if (scale[0] > 0.7f) {
         return false;
    }
    return true;
}


void bubble::activate() {
    _is_activate = true;
    play_shoot_effect();
}

void bubble::on_render() {
    pos[2] = -3;
    //iprintf("on render bubble");
	glMaterialf(GL_AMBIENT, RGB15(16,16,16));
	glMaterialf(GL_DIFFUSE, RGB15(16,16,16));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8,8,8));
	glMaterialf(GL_EMISSION, RGB15(16,16,16));

	//ds uses a table for shinyness..this generates a half-ass one
	glMaterialShinyness();

	//not a real gl function and will likely change
	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK);

	glBindTexture(0, textureID);

		//draw the obj
	glBegin(GL_QUAD);
		glNormal(NORMAL_PACK(0,inttov10(-1),0));

		GFX_TEX_COORD = (TEXTURE_PACK(0, inttot16(32)));
		glVertex3v16(floattov16(-1.0f),	floattov16(-1.0f), 0 );

		GFX_TEX_COORD = (TEXTURE_PACK(inttot16(32),inttot16(32)));
		glVertex3v16(floattov16(1.0f),	floattov16(-1.0f), 0 );

		GFX_TEX_COORD = (TEXTURE_PACK(inttot16(32), 0));
		glVertex3v16(floattov16(1.0f),	floattov16(1.0f), 0 );

		GFX_TEX_COORD = (TEXTURE_PACK(0,0));
		glVertex3v16(floattov16(-1.0f),	floattov16(1.0f), 0 );

	glEnd();
}