#ifndef DRIVEDISTANCE_H
#define DRIVEDISTANCE_H

#include "task.h"

class DriveDistance : public Task
{
public:
    DriveDistance(float);
    void Init(Robot&);
    bool Run(Robot&);
    void Finish(Robot&);
    bool isEnd();
private:
    float Distance;
};

#endif // DRIVEDISTANCE_H
