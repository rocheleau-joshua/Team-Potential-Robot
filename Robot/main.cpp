#include <robot.h>
#include <math.h>
#include <task.h>
#include <drive.h>
#include <waitfortouch.h>
#include <waitforlight.h>
#include <end.h>
#include <orient.h>
#include <moveservo.h>
#include <drivedistance.h>
#include <readlight.h>

#define SERVOMIN 500
#define SERVOMAX 2286

#define CDSMIN 0.0
#define CDSMAX 3.3

#define MIMIMUM_POWER 15

#define POWER_RATIO 1
#define DEGREE_RATIO 1

Robot robot;

int main(void)
{
    RPS.InitializeTouchMenu();

    LCD.Clear();

    while (false)
    {
        robot.Update();
        LCD.Clear();
        LCD.WriteLine(robot.GetX());
        LCD.WriteLine(robot.GetY());
        LCD.WriteLine(robot.GetHeading());
        Sleep(.1);
    }

    Task* tasks[] = {
        new MoveServo(180),
        new WaitForTouch(),
        //new WaitForLight(),
        new Drive(6, 23),
        new Orient(0),
        new ReadLight(),
        new Drive(18, 20),
        new Drive(20, 50),
        new Orient(320),
        new MoveServo(85),
        new DriveDistance(20),
        new DriveDistance(-20),
        new MoveServo(180),
        new Drive(16, 22),
        new Orient(90),
        new DriveDistance(10),
        new MoveServo(85),
        new DriveDistance(-10),
        new MoveServo(180),
        new Drive(6, 20),
        new Drive(5, 25),
        new End()};

    Task* task = tasks[0];
    for (int task_number = 0; !task->isEnd(); task_number++)
    {
        task = tasks[task_number];

        robot.Update();
        task->Init(robot);

        robot.Update();
        while (!task->Run(robot))
        {
            robot.Update();
        }

        robot.Update();
        task->Finish(robot);

        Sleep(1.0);
    }

    return 0;
}
