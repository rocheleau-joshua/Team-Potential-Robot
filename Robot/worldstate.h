#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include <FEHIO.h>

class WorldState
{
    public:
        WorldState(DigitalEncoder, DigitalEncoder, DigitalInputPin, DigitalInputPin, DigitalInputPin, DigitalInputPin, DigitalInputPin, DigitalInputPin, AnalogInputPin);
        void LogReport(bool);
        bool SelfCheck(int, int, bool, bool, bool, bool, bool, bool, float, float);

    private:
        DigitalEncoder RIEncoder;
        DigitalEncoder LIEncoder;
        DigitalInputPin FRBumper;
        DigitalInputPin FLBumper;
        DigitalInputPin BRBumper;
        DigitalInputPin BLBumper;
        DigitalInputPin SideBumper;
        DigitalInputPin ArmBumper;
        AnalogInputPin CDSCell;
};

#endif // WORLDSTATE_H

