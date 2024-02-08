#define BLYNK_TEMPLATE_ID "TMPL3SwQmPtQf"
#define BLYNK_TEMPLATE_NAME "BlynkTest"
#define BLYNK_AUTH_TOKEN "uW_Rj7qzaHcbhy7NZ-LUsJAK8HsQROlM"
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>  
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
 
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "NMPI";  // Enter your Wifi Username
char pass[] = "nmpi0406";  // Enter your Wifi password
#define VPIN_BUTTON_1    V1 //area
#define VPIN_BUTTON_2    V0 //level
#define VPIN_BUTTON_3    V2//flow rate
#define VPIN_BUTTON_4    V3//volume

const int trigPin1 = 12;//D6 
const int echoPin1 = 13;//D7
unsigned long startMillis; 
unsigned long currentMillis;

const unsigned long period = 10000; 

long duration1; 
int distance; 
volatile int flow_frequency; // Measures flow sensor pulses

// Calculated litres/hour

 float vol = 0.0,l_minute;

unsigned char flowsensor = 2; // Sensor Input D4

unsigned long currentTime;

unsigned long cloopTime;

void ICACHE_RAM_ATTR flow () // Interrupt function

{

   flow_frequency++;

}

void setup() 
{
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);  
   pinMode(trigPin1, OUTPUT); 
  pinMode(echoPin1, INPUT);
   pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
  attachInterrupt(digitalPinToInterrupt(flowsensor), flow, RISING); // Setup Interrupt
   currentTime = millis();
   cloopTime = currentTime;
  delay(500); // put your setup code here, to run once:
}

void loop() 
{
currentTime = millis();
 digitalWrite(trigPin1, LOW); 
 delayMicroseconds(2); 
 digitalWrite(trigPin1, HIGH); 
 delayMicroseconds(10); 
 digitalWrite(trigPin1, LOW); 
 duration1 = pulseIn(echoPin1, HIGH); 
 distance = duration1 * 0.034 / 2; 
 Serial.println(distance); // put your main code here, to run repeatedly:
 Blynk.virtualWrite(VPIN_BUTTON_2,distance);
 
 delay(200);
if(currentTime >= (cloopTime + 1000))
   {
    cloopTime = currentTime; // Updates cloopTime

    if(flow_frequency != 0){

      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.

      l_minute = (flow_frequency / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour

      Serial.println("Rate: ");

      Serial.println(l_minute);

      Serial.println(" L/M");
     
      l_minute = l_minute/60;

      vol = vol +l_minute;

      Serial.println("Vol:");

      Serial.println(vol);

      Serial.println(" L");

      flow_frequency = 0; // Reset Counter

      Serial.println(l_minute, DEC); // Print litres/hour

      Serial.println(" L/Sec");
         
    }

    else 
    {

      Serial.println(" flow rate = 0 ");

       Serial.println("Rate: ");

       Serial.println( flow_frequency );

       Serial.println(" L/M");

      Serial.println("Vol:");

       Serial.println(vol);

      Serial.println(" L");
     Blynk.virtualWrite(VPIN_BUTTON_3,l_minute);
      Blynk.virtualWrite(VPIN_BUTTON_4,vol);
    }
if ((distance > 15)&&(distance <20) && (flow_frequency>2))
{
     Serial.println("A");
      Blynk.virtualWrite(VPIN_BUTTON_1,"Area A infected");
      Blynk.virtualWrite(VPIN_BUTTON_3,l_minute);
      Blynk.virtualWrite(VPIN_BUTTON_4,vol);
      Blynk.logEvent("flood_monitoring",String("water level is:") + distance);
      delay(100);
   }
   
   else if((distance > 10) && (distance <15))
 
   { 
    
    Serial.println("B");
    Blynk.virtualWrite(VPIN_BUTTON_1,"Area B infected");
    Blynk.logEvent("flood_monitoring",String("water level is:") + distance);
    
    
   }
  else if((distance > 5) && (distance < 10 ))
   {
     Serial.println("C");
     Blynk.virtualWrite(VPIN_BUTTON_1,"Area C infected");
     Blynk.logEvent("flood_monitoring",String("water level is:") + distance);
         
   }
   else if(distance<5)
   {
    Serial.println("D");
    Blynk.virtualWrite(VPIN_BUTTON_1,"Area D infected");
    Blynk.logEvent("flood_monitoring",String("water level is:") + distance);
    Blynk.logEvent("flood_monitoring",String("water volume is:")+ vol);
    }
 
 Blynk.run(); 
   }
   }
