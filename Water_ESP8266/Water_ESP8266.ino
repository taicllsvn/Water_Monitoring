#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#define NumOfSensor 7
/* Put your SSID & Password which you want*/
const char* ssid = "WaterMonitoring";  //  SSID
const char* password = "12345678";  // Password

/* Put IP Address details */
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

ESP8266WebServer server(80);
volatile static uint8_t initSession = 0;
volatile static uint8_t for_initSession = 0;
volatile static bool check = false;
volatile bool Run_status = false;
volatile bool Save_status = false;
volatile bool Lastest = false;

String data[NumOfSensor];
String BatteryBoat = "";
String BatterySensor = "";
int delay_for_battery = 0;
static int index_for_pin = 7;
static bool check_for_pin = false;
void setup() {
  for (int i = 0; i < NumOfSensor; i++)
    data[i] = " ";
  
  Serial.begin(115200);
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  server.on("/", handle_OnConnect);
  server.on("/start", handle_start);
  server.on("/stop", handle_stop);
  server.on("/save", handle_save);
  server.on("/update", handle_update);
  server.on("/lastest", handle_lastest);
  server.onNotFound(handle_NotFound);
  while (Serial.available())
  {
    Serial.read();
  }
  server.begin();
  //Serial.println("HTTP server started");
}
void loop() {
	server.handleClient();
	static uint8_t index = 0; 

    //section
    String temp;
    temp = "";
    byte x;
    if (Run_status == true)
    {
      delay(250);
      if ( (index == 0) && (check == false)) {  Serial.write("0"); check = true;  }
      else if ( (index == 1) && (check == false)) {  Serial.write("1"); check = true;}
      else if ( (index == 2) && (check == false)) {  Serial.write("2"); check = true;}
      else if ( (index == 3) && (check == false)) {  Serial.write("3"); check = true;}
      else if ( (index == 4) && (check == false)) {  Serial.write("4"); check = true;}
      else if ( (index == 5) && (check == false)) {  Serial.write("5"); check = true;}
      else if ( (index == 6) && (check == false)) {  Serial.write("6"); check = true;}
      else if ( (index == 7) && (check == false)) {  Serial.write("7"); check = true;}
      else if ( (index == 8) && (check == false)) {  Serial.write("8"); check = true;}
      while (Serial.available())
      {
       x = Serial.read();
       temp += char(x);
      }
     if ((temp != "")) 
     {
      delay_for_battery++;      
      if ((index <= 6))
      {        
        if ( index == 0 && temp[0] == '1' && temp[1] == '.')
        {
          data[index++] = temp;
        }
        else if ( index == 1 && temp[0] == '2' && temp[1] == '.')
        {
          data[index++] = temp;
        }
        else if ( index == 2 && temp[0] == '3' && temp[1] == '.')
        {
          data[index++] = temp;
        }
        else if ( index == 3 && temp[0] == '4' && temp[1] == '.')
        {
          data[index++] = temp;
        }
        else if ( index == 4 && temp[0] == '5' && temp[1] == '.')
        {
          data[index++] = temp;
        }
        else if ( index == 5 && temp[0] == '6' && temp[1] == '.')
        {
          data[index++] = temp;
        }
        else if ( index == 6 && temp[0] == '7' && temp[1] == '.')
        {
          data[index] = temp;
          if (delay_for_battery < 9) index++;
          else if (delay_for_battery >= 9 && delay_for_battery < 25) { index = 0; }
          else 
          { 
            check = false;
            delay_for_battery = 0; 
            index++; 
          }
        }
      }
      else if (index == 7) 
      {
        if ( temp[0] != '7' && temp[1] != '.')
          {
            BatteryBoat = "Battery of Boats: " + temp;
            index++;
          }
      }
      else if (index == 8) 
      {
        BatterySensor = "Battery of Sensors: " + temp;
        index++;
      }
     }
     temp ="";
        if (index == 9) index = 0;
        server.send(200, "text/html",SendHTML()); 
    }
    else
    {
      
      String temp;
      temp = "";
      delay(100);
     if ( (index_for_pin == 0) && (check_for_pin == false) ) {  Serial.write("0"); check = true;  }
      else if ( (index_for_pin == 1) && (check_for_pin == false)) {  Serial.write("1"); check_for_pin = true;}
      else if ( (index_for_pin == 2) && (check_for_pin == false)) {  Serial.write("2"); check_for_pin = true;}
      else if ( (index_for_pin == 3) && (check_for_pin == false) ) {  Serial.write("3"); check_for_pin = true;}
      else if ( (index_for_pin == 4) && (check_for_pin == false) ) {  Serial.write("4"); check_for_pin = true;}
      else if ( (index_for_pin == 5) && (check_for_pin == false) ) { Serial.write("5"); check_for_pin = true;}
      else if ( (index_for_pin == 6) && (check_for_pin == false) ) { Serial.write("6"); check_for_pin = true;}
      else if ( (index_for_pin == 7) && (check_for_pin == false) ) {  Serial.write("7"); check_for_pin = true;}
      else if ( (index_for_pin == 8) && (check_for_pin == false) ) {  Serial.write("8"); check_for_pin = true; delay(30);}
      byte x;
      while (Serial.available())
      {
       x = Serial.read();
       temp += char(x);
      }
      
      if (temp != "")
      {
        if ((index_for_pin <= 6))
      {        
        if ( (index_for_pin == 0 && temp[0] == '1' && temp[1] == '.') || temp[0] == 'N' || temp[0] == ' ')
        {
          data[index_for_pin++] = temp;
          //check = false;
        }
        else if ( (index_for_pin == 1 && temp[0] == '2' && temp[1] == '.') || temp[0] == ' ')
        {
          data[index_for_pin++] = temp;
          //check = false;
        }
        else if ( (index_for_pin == 2 && temp[0] == '3' && temp[1] == '.') || temp[0] == ' ')
        {
          data[index_for_pin++] = temp;
          //check = false;
        }
        else if ( (index_for_pin == 3 && temp[0] == '4' && temp[1] == '.') || temp[0] == ' ')
        {
          data[index_for_pin++] = temp;
          //check = false;
        }
        else if ( (index_for_pin == 4 && temp[0] == '5' && temp[1] == '.') || temp[0] == ' ')
        {
          data[index_for_pin++] = temp;
          //check = false;
        }
        else if ( (index_for_pin == 5 && temp[0] == '6' && temp[1] == '.' ) || temp[0] == ' ')
        {
          data[index_for_pin++] = temp;
          //check = false;
        }
        else if ( (index_for_pin == 6 && temp[0] == '7' && temp[1] == '.') || temp[0] == ' ')
        {
          data[index_for_pin++] = temp;
          
        }
      }
        else if (index_for_pin == 7) 
        {
          if ( temp[0] != '7' && temp[1] != '.' || temp[0] != ' ')
          {
           BatteryBoat = "Battery of Boats: " + temp;
           check_for_pin = false;
           index_for_pin++;
          }
        }
        else if (index_for_pin == 8) 
        {
          BatterySensor = "Battery of Sensors: " + temp;
          check_for_pin = false;
          index_for_pin = 0;
          
        }
      }
    }
    if ( for_initSession == 0) initSession = 0;
}

