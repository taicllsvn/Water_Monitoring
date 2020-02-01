#ifndef _DS_1307_
#define _DS_1307_

#include <Arduino.h>
#include <Wire.h>
#include <string.h>


class DS1307{
private:
	byte my_Address;
	byte NumberOfFields;
	int second, minute, hour, day, wday, month, year;
public:
	DS1307(){};
	~DS1307(){};
	void setup();
	int bcd2dec(byte num);
	int dec2bcd(byte num);
	void digitalClockDisplay();
	void readDS1307();
	void setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr);
	String getTimeString();
};

#endif
