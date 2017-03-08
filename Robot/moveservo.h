#ifndef MOVESERVO_H
#define MOVESERVO_H

#include "task.h"

class MoveServo : public Task
{
public:
    MoveServo(float);
    void Init(Robot&);
    bool Run(Robot&);
    void Finish(Robot&);
    bool isEnd();
private:
    float Degree;
};

#endif // MOVESERVO_H
