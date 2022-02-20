//#include <ESP32Servo.h>

//Servo myservo;
#include <MQ135.h>
#include "DHTStable.h"
#include <Arduino.h>
 #include <WiFi.h>
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"


#define WIFI_SSID "JioFiber-2"
#define WIFI_PASSWORD "90519051"

#define API_KEY "AIzaSyAJiAkNj228ykq_xEkXCGfdQ2uFnfcsidw"
#define DATABASE_URL "https://airguard-b76e2-default-rtdb.firebaseio.com/"
 
DHTStable DHT;
MQ135 mq135_sensor(33);


#define DHT11_PIN       14

float temp = 0.0, humid = 0.0, aqi = -.0;
long ls = 0;
boolean signupOK=false;

FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

TaskHandle_t t1;
TaskHandle_t t2;

void setup() {

  Serial.begin(115200);
  setupbase();

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
      0,  /* Priority of the task */
      &t2,  /* Task handle. */
      0); /* Core where the task should run */
  pinMode(15, OUTPUT);
  pinMode(2, OUTPUT);
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



  Serial.print(humid);
  Serial.print("  ");
  Serial.print(temp);
  Serial.print(" ");
  Serial.println(aqi);

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
