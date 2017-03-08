#ifndef END_H
#define END_H

#include "task.h"

class End : public Task
{
public:
    End();
    void Init(Robot&);
    bool Run(Robot&);
    void Finish(Robot&);
    bool isEnd();
private:
    bool touched;
    int x, y;
};

#endif // END_H
