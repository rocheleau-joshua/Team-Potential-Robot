#ifndef TASK_H
#define TASK_H

#include <robot.h>

class Task
{
public:
    virtual bool Run(Robot&) = 0;
    virtual void Init(Robot&) = 0;
    virtual void Finish(Robot&) = 0;
    virtual bool isEnd() = 0;
};

#endif // TASK_H
