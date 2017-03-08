#ifndef WAITFORLIGHT_H
#define WAITFORLIGHT_H

#include "task.h"

class WaitForLight : public Task
{
public:
    WaitForLight();
    void Init(Robot&);
    bool Run(Robot&);
    void Finish(Robot&);
    bool isEnd();
private:
    float PreviousValue;
};

#endif // WAITFORLIGHT_H
