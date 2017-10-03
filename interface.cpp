#include "global.h"
#include "light.h"
#include "interface.h"

GLfloat MenuWidth, MenuHeight;

void DrawInterface(GLfloat w, GLfloat h, bool butterflyTrue)
{
	MenuWidth = w;
	MenuHeight = h;
	if (!butterflyTrue) {
		DrawText(10.0, 30.0, "Press [B]", 0.8);
	}
	else {
		DrawText(10.0, 30.0, "Press [B] again ...", 0.8);
		DrawText(10.0, MenuHeight - 20.0, "Use the Mouse(buttons)!", 0.7);
	}
	DrawText(MenuWidth - 175.0, 30.0, "or [Esc] to leave", 0.8);
}

void DrawText(GLfloat Xpos, GLfloat Ypos, std::string text, GLfloat size)
{
	std::string::iterator c;
	int ItCount = 0;
	glPushMatrix();
	glTranslatef(Xpos, MenuHeight - Ypos, 0.0);
	glScalef(0.15*size, 0.15*size, 1.0);
	material(0.5, 1.0, 1.0, 1.0);
	for (c = text.begin(); c != text.end(); c++)
	{
		Xpos = 9.0;
		if ((ItCount > 0) && (isupper(text[ItCount - 1]))) // Previous character was uppercase
		{
			Xpos += 5.0;
		}
		if ((ItCount > 0) && ((text[ItCount - 1] == 'i') || (text[ItCount - 1] == 'l') || (text[ItCount - 1] == 'r') || (text[ItCount - 1] == ' ')))
		{
			Xpos -= 2.0;
		}
		glTranslatef(Xpos, 0.0, 0.0);
		glutStrokeCharacter(GLUT_STROKE_ROMAN, text[ItCount]);
		ItCount++;
	}
	glPopMatrix();
}
