#include "global.h"
#include "light.h"

//diffuse light color variables
GLfloat dlr = 1.0;
GLfloat dlg = 1.0;
GLfloat dlb = 1.0;

//ambient light color variables
GLfloat alr = 1.0;
GLfloat alg = 1.0;
GLfloat alb = 1.0;

//light position variables
GLfloat lx = 1.0;
GLfloat ly = 1.0;
GLfloat lz = 0.0;
GLfloat lw = 0.0;

void InitLight()
{
	// Initialize the lights
	GLfloat DiffuseLight[] = { dlr, dlg, dlb, 1.0 }; //set DiffuseLight[] to the specified values
	GLfloat AmbientLight[] = { alr, alg, alb, 1.0 }; //set AmbientLight[] to the specified values
	GLfloat SpecularLight[] = { 1.0, 1.0, 1.0, 1.0 };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffuseLight); //change the light accordingly
	glLightfv(GL_LIGHT1, GL_AMBIENT, AmbientLight); //change the light accordingly
	glLightfv(GL_LIGHT2, GL_SPECULAR, SpecularLight);

	GLfloat LightPosition[] = { lx, ly, lz, lw }; //set the LightPosition to the specified values
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition); //change the light accordingly
	glLightfv(GL_LIGHT2, GL_POSITION, LightPosition);

	glEnable(GL_LIGHTING); //enable the lighting
	glEnable(GL_LIGHT0); //enable LIGHT0, Diffuse Light
	glEnable(GL_LIGHT1); //enable LIGHT1, Ambient Light
	glEnable(GL_LIGHT2); //enable LIGHT1, Specular Light
}

void material(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha)
{
	GLfloat no_mat[] = { 0.0, 0.0, 0.0, alpha };
	GLfloat default_ambient[] = { red * 0.2, green * 0.2, blue * 0.2, alpha };
	GLfloat mat_ambient[] = { red * 0.7, green * 0.7, blue * 0.7, alpha };
	GLfloat mat_ambient_color[] = { red *0.8, green * 0.8, blue * 0.2, alpha };
	GLfloat mat_diffuse[] = { red * 0.5, green * 0.5, blue * 0.5, alpha };
	GLfloat default_diffuse[] = { red * 0.8, green * 0.8, blue * 0.8, alpha };
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, alpha };
	GLfloat no_shininess[] = { 0.0 };
	GLfloat low_shininess[] = { 5.0 };
	GLfloat high_shininess[] = { 100.0 };
	GLfloat mat_emission[] = { red * 0.2, green *0.2, blue * 0.2, 0.0 };

	glPushMatrix();
	glTranslatef(lx, ly, lz);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, default_ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, low_shininess);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, mat_emission);
	glPopMatrix();
}