#include "My_Sensor.h"
#include "DisplayOn5110.h"
#include "IN_OUT_SD.h"
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <SPI.h>
#include <SD.h>
#include <time.h>
#include <Wire.h>
#include "ds1307.h"
static IN_OUT_SD MySD;
static DS1307 My_realTime;
static My_Sensor All_Sensor;
static char **data; // 7 sensor;         //  data[0] TEMP,data[1] TDS , data[2] Turbility, data[3] pH ... data[4] EC , data[5] ORP
int analogInputSensor = A8;
int analogInputBoat = A6;
float voutBoat = 0.0;
float vinBoat = 0.0;
float voutSensor = 0.0;
float vinSensor = 0.0;
float R1 = 30000.0; //  
float R2 = 7500.0; // 
int ADCvalueBoat = 0;
int ADCvalueSensor = 0;
int percentBoat = 0;
int percentSensor = 0;
char percentStrBoat[25];
char percentStrSensor[25];
//config LCD 5110
Adafruit_PCD8544 display = Adafruit_PCD8544( 8, 7, 6, 5, 4);
DisplayOn5110 MyDisplay;
static Pages_Struct Main_Pages;
static Pages_Struct Result_Pages;
static Pages_Struct Lastest_Result_Pages;
static Pages_Struct SaveToSdcard_Pages;

int findsubstr(const char *str, char *sub);
void Read_sdCard(){
  MySD.Read();
  MyDisplay.Update_PagesLatestResult(&Lastest_Result_Pages,MySD.isSuccess,MySD.DataReaded);
}
char* sendSDdata(char *temp)
{
  int j = findsubstr(temp," ok");
  static char data[30];
  memset(data,' ',30);
  for (int i = 0; i < j; i++)
    data[i] = temp[i];
    return data;
}
void Write_sdCard(String realTime){
  MySD.Write(realTime,data);
  MyDisplay.Update_SaveToSdcardPages(&SaveToSdcard_Pages,MySD.isSuccess);
  Draw_Pages(&SaveToSdcard_Pages);
}

void Write_sdCard_Command(String realTime){
  MySD.Write(realTime,data);
}
void getBattery()
{
       ADCvalueSensor = analogRead(analogInputSensor);
       //if (ADCvalueSensor < 720) sprintf(percentStrSensor,"%s","no power supply!");
       //else
       {
          voutSensor = (ADCvalueSensor / 1024.0) * 5.0;
          vinSensor = voutSensor / (R2/(R1+R2));
          percentSensor = ((vinSensor-8.3)/4.1)*100;
          if (percentSensor <= 0) sprintf(percentStrSensor,"%s","no power supply!");
          else sprintf(percentStrSensor,"%d %s",percentSensor,"%");
          //sprintf(percentStrSensor,"%d %s",percentSensor,"%");
       }
       
       ADCvalueBoat = analogRead(analogInputBoat);
       //if (ADCvalueBoat < 720) sprintf(percentStrBoat,"%s","no power supply!");
       //else 
       {
          voutBoat = (ADCvalueBoat / 1024.0) * 5.0;
          vinBoat = voutBoat / (R2/(R1+R2)); 
          percentBoat = ((vinBoat-9.0)/4.1)*100;
          if (percentBoat <= 0) sprintf(percentStrBoat,"%s","no power supply!");
          else sprintf(percentStrBoat,"%d %s",percentBoat, "%");
          //sprintf(percentStrBoat,"%d %s",percentBoat, "%");
       }
//       Serial.print("vinBoat:");
//       Serial.println(vinBoat);
//       Serial.print("vinSensor:");
//       Serial.println(vinSensor);
//       Serial.println(percentBoat);
//       Serial.println(percentSensor); 
}
void setup(void)
{
  data = (char**)malloc(7*sizeof(char*)); // 7 sensor
  for (int i = 0 ; i < 7; i++){
    data[i] = (char*)malloc(30*sizeof(char));
  }
  strcpy(data[0],"1.Temp=...");
  strcpy(data[1],"2.TDS=...");
  strcpy(data[2],"3.Tur=...");
  strcpy(data[3],"4.pH=...");
  strcpy(data[4],"5.EC=...");
  strcpy(data[5],"6.ORP=...");
  strcpy(data[6],"7.DO=...");
  // start serial port
  Serial.begin(115200);
  Serial3.begin(115200);
  pinMode(analogInputBoat, INPUT);
  pinMode(analogInputSensor, INPUT);
  pinMode(2, INPUT_PULLUP); //CENTER
  pinMode(3, INPUT_PULLUP); //DOWN
  pinMode(9,OUTPUT);
  digitalWrite(9,HIGH); //Turn Backlight ON
  
  attachInterrupt(0, checkIfSelectButtonIsPressed, LOW); //Interupt line 0
  attachInterrupt(1, checkIfDownButtonIsPressed, LOW); //Interupt line 1

  display.begin();
  display.setContrast(50); //Set contrast to 50
  display.clearDisplay(); 
  display.display();
  MyDisplay.Run(&Main_Pages,&Result_Pages,&Lastest_Result_Pages,&SaveToSdcard_Pages);
  Draw_Pages(&Main_Pages);
  MySD.Setup(53);
  My_realTime.setup();
  All_Sensor.Setup_Sensor();
  while ( Serial3.available())   
      Serial3.read();   
}
void loop(void)
{
    //String realtime = My_realTime.getTimeString();
    //My_realTime.setTime(16,23,0,7,11,1,20); //set time for ds1307
    //Serial.println(realtime);
    getBattery();
    String CommandReceive;
    CommandReceive = "";
    byte x;
    delay(150);
    while ( Serial3.available())   
    {  
      x = Serial3.read(); 
      CommandReceive += char(x);
    }
    
    if (CommandReceive == "s") //start
    {
      Main_Pages.status = false;
      Result_Pages.status = true;
      Serial.println(CommandReceive);
    }
    else if ( (CommandReceive == "0")) { Serial3.write(" ");}
    else if ( (CommandReceive == "1") ) { Serial3.write(" ");}
    else if ( (CommandReceive == "2") ) { Serial3.write(" ");}
    else if ( (CommandReceive == "3") ) { Serial3.write(" ");}
    else if ( (CommandReceive == "4") ) { Serial3.write(" ");}
    else if ( (CommandReceive == "5") ) { Serial3.write(" ");}
    else if ( (CommandReceive == "6") ) { Serial3.write(" ");}
    else if ( (CommandReceive == "7") ) { Serial3.write(percentStrBoat);}
    else if ( (CommandReceive == "8") ) { Serial3.write(percentStrSensor);}
    else if ( (CommandReceive == "r") ) 
    { 
    bool check_D = false;
    String Command = CommandReceive;
    if ( MySD.isSuccess == true ) MySD.Read();
    do
    {
    delay(150);
    byte temp;
    Command = "";
    while (Serial3.available())
    {
        temp = Serial3.read();
        Command += char(temp);
        check_D = false;
    }
    Serial.println(Command);
    if ( MySD.isSuccess == true )
    {
      
      if ( (Command == "0") && (check_D == false)) {  Serial3.write(sendSDdata(MySD.DataReaded[0])); check_D = true;}
      else if ( (Command == "1") && (check_D == false)) { Serial3.write( sendSDdata(MySD.DataReaded[1]));  check_D = true;}
      else if ( (Command == "2") && (check_D == false)) { Serial3.write( sendSDdata(MySD.DataReaded[2]));  check_D = true;}
      else if ( (Command == "3") && (check_D == false)) { Serial3.write( sendSDdata(MySD.DataReaded[3]));  check_D = true;}
      else if ( (Command == "4") && (check_D == false)) { Serial3.write( sendSDdata(MySD.DataReaded[4]));  check_D = true;}
      else if ( (Command == "5") && (check_D == false)) { Serial3.write( sendSDdata(MySD.DataReaded[5]));  check_D = true;}
      else if ( (Command == "6") && (check_D == false)) { Serial3.write( sendSDdata(MySD.DataReaded[6]));  check_D = true;}
      else if ( (Command == "7") && (check_D == false)) { Serial3.write(percentStrBoat);  check_D = true;}
      else if ( (Command == "8") && (check_D == false)) { Serial3.write(percentStrSensor);  check_D = true;}
      }
      else
      {
        
        if ( (Command == "0") && (check_D == false)) { Serial3.write("No SD card!"); check_D = true;}
        else if ( (Command == "1") && (check_D == false)) { Serial3.write(" ");  check_D = true;}
        else if ( (Command == "2") && (check_D == false)) { Serial3.write(" ");  check_D = true;}
        else if ( (Command == "3") && (check_D == false)) { Serial3.write(" ");  check_D = true;}
        else if ( (Command == "4") && (check_D == false)) { Serial3.write(" ");  check_D = true;}
        else if ( (Command == "5") && (check_D == false)) { Serial3.write(" ");  check_D = true;}
        else if ( (Command == "6") && (check_D == false)) { Serial3.write(" ");  check_D = true;}
        else if ( (Command == "7") && (check_D == false)) { Serial3.write(percentStrBoat); check_D = true;}
        else if ( (Command == "8") && (check_D == false)) { Serial3.write(percentStrSensor); check_D = true;}
        } 
        }
        while (Command != "s");
          Main_Pages.status = false;
          Result_Pages.status = true;
          Serial.println(CommandReceive);
          
    }
    
    Serial.println(CommandReceive);
    
    if (!Result_Pages.status && Main_Pages.status) Draw_Pages(&Main_Pages);
       else if (Result_Pages.status && !Main_Pages.status) Draw_Pages(&Result_Pages);
       
  while ((false == Main_Pages.status) && (true == Result_Pages.status)){ 
        //Serial.println("INSIDE");
    All_Sensor.Run_Sensor();
    static unsigned long SampleTimeDraw = millis();
    if ((unsigned long)(millis() - SampleTimeDraw) > 280U){
      SampleTimeDraw = millis();
      All_Sensor.Get_Data_From_Sensor();
      for (int i = 0; i < 7; i++){
        strcpy(data[i],All_Sensor.m_data[i]);
      }
      
      if (!Result_Pages.status && Main_Pages.status) Draw_Pages(&Main_Pages);
       else if (Result_Pages.status && !Main_Pages.status) Draw_Pages(&Result_Pages);
       //Update LCD
       MyDisplay.Update_PagesResult(&Result_Pages,data);
       
    }
    static unsigned long SampleTimeDrawBattery = millis();
    if ((unsigned long)(millis() - SampleTimeDrawBattery) > 20000U){
      getBattery();
    }
    
    byte temp;
    //percentStr = String(percent);
    static bool check_Dupl = false;
    String Command;
    Command = "";
    while (Serial3.available())
    {
        temp = Serial3.read();
        //Serial.println(char(temp));
        Command += char(temp);
        check_Dupl = false;
    }
        
        Serial.println(Command);
        
      if ( (Command == "0") && (check_Dupl == false)) { Serial3.write(data[0]); check_Dupl = true;}
      else if ( (Command == "1") && (check_Dupl == false)) { Serial3.write(data[1]);  check_Dupl = true;}
      else if ( (Command == "2") && (check_Dupl == false)) { Serial3.write(data[2]);  check_Dupl = true;}
      else if ( (Command == "3") && (check_Dupl == false)) { Serial3.write(data[3]);  check_Dupl = true;}
      else if ( (Command == "4") && (check_Dupl == false)) { Serial3.write(data[4]);  check_Dupl = true;}
      else if ( (Command == "5") && (check_Dupl == false)) { Serial3.write(data[5]);  check_Dupl = true;}
      else if ( (Command == "6") && (check_Dupl == false)) { Serial3.write(data[6]);  check_Dupl = true;}
      else if ( (Command == "7") && (check_Dupl == false)) { Serial3.write(percentStrBoat);  check_Dupl = true;}
      else if ( (Command == "8") && (check_Dupl == false)) { Serial3.write(percentStrSensor);  check_Dupl = true;}
      else if ( Command == "p") {   //stop        
            Main_Pages.status = true;
            Result_Pages.status = false;
            
            while(Serial3.available())
              Serial3.read();
            Serial.println("Command stop Arduino ");
            delay(200);
            break; 
          }
       else if ( Command == "l") { //save
            String realtime = My_realTime.getTimeString();
            Serial.println(realtime);
            Serial.println("save command");
            Write_sdCard_Command(realtime);
            
          }
          if (!Result_Pages.status && Main_Pages.status) break;
          //if (percentSensor < 7.0) break;
  }
}


void checkIfDownButtonIsPressed()
{
  if ( Main_Pages.status && !Result_Pages.status ){
    Main_Pages.Menu_Items_Current++;
    if (Main_Pages.Menu_Items_Current == (Main_Pages.Number_MenuItems)){
      Main_Pages.Menu_Items_Current = 0;
    }
    Draw_Pages(&Main_Pages); 
    if (digitalRead(3) == LOW) { while (true){
      delay(50);
        if (digitalRead(3) == HIGH) return;
      }
    }
  }
  else if ( Result_Pages.status && !Main_Pages.status ){
    Result_Pages.Menu_Items_Current++;
    if (Result_Pages.Menu_Items_Current == (Result_Pages.Number_MenuItems)){
      Result_Pages.Menu_Items_Current = 0;
    }
    Draw_Pages(&Result_Pages); 
    if (digitalRead(3) == LOW) { while (true){
      delay(50);
        if (digitalRead(3) == HIGH) return;
      }
    }
  }
  else if ( Lastest_Result_Pages.status && !Main_Pages.status ){
    Lastest_Result_Pages.Menu_Items_Current++;
    if (Lastest_Result_Pages.Menu_Items_Current == (Lastest_Result_Pages.Number_MenuItems)){
      Lastest_Result_Pages.Menu_Items_Current = 0;
    }
    Draw_Pages(&Lastest_Result_Pages); 
    if (digitalRead(3) == LOW) { while (true){
      delay(50);
        if (digitalRead(3) == HIGH) return;
      }
    }
  }
  else if ( SaveToSdcard_Pages.status && !Main_Pages.status && !Result_Pages.status){
//    SaveToSdcard_Pages.Menu_Items_Current++;
//    if (SaveToSdcard_Pages.Menu_Items_Current == (SaveToSdcard_Pages.Number_MenuItems)){
//      SaveToSdcard_Pages.Menu_Items_Current = 0;
//    }
    Draw_Pages(&SaveToSdcard_Pages); 
    
  }
   if (digitalRead(3) == LOW) { while (true){
    delay(50);
        if (digitalRead(3) == HIGH) return;
      }
    }
}


