#include "readlight.h"

ReadLight::ReadLight()
{
}

void ReadLight::Init(Robot& robot)
{
}

bool ReadLight::Run(Robot& robot)
{
    if(robot.GetCdSCellValue() < .8)
    {
        robot.RedLight = true;
    }
    else
    {
        robot.RedLight = false;
    }
    return true;
}

void ReadLight::Finish(Robot& robot)
{
}

bool ReadLight::isEnd()
{
    return false;
}
