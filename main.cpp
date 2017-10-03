// Final Project CG

#include "global.h"
#include "UserInput.h"
#include "light.h"
#include "interface.h"
#include <iostream>

#define MAXPARTICLES 100
#define MAXFIREWORKS 5

//Original screen size
GLint width = 800;
GLint height = 600;
//Starting position
GLfloat g_posX = 0.0, g_posY = 20.0, g_posZ = 40.0;
GLfloat g_orientation = 0.0; // y axis
Vector3 viewpoint;
Vector3 viewtarget;
//Functions & Global variables
Vector3 CalculateBezier(GLfloat t, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3);
Vector3 CalculateSpiral(GLfloat t, GLint j);
void getMousePosition(GLdouble& objX, GLdouble& objY, GLdouble& objZ);
bool butterflyTrue = false;
bool goCrazy = false;
GLfloat zRange = 0.0;
GLint mouse_x, mouse_y, mouse_z;
GLfloat size[MAXPARTICLES];
//Music
Mix_Music *music = NULL;
Mix_Chunk *boom = NULL;
Mix_Chunk *birds = NULL;

class Firework
{
private:
	GLfloat fw_x[MAXPARTICLES], fw_y[MAXPARTICLES], fw_z[MAXPARTICLES];
	GLfloat fw_vx[MAXPARTICLES], fw_vy[MAXPARTICLES], fw_vz[MAXPARTICLES];
public:
	// Firework parameters
	GLfloat red, green, blue; //Colorvalues of the explosion
	GLfloat counter;
	bool exploded;
	bool WingsDown;
	GLfloat alpha;
	GLfloat wings_angle[MAXPARTICLES];

	Firework();

	void init();
	void move();
	void explode();
	void butterfly();
	GLfloat getX(int j);
	GLfloat getY(int j);
	GLfloat getZ(int j);
};

Firework::Firework()
{
	init();
}

Firework fireworks[MAXFIREWORKS]; // Array including all present fireworks

void Firework::init() {
	// Firework color
	red = 1.0 * (rand() / (GLfloat)RAND_MAX);
	green = 1.0 * (rand() / (GLfloat)RAND_MAX);
	blue = 1.0 * (rand() / (GLfloat)RAND_MAX);
	// Random initial coordinates
	GLfloat x_coord = 20.0 * (rand() / (GLfloat)RAND_MAX) - 10.0;
	GLfloat y_coord = 10.0;
	GLfloat z_coord = 20.0 * (rand() / (GLfloat)RAND_MAX) - 10.0;
	// Random initial velocity
	GLfloat x_velo = 3.0 * (rand() / (GLfloat)RAND_MAX) - 1.5;
	GLfloat y_velo = 15.0 * (rand() / (GLfloat)RAND_MAX) + 7.5; // Always upwards
	GLfloat z_velo = 3.0 * (rand() / (GLfloat)RAND_MAX) - 1.5;

	alpha = 1.0f;

	unsigned int i;
	for (i = 0; i < MAXPARTICLES; i = i + 1) {
		// Random initial velocity
		fw_vx[i] = x_velo;
		fw_vy[i] = y_velo;
		fw_vz[i] = z_velo;
		fw_x[i] = x_coord;
		fw_y[i] = y_coord;
		fw_z[i] = z_coord;
		wings_angle[i] = (GLfloat)i;
		size[i] = 0.05 * (rand() / (GLfloat)RAND_MAX); //size between 0.0 and 0.05 pixels
	}
	exploded = false;
	WingsDown = false;
	counter = 0.0;
	zRange = 0.0;
}

//Move the fireworks, before exploding
void Firework::move()
{
	if (fw_vy[0] > 0) {
		unsigned int i;
		for (i = 0; i < MAXPARTICLES; i = i + 1)
		{
			fw_vy[i] = fw_vy[i] - 0.1;
			fw_x[i] = fw_x[i] + fw_vx[i] * 0.01;
			fw_y[i] = fw_y[i] + fw_vy[i] * 0.01;
			fw_z[i] = fw_z[i] + fw_vz[i] * 0.01;
		}
		if (butterflyTrue) // time to explode anyway
		{
			fw_vx[0] = 0.0;
			fw_vy[0] = 0.0;
			fw_vz[0] = 0.0;
		}
	}
	else { // it is time to explode
		Mix_PlayChannel(-1, boom, 0);
		exploded = true;
		unsigned int j;
		for (j = 0; j < MAXPARTICLES; j = j + 1) {
			fw_vx[j] = 4.0 * (rand() / (GLfloat)RAND_MAX) - 2.0;
			fw_vy[j] = 4.0 * (rand() / (GLfloat)RAND_MAX) - 2.0;
			fw_vz[j] = 4.0 * (rand() / (GLfloat)RAND_MAX) - 2.0;
		}
	}
}

//Explode or transform into butterflies
void Firework::explode()
{
	unsigned int j;
	if (butterflyTrue && alpha < 0.8) //Release butterflies after alpha has reached 0.8
	{
		if (counter < 0.02) Mix_PlayChannel(-1, birds, 0);
		for (j = 0; j < MAXPARTICLES; j = j + 1) {
			fw_x[j] = fw_x[j] + fw_vx[j] * 0.03;
			fw_y[j] = fw_y[j] + fw_vy[j] * 0.03;
			fw_z[j] = fw_z[j] + fw_vz[j] * 0.03;
			Vector3 circle = CalculateSpiral((GLfloat)(j + counter), j);
			if (goCrazy)
			{
				fw_vx[j] = 0.0;
				fw_vy[j] = 0.0;
				fw_vz[j] = 0.0;
			}
			//Calculate new positions
			fw_vx[j] = (fw_vx[j] + circle.x() - fw_x[j])*(0.5 * rand() / (GLfloat)RAND_MAX);
			fw_vy[j] = (fw_vy[j] + circle.y() - fw_y[j])*(0.5 * rand() / (GLfloat)RAND_MAX);
			fw_vz[j] = (fw_vz[j] + circle.z() - fw_z[j])*(0.5 * rand() / (GLfloat)RAND_MAX);
			//Calculate the butterfly's new wing angle (relative to its body)
			if (wings_angle[j] > 70.0) {
				WingsDown = true;
			}
			else if (wings_angle[j] <= -70.0) {
				WingsDown = false;
			}
			if (WingsDown) {
				wings_angle[j] -= 14.0;
			}
			else {
				wings_angle[j] += 14.0;
			}
		}
		counter = counter + 0.01; // "time" parameter for CalculateSpiral function
	}
	else //Explode like a normal firework
	{
		for (j = 0; j < MAXPARTICLES; j = j + 1) {
			fw_x[j] = fw_x[j] + fw_vx[j] * 0.05;
			fw_y[j] = fw_y[j] + fw_vy[j] * 0.05;
			fw_z[j] = fw_z[j] + fw_vz[j] * 0.05;

			fw_vy[j] = fw_vy[j] - 0.01;
			fw_vx[j] = 0.99*fw_vx[j];
			fw_vz[j] = 0.99*fw_vz[j];
		}
		if (alpha > 0.0f)
		{
			alpha -= 0.01f;
		}
		else // Once the alpha hits zero reset the firework
		{
			init();
		}
	}
}

GLfloat Firework::getX(int j)
{
	return fw_x[j];
}

GLfloat Firework::getY(int j)
{
	return fw_y[j];
}

GLfloat Firework::getZ(int j)
{
	return fw_z[j];
}

//Return the position of the mouse cursor
void getMousePosition(GLdouble& objX, GLdouble& objY, GLdouble& objZ)
{
	SDL_GetMouseState(&mouse_x, &mouse_y);

	GLint viewport[4];
	GLdouble modelview[16];
	GLdouble projection[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	glGetIntegerv(GL_VIEWPORT, viewport);

	mouse_y = (float)viewport[3] - (float)mouse_y;
	mouse_z = 1.0;
	gluUnProject(mouse_x, mouse_y, mouse_z, modelview, projection, viewport, &objX, &objY, &objZ);

	objX = objX * 0.1;
	objY = objY * 0.1;
	objZ = objZ * 0.1;
}

// Calculate point at time t, on Bezier Curve corresponding to control points (p0, p1, p2, p3)
Vector3 CalculateBezier(GLfloat t, Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3)
{
	GLfloat u = 1 - t;
	GLfloat uu = u * u;
	GLfloat uuu = uu * u;
	GLfloat tt = t * t;
	GLfloat ttt = tt * t;

	Vector3 bezier = (p0.operator*(uuu)) + p1.operator*(3 * uu * t) + p2.operator*(3 * u * tt) + p3.operator*(ttt);
	return bezier;
}

// Calculate the 'spiral' pattern for the butterfly movement
Vector3 CalculateSpiral(GLfloat t, GLint j)
{
	GLdouble tarX, tarY, tarZ; // Get Mouse target
	getMousePosition(tarX, tarY, tarZ);
	GLfloat xx, yy, zz;
	if (mouse_x <= 20.0 || mouse_y <= 20.0 || mouse_x >= (width - 20.0) || mouse_y >= (height - 20.0)) //If mouse is close to window border
	{
		//Let the butterflies go wild
		goCrazy = true;
		xx = ((GLfloat)j * cos(t) * sin(t));
		yy = ((GLfloat)j * sin(t) * sin(t));
		zz = ((GLfloat)j * cos(t));
	}
	else
	{
		//Let the butterflies follow your mouse cursor
		goCrazy = false;
		xx = (5.0 * rand() / (GLfloat)RAND_MAX) * cos(2 * t) + tarX;
		yy = (5.0 * rand() / (GLfloat)RAND_MAX) * sin(2 * t) + tarY;
		zz = (5.0 * rand() / (GLfloat)RAND_MAX) + zRange;
	}
	return Vector3(xx, yy, zz);
}


//Draw the butterfly object
void drawButterfly(GLfloat angle, GLfloat size, GLfloat red, GLfloat green, GLfloat blue)
{
	Vector3 BezCurve;
	GLfloat res = 10;
	material(red, green, blue, 1.0);
	glPushMatrix();
	glRotatef(angle, 0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	for (unsigned int k = 0; k < res * 2; k = k + 1)
	{
		GLfloat p = (GLfloat)k;
		if (k < res) {
			BezCurve = CalculateBezier(p / res, Vector3(0.0, -1.0, 0.0), Vector3(-1.5, -3.5, 0.0), Vector3(-2.5, -1.5, 0.0), Vector3(-1.0, -.5, 0.0));
			// Small wing left
		}
		else {
			BezCurve = CalculateBezier(((p - res) / res), Vector3(-1.0, -0.5, 0.0), Vector3(-4.0, 0.0, 0.0), Vector3(-1.5, 4.0, 0.0), Vector3(0.0, 1.0, 0.0));
			// Big wing left
		}
		glVertex3f(BezCurve.x()*size, BezCurve.y()*size, BezCurve.z()*size);
	}
	glVertex3f(0.0, 1.0*size, 0.0); // Close shape
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glRotatef(-angle, 0.0, 1.0, 0.0);
	glBegin(GL_POLYGON);
	for (unsigned int k = res * 2; k < res * 4; k = k + 1)
	{
		GLfloat p = (GLfloat)k;
		if (k < res * 3) {
			BezCurve = CalculateBezier(((p - 2.0 * res) / res), Vector3(0.0, 1.0, 0.0), Vector3(1.5, 4.0, 0.0), Vector3(4.0, 0.0, 0.0), Vector3(1.0, -.5, 0.0));
			// Big wing right
		}
		else {
			BezCurve = CalculateBezier(((p - 3.0 * res) / res), Vector3(1.0, -0.5, 0.0), Vector3(2.5, -1.5, 0.0), Vector3(1.5, -3.5, 0.0), Vector3(0.0, -1.0, 0.0));
			// Small wing right					
		}
		glVertex3f(BezCurve.x()*size, BezCurve.y()*size, BezCurve.z()*size);
	}
	glVertex3f(0.0, -1.0*size, 0.0); // Close shape
	glEnd();
	glPopMatrix();

	glBegin(GL_POLYGON);
	for (unsigned int k = res * 4; k < res * 6; k = k + 1)
	{
		GLfloat p = (GLfloat)k;
		if (k < res * 5) {
			BezCurve = CalculateBezier(((p - 4.0 * res) / res), Vector3(0.0, -2.0, 0.0), Vector3(0.5, -1.0, 0.0), Vector3(0.5, 1.0, 0.0), Vector3(0.0, 2.0, 0.0));
			// Body right
		}
		else if (k < res * 6) {
			BezCurve = CalculateBezier(((p - 5.0 * res) / res), Vector3(0.0, 2.0, 0.0), Vector3(-0.5, 1.0, 0.0), Vector3(-0.5, -1.0, 0.0), Vector3(0.0, -2.0, 0.0));
			// Body left
		}
		glVertex3f(BezCurve.x()*size, BezCurve.y()*size, BezCurve.z()*size);
	}
	glVertex3f(0.0, -2.0*size, 0.0); // Close shape
	glEnd();
}

void ResizeWindow(int w, int h) {

	//glClear(GL_ACCUM_BUFFER_BIT); //Prevent the screen from flickering in weird colours
	// Prevent division by 0
	if (h == 0) h = 1;
	if (w == 0) w = 1;

	GLfloat ratio;
	ratio = (GLfloat)w / (GLfloat)h;

	glMatrixMode(GL_PROJECTION); //set the matrix to projection        
	glLoadIdentity();

	glViewport(0, 0, w, h); //set the viewport to the current window specifications
	gluPerspective(60, ratio, 1.0, 1000.0); //set the perspective (angle of sight, width, height, , depth)

	glMatrixMode(GL_MODELVIEW); //set the matrix back to model
}

//Update the movement and positions of the particles or butterflies, get user input
void Update(GLint wdith, GLint height, UserInput input)
{
	unsigned int i;
	for (i = 0; i < MAXFIREWORKS; i = i + 1)
	{
		if (!fireworks[i].exploded) {
			fireworks[i].move();
		}
		else {
			fireworks[i].explode();
		}
	}

	if (input.keyIsDown(SDLK_UP)) { // up (viewer is moving up)
		g_posY = g_posY + 0.5;
	}
	if (input.keyIsDown(SDLK_DOWN)) { // down (viewer is moving down)
		g_posY = g_posY - 0.5;
	}
	if (input.keyIsDown(SDLK_LEFT)) { // left (viewer is moving left)
		g_orientation = g_orientation - 0.5;
	}
	if (input.keyIsDown(SDLK_RIGHT)) { // right (viewer is moving left)
		g_orientation = g_orientation + 0.5;
	}
	if (input.keyIsDown(SDLK_RSHIFT)) {
		if (input.keyIsDown(SDLK_UP)) { // forwards (viewer is moving forwards)
			g_posX = g_posX + sin(g_orientation / 180.0 * M_PI);
			g_posZ = g_posZ - cos(g_orientation / 180.0 * M_PI);
		}
		if (input.keyIsDown(SDLK_DOWN)) { // backwards (viewer is moving backwards)
			g_posX = g_posX - sin(g_orientation / 180.0 * M_PI);
			g_posZ = g_posZ + cos(g_orientation / 180.0 * M_PI);
		}
	}
	if (input.keyIsPressed(SDLK_b)) { // Butterfly effect
		if (!butterflyTrue) butterflyTrue = true;
		else butterflyTrue = false;
	}
	if (input.keyIsDown(SDL_BUTTON_LEFT)) {
		zRange += 5.5;
	}
	if (input.keyIsDown(SDL_BUTTON_RIGHT)) {
		zRange -= 2.5;
	}
}

//Draw the objects on the screen
void DrawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear the color buffer and the depth buffer

	if (!butterflyTrue) glAccum(GL_RETURN, 0.95f);
	glClear(GL_ACCUM_BUFFER_BIT); //Accumulate buffer is used for the firework trails

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(g_orientation, 0.0, 1.0, 0.0); // rotate in y axis
	glTranslatef(-g_posX, -g_posY, -g_posZ);

	glEnable(GL_DEPTH_TEST); //Working in 3D
	unsigned int i, j;
	for (i = 0; i < MAXFIREWORKS; i = i + 1)
	{
		for (j = 0; j < MAXPARTICLES; j = j + 1)
		{
			if (!fireworks[i].exploded) {
				GLfloat alpha = 1.0f - (GLfloat)(j / MAXPARTICLES);
				GLfloat x = fireworks[i].getX(j);
				GLfloat y = fireworks[i].getY(j);
				GLfloat z = fireworks[i].getZ(j);
				glPushMatrix();
				glTranslatef(x, y, z);
				material(1.0f, 1.0f, 0.0f, alpha);
				glutSolidSphere(0.15, 6, 6);
				glPopMatrix();
			}
			else
			{
				GLfloat x = fireworks[i].getX(j);
				GLfloat y = fireworks[i].getY(j);
				GLfloat z = fireworks[i].getZ(j);
				GLfloat red = fireworks[i].red;
				GLfloat green = fireworks[i].green;
				GLfloat blue = fireworks[i].blue;
				glPushMatrix();
				material(red, green, blue, fireworks[i].alpha);
				glTranslatef(x, y, z);
				if (butterflyTrue)
				{
					drawButterfly(fireworks[i].wings_angle[j], size[j], red, green, blue);
				}
				else
				{
					glutSolidSphere(0.05, 6, 6);
					glPushMatrix();
					material(red, green, blue, fireworks[i].alpha * 0.3);
					glutSolidSphere(0.1, 6, 6);
					glPopMatrix();
				}
				glPopMatrix();
			}
		}
	}

	//Setup 2D environment
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0, width, 0.0, height, -1.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glLoadIdentity();

	DrawInterface(width, height, butterflyTrue); //Draw text on the screen

	// Back to 3D!
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);

	SDL_GL_SwapBuffers();
	glAccum(GL_ACCUM, 0.9f);
}

