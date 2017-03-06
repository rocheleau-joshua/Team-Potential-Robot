#ifndef DRIVE_H
#define DRIVE_H

#include <FEHLCD.h>
#include "task.h"

class Drive : public Task
{
public:
    Drive(int);
    bool Run();
private:
    int test_number;
};

#endif // DRIVE_H
