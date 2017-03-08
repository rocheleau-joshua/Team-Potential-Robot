#include "moveservo.h"

MoveServo::MoveServo(float degree)
{
    Degree = degree;
}

void MoveServo::Init(Robot& robot)
{
}

bool MoveServo::Run(Robot& robot)
{
    robot.Servo.SetDegree(Degree);
    return true;
}

void MoveServo::Finish(Robot& robot)
{
}

bool MoveServo::isEnd()
{
    return false;
}
