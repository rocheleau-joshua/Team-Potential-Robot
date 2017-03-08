#include "waitforlight.h"

WaitForLight::WaitForLight()
{
    PreviousValue = 0;
}

void WaitForLight::Init(Robot& robot)
{
    PreviousValue = robot.GetCdSCellValue();
}

bool WaitForLight::Run(Robot& robot)
{
    Sleep(.2);
    bool start = PreviousValue - robot.GetCdSCellValue() > .3;
    PreviousValue = robot.GetCdSCellValue();
    return start;
}

void WaitForLight::Finish(Robot& robot)
{
}

bool WaitForLight::isEnd()
{
    return false;
}
