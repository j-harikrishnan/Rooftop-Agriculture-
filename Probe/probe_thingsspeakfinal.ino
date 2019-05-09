#include <ESP8266WiFi.h>
 
String apiKey = "XESBHIOYELO1U0QC";     //  Enter your Write API key from ThingSpeak

const char *ssid =  "HK";     // replace with your wifi ssid and wpa2 key
const char *pass =  "amma1234";
const char* server = "api.thingspeak.com";


 


WiFiClient client;


#include <Wire.h>
#include "DHT.h"
#include <Adafruit_ADS1015.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>       
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin 0
#define OLED_RESET D5
DHT dht(dht_dpin, DHTTYPE); 


Adafruit_ADS1115 ads(0x48);
Adafruit_SSD1306 display(OLED_RESET);

int li_val,dht_val,moist_val,water_val;
int h,t;

void setup(void) 
{ dht.begin();
  Serial.begin(9600); 
  ads.begin();
  //ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  ads.setGain(GAIN_ONE);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();  
  display.setTextSize(2);  
  display.setTextColor(WHITE);  
  Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
}

void loop(void) 
{
  int16_t adc0,adc1,adc2,adc3;  // we read from the ADC, we have a sixteen bit integer as a result
 
  int t1 = dht.readHumidity();
  if(t1!=2147483647)
  {
    h = t1;
   }
   
  int t2 = dht.readTemperature(); 
   if(t2!=2147483647)
  {
    t = t2;
   }
  adc0 = ads.readADC_SingleEnded(0);
  adc1 = ads.readADC_SingleEnded(1);
  adc2 = ads.readADC_SingleEnded(2);

  display.clearDisplay();

  li_val= round((adc0 * 0.1875)/(1000*0.00322265625));
  //moist_val= round((adc1 * 0.1875)/(1000*0.00322265625));
  water_val= round((adc2 * 0.1875)/(1000*0.00322265625));
  moist_val=adc1-11100;

  li_val = round((li_val *5999/1023));
  moist_val = map(moist_val,0,12000,100,0);
  //h = round((h*0.0879765395894428));
  //water_val = round((water_val *60)/7.5);
 // t= ((t*(3300/1024))-500)/10;
  char light_value[5],moist_value[5],temp[5],humi[5],water_value[5];
  

  
  Serial.print("Light: "); 
  Serial.println(li_val);
  Serial.print("Moisture: "); 
  Serial.println(moist_val);
          
    Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
    itoa(li_val,light_value,10); ///////light value to percentage
    itoa(moist_val,moist_value,10);
    itoa(water_val,water_value,10);
    itoa(t,temp,10);
    itoa(h,humi,10);
    
   drawStr(0,10, "Light");
    drawStr(20, 30, light_value);
    drawStr(70,30, "lux");
    display.display();
    delay(1000);
    display.clearDisplay();
    drawStr(0,10, "Moisture");
    drawStr(20, 30, moist_value);
    drawStr(70,30, "%");
    display.display();
    delay(1000);
    display.clearDisplay();
    drawStr(0,10, "Temp");
    drawStr(20, 30, temp);
    drawStr(70,30, "C");
    display.display();
    delay(1000);
    display.clearDisplay();
    drawStr(0,10, "Humidity");
    drawStr(20, 30, humi);
    drawStr(70,30, "RH");
    display.display();
    delay(1000);
    
    //delay(1000);

      if (isnan(moist_val)) 
                 {
                     Serial.println("Failed to read from DHT sensor!");
                      return;
                 }

                         if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(moist_val);

                             postStr +="&field2=";
                             postStr += String(li_val);

                             postStr +="&field3=";
                             postStr += String(t);

                             postStr +="&field4=";
                             postStr += String(h);
                             
                             
                             
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);                            
                             Serial.println("%. Send to Thingspeak.");
                        }
                        client.stop();
                        
                         /////////////////////////////////////////////////////////////////////////////

       
}
void drawStr(uint8_t x, uint8_t y, char* str){
  display.setCursor(x, y);  /* Set x,y coordinates */
  display.println(str);
}
