#ifndef _DO_H
#define _DO_H
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include <Arduino.h>
#define ReceivedBufferLength 20
#define SCOUNT  30   
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

#define SaturationDoVoltageAddress 12          //the address of the Saturation Oxygen voltage stored in the EEPROM
#define SaturationDoTemperatureAddress 16  
class DO //: public My_Sensor
{
private:
    int m_pin;
    float m_vref;
    float m_temperature;
	char receivedBuffer[ReceivedBufferLength+1];
	byte receivedBufferIndex = 0;
    int analogBuffer[SCOUNT];    //store the analog value in the array, readed from ADC
	int analogBufferTemp[SCOUNT];
	int analogBufferIndex = 0,copyIndex = 0;
    float averageVoltage;
	float SaturationDoVoltage,SaturationDoTemperature;
    double doValue = 0;
	
	const float SaturationValueTab[41] PROGMEM = {      //saturation dissolved oxygen concentrations at various temperatures
	14.46, 14.22, 13.82, 13.44, 13.09,
	12.74, 12.42, 12.11, 11.81, 11.53,
	11.26, 11.01, 10.77, 10.53, 10.30,
	10.08, 9.86,  9.66,  9.46,  9.27,
	9.08,  8.90,  8.73,  8.57,  8.41,
	8.25,  8.11,  7.96,  7.82,  7.69,
	7.56,  7.43,  7.30,  7.18,  7.07,
	6.95,  6.84,  6.73,  6.63,  6.53,
	6.41,
	};
    float getMedianNum(int* dataArray, int arrayLength);
public:
	DO(int pin, float vref,float temperature);
	~DO();
	static char ** data_string;
	void setTemperature(float temp);
	void setup();
	void Run();
	double getValue();  
	char** Get_String_Data();
	void readDoCharacteristicValues(void);
	bool serialDataAvailable(void);
	byte uartParse(void);
	void doCalibration(byte mode);
};  

#endif