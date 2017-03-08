#define PI 3.14159265
#define RM -1
#define LM 1

#include "drive.h"
#include <math.h>

Drive::Drive(float x, float y)
{
    X = x;
    Y = y;
}

void Drive::Init(Robot& robot) {
    float dx = X - robot.GetX();
    float dy = Y - robot.GetY();
    float desired_heading = atan2f(dy, dx) * 180 / PI;
    if (desired_heading <= -180)
    {
        desired_heading += 360;
    }
    float heading_difference = desired_heading - robot.GetHeading();
    if (heading_difference > 180)
    {
        heading_difference -= 360;
    }
    if (heading_difference <= -180)
    {
        heading_difference += 360;
    }
    robot.Turn(heading_difference, 20);
}

bool Drive::Run(Robot& robot)
{
    float dx = X - robot.GetX();
    float dy = Y - robot.GetY();

    if (dx*dx + dy*dy < 2)
    {
        return true;
    }

    float desired_heading = atan2f(dy, dx);
    if (desired_heading <= -PI)
    {
        desired_heading += 2*PI;
    }
    float heading_difference = desired_heading - (robot.GetHeading() * PI/180.0);
    if (heading_difference > PI)
    {
        heading_difference -= 2*PI;
    }
    if (heading_difference <= -PI)
    {
        heading_difference += 2*PI;
    }

    float r_mult = 1;
    float l_mult = 1;

    if (heading_difference < PI/2.0 && heading_difference > -PI/2.0)
    {
        float percentageShift = pow(cos(heading_difference*2.0),3);
        if (heading_difference < 0)
        {
            r_mult = percentageShift;
        }
        else
        {
            l_mult = percentageShift;
        }
    }
    else
    {
        if (heading_difference < 0)
        {
            r_mult = -1;
        }
        else
        {
            l_mult = -1;
        }
    }

    LCD.WriteLine(heading_difference * 180 / PI);

    robot.RightMotor.SetPercent(RM * 40 * r_mult);
    robot.LeftMotor.SetPercent(LM * 40 * l_mult);

    return false;
}

void Drive::Finish(Robot& robot)
{
    robot.RightMotor.Stop();
    robot.LeftMotor.Stop();
}

bool Drive::isEnd()
{
    return false;
}
