/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial



#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);

/*#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif*/



int count1=0;
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

char auth[] = "ac06a1a12e094cb4a7abb06be1187cca";
char ssid[] = "Nokia 6";// Your WiFi credentials.
char pass[] = "rajib986";// Set password to "" for open networks.

#define DHTPIN D6          // What digital pin we're connected to
#define DHTTYPE DHT11     // DHT 11
DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
//WidgetTerminal terminal(V5); //////For blynk terminal
WidgetLED Gled(V0); //register to led virtual pin 0

void sendSensor()/////DHT11 Sensor
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V1, h);
  Blynk.virtualWrite(V2, t);;
   display.setTextSize(1.5);
  display.setTextColor(WHITE);
  display.setCursor(5,0);
  display.print("Temp ");
  display.println(t);
  //display.println("C");
  display.setCursor(5,10);
  display.print("humi ");
  display.println(h);
  //display.println("%");
  display.display();
  //display.clearDisplay();

}

int sensorData;

BLYNK_READ(V3) //Blynk_READ means uC send data to Blynk 
{
  sensorData = analogRead(A0); //reading the sensor on D1
  Blynk.virtualWrite(V3, sensorData); //sending to Blynk
}
int count =0;
void myTimerEvent()
{
  sensorData = analogRead(A0); //reading the sensor on D1
  Blynk.virtualWrite(V3, sensorData); //sending to Blynk
  //Serial.println(count);
  if(sensorData > 420){
    count++;
  }
  else if(sensorData < 370){
    count=0;
  }
  if(count == 1){
  Blynk.notify("GAS DETECTED");
  Blynk.email("rajiblochan039@gmail.com", "DEVICE Alert", "GAS detected!");
  }
  if(count == 11){
     Blynk.notify("GAS DETECTED");
    
    count=1;
  }

    display.setCursor(5,20);
  display.print("Gas ");
  display.println(sensorData);
  //display.println("%");
  display.display();
  //display.clearDisplay();
}
void setup()
{
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  dht.begin();

 display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  // Clear the buffer.
  display.clearDisplay();
  display.display();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("welcome");
  display.display();


  
  timer.setInterval(1000L, sendSensor);
  timer.setInterval(1000L, myTimerEvent);
  

}
void loop()
{
  Blynk.run();
  timer.run();
  display.clearDisplay();
}
