#include <FEHUtility.h>
#include <FEHLCD.h>
#include <FEHSD.h>
#include <FEHRPS.h>
#include <FEHBuzzer.h>

#include "worldstate.h"

using namespace std;

/**
 * @brief WorldState::WorldState Constructor for the World State object
 * @param riEncoder The right Igwan encoder
 * @param liEncoder The left Igwan encoder
 * @param frBumper The front right bumper switch
 * @param flBumper The front left bumper switch
 * @param brBumper The back right bumper switch
 * @param blBumper The back left bumper switch
 * @param sideBumper The side bumper switch
 * @param armBumper The arm bumper switch
 * @param cdsCell The CDS cell
 */
WorldState::WorldState(
        DigitalEncoder riEncoder, DigitalEncoder liEncoder, DigitalInputPin frBumper,
        DigitalInputPin flBumper, DigitalInputPin brBumper, DigitalInputPin blBumper,
        DigitalInputPin sideBumper, DigitalInputPin armBumper, AnalogInputPin cdsCell) :
        RIEncoder(riEncoder), LIEncoder(liEncoder),FRBumper(frBumper), FLBumper(flBumper), BRBumper(brBumper),
        BLBumper(blBumper), SideBumper(sideBumper), ArmBumper(armBumper), CDSCell(cdsCell)
        {
            // Open log and write out the header
            SD.OpenLog();
            SD.Printf("Run Time    |Rt IGAWN    |Lf IGWAN  |FR Bumper  |FL Bumper  |BR Bumper  |BL Bumper  |Sd Bumper  |Ar Bumper  |CDS Cell   \n\
               ------------------------------------------------------------------------------------------------------------------------\n");
        }

/**
 * @brief WorldState::LogReport Prints all WorldState data to a log file.
 * @param printToScreen If true, the report will be printed to the proteus screen as well as the log file.
 */
void WorldState::LogReport(bool printToScreen) {

    char logRow[] = "%f\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%f\t\n";

    // Write row out to the log
    SD.Printf(logRow,
              TimeNow(), RIEncoder.Counts(), LIEncoder.Counts(),
              FRBumper.Value(), FLBumper.Value(), BRBumper.Value(), BLBumper.Value(),
              SideBumper.Value(), ArmBumper.Value(), CDSCell.Value());

    // Print to screen
    if (printToScreen) {
        LCD.Write("********* WORLD STATE at %f seconds *********\n\n");
        LCD.Write("\tRt IGWAN: ");
        LCD.Write(RIEncoder.Counts());
        LCD.Write("\n\tLf IGWAN: ");
        LCD.Write(LIEncoder.Counts());
        LCD.Write("\n\tFR Bumper: ");
        LCD.Write(FRBumper.Value());
        LCD.Write("\n\tFL Bumper: ");
        LCD.Write(FLBumper.Value());
        LCD.Write("\n\tBR Bumper: ");
        LCD.Write(BRBumper.Value());
        LCD.Write("\n\tBL Bumper: ");
        LCD.Write(BLBumper.Value());
        LCD.Write("\n\tSd Bumper: ");
        LCD.Write(SideBumper.Value());
        LCD.Write("\n\tAr Bumper: ");
        LCD.Write(ArmBumper.Value());
        LCD.Write("\n\tCDS Cell: ");
        LCD.Write(CDSCell.Value());
        LCD.Write("\n*********************************************\n");
    }

}

/**
 * @brief WorldState::SelfCheck Checks that all sensorts are reporting their expected values
 * @param rieTicks Expected ticks on right Igwan encoder
 * @param lieTicks Expected ticks on left Igwan encoder
 * @param frbVal Expected value of front right bumper
 * @param flbVal Expected value of front left bumper
 * @param brbVal Expected value of back right bumper
 * @param blbVal Expected value of back left bumper
 * @param sbVal Expected value of side bumper
 * @param abVal Expected value of arm bumper
 * @param cdsLow Bottom of expected range for CDS cell
 * @param cdsHigh Top of expected range for CDS cell
 * @return A boolean that is true if the robot passed the test, false if it failed
 */
bool WorldState::SelfCheck(int rieTicks, int lieTicks, bool frbVal, bool flbVal, bool brbVal, bool blbVal, bool sbVal, bool abVal, float cdsLow, float cdsHigh) {

    // Check actual values of all sensorts vs the expected values from the parameters
    bool riePass = (rieTicks == RIEncoder.Counts());
    bool liePass = (lieTicks == LIEncoder.Counts());
    bool frbPass = (frbVal == FRBumper.Value());
    bool flbPass = (flbVal == FLBumper.Value());
    bool brbPass = (brbVal == BRBumper.Value());
    bool blbPass = (blbVal == BLBumper.Value());
    bool sbPass = (sbVal == SideBumper.Value());
    bool abPass = (abVal == ArmBumper.Value());
    bool cdsPass = (CDSCell.Value() > cdsLow && CDSCell.Value() < cdsHigh);

    if (riePass && liePass && frbPass && flbPass && brbPass && blbPass && sbPass && abPass && cdsPass) {

        // If all this is true, the robot passed the self check
        SD.OpenLog();
        SD.Printf("------ SELF CHECK at %f seconds PASSED ------\n\n", TimeNow());
        SD.CloseLog();

        // A friendly beep to signal a pass
        Buzzer.Beep();
        return true;
    } else {

        // Else, the robot failed the test
        char errStr[] = "------ SELF CHECK at %f seconds FAILED ------\n\n\
                           Sensor      Expected    Actual  Pass?\n\
                           Rt IGWN     %d          %d      %s\n\
                           Lf IGWN     %d          %d      %s\n\
                           FR Bump     %d          %d      %s\n\
                           FL Bump     %d          %d      %s\n\
                           BR Bump     %d          %d      %s\n\
                           BL Bump     %d          %d      %s\n\
                           SD Bump     %d          %d      %s\n\
                           AR Bump     %d          %d      %s\n\
                           CDS CLL     %f - %f     %f      %s\n\n\
                        --------------------------------------------";


        SD.Printf(errStr, TimeNow(),
                  rieTicks, RIEncoder.Counts(), (riePass ? "YES" : "NO"),
                  lieTicks, LIEncoder.Counts(), (liePass ? "YES" : "NO"),
                  frbVal, FRBumper.Value(), (frbPass ? "YES" : "NO"),
                  flbVal, FLBumper.Value(), (flbPass ? "YES" : "NO"),
                  brbVal, BRBumper.Value(), (brbPass ? "YES" : "NO"),
                  blbVal, BLBumper.Value(), (blbPass ? "YES" : "NO"),
                  sbVal, SideBumper.Value(), (sbPass ? "YES" : "NO"),
                  abVal, ArmBumper.Value(), (abPass ? "YES" : "NO"),
                  cdsLow, cdsHigh, CDSCell.Value(), (cdsPass ? "YES" : "NO"));


        // Audial warning of a failed self check
        Buzzer.Buzz(1.0);
        return false;
    }

}
