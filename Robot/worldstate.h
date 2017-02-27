#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include <FEHIO.h>

class WorldState
{
    public:
        WorldState(DigitalEncoder, DigitalEncoder, DigitalInputPin, DigitalInputPin, DigitalInputPin, DigitalInputPin, DigitalInputPin, DigitalInputPin, AnalogInputPin);
        void Update();
        void LogReport(bool);
        bool SelfCheck(int, int, bool, bool, bool, bool, bool, bool, float, float);

        int GetRIEncoderCounts();
        int GetLIEncoderCounts();
        bool GetFRBumperStatus();
        bool GetFLBumperStatus();
        bool GetBRBumperStatus();
        bool GetBLBumperStatus();
        bool GetSideBumperStatus();
        bool GetArmBumperStatus();
        bool GetCDSCellValue();

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

        int RIEncoderCounts;
        int LIEncoderCounts;
        bool FRBumperStatus;
        bool FLBumperStatus;
        bool BRBumperStatus;
        bool BLBumperStatus;
        bool SideBumperStatus;
        bool ArmBumperStatus;
        float CDSCellValue;

};

#endif // WORLDSTATE_H
