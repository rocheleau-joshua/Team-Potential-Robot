#ifndef READLIGHT_H
#define READLIGHT_H

#include "task.h"

class ReadLight : public Task
{
public:
    ReadLight();
    void Init(Robot&);
    bool Run(Robot&);
    void Finish(Robot&);
    bool isEnd();
};

#endif // READLIGHT_H