void checkIfSelectButtonIsPressed()
{
  if ( Main_Pages.status && !Result_Pages.status && !Lastest_Result_Pages.status){
    if (Main_Pages.Menu_Items_Current == 0){
      Main_Pages.status = false;
      Result_Pages.status = true;
      Draw_Pages(Main_Pages.NameEach[0].My_Pages);
        if (digitalRead(2) == LOW) { 
          while(1){ 
            delay(50);
          if (digitalRead(2) == HIGH) return;
          }
         }
    }
    else if (Main_Pages.Menu_Items_Current == 2){
      Main_Pages.status = false;
      Lastest_Result_Pages.status = true;
      Read_sdCard();
      Draw_Pages(Main_Pages.NameEach[2].My_Pages);
        if (digitalRead(2) == LOW) { 
          while(1){ 
            delay(50);
          if (digitalRead(2) == HIGH) return;
          }
         }
    }
    else if ((Main_Pages.Menu_Items_Current == 1)){
      if (digitalRead(9) == HIGH)
        digitalWrite(9,LOW); 
        else digitalWrite(9,HIGH);
      Draw_Pages(&Main_Pages);
        if (digitalRead(2) == LOW) { while(true){ 
          delay(50);
          if (digitalRead(2) == HIGH) return;
          }
         }
    }
     if (digitalRead(2) == LOW) { while(true){ 
        delay(50);
          if (digitalRead(2) == HIGH) return;
          }
         }
  }
  if ( Result_Pages.status && !Main_Pages.status && !SaveToSdcard_Pages.status){
    if (Result_Pages.Menu_Items_Current == 7 ){
      Result_Pages.status = false;
      Main_Pages.status = false;
      SaveToSdcard_Pages.status = true;
      Draw_Pages(Result_Pages.NameEach[7].My_Pages);
      delay(3000);
      String timeTemp = My_realTime.getTimeString();
      Write_sdCard(timeTemp);
      Draw_Pages(Result_Pages.NameEach[7].My_Pages);
      if (digitalRead(2) == LOW) { while(true){ 
        delay(50);
        if (digitalRead(2) == HIGH) return;
       }
      }
    }
    if (Result_Pages.Menu_Items_Current == 8 ){
      Result_Pages.status = false;
      Main_Pages.status = true;  
      Draw_Pages(Result_Pages.NameEach[8].My_Pages);
      Result_Pages.Menu_Items_Current = 2;
      if (digitalRead(2) == LOW) { while(true){ 
        delay(50);
        if (digitalRead(2) == HIGH) return;
       }
      }
    }
  }
//  if ( Result_Pages.status && !Main_Pages.status && !SaveToSdcard_Pages.status){
//    
//  }
  else if ( !Result_Pages.status && !Main_Pages.status && SaveToSdcard_Pages.status){
    if (SaveToSdcard_Pages.Menu_Items_Current == 1 ){
      Result_Pages.status = true;
      Main_Pages.status = false;  
      SaveToSdcard_Pages.status = false;
      Draw_Pages(SaveToSdcard_Pages.NameEach[1].My_Pages);
      strcpy(SaveToSdcard_Pages.NameEach[0].NameMenuItems," SAVING" ); 
      SaveToSdcard_Pages.Menu_Items_Current = 1;
      if (digitalRead(2) == LOW) { while(true){ 
        delay(50);
        if (digitalRead(2) == HIGH) return;
       }
      }
    }
  }
  if ( Lastest_Result_Pages.status && !Main_Pages.status){
    if (Lastest_Result_Pages.Menu_Items_Current == 8 ){
      Lastest_Result_Pages.status = false;
      Main_Pages.status = true;
      Draw_Pages(Lastest_Result_Pages.NameEach[8].My_Pages);
      Lastest_Result_Pages.Menu_Items_Current = 2;
      if (digitalRead(2) == LOW) { while(true){ 
        delay(50);
        if (digitalRead(2) == HIGH) return;
       }
      }
    }
  }
  if (digitalRead(2) == LOW) { while(true){ 
        delay(50);
        if (digitalRead(2) == HIGH) return;
       }
     }
  delay(300);
}