//Setup the openGl environment
void init(void) {
	srand(time(NULL));

	glEnable(GL_NORMALIZE); //For lighting
	glShadeModel(GL_SMOOTH); //set the shader to smooth shader

							 // Enable blending (for using alpha comments)
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClearAccum(0.0f, 0.0f, 0.0f, 1.0f); // Set the accumulation buffer clearing colour to black at 0,0f alpha
	InitLight();
	glEnable(GL_DEPTH_TEST); //enable the depth testing
	glMatrixMode(GL_PROJECTION);
	gluPerspective(120.0, 1.0, 1.0, 1000.0);
}

//Intialize video and audio settings
bool initVideoAudio()
{
	//SDLinit(); /SDL initialization
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	//Initialize SDL_mixer
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}

	//set depth buffer to 24 bits for better precision
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	// Set up OpenGL for double buffering in SDL
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	//set window title
	SDL_WM_SetCaption("Butterflies", NULL);

	//create a window
	if (!SDL_SetVideoMode(width, height, 16, SDL_GL_DOUBLEBUFFER | SDL_OPENGL | SDL_RESIZABLE))
	{
		printf("Window not created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	return true;
}

//Main function
int main(int argc, char *argv[]) {

	bool done = false;
	UserInput input;

	
	if (!initVideoAudio()) //intialize the video and audio settings
	{
		std::cout << "False" << std::endl;
	}
	glutInit(&argc, argv);
	init(); //call the init function
	
	//Load and play the Music
	music = Mix_LoadMUS("Sounds/06_Ghosts_I.wav");
	if (music == NULL)
	{
		std::cout << "Music not found" << std::endl;
	}
	Mix_PlayMusic(music, -1); // In a loop

	/*
	//Load the Sound effects
	boom = Mix_LoadWAV("Sounds/explosion.wav");
	birds = Mix_LoadWAV("Sounds/birds031.wav");
	if ((boom == NULL) || (birds == NULL))
	{
		std::cout << "Effect not found" << std::endl;
		//done = true;
	}
	//Mix_VolumeChunk(birds, 30);
	//Mix_VolumeChunk(boom, 90);
	*/
	ResizeWindow(width, height);

	//Start the control loop
	while (!done)
	{
		if (input.keyIsPressed(SDLK_ESCAPE))
		{
			done = true;
			break;
		}
		if (input.keyIsPressed(SDLK_q))
		{
			done = true;
			break;
		}

		input.keyResetPress();

		//Handle User input, using SDL events
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				done = true;
				break;
			case SDL_KEYDOWN:
				input.keyDown(event.key.keysym.sym);
				break;
			case SDL_KEYUP:
				input.keyUp(event.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				input.keyDown(event.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				input.keyUp(event.button.button);
				break;
			case SDL_VIDEORESIZE:
				width = event.resize.w;
				height = event.resize.h;
		//		SDL_SetVideoMode(width, height, 32, SDL_GL_DOUBLEBUFFER | SDL_OPENGL | SDL_RESIZABLE);
		//		ResizeWindow(width, height);
				break;
			default:
				break;
			}
		}

		Update(width, height, input);
		ResizeWindow(width, height);
		DrawScene(); // Draw everyting on the screen
		//SDL_Delay(16); // Delay framerate to 60fps

	} // End of While !done loop
	
	  //Clean up
//	Mix_FreeChunk(birds);
//	birds = NULL;
//	Mix_FreeChunk(boom);
//	boom = NULL;

	Mix_FreeMusic(music);
	music = NULL;
	Mix_Quit();
	SDL_Quit();

	return 0; //Done!
}