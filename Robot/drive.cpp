#include "drive.h"

Drive::Drive(int n)
{
    test_number = n;
}

bool Drive::Run()
{
    LCD.Write("Running Drive");
    LCD.WriteLine(test_number);
    return true;
}
