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

#define MIMIMUM_POWER 15

#define POWER_RATIO 1
#define DEGREE_RATIO 1


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
    percent *= POWER_RATIO;

    int counts = distance * COUNT_RATIO;

    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    int current_counts = 0;
    while(current_counts < counts)
    {
        int right_counts = right_encoder.Counts();
        int left_counts = left_encoder.Counts();
        current_counts = (left_counts + right_counts) / 2.;
        float percent_complete = (float) current_counts / counts;
        float power_multiplier = exp(-(3*percent_complete-1)*(3*percent_complete-1));
        float power = power_multiplier * percent;

        if (power < MIMIMUM_POWER && power > 0)
        {
            power = MIMIMUM_POWER;
        }
        if (power > -MIMIMUM_POWER && power < 0)
        {
            power = -MIMIMUM_POWER;
        }

        power = percent;

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
    percent *= POWER_RATIO;
    degrees *= DEGREE_RATIO;

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

        float percent_complete = (float) current_counts / counts;
        float power_multiplier = exp(-(3*percent_complete-1)*(3*percent_complete-1));
        float power = power_multiplier * percent;
        if (power < MIMIMUM_POWER && power > 0)
        {
            power = MIMIMUM_POWER;
        }
        if (power > -MIMIMUM_POWER && power < 0)
        {
            power = -MIMIMUM_POWER;
        }

        power = percent;

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
    LCD.Clear();

    // Wait until the screen is touched.
    float x, y;
    while (!LCD.Touch(&x, &y));
    while (LCD.Touch(&x, &y));

    /*drive_backward(-20,.5);
    drive_backward(-50,.5);
    drive_backward(-70,3);
    right_motor.Stop();
    left_motor.Stop();*/

    LCD.WriteLine("Start!");

    Sleep(1.0);

    LCD.WriteLine("Waiting ...");

    float previous_cds_value = cds_cell.Value();
    while(previous_cds_value - cds_cell.Value() < 0.3)
    {
        previous_cds_value = cds_cell.Value();
        Sleep(.2);
    }

    encoder_drive(10, -20);
    Sleep(1.0);
    encoder_turn_in_place(-90, 20);
    Sleep(1.0);
    encoder_drive(13, -20);
    Sleep(1.0);
    encoder_turn_in_place(-92, 20);
    Sleep(2.0);
    // Drive up ramp
    encoder_drive(28, -40);
    Sleep(1.0);
    encoder_turn_in_place(90, 20);
    Sleep(1.0);
    encoder_drive(15, -25);
    Sleep(1.0);
    encoder_drive(5, 15);
    Sleep(1.0);
    encoder_turn_in_place(-90, 20);
    Sleep(1.0);
    encoder_drive(15, -40);
    right_motor.SetPercent(40);
    left_motor.SetPercent(-40);
    Sleep(7.0);
    right_motor.Stop();
    left_motor.Stop();
    Sleep(1.0);
    encoder_drive(16, 20);
    Sleep(1.0);
    encoder_turn_in_place(-90, 20);
    Sleep(1.0);
    encoder_drive(40, -20);


    return 0;
}
