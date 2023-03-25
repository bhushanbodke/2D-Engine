#include "Mouse.h"

 double Mouse::x = 0 ;
 double Mouse::y = 0 ;

 double Mouse::lastx= 0;
 double Mouse::lasty= 0;
 
 double Mouse::dx= 0;
 double Mouse::dy= 0;

 double Mouse::scrolldx = 0;
 double Mouse::scrolldy = 0;

 bool Mouse::FirstMouse = true;

 bool Mouse::buttons[GLFW_MOUSE_BUTTON_LAST] = {0};
 bool Mouse::buttonsChanged[GLFW_MOUSE_BUTTON_LAST] = {0};

void Mouse::MouseCallback(GLFWwindow* window, double _x, double _y)
{
    x = _x;
    y = _y;
    if (FirstMouse)
    {
        lastx = x;
        lasty = y;
        FirstMouse = false;
    }
    dx = x - lastx; 
    dy = lasty - y;
    lastx = x;
    lasty  = y;
}

void Mouse::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (action != GLFW_RELEASE)
    {
        if (!buttons[button])
        {
            buttons[button] = true;
        }
    }
    else
    {
        buttons[button] = false;
    }
    buttonsChanged[button] = action != GLFW_REPEAT;
}

void Mouse::MouseWheelCallback(GLFWwindow* window, double dx, double dy)
{
    scrolldx = dx;
    scrolldy = dy;
}

double Mouse::GetMouseX()
{
    return x;
}

double Mouse::GetMouseY()
{
    return y;
}

double Mouse::GetDX()
{
    double temp = dx;
    dx = 0;
    return temp;
}

double Mouse::GetDY()
{
    double temp = dy;
    dy = 0;
    return temp;
}

double Mouse::GetScrollDX()
{
    double temp = scrolldx;
    scrolldx = 0;
    return temp;
}

double Mouse::GetScrollDY()
{
    double temp = scrolldy;
    scrolldy = 0;
    return temp;
}

bool Mouse::button(int button)
{
    return buttons[button];
}

bool Mouse::buttonChange(int button)
{
    bool temp = buttonsChanged[button];
    buttonsChanged[button] = false;
    return temp;
}

bool Mouse::buttonWentUp(int button)
{
    return !buttons[button] && buttonChange(button);
}

bool Mouse::buttonWentDown(int button)
{
    return buttons[button] && buttonChange(button);
}
