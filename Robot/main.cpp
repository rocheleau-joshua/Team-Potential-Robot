#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHRPS.h>
#include <math.h>

#define SERVOMIN 500
#define SERVOMAX 2286

#define CDSMIN 0.0
#define CDSMAX 3.3

FEHMotor right_motor(FEHMotor::Motor3, 12.0);
FEHMotor left_motor(FEHMotor::Motor0, 12.0);

FEHServo servo(FEHServo::Servo0);

DigitalInputPin front_right_bumper(FEHIO::P0_0);
DigitalInputPin front_left_bumper(FEHIO::P0_1);
DigitalInputPin back_right_bumper(FEHIO::P0_2);
DigitalInputPin back_left_bumper(FEHIO::P0_3);

AnalogInputPin cds_cell(FEHIO::P1_0);

void drive_forward(int percent)
{
    right_motor.SetPercent(percent * .95);
    left_motor.SetPercent(percent);
    while (front_right_bumper.Value() || front_left_bumper.Value());
    right_motor.Stop();
    left_motor.Stop();
    Sleep(.25);
}

void drive_backward(int percent, float time)
{
    right_motor.SetPercent(-percent * .95);
    left_motor.SetPercent(-percent);
    Sleep(time);
    right_motor.Stop();
    left_motor.Stop();
    Sleep(.25);
}

void turn_backward(int right_percent, int left_percent)
{
    right_motor.SetPercent(right_percent);
    left_motor.SetPercent(left_percent);
    while (back_right_bumper.Value() || back_left_bumper.Value());
    right_motor.Stop();
    left_motor.Stop();
    Sleep(.25);
}

float go_to(float x_target, float y_target, float precision)
{
    float x = RPS.X();
    float y = RPS.Y();
    while (pow(x - x_target, 2) + pow(y - y_target, 2) < pow(precision, 2))
    {
        x = RPS.X();
        y = RPS.Y();

    }

    return pow(x - x_target, 2) + pow(y - y_target, 2) < pow(precision, 2);
}

int main(void)
{
    // Initialize RPS setup process.
    RPS.InitializeTouchMenu();

    // Wait until the screen is touched.
    float x, y;
    while (!LCD.Touch(&x, &y));

    drive_forward(50);

    return 0;
}
