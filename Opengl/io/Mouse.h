#pragma once 

#include<glad/glad.h>
#include<GLFW/glfw3.h>

class Mouse
{
public:
	static  void MouseCallback(GLFWwindow* window, double _x, double _y);
	static  void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static  void MouseWheelCallback(GLFWwindow* window, double dx, double dy);

	static  double GetMouseX();
	static  double GetMouseY();

	static  double GetDX();
	static  double GetDY();

	static  double GetScrollDX();
	static  double GetScrollDY();

	static bool button(int button);
	static bool buttonChange(int button);

	static bool buttonWentUp(int button);
	static bool buttonWentDown(int button);
private:
	static double x;
	static double y;

	static double lastx;
	static double lasty;

	static double dx;
	static double dy;

	static double scrolldx;
	static double scrolldy;

	static bool FirstMouse;

	static bool buttons[];
	static bool buttonsChanged[];


};