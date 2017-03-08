#define PI 3.14159265
#define COUNTS_PER_INCH 33.7408479355
#define COUNTS_PER_DEGREE 2.09055555556

#define RM -1
#define LM 1
#define LCM 1
#define RCM 1.05
#define CORRECTION_MULTIPLIER .75

#include "robot.h"
#include <math.h>

Robot::Robot()
    : RightMotor(FEHMotor::Motor1, 12.0),
      LeftMotor(FEHMotor::Motor0, 12.0),
      Servo(FEHServo::Servo7),
      RightEncoder(FEHIO::P0_0),
      LeftEncoder(FEHIO::P0_2),
      RightOptosensor(FEHIO::P1_1),
      MiddleOptosensor(FEHIO::P1_2),
      LeftOptosensor(FEHIO::P1_4),
      CdSCell(FEHIO::P1_6),
      SkidBumper(FEHIO::P2_0),
      BLBumper(FEHIO::P2_2),
      BRBumper(FEHIO::P2_4),
      FLBumper(FEHIO::P2_6),
      ArmBumper(FEHIO::P3_0),
      SideBumper(FEHIO::P3_2)
{
    RedLight = true;
}

void Robot::DriveStraight(float inches, float percent)
{
    percent *= -1;
    if (inches < 0)
    {
        inches *= -1;
        percent *= -1;
    }
    RightEncoder.ResetCounts();
    LeftEncoder.ResetCounts();

    int counts = COUNTS_PER_INCH * inches;

    int r_counts = RightEncoder.Counts() * RCM;
    int l_counts = LeftEncoder.Counts() * LCM;

    while (r_counts + l_counts < counts * 2)
    {
        r_counts = RightEncoder.Counts() * RCM;
        l_counts = LeftEncoder.Counts() * LCM;

        float r_percent = percent;
        float l_percent = percent;

        if (r_counts < l_counts)
        {
            l_percent *= CORRECTION_MULTIPLIER;
        }
        else if (l_counts < r_counts)
        {
            r_percent *= CORRECTION_MULTIPLIER;
        }

        RightMotor.SetPercent(RM * r_percent);
        LeftMotor.SetPercent(LM * l_percent);
    }
    RightMotor.Stop();
    LeftMotor.Stop();
}

void Robot::Turn(float degrees, float percent)
{
    if (degrees < 0)
    {
        degrees *= -1;
        percent *= -1;
    }
    RightEncoder.ResetCounts();
    LeftEncoder.ResetCounts();

    int counts = COUNTS_PER_DEGREE * degrees;

    int r_counts = RightEncoder.Counts() * RCM;
    int l_counts = LeftEncoder.Counts() * LCM;

    while (r_counts + l_counts < counts * 2)
    {
        r_counts = RightEncoder.Counts() * RCM;
        l_counts = LeftEncoder.Counts() * LCM;

        float r_percent = percent;
        float l_percent = percent;

        if (r_counts < l_counts)
        {
            l_percent *= CORRECTION_MULTIPLIER;
        }
        else if (l_counts < r_counts)
        {
            r_percent *= CORRECTION_MULTIPLIER;
        }

        RightMotor.SetPercent(RM * r_percent);
        LeftMotor.SetPercent(-LM * l_percent);
    }
    RightMotor.Stop();
    LeftMotor.Stop();
}

float Robot::GetX()
{
    return X;
}

float Robot::GetY()
{
    return Y;
}

float Robot::GetHeading()
{
    return Heading;
}

float Robot::GetCdSCellValue()
{
    return CdSCellVallue;
}

void Robot::Update()
{
    X = RPS.X();
    Y = RPS.Y();
    Heading = RPS.Heading();
    float radians = RPS.Heading() * PI/180.0;
    float dx = cos(radians);
    float dy = sin(radians);
    X -= 7 * dx;
    Y -= 7 * dy;
    CdSCellVallue = CdSCell.Value();
}
