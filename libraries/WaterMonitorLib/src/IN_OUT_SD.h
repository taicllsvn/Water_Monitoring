#ifndef _INOUTSD_H
#define _INOUTSD_H

#define Number_Of_Sensor 7
#include <Arduino.h>
#include <SD.h>
#include <string.h>
#include <SPI.h>

class IN_OUT_SD{
public:
	IN_OUT_SD(){};
	~IN_OUT_SD();
	static File myFile;
	static int index;
	static char ** DataReaded;
	bool isSuccess;
	void Setup(int CSpin);
	void Write(String realTime,char **data);
	void Read();
	void WriteEachParameter(char name[] ,char data[]);
	void ReadEachParameter(char name[]);
	char ** Get_dataReaded();
	int findsubstr(const char *str, char *sub);
};



#endif
