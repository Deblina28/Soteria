#include <ESP32Servo.h>

Servo myservo;
#include <MQ135.h>
#include "DHTStable.h"
#include <Arduino.h>
 #include <WiFi.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"


#define WIFI_SSID "WifiSSID"
#define WIFI_PASSWORD "90519051"

#define API_KEY "Your API key"
#define DATABASE_URL "Your database URL"
 
DHTStable DHT;
MQ135 mq135_sensor(33);


#define DHT11_PIN       14

float temp = 0.0, humid = 0.0, aqi = -.0;
long ls = 0;
boolean signupOK=false;
int pingPin = 13;
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

TaskHandle_t t1;
TaskHandle_t t2;
int lf=32, rt=25;
void setup() {

  Serial.begin(115200);

  pinMode(lf, OUTPUT);
  pinMode(rt, OUTPUT);
 
  setupbase();

  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);    // standard 50 hz servo
  myservo.attach(27, 1000, 2000); 

  

xTaskCreatePinnedToCore(publish,
      "t1",
      10000,  
      NULL,  
      0,  /* Priority of the task */
      &t1,  /* Task handle. */
      0); /* Core where the task should run */
     
  xTaskCreatePinnedToCore(runbot,
      "t1",
      10000,  
      NULL,  
      1,  /* Priority of the task */
      &t2,  /* Task handle. */
      0); /* Core where the task should run */
 
  // put your setup code here, to run once:


  

}



void setupbase()
{
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }

  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
 
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

}

void loop() {


 DHT.read11(DHT11_PIN);
 
  humid = DHT.getHumidity();
 
  temp = DHT.getTemperature();

  aqi = mq135_sensor.getPPM();



 /* Serial.print(humid);
  Serial.print("  ");
  Serial.print(temp);
  Serial.print(" ");
  Serial.println(aqi);*/

delay(500);
  // put your main code here, to run repeatedly:

}



void publish(void *pv)
{
  for(;;)
  {
  if (Firebase.ready() && signupOK && (millis() - ls > 5000 || ls == 0)){
    ls = millis();
    // Write an Int number on the database path test/int
    if (Firebase.RTDB.setFloat(&fbdo, "test/humidity", humid)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
   
   
    // Write an Float number on the database path test/float
    if (Firebase.RTDB.setFloat(&fbdo, "test/temperature ", temp)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

     if (Firebase.RTDB.setFloat(&fbdo, "test/aqi ", aqi)){
      Serial.println("PASSED");
      Serial.println("PATH: " + fbdo.dataPath());
      Serial.println("TYPE: " + fbdo.dataType());
    }
    else {
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }
   
  }
}
}
long dist=0, ldist=0, rdist=0;
void runbot(void *pv)
{
for(;;)
{
 dist=caldist();
 myservo.write(90);
 
if(dist<35)
{
stp();
delay(2000);
myservo.write(180);
rdist=0;
for(int i=0;i<100;i++)
rdist+=caldist();

rdist/=100;
delay(1500);


myservo.write(00);
ldist=0;
for(int i=0;i<100;i++)
ldist+=caldist();

ldist/=100;
delay(1500);

if(ldist<rdist)
{
left();
delay(1500);
}

else
{
rit();
delay(1500);
}

}

else 
fwd();
  Serial.print(dist);
  Serial.println("CM");

 
 
  delay(100);
}
}

void fwd()
{
 
  digitalWrite(lf, HIGH);
  digitalWrite(rt, HIGH);
}

void stp()
{
  digitalWrite(lf, LOW);
  digitalWrite(rt, LOW);
}

void left()
{
  stp();
  delay(100);
  digitalWrite(rt, HIGH);
  digitalWrite(lf, LOW);
}

void rit()
{
  stp();
  delay(100);
  digitalWrite(rt, LOW);
  digitalWrite(lf, HIGH);
}

long caldist()
{
   pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  long  duration = pulseIn(pingPin, HIGH);

  return duration / 29 / 2;
}
