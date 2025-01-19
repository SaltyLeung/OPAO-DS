#include <nds.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>

//texture_bin.h is created automagicaly from the texture.bin placed in arm9/resources
//texture.bin is a raw 128x128 16 bit image.  I will release a tool for texture conversion
//later
//#include "texture_bin.h"
#include "anya.h"					// single image
#include "sub_bg.h"					// single image
#include "game_scene.h"
#include "sound_manage.h"
#include "eye.h"
//glImage  Anya[1];


void DisplayEnableMotionBlur()
{
	u32 dispcnt = REG_DISPCNT;
	//set main display to display from VRAM
	dispcnt &= ~(0x00030000); dispcnt |= 2<<16; //choose to display screen from VRAM
	dispcnt &= ~(0x000C0000); dispcnt |= 1<<18; //choose to display screen from VRAM_B
	REG_DISPCNT = dispcnt;
}

void DisplayEnableNormal()
{
	u32 dispcnt = REG_DISPCNT;
	dispcnt &= ~(0x00030000); dispcnt |= 1<<16; //choose to display screen from normal layer composition
	REG_DISPCNT = dispcnt;
}

int main()
{
	int i;
	float rotateX = 0.0;
	float rotateY = 0.0;
	sound_init();
	//set mode 0, enable BG0 and set it to 3D
	videoSetMode(MODE_0_3D);


	// Sprite initialisation
	oamInit(&oamSub, SpriteMapping_1D_32, false);

	vramSetBankE(VRAM_E_MAIN_BG);
	vramSetBankD(VRAM_D_SUB_SPRITE);


	// load sprite palette
	for(unsigned i = 0; i < 256; i++)
		SPRITE_PALETTE[i] = ((u16*)eyePal)[i];

	// load sprite graphics
	for(unsigned i = 0; i< 32*16; i++)
		SPRITE_GFX[i] = ((u16*)eyeTiles)[i];

	consoleInit(0,1, BgType_Text4bpp, BgSize_T_256x256, 31,0, true, true);

	//put bg 0 at a lower priority than the text background
	bgSetPriority(0, 1);
	//iprintf("\n\n343434\n");
	// initialize gl
	glInit();

	//enable textures
	glEnable(GL_TEXTURE_2D);

	//this should work the same as the normal gl call
	glViewport(0,0,255,191);

	// enable antialiasing
	glEnable(GL_ANTIALIAS);

	// setup the rear plane
	glClearColor(0,0,0,31); // BG must be opaque for AA to work
	glClearPolyID(63); // BG must have a unique polygon ID for AA to work
	glClearDepth(0x7FFF);

	vramSetBankB(VRAM_B_LCD);
	//REG_DISPCAPCNT =
	//		DCAP_MODE(DCAP_MODE_BLEND) //blend source A and source B
	//	//|	DCAP_SRC_ADDR //this is not used since we are setting the display to render from VRAM
	//	|	DCAP_SRC_B(DCAP_SRC_B_VRAM)
	//	|	DCAP_SRC_A(DCAP_SRC_A_3DONLY)
	//	|	DCAP_SIZE(DCAP_SIZE_256x192)
	//	|	DCAP_OFFSET(0) //where to write the captured data within our chosen VRAM bank
	//	|	DCAP_BANK(DCAP_BANK_VRAM_B)
	//	|	DCAP_B(12) //blend mostly from B to make a very dramatic effect
	//	|	DCAP_A(4) //and blend only a little bit from the new scene
	//	;
	//but, dramatic effects tend to leave some garbage on the screen since the precision of the math is low,
	//and we're not putting a lot of dampening on the effect.
	//a more realistic value might be 8 and 8, but perhaps in a more complex 3d scene the garbage isn't such a bad thing
	//since the scene is changing constantly


	vramSetBankA(VRAM_A_TEXTURE);

	//int textureID;
	//glGenTextures(1, &textureID);
	//glBindTexture(0, textureID);
	//glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128 , TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)anyaBitmap);


	//any floating point gl call is being converted to fixed prior to being implemented
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45, 256.0 / 192.0, 0.1, 40);
	glOrtho(-4,4,-3,3,0.1,10);

	gluLookAt(	0.0, 0.0, 5.0,		//camera position
				0.0, 0.0, 0.0,		//look at
				0.0, 1.0, 0.0);		//up

	glEnable(GL_BLEND);

	videoSetModeSub(MODE_5_2D);
	vramSetBankC(VRAM_C_SUB_BG);
	int bg = bgInitSub(2, BgType_Bmp16, BgSize_B16_256x256, 0, 0);
  	//dmaCopy((u8*)anyaBitmap,  bgGetGfxPtr(bg), anyaBitmapLen);


	//bgInit(3, BgType_Bmp16, BgSize_B16_256x256, 0,0);
  	dmaCopy((u8*)sub_bgBitmap,  bgGetGfxPtr(bg), sub_bgBitmapLen);

		// Move and display sprite
		oamSet(&oamSub, 0,
			(20), // X position
			(20), // Y position
			0,
			0,
			SpriteSize_32x32, SpriteColorFormat_256Color,
			SPRITE_GFX,
			-1,
			false, false, false, false, false);
	//bgInitSub(2, BgType_Bmp16, BgSize_B16_256x256, 0,0);
	//micrecordInit();
	game_scene* scn = new game_scene();
	//decompress(drunkenlogoBitmap, BG_GFX,  LZ77Vram);
	//decompress(drunkenlogoBitmap, BG_GFX_SUB,  LZ77Vram);
	//decompress(anya2Bitmap, BG_GFX_SUB,  LZ77Vram);
	int frame_count = 0;
	while(pmMainLoop()) {

		oamUpdate(&oamSub);
		consoleClear();
		glLight(0, RGB15(31,31,31) , 0,				  floattov10(-1.0),		 0);
		glLight(1, RGB15(31,0,31),   0,				  floattov10(1) - 1,			 0);
		glLight(2, RGB15(0,31,0) ,   floattov10(-1.0), 0,					 0);
		glLight(3, RGB15(0,0,31) ,   floattov10(1.0) - 1,  0,					 0);
		if (scn->request_new) {
			delete scn;
			scn = new game_scene();
			frame_count = 0;
		}
		scn->update(++frame_count);
		//for (int x = 0; x < 1; ++x) {
		//	glPushMatrix();
		////move it away from the camera
		//	glTranslatef32(0, floattof32(0), floattof32(-1));
//
		//	glRotateX(rotateX);
		//	glRotateY(rotateY);
//
		//	glMatrixMode(GL_TEXTURE);
		//	glLoadIdentity();
//
		//	glMatrixMode(GL_MODELVIEW);
//
		//	glMaterialf(GL_AMBIENT, RGB15(8,8,8));
		//	glMaterialf(GL_DIFFUSE, RGB15(16,16,16));
		//	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8,8,8));
		//	glMaterialf(GL_EMISSION, RGB15(5,5,5));
//
		//	//ds uses a table for shinyness..this generates a half-ass one
		//	glMaterialShinyness();
//
		//	//not a real gl function and will likely change
		//	glPolyFmt(POLY_ALPHA(31) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 |
		//												POLY_FORMAT_LIGHT2 | POLY_FORMAT_LIGHT3 ) ;
//
		//	scanKeys();
//
		//	u16 keys = keysHeld();
//
		//	if((keys & KEY_UP)) rotateX += 3;
		//	if((keys & KEY_DOWN)) rotateX -= 3;
		//	if((keys & KEY_LEFT)) rotateY += 3;
		//	if((keys & KEY_RIGHT)) rotateY -= 3;
//
		//	glBindTexture(0, textureID);
//
		//	//draw the obj
		//	glBegin(GL_QUAD);
		//		for(i = 0; i < 6; i++)
		//			drawQuad(i);
//
		//	glEnd();
//
		//	glPopMatrix(1);
		//}

		glFlush(0);

		swiWaitForVBlank();

		//if(keys & KEY_START) break;

		//the display capture enable bit must be set again each frame if you want to continue capturing.
		REG_DISPCAPCNT |= DCAP_ENABLE;
	}

	return 0;
}//end main
