#include "KeyBoard.h"

bool KeyBoard::keys[GLFW_KEY_LAST] = { 0 };
bool KeyBoard::keysChanged[GLFW_KEY_LAST] = { 0 };

void KeyBoard::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_RELEASE)
	{
		if (!keys[key])
		{
			keys[key] = true;
		}
	}
	else
	{
		keys[key] = false;
	}
	// if action is repeat then key changed is false otherwise true
	keysChanged[key] = action != GLFW_REPEAT;
}

bool KeyBoard::key(int key)
{
	return keys[key];
}

bool KeyBoard::keyChanged(int key)
{
	bool temp = keysChanged[key];
	keysChanged[key] = false;
	return temp;
}

bool KeyBoard::keyWentUp(int key)
{
	return !keys[key] && keyChanged(key);
}

bool KeyBoard::keyWentDown(int key)
{
	return keys[key] && keyChanged(key);
}
