#ifndef USERINPUT_H
#define USERINPUT_H

class UserInput
{
public:
	UserInput();

	void keyDown(int key);
	void keyUp(int key);
	void mouseButtonDown();
	void mouseButtonUp();

	bool keyIsPressed(int key);
	bool keyIsDown(int key);

	void keyResetPress();

private:
	bool keydown[SDLK_LAST];
	bool keypress[SDLK_LAST];

	bool mousedrag;
};

#endif