#ifndef ROBOT_H
#define ROBOT_H

#include <FEHMotor.h>
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHRPS.h>
#include <worldstate.h>

class Robot
{
public:
    Robot();
    void DriveStraight(float, float);
    void Turn(float, float);
    float GetX();
    float GetY();
    float GetHeading();
    float GetCdSCellValue();
    void Update();
    FEHMotor RightMotor;
    FEHMotor LeftMotor;
    FEHServo Servo;
    DigitalEncoder RightEncoder;
    DigitalEncoder LeftEncoder;
    AnalogInputPin CdSCell;
    AnalogInputPin LeftOptosensor;
    AnalogInputPin MiddleOptosensor;
    AnalogInputPin RightOptosensor;
    DigitalInputPin SkidBumper;
    DigitalInputPin BLBumper;
    DigitalInputPin BRBumper;
    DigitalInputPin FLBumper;
    DigitalInputPin ArmBumper;
    DigitalInputPin SideBumper;
    bool RedLight;
private:
    float X;
    float Y;
    float Heading;
    float CdSCellVallue;
};

#endif // ROBOT_H
