#ifndef SENSOR_H
#define SENSOR_H

#include <component.h>

class Sensor : public Component
{
public:
    virtual void UpdateValue() = 0;
};

#endif // SENSOR_H
