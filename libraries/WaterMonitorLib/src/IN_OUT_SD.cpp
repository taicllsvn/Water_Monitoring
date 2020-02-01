#include "IN_OUT_SD.h"

static char ** IN_OUT_SD::DataReaded = new char*[Number_Of_Sensor];
static int IN_OUT_SD::index = 0;
static File IN_OUT_SD::myFile;

IN_OUT_SD::~IN_OUT_SD(){
	for (int i = 0; i < Number_Of_Sensor; i++){
		delete [] DataReaded[i];
	}
	delete[] DataReaded;
}

void IN_OUT_SD::Setup(int CSpin){
	if (!SD.begin(53)) {
		Serial.println("initialization failed!");
		this->isSuccess = false;
		return;
	}
	Serial.println("initialization done.");
	for (int i = 0; i < Number_Of_Sensor; i++){
		DataReaded[i] = new char[30*sizeof(char)];
	}
	this->isSuccess = true;
	this->index = 0;
}

void IN_OUT_SD::Write(String realTime,char **data){
	this->myFile = SD.open("Water.txt",FILE_WRITE);
  // if the file opened okay, write to it:
  if (this->myFile) {
    Serial.print("Writing to Water_Parameters.txt...");
    myFile.println("=====================");
	myFile.println(realTime);
    for (int i = 0; i < 7 ; i++){
     myFile.println(data[i]);
    }
    // close the file:
    this->myFile.close();
    Serial.println("done.");
  } //else this->isSuccess = false;
  for (int i = 0; i < 7 ; i++){
     strcat(data[i],"ok");
    }
  this->WriteEachParameter("Temp.txt",data[index++]);
  this->WriteEachParameter("TDS.txt",data[index++]);
  this->WriteEachParameter("Tur.txt",data[index++]);
  this->WriteEachParameter("pH.txt",data[index++]);
  this->WriteEachParameter("EC.txt",data[index++]);
  this->WriteEachParameter("ORP.txt",data[index++]);
  this->WriteEachParameter("DO.txt",data[index++]);
  if (this->index == (Number_Of_Sensor)) this->index = 0;
}
void IN_OUT_SD::Read(){
	this->ReadEachParameter("Temp.txt");
	this->ReadEachParameter("TDS.txt");
	this->ReadEachParameter("Tur.txt");
	this->ReadEachParameter("pH.txt");
	this->ReadEachParameter("EC.txt");
	this->ReadEachParameter("ORP.txt");
	this->ReadEachParameter("DO.txt");
	if ((this->index) == (Number_Of_Sensor)) 
	{
		this->index = 0;
	}
}
  void IN_OUT_SD::WriteEachParameter(char name[] ,char data[]){
	if (!SD.exists(name))
	{
		this->myFile = SD.open(name,FILE_WRITE);
		this->myFile.print(data);
		this->myFile.close();
	} else { 
		SD.remove(name);
		this->myFile = SD.open(name,FILE_WRITE);
		this->myFile.print(data);
		this->myFile.close();
	}
}
  void IN_OUT_SD::ReadEachParameter(char name[]){
	if (!SD.exists(name)){
    //strcpy(Lastest_Result_Pages.NameEach[0].NameMenuItems,"EMPTY" );
	strcpy(this->DataReaded[index++],"EMPTY");
  }
  else {
    this->myFile = SD.open(name,FILE_READ);
    //Serial.println(index);
    if (this->myFile) {
      //Serial.println("READ Results: ");
      char temp[20];
      static byte x;
      int i;
      i = 0;
      // read from the file until there's nothing else in it:
      while (myFile.available()) {
        //Serial.write(myFile.read());
        x = myFile.read();
        temp[i] = char(x);
        i++;
      }
      Serial.println(temp);
	  String temp1;
	  /*int j = findsubstr(temp," ok");
	  if (j != -1)
	  {
	  for (int i = 0; i < (j); i++)
	  {
		  temp1[i] = temp[i];
	  }
	  }*/
	  //else strcpy(temp1,temp);
	  
	  //strncpy(temp1,temp,(j-4));
	  Serial.println(temp1);
	  strcpy(this->DataReaded[(this->index)++],temp);
      myFile.close();
    } else {
      // if the file didn't open, print an error:
      Serial.println("error opening file");
	  //this->isSuccess = false;
    }
  }
}

 char ** IN_OUT_SD::Get_dataReaded()
 {
	 return this->DataReaded;
 }
 
 int IN_OUT_SD::findsubstr(const char *str, char *sub)
{
const char *p = str;
int len = strlen(sub);
while(*p != NULL)
{
if (strlen(p) >= len)
{
if (strncmp(p, sub, strlen(sub)) == 0)
{
return (p - str);
}
}
else
{
break;
}
p++;
}
return -1;
}