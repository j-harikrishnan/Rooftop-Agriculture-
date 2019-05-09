#include <Wire.h>
#include <SPI.h>
#include "DHT.h"
#include <Adafruit_ADS1015.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>       
#define DHTTYPE DHT11   // DHT 11
#define dht_dpin 0
#define OLED_RESET D5
DHT dht(dht_dpin, DHTTYPE); 
// ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
Adafruit_ADS1115 ads(0x48);
Adafruit_SSD1306 display(OLED_RESET);
int li_val,dht_val,moist_val,water_val;
int h,t;

void setup(void) 
{ dht.begin();
  Serial.begin(9600); 
  ads.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();  
  display.setTextSize(2);  
  display.setTextColor(WHITE);  
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
  water_val= round((adc1 * 0.1875)/(1000*0.00322265625));


  li_val = round((li_val *5999/1023));
  moist_val = map(adc1,0,15111,100,0);
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
    itoa(li_val,light_value,10);
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
}
void drawStr(uint8_t x, uint8_t y, char* str){
  display.setCursor(x, y);  /* Set x,y coordinates */
  display.println(str);
}
