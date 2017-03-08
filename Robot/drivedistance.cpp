#include "drivedistance.h"

DriveDistance::DriveDistance(float distance)
{
    Distance = distance;
}

void DriveDistance::Init(Robot& robot)
{
}

bool DriveDistance::Run(Robot& robot)
{
    robot.DriveStraight(Distance, 45);
    return true;
}

void DriveDistance::Finish(Robot& robot)
{
}

bool DriveDistance::isEnd()
{
    return false;
}
