#include "gameobject.h"

void gameobject::do_render() {
    pos[0] = logic_x * 125;
    pos[1] = logic_y * 70;
	glMatrixMode(GL_TEXTURE);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
    //iprintf("\non render gameobject\n");
	glPushMatrix();

	//move it away from the camera
	glTranslatef32(pos[0], pos[1], floattof32(pos[2] - 1));
	glScalef32(floattof32(scale[0]),floattof32(scale[1]),floattof32(1.0f));

	glRotateX(rotation[0]);
	glRotateY(rotation[1]);

    on_render();

    glPopMatrix(1);
}