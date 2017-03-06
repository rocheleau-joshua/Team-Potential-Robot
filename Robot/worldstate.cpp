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
 * @brief WorldState::Update Updates all sensor values in the WorldState instance
 */
void WorldState::Update() {
    RIEncoderCounts = RIEncoder.Counts();
    LIEncoderCounts = LIEncoder.Counts();
    FRBumperStatus = FRBumper.Value();
    FLBumperStatus = FLBumper.Value();
    BRBumperStatus = BRBumper.Value();
    BLBumperStatus = BLBumper.Value();
    SideBumperStatus = SideBumper.Value();
    ArmBumperStatus = ArmBumper.Value();
    CDSCellValue = CDSCell.Value();
}

/**
 * @brief WorldState::LogReport Prints all WorldState data to a log file.
 * @param printToScreen If true, the report will be printed to the proteus screen as well as the log file.
 */
void WorldState::LogReport(bool printToScreen) {

    char logRow[] = "%f\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%d\t|%f\t\n";

    // Write row out to the log
    SD.Printf(logRow,
              TimeNow(), RIEncoderCounts, LIEncoderCounts,
              FRBumperStatus, FLBumperStatus, BRBumperStatus, BLBumperStatus,
              SideBumperStatus, ArmBumperStatus, CDSCellValue);

    // Print to screen
    if (printToScreen) {
        LCD.Write("********* WORLD STATE at %f seconds *********\n\n");
        LCD.Write("\tRt IGWAN: ");
        LCD.Write(RIEncoderCounts);
        LCD.Write("\n\tLf IGWAN: ");
        LCD.Write(LIEncoderCounts);
        LCD.Write("\n\tFR Bumper: ");
        LCD.Write(FRBumperStatus);
        LCD.Write("\n\tFL Bumper: ");
        LCD.Write(FLBumperStatus);
        LCD.Write("\n\tBR Bumper: ");
        LCD.Write(BRBumperStatus);
        LCD.Write("\n\tBL Bumper: ");
        LCD.Write(BLBumperStatus);
        LCD.Write("\n\tSd Bumper: ");
        LCD.Write(SideBumperStatus);
        LCD.Write("\n\tAr Bumper: ");
        LCD.Write(ArmBumperStatus);
        LCD.Write("\n\tCDS Cell: ");
        LCD.Write(CDSCellValue);
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
    bool riePass = (rieTicks == RIEncoderCounts);
    bool liePass = (lieTicks == LIEncoderCounts);
    bool frbPass = (frbVal == FRBumperStatus);
    bool flbPass = (flbVal == FLBumperStatus);
    bool brbPass = (brbVal == BRBumperStatus);
    bool blbPass = (blbVal == BLBumperStatus);
    bool sbPass = (sbVal == SideBumperStatus);
    bool abPass = (abVal == ArmBumperStatus);
    bool cdsPass = (CDSCellValue > cdsLow && CDSCellValue < cdsHigh);

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
                  rieTicks, RIEncoderCounts, (riePass ? "YES" : "NO"),
                  lieTicks, LIEncoderCounts, (liePass ? "YES" : "NO"),
                  frbVal, FRBumperStatus, (frbPass ? "YES" : "NO"),
                  flbVal, FLBumperStatus, (flbPass ? "YES" : "NO"),
                  brbVal, BRBumperStatus, (brbPass ? "YES" : "NO"),
                  blbVal, BLBumperStatus, (blbPass ? "YES" : "NO"),
                  sbVal, SideBumperStatus, (sbPass ? "YES" : "NO"),
                  abVal, ArmBumperStatus, (abPass ? "YES" : "NO"),
                  cdsLow, cdsHigh, CDSCellValue, (cdsPass ? "YES" : "NO"));


        // Audial warning of a failed self check
        Buzzer.Buzz(1.0);
        return false;
    }

}

/**
 * @brief WorldState::GetRIEncoderCounts Gets the counts on the right Igwan encoder
 * @return Counts on the Right Igwan Encoder
 */
int WorldState::GetRIEncoderCounts() {
    return RIEncoderCounts;
}

/**
 * @brief WorldState::GetLIEncoderCounts Gets the counts on the left Igwan encoder
 * @return Counts on the left Igwan encoder
 */
int WorldState::GetLIEncoderCounts() {
    return LIEncoderCounts;
}

/**
 * @brief WorldState::GetFRBumperStatus Gets status of the front right bumper
 * @return True if front right bumper is open, false if closed
 */
bool WorldState::GetFRBumperStatus() {
    return FRBumperStatus;
}

/**
 * @brief WorldState::GetFLBumperStatus Gets status of front left bumper
 * @return True if front left bumper is open, false if closed
 */
bool WorldState::GetFLBumperStatus() {
    return FLBumperStatus;
}

/**
 * @brief WorldState::GetBRBumperStatus Gets status of back right bumper
 * @return True if back right bumper is open, false if closed
 */
bool WorldState::GetBRBumperStatus() {
    return BRBumperStatus;
}

/**
 * @brief WorldState::GetBLBumperStatus Gets status of back left bumper
 * @return True if back left bumper is open, false if closed
 */
bool WorldState::GetBLBumperStatus() {
    return BLBumperStatus;
}

/**
 * @brief WorldState::GetSideBumperStatus Gets status of side bumper
 * @return True if side bumper is open, false if closed
 */
bool WorldState::GetSideBumperStatus() {
    return SideBumperStatus;
}

/**
 * @brief WorldState::GetArmBumperStatus Gets status of arm bumper
 * @return True if arm bumper is open, false if closed
 */
bool WorldState::GetArmBumperStatus() {
    return ArmBumperStatus;
}

/**
 * @brief WorldState::GetCDSCellValue Gets value of CDS Cell
 * @return Float value between 0 and 3.3
 */
bool WorldState::GetCDSCellValue() {
    return CDSCellValue;
}