void handle_OnConnect() {
  
  Run_status = false;
  Save_status = false;
  Lastest = false;
  check_for_pin = false;
  delay(100);
  server.send(200, "text/html", SendHTML()); 
}

void handle_lastest() {
  Run_status = false;
  check_for_pin = false;
  Save_status = false;
  Lastest = true;
  delay(80);
  Serial.write("r");
  delay(80);
  server.send(200, "text/html", SendHTML()); 
}

void handle_start() {  
  if ( initSession  == 0 )
  {   
    delay(250);
    Serial.write("s");
    delay(300);
    Run_status = true;
    Lastest = false;
    for_initSession = 1;
  }
  server.send(200, "text/html",SendHTML() ); 
}
void handle_update() {  
  if ( initSession  == 0 )
  {   
    delay(50);
    check = false;
    Run_status = true;
    Lastest = false;
    for_initSession = 1;
  }
  server.send(200, "text/html",SendHTML() ); 
}
void handle_stop() {
  initSession = 1;
  for_initSession = 0;
  Run_status = false;
  Lastest = false;
  delay(360);
  Serial.write("p");
  delay(290);
  check = false;
  server.send(200, "text/html",SendHTML() ); 
}

void handle_save() {
  check = false;
  delay(360);
  Serial.write("l");
  delay(290);
  Run_status = true;
  Save_status = true;
  server.send(200, "text/html", SendHTML()); 
}

