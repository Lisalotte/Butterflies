#include "global.h"
#include "UserInput.h"


UserInput::UserInput() : mousedrag(false)
{
	int i;
	for (i = 0; i < SDLK_LAST; i++)
	{
		keydown[i] = false;
		keypress[i] = false;
	}
}

void UserInput::keyDown(int key)
{
	keydown[key] = true;
	keypress[key] = true;
}

void UserInput::mouseButtonDown()
{
	mousedrag = true;
}
void UserInput::mouseButtonUp()
{
	mousedrag = false;
}

void UserInput::keyUp(int key)
{
	keydown[key] = false;
}

bool UserInput::keyIsPressed(int key)
{
	if (keypress[key])
	{
		keypress[key] = false;
		return true;
	}
	return false;
}
bool UserInput::keyIsDown(int key)
{
	return keydown[key];
}

void UserInput::keyResetPress()
{
	int i;
	for (i = 0; i < SDLK_LAST; i++)
	{
		keypress[i] = false;
	}
}
