#include "emstransmission.h"

EMSTransmission::EMSTransmission()
{
    message = "";
}

void EMSTransmission::setChannel(uint channel)
{
    if(channel > 999) return;

    message.append("C");
    message.append(intToStr(channel));
}

void EMSTransmission::setIntensity(uint intensity)
{
   // if(intensity > 99) return;

    message.append("I");
    message.append(intToStr(intensity));
}

void EMSTransmission::setOnTime(uint onTime)
{
    message.append("T");
    message.append(intToStr(onTime));
}

void EMSTransmission::setOffTime(uint offTime){
    message.append("_OF");
    message.append(intToStr(offTime));
}

void EMSTransmission::setIP(string ip)
{
    message.append("_IP");
    message.append(ip);
}

void EMSTransmission::setSSID(string ssid)
{
    message.append("_SS");
    message.append(ssid);
}

void EMSTransmission::setSync(uint channel)
{
    // Not implemented yet
    //message.append("_SY");
    //message.append(ip);
}

void EMSTransmission::setOn(uint channel)
{
    message.append("_O");

    stringstream ss;
    ss << channel;
    message.append(ss.str());
    message.append("1");
}

void EMSTransmission::setOff(uint channel)
{
    message.append("_O");

    stringstream ss;
    ss << channel;
    message.append(ss.str());
    message.append("0");
}

void EMSTransmission::setFrequency(uint frequency)
{
    message.append("_FR");
    message.append(intToStr(frequency));
}

void EMSTransmission::setMinimalChangeTime(uint minChangeTime)
{
    message.append("_CT");
    message.append(intToStr(minChangeTime));
}

void EMSTransmission::setMinimumCalibration(uint channel, uint intensity)
{
    if(channel > 999 || intensity > 99) return;

    message.append("_MI");
    message.append(intToStr(channel));
    message.append(intToStr(intensity));
}
void EMSTransmission::setMaximumCalibration(uint channel, uint intensity)
{
    if(channel > 999 || intensity > 99) return;

    message.append("_MA");
    message.append(intToStr(channel));
    message.append(intToStr(intensity));
}


void EMSTransmission::clearMessage()
{
    message = "";
}

string EMSTransmission::getMessage()
{
    return message;
}