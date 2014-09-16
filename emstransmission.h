#ifndef EMSTRANSMISSION_H
#define EMSTRANSMISSION_H

#define MAX_CHANNEL 999

#include <string>
#include <bitset>
#include <vector>
#include <sstream>




enum PadStates{
    ON,
    OFF,
    UNCHANGED
};

typedef unsigned int uint;

struct Pad{
    uint channel;
    uint intensity;
    uint minCalibration;
    uint maxCalibration;
    uint minChangeTime;
    uint onTime;
    uint offTime;
    uint frequency;
    uint state;
};

using namespace std;

/**
 * @brief The EMSTransmission class
 */
class EMSTransmission
{
private:
    string message;
public:
    EMSTransmission();

    void setChannel(uint channel);
    void setIntensity(uint intensity);
    void setOnTime(uint onTime);

    void setProperty(string identifier, uint value);
    void setIP(string ip);
    void setSSID(string ssid);
    void setSync(uint channel);
    void setOn(uint channel);
    void setOff(uint channel);
    void setFrequency(uint frequency);
    void setMinimalChangeTime(uint minChangeTime);
    void setMinimumCalibration(uint channel, uint intensity);
    void setMaximumCalibration(uint channel, uint intensity);
    void setOffTime(uint offTime);

    void clearMessage();
    string getMessage();

    inline string intToStr(int value){
        ostringstream ss;
        ss << value;

        return ss.str();
    }
};

#endif // EMSTRANSMISSION_H
