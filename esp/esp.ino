#include <HX711_ADC.h>
#include <Servo.h>

#include <WiFi.h>
#include <FirebaseESP32.h>

#define FIREBASE_HOST "firsttest-45a5a.firebaseio.com"
#define FIREBASE_AUTH "LEjdDsbYnWn3EwttN7q31jFLPc4fFsRkwBJ8GD4n"
#define WIFI_SSID "WIFI SSID"
#define WIFI_PASSWORD "WIFI PASSWORD"

static const int servoPin = 21;
const int HX711_dout = 19;
const int HX711_sck = 18;

HX711_ADC LoadCell(HX711_dout, HX711_sck);

FirebaseData firebaseData;
Servo servo1;
int weightData, statusData;
int posDegrees = 0;
int counter = 2;
int t1 = 0;
int t2 = 0;
int uploadInterval = 1000;
int turnInterval = 200;

void setup(){
	Serial.begin(115200);

	//WIFI SETUP///////////////////////////////////////
	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
	Serial.print("Connecting to Wi-Fi");
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print(".");
		delay(300);
	}
	Serial.println();
	Serial.print("Connected with IP: ");
	Serial.println(WiFi.localIP());
	Serial.println();
	
	
	//SERVER SETUP//////////////////////////////////////
	Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
	Firebase.reconnectWiFi(true);
	
	
	//SERVO SETUP///////////////////////////////////////
	servo1.attach(servoPin);
	servo1.write(0);
	
	
	//SENSOR SETUP//////////////////////////////////////
	LoadCell.begin();
	float calibrationValue = 185.93;
	long stabilizingtime = 2000;
	boolean _tare = true;
	LoadCell.start(stabilizingtime, _tare);
	if (LoadCell.getTareTimeoutFlag()){
		Serial.println("Sensor error.");
		while (1);
	}else{
		LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
		Serial.println("Sensor setup complete.");
	}
}

void loop(){
	//SENSOR/////////////////////////////////////////////////////////
	LoadCell.update();
	weightData = LoadCell.getData();

  if(millis() > t1 + uploadInterval){
    //SEND DATA//////////////////////////////////////////////////////
    if(Firebase.setInt(firebaseData, "/object/Sensor", weightData)){
      Serial.print("Sent: ");
      Serial.print(weightData);
    }else{
      Serial.print("Send Error: ");
      Serial.println(firebaseData.errorReason());
    }
    
    //RECEIVE DATA///////////////////////////////////////////////////
    if(Firebase.getInt(firebaseData, "/object/Status")){
      Serial.print(" | Received: ");
      statusData = firebaseData.intData();
      Serial.println(statusData);
    }else{
      Serial.print("Receive Error: ");
      Serial.println(firebaseData.errorReason());
    }
    
    t1 = millis(); 
  }

  if(millis() > t2 + turnInterval){
    //SERVO//////////////////////////////////////////////////////////
    if(statusData == 1){
      if(counter % 2 == 0){
        posDegrees = posDegrees + 90;
      }else{
        posDegrees = posDegrees - 90;
      }
      servo1.write(posDegrees);
      counter++;
      
      t2 = millis();
    }
  }
}
