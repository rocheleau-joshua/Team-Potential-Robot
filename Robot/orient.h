#ifndef ORIENT_H
#define ORIENT_H

#include <FEHLCD.h>
#include <FEHMotor.h>
#include "task.h"
#include <math.h>

class Orient : public Task
{
public:
    Orient(float);
    void Init(Robot& robot);
    bool Run(Robot& robot);
    void Finish(Robot& robot);
    bool isEnd();
private:
    float Orientation;
};
#endif // ORIENT_H
