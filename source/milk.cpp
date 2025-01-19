#include "milk.h"
#include "anya.h"					// single image
#include "bubble.h"
#include "opao2.h"
static int textureID = -1;
static int textureID2 = -1;



//verticies for the cube
v16 CubeVectors[] = {
 		floattov16(-0.5), floattov16(-0.8), floattov16(0.3),
		floattov16(0.5),  floattov16(-0.8), floattov16(0.3),
		floattov16(0.5),  floattov16(-0.8), floattov16(-0.3),
		floattov16(-0.5), floattov16(-0.8), floattov16(-0.3),
		floattov16(-0.5), floattov16(0.8),  floattov16(0.3),
		floattov16(0.5),  floattov16(0.8),	floattov16(0.3),
		floattov16(0.5),  floattov16(0.8),  floattov16(-0.3),
		floattov16(-0.5), floattov16(0.8),  floattov16(-0.3)
};

//polys
u8 CubeFaces[] = {
	3,2,1,0,
	0,1,5,4,
	1,2,6,5,
	2,3,7,6,
	3,0,4,7,
	5,6,7,4
};

//texture coordinates
u32 uv[] =
{

	TEXTURE_PACK(inttot16(128), 0),
	TEXTURE_PACK(inttot16(128),inttot16(128)),
	TEXTURE_PACK(0, inttot16(128)),
	TEXTURE_PACK(0,0)
};

u32 normals[] =
{
	NORMAL_PACK(0,floattov10(-.97),0),
	NORMAL_PACK(0,0,floattov10(.97)),
	NORMAL_PACK(floattov10(.97),0,0),
	NORMAL_PACK(0,0,floattov10(-.97)),
	NORMAL_PACK(floattov10(-.97),0,0),
	NORMAL_PACK(0,floattov10(.97),0)

};

//draw a cube face at the specified color
 void drawQuad(int poly)
{

	u32 f1 = CubeFaces[poly * 4] ;
	u32 f2 = CubeFaces[poly * 4 + 1] ;
	u32 f3 = CubeFaces[poly * 4 + 2] ;
	u32 f4 = CubeFaces[poly * 4 + 3] ;


	glNormal(normals[poly]);

	GFX_TEX_COORD = (uv[0]);
	glVertex3v16(CubeVectors[f1*3], CubeVectors[f1*3 + 1], CubeVectors[f1*3 +  2] );

	GFX_TEX_COORD = (uv[1]);
	glVertex3v16(CubeVectors[f2*3], CubeVectors[f2*3 + 1], CubeVectors[f2*3 + 2] );

	GFX_TEX_COORD = (uv[2]);
	glVertex3v16(CubeVectors[f3*3], CubeVectors[f3*3 + 1], CubeVectors[f3*3 + 2] );

	GFX_TEX_COORD = (uv[3]);
	glVertex3v16(CubeVectors[f4*3], CubeVectors[f4*3 + 1], CubeVectors[f4*3 + 2] );
}
int milk::milk_count = 0;
milk::milk() {
    ++milk_count;
    if (textureID == -1) {
	    glGenTextures(1, &textureID);
	    glBindTexture(0, textureID);
	    glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128, 
            TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)anyaBitmap);

	    glGenTextures(1, &textureID2);
	    glBindTexture(0, textureID2);
	    glTexImage2D(0, 0, GL_RGB, TEXTURE_SIZE_128, 
            TEXTURE_SIZE_128, 0, TEXGEN_TEXCOORD, (u8*)opao2Bitmap);
    }
    float s = 0.4f;
    scale[0] = s;
    scale[1] = s;
    scale[2] = s;
    idx = rand() % 2;
}

milk::~milk() {
    --milk_count;
}

void milk::update(int f) {
    if (following) {
        auto b = (bubble*)following;
        logic_x = b->logic_x;
        logic_y = b->logic_y;
    } else {
        v += 0.006f;
        logic_y -= v;
    }
    rotation[0] += 1;
    rotation[1] += 1;
    //iprintf("milk logic xy %d %d", logic_x, logic_y);
}

void milk::on_render() {
    //pos[0] = logic_x * 125;
    //pos[1] = logic_y * 70;
    //iprintf("milk on render");
	glMaterialf(GL_AMBIENT, RGB15(8,8,8));
	glMaterialf(GL_DIFFUSE, RGB15(16,16,16));
	glMaterialf(GL_SPECULAR, BIT(15) | RGB15(8,8,8));
	glMaterialf(GL_EMISSION, RGB15(5,5,5));


	//ds uses a table for shinyness..this generates a half-ass one
	glMaterialShinyness();
	//not a real gl function and will likely change
	glPolyFmt(POLY_ALPHA(20) | POLY_CULL_BACK | POLY_FORMAT_LIGHT0 | POLY_FORMAT_LIGHT1 |
												POLY_FORMAT_LIGHT2 | POLY_FORMAT_LIGHT3 ) ;
    
	glBindTexture(0, idx == 0 ? textureID : textureID2);
	glBegin(GL_QUAD);
		for(int i = 0; i < 6; i++)
			drawQuad(i);

	glEnd();
}