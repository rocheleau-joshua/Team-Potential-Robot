#ifndef DRIVE_H
#define DRIVE_H

#include <FEHLCD.h>
#include <FEHMotor.h>
#include "task.h"

class Drive : public Task
{
public:
    Drive(float, float);
    void Init(Robot& robot);
    bool Run(Robot& robot);
    void Finish(Robot& robot);
    bool isEnd();
private:
    float X, Y;
};

#endif // DRIVE_H
