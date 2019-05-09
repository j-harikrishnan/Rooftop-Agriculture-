#include <ESP8266WiFi.h>
#include <time.h>
#define pump D1
#define light D3
#define humi D2
#define exh D4
const char* ssid = "Shit";
const char* password = "123456789";

//int timezone = -5;
//int dst = 0;
struct tm * timeinfo;

void setup() {
  // initialize digital pin for the pump and the grow light as OUTPUT
  Serial.begin(115200);
  pinMode(pump, OUTPUT);
  pinMode(light,OUTPUT);
  pinMode(humi,OUTPUT);
  pinMode(exh,OUTPUT);
   Serial.setDebugOutput(true);
   digitalWrite(pump,HIGH);
   digitalWrite(humi,HIGH);
   digitalWrite(light,HIGH);
   digitalWrite(exh,HIGH);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  configTime(6 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  Serial.println("\nWaiting for time");
  while (!time(nullptr)) {
   Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  
     digitalWrite(exh,HIGH);

}


void loop() {
  
  time_t now = time(nullptr);
  Serial.println(ctime(&now));
  delay(1000);
  timeinfo = localtime(&now);  
  //Serial.println(timeinfo->tm_hour);
 if(timeinfo->tm_hour==9 && timeinfo->tm_min==07&& timeinfo->tm_sec==00 )
 {pump_relay(0);
  humi_relay(0);
 delay(15000);
 }
 else
 {pump_relay(1);
 humi_relay(1);
 }
 ///////
  if(timeinfo->tm_hour>=18 || timeinfo->tm_hour<=6 )
 {light_relay(0);
 Serial.println("light on");
 }
 else
 light_relay(1);
}


void pump_relay(int pump_control)//      0->Turn off        |->Turn on
{
  if(pump_control ==0)
    digitalWrite(pump,LOW);
  else if(pump_control ==1)
    digitalWrite(pump,HIGH);
  }

int pump_status()               //Return 0 if off
{return digitalRead(pump);
  }
  
void light_relay(int light_control)//      0->Turn off        |->Turn on
{
  if(light_control ==0)
    digitalWrite(light,LOW);
  else if(light_control ==1)
    digitalWrite(light,HIGH);
  }

int light_status()               //Return 0 if off
{return digitalRead(light);
  }

void humi_relay(int humi_control)//      0->Turn off        |->Turn on
{
  if(humi_control ==0)
    digitalWrite(humi,LOW);
  else if(humi_control ==1)
    digitalWrite(humi,HIGH);
  }

int humi_status()               //Return 0 if off
{return digitalRead(humi);
}

void exh_relay(int exh_control)//      0->Turn off        |->Turn on
{
  if(exh_control ==0)
    digitalWrite(exh,LOW);
  else if(exh_control ==1)
    digitalWrite(exh,HIGH);
  }

int exh_status()               //Return 0 if off
{return digitalRead(exh);
  }