//void handle_saved() {
//  Save_status = false;
//  server.send(200, "text/html", SendHTML()); 
//}

void handle_NotFound(){
  server.send(404, "text/plain", "Not found");
}


String SendHTML(){
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 5px auto; text-align: left;}\n";
  ptr += "body{margin-top: 10px;} h1 {color: #444444;margin: 20px auto 20px;text-align: center;} h3 {color: #444444;margin-bottom: 1px; margin-left: 20px; text-align: left;} note {color: #444400; margin-left: 20px; text-align: left; }\n";
  ptr += ".button {display: block;width: 50px;background-color: #1abc9c;border: none;color: white;padding: 10px 25px;text-decoration: none;font-size: 15px;margin: 0px auto 25px;cursor: pointer;border-radius: 5px;}\n";
  ptr += ".button-on {background-color: #1abc9c;}\n";
  ptr += ".button-on:active {background-color: #16a085;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr +=  "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += ".container { width: 325px; border: 1px solid #333; padding: 10px; text-align: center; margin: 0px auto; }\n";
  ptr += "#post { margin-left:25px ; margin-bottom:15px ; width: 70px; height: 40px; float: left;}\n";
  ptr += "#sidebar { margin-right:50px ;width: 70px;height: 40px;float: right;}\n";
  ptr += ".clear { clear: both }\n";
  
  ptr +="</style>\n";
  
  if (Run_status && for_initSession == 1)
  {
  ptr +="<script>\n";
  ptr +="setInterval(loadDoc,200);\n";
  ptr +="function loadDoc() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.getElementById(\"layout\").innerHTML = this.responseText}\n";
  ptr +="};\n";
  ptr +="xhttp.open(\"GET\", \"/update\", true);\n";
  ptr +="xhttp.send();\n";
  ptr +="}\n";
  ptr +="</script>\n";
  }
  else if (!Run_status && !Lastest) {
    ptr +="<script>\n";
  ptr +="setInterval(loadDoc,2000);\n";
  ptr +="function loadDoc() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.getElementById(\"layout\").innerHTML = this.responseText}\n";
  ptr +="};\n";
  ptr +="xhttp.open(\"GET\", \"/\", true);\n";
  ptr +="xhttp.send();\n";
  ptr +="}\n";
  ptr +="</script>\n";
  }
  else if (!Run_status && Lastest) {
    ptr +="<script>\n";
  ptr +="setInterval(loadDoc,2000);\n";
  ptr +="function loadDoc() {\n";
  ptr +="var xhttp = new XMLHttpRequest();\n";
  ptr +="xhttp.onreadystatechange = function() {\n";
  ptr +="if (this.readyState == 4 && this.status == 200) {\n";
  ptr +="document.getElementById(\"layout\").innerHTML = this.responseText}\n";
  ptr +="};\n";
  ptr +="xhttp.open(\"GET\", \"/lastest\", true);\n";
  ptr +="xhttp.send();\n";
  ptr +="}\n";
  ptr +="</script>\n";
  }
  ptr +="</head>\n";
  ptr += "<body id=\"layout\">\n";
  ptr += "<h1>WATER MONITERING</h1>\n";

  String temp;
  if(!Run_status)
  {
    ptr+= "<div id=\"content\" class=\"container\">\n";
    ptr+= "  <div id=\"post\"> <a class=\"button button-on\" href=\"/start\">START</a> </div>\n";
    ptr+= " <div id=\"sidebar\"><a class=\"button button-on\" href=\"/lastest\">LASTEST</a> </div>\n";
    ptr+= " <div class=\"clear\"></div>\n";
    ptr+= "<div id=\"DisplaySensor\">\n";
    if (Lastest)
    {
      ptr +="<h3>";
    ptr += data[0];
    ptr += "</h3>\n";
     if (data[1] != " " || data[2] != " " || data[3] != " " || data[4] != " " || data[5] != " " || data[6] != " ") 
     { 
    ptr += "<note>( Temperature *C )</note>\n";
    
    ptr +="<h3>";
    ptr += data[1];
    ptr += "</h3>\n";
    ptr += "<note>( Total Dissolved Solids mg/l)</note>\n";
    
    ptr +="<h3>";
    ptr += data[2];
    ptr += "</h3>\n";
    ptr += "<note>( TURBILITY NTU )</note>\n";

    ptr +="<h3>";
    ptr += data[3];
    ptr += "</h3>\n";
    ptr += "<note>( pH Axit < 7 < Bazo )</note>\n";
    

    ptr +="<h3>";
    ptr += data[4];
    ptr += "</h3>\n";
    ptr += "<note>( Electrical Conductivity mS/cm )</note>\n";
    
    ptr +="<h3>";
    ptr += data[5];
    ptr += "</h3>\n";
    ptr += "<note>(  Oxygen Reducton Potential mV)</note>\n";

    ptr +="<h3>";
    ptr += data[6];
    ptr += "</h3>\n";
    ptr += "<note>( Dessolved Oxygen ppm )</note>\n";
     }
    ptr +="<h3>";
    ptr += BatteryBoat;
    ptr += "</h3>\n";
    ptr +="<h3>";
    ptr += BatterySensor;
    ptr += "</h3>\n";
    }
    else 
    {
      ptr +="<h3>";
    ptr += BatteryBoat;
    ptr += "</h3>\n";
    ptr +="<h3>";
    ptr += BatterySensor;
    ptr += "</h3>\n";
    }
    
    
    ptr+= "  </div>\n";   
    ptr+= "  </div>\n";
  }
  else if (Run_status)
  {
    ptr+= "<div id=\"content\" class=\"container\">\n";
    ptr+= "  <div id=\"post\"> <a class=\"button button-off\" href=\"/stop\">STOP</a> </div>\n";
    if ( Save_status )
    {
      ptr+= " <div id=\"sidebar\"><a class=\"button button-off\" href=\"/save\">SAVED</a> </div>\n";
	  Save_status = false;
    }
    else ptr+= " <div id=\"sidebar\"><a class=\"button button-on\" href=\"/save\">SAVE</a> </div>\n";
    ptr+= " <div class=\"clear\"></div>\n";

    ptr+= "<div id=\"DisplaySensor\">\n";

    ptr +="<h3>";
    ptr += data[0];
    ptr += "</h3>\n";
    ptr += "<note>( Temperature *C )</note>\n";
    
    ptr +="<h3>";
    ptr += data[1];
    ptr += "</h3>\n";
    ptr += "<note>( Total Dissolved Solids mg/l)</note>\n";
    
    ptr +="<h3>";
    ptr += data[2];
    ptr += "</h3>\n";
    ptr += "<note>( TURBILITY NTU )</note>\n";

    ptr +="<h3>";
    ptr += data[3];
    ptr += "</h3>\n";
    ptr += "<note>( pH Axit < 7 < Bazo )</note>\n";
    

    ptr +="<h3>";
    ptr += data[4];
    ptr += "</h3>\n";
    ptr += "<note>( Electrical Conductivity mS/cm )</note>\n";
    
    ptr +="<h3>";
    ptr += data[5];
    ptr += "</h3>\n";
    ptr += "<note>(  Oxygen Reducton Potential mV)</note>\n";

    ptr +="<h3>";
    ptr += data[6];
    ptr += "</h3>\n";
    ptr += "<note>( Dessolved Oxygen ppm )</note>\n";

    ptr +="<h3>";
    ptr += BatteryBoat;
    ptr += "</h3>\n";
    ptr +="<h3>";
    ptr += BatterySensor;
    ptr += "</h3>\n";
    
    ptr+= "  </div>\n";   
    ptr+= "  </div>\n"; 
  }
    
  ptr +="</body>\n";
  ptr +="</html>\n";
  return ptr;
}
