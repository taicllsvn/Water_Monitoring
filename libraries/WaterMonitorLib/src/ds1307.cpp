#include <ds1307.h>

void DS1307::setup()
{
	Wire.begin();
	this->NumberOfFields = 7;
	this->my_Address = 0x68;
	this->second = 0;
	this->minute = 0;
	this->hour = 0;
	this->day = 0;
	this->wday = 0;
	this->month = 0;
	this->year = 0;
}

void DS1307::readDS1307()
{
        Wire.beginTransmission(this->my_Address);
        Wire.write((byte)0x00);
        Wire.endTransmission();
        Wire.requestFrom(this->my_Address, this->NumberOfFields);
        
        this->second = bcd2dec(Wire.read() & 0x7f);
        this->minute = bcd2dec(Wire.read() );
        this->hour   = bcd2dec(Wire.read() & 0x3f); // chế độ 24h.
        this->wday   = bcd2dec(Wire.read() );
        this->day    = bcd2dec(Wire.read() );
        this->month  = bcd2dec(Wire.read() );
        this->year   = bcd2dec(Wire.read() );
        this->year += 2000;    
}
/* Chuyển từ format BCD (Binary-Coded Decimal) sang Decimal */
int DS1307::bcd2dec(byte num)
{
        return ((num/16 * 10) + (num % 16));
}
/* Chuyển từ Decimal sang BCD */
int DS1307::dec2bcd(byte num)
{
        return ((num/10 * 16) + (num % 10));
}
 
/* cài đặt thời gian cho DS1307 */
void DS1307::setTime(byte hr, byte min, byte sec, byte wd, byte d, byte mth, byte yr)
{
        Wire.beginTransmission(this->my_Address);
        Wire.write(byte(0x00)); // đặt lại pointer
        Wire.write(dec2bcd(sec));
        Wire.write(dec2bcd(min));
        Wire.write(dec2bcd(hr));
        Wire.write(dec2bcd(wd)); // day of week: Sunday = 1, Saturday = 7
        Wire.write(dec2bcd(d)); 
        Wire.write(dec2bcd(mth));
        Wire.write(dec2bcd(yr));
        Wire.endTransmission();
}


String DS1307::getTimeString()
{
	String temp = "" , weekday = "";
	this->readDS1307();
	if (1 == this->wday) weekday = "Sunday";
	else if (2 == this->wday) weekday = "Monday";
	else if (3 == this->wday) weekday = "Tuesday";
	else if (4 == this->wday) weekday = "Wednesday";
	else if (5 == this->wday) weekday = "Thursday";
	else if (6 == this->wday) weekday = "Friday";
	else if (7 == this->wday) weekday = "Saturday";
	String minuteStr="", secondStr="";
	if ( this->minute < 10 ) minuteStr = "0" + String(this->minute) + "m";
	else minuteStr = String(this->minute) + "m";
	if ( this->second < 10 ) secondStr = "0" + String(this->second) + "s";
	else secondStr = String(this->second) + "s";
	temp = "Time: " + String(this->hour) + "h"
	+ ":" + minuteStr
	+ ":" + secondStr
	+ " " + weekday + " " + String(this->day)
	+ "/" + String(this->month) 
	+ "/" + String(this->year); 
	return temp;
}
