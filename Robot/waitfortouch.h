#ifndef WAITFORTOUCH_H
#define WAITFORTOUCH_H

#include "task.h"
#include <FEHLCD.h>

class WaitForTouch : public Task
{
public:
    WaitForTouch();
    void Init(Robot&);
    bool Run(Robot&);
    void Finish(Robot&);
    bool isEnd();
private:
    bool touched;
    float x, y;
};

#endif // WAITFORTOUCH_H