void Draw_Pages(Pages_Struct *Pages){
    //display.setTextSize(1);
    display.clearDisplay();
    display.setTextColor(BLACK, WHITE);
    display.setCursor(15, 0);
    display.print(Pages->title);
    display.drawFastHLine(0,9,83,BLACK);
    uint8_t i;
    uint8_t y = 13;
    if ( Pages->Menu_Items_Current > 3 ){
      i = Pages->Menu_Items_Current - 3;
    } else i = 0;
    for (i; i < Pages->Number_MenuItems ; i++){
      if (Pages->Menu_Items_Current == i){
         display.setTextColor(WHITE ,BLACK);
        } else display.setTextColor(BLACK, WHITE);
      display.setCursor(0, y);
      display.print(Pages->NameEach[i].NameMenuItems);
        //On Or Off Light
        if (Main_Pages.status && !(Result_Pages.status) && !(SaveToSdcard_Pages.status) && i == 1 ) { 
          if (digitalRead(9) == HIGH) { display.print("ON"); } 
          else { display.print("OFF"); }
          }
      y += 8;
    }
    display.display();
}
 int findsubstr(const char *str, char *sub)
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
//
//void loop()
//{
//    // listen for communication from the ESP8266 and then write it to the serial monitor
//    if ( Serial3.available() )   {  Serial.write( Serial3.read() );  }
// 
//    // listen for user input and send it to the ESP8266
//    if ( Serial.available() )       {  Serial3.write( Serial.read() );  }
//}
