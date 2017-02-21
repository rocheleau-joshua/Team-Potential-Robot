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

#define ENCODER_DRIVE 0
#define BUTTON_DRIVE 1
#define RPS_DRIVE 2
#define BUTTON_PRESS 3
#define WAIT_FOR_LIGHT 4

#define PI 3.14159265
#define COUNT_RATIO 33.7408479355
#define COUNTS_PER_DEGREE 2.09055555556

int commands[] = {WAIT_FOR_LIGHT};

FEHMotor right_motor(FEHMotor::Motor0, 12.0);
FEHMotor left_motor(FEHMotor::Motor1, 12.0);

FEHServo servo(FEHServo::Servo0);

DigitalInputPin front_right_bumper(FEHIO::P1_0);
DigitalInputPin front_left_bumper(FEHIO::P1_1);
DigitalInputPin back_right_bumper(FEHIO::P1_2);
DigitalInputPin back_left_bumper(FEHIO::P1_3);

DigitalEncoder right_encoder(FEHIO::P0_2);
DigitalEncoder left_encoder(FEHIO::P0_3);

AnalogInputPin cds_cell(FEHIO::P0_0);

void drive_forward(int percent)
{
    right_motor.SetPercent(-percent);
    left_motor.SetPercent(percent);
    while (front_right_bumper.Value() || front_left_bumper.Value());
    right_motor.Stop();
    left_motor.Stop();
    Sleep(.25);
}

void drive_backward(int percent, float time)
{
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-percent);
    Sleep(time);
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

void encoder_drive(float distance, float percent)
{
    int counts = distance * COUNT_RATIO;

    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    int current_counts = 0;
    while(current_counts < counts)
    {
        int right_counts = right_encoder.Counts();
        int left_counts = left_encoder.Counts();
        current_counts = (left_counts + right_counts) / 2.;
        float percent_complete = current_counts / counts;
        float power_multiplier = exp(-(3*percent_complete-1)*(3*percent_complete-1));
        float power = power_multiplier * percent;

        if (power < 10 && power > 0)
        {
            power = 10;
        }
        if (power > -10 && power < 0)
        {
            power = -10;
        }

        LCD.WriteLine(power);

        if (left_counts > right_counts)
        {
            left_motor.SetPercent(power * .9);
        }
        else
        {
            left_motor.SetPercent(power);
        }

        if (right_counts > left_counts)
        {
            right_motor.SetPercent(-power * .9);
        }
        else
        {
            right_motor.SetPercent(-power);
        }
    }

    right_motor.Stop();
    left_motor.Stop();
}

void encoder_turn_in_place(float degrees, float percent)
{
    float counts = COUNTS_PER_DEGREE * degrees;
    if (counts < 0)
    {
        counts *= -1;
    }

    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    int current_counts = 0;
    while(current_counts < counts)
    {
        int right_counts = right_encoder.Counts();
        int left_counts = left_encoder.Counts();
        current_counts = (left_counts + right_counts) / 2.;

        float percent_complete = current_counts / counts;
        float power_multiplier = exp(-(3*percent_complete-1)*(3*percent_complete-1));
        float power = power_multiplier * percent;
        if (power < 10 && power > 0)
        {
            power = 10;
        }
        if (power > -10 && power < 0)
        {
            power = -10;
        }

        if (degrees < 0)
        {
            power *= -1;
        }

        if (left_counts > right_counts)
        {
            left_motor.SetPercent(power * .9);
        }
        else
        {
            left_motor.SetPercent(power);
        }

        if (right_counts > left_counts)
        {
            right_motor.SetPercent(power * .9);
        }
        else
        {
            right_motor.SetPercent(power);
        }
    }

    right_motor.Stop();
    left_motor.Stop();
}


int main(void)
{
    // Initialize RPS setup process.
    //RPS.InitializeTouchMenu();

    // Wait until the screen is touched.
    float x, y;
    while (!LCD.Touch(&x, &y));

    /*drive_backward(-20,.5);
    drive_backward(-50,.5);
    drive_backward(-70,3);
    right_motor.Stop();
    left_motor.Stop();*/

    encoder_drive(24, 100);
    Sleep(1);
    encoder_turn_in_place(90, 50);
    Sleep(1);
    encoder_drive(12, 100);

    return 0;
}
