#include "waitfortouch.h"

WaitForTouch::WaitForTouch()
{
    touched = false;
}

void WaitForTouch::Init(Robot& robot)
{
    LCD.WriteLine("Waiting for Touch ...");
}

bool WaitForTouch::Run(Robot& robot)
{
    if (touched)
    {
        if (!LCD.Touch(&x, &y))
        {
            return true;
        }
    }
    else
    {
        if (LCD.Touch(&x, &y))
        {
            touched = true;
        }
    }
    return false;
}

void WaitForTouch::Finish(Robot& robot)
{
    LCD.WriteLine("Touched.");
}

bool WaitForTouch::isEnd()
{
    return false;
}
