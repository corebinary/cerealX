#include <Arduino.h>
#include <SoftwareSerial.h>

void diag();
void pixhawk_USB();
void RFD900_Pixhawk();
void RFD900_Jetson();
void RFD900_OPENHD_Pixhawk();

SoftwareSerial softSerial(22, 23); // RX, TX
SoftwareSerial softSerial1(19, 21); // RX, TX

int GPIO_STATE = 0;
int GPIO36 = 36;
int GPIO39 = 39;
int GPIO34 = 34;

int incomingByte = 0; // for incoming serial data

void setup() {
  softSerial.begin(57600); //PIXHAWK  - Tx=GPIO23 Rx=GPIO22 
  softSerial1.begin(57600); //OPENHD  - Tx=GPIO2 Rx=GPIO4
  Serial.begin(57600);  //JETSON - USB
  Serial2.begin(57600); //RFD900 - Tx=GPIO17 Rx=GPIO16 
  pinMode(14,OUTPUT); //PIXhawk active
  pinMode(12,OUTPUT); //Jetson Active
  pinMode(36, INPUT); // RFD GPIO 3 
  pinMode(39, INPUT); // RFD GPIO 4
  pinMode(34, INPUT); // RFD GPIO 5
}

void loop() {
  //RFD900_Pixhawk();
  //pixhawk_USB();
  GPIO_STATE = 0;
  //GET THE GPIO SETTINGS
  if (digitalRead(36)){
    GPIO_STATE = GPIO_STATE + 4;
  }

  if (digitalRead(39)){
    GPIO_STATE = GPIO_STATE + 2;
  }

  if (digitalRead(34)){
    GPIO_STATE = GPIO_STATE + 1;
  }

  switch (GPIO_STATE){
    case 5: //ESP Mode
      diag(); //Test
      digitalWrite(14,HIGH);
      digitalWrite(12,HIGH);
      break;
    case 6: //Jetson Mode
      RFD900_Jetson();
      digitalWrite(14,LOW);
      digitalWrite(12,HIGH);
      break;
    default: //Pixhawk mode
      //RFD900_Pixhawk();
      RFD900_OPENHD_Pixhawk();
      digitalWrite(14,HIGH);
      digitalWrite(12,LOW);
      break;
  }
}

void diag(){
  incomingByte = 0;
  if (Serial2.available()) {     
    Serial2.write("***DIAGNOSTOC MODE: Press 1 to reboot ESP32:***\n"); 
     incomingByte = Serial2.read();  
     if (incomingByte==49){Serial2.write("REBOOTING in 3 seconds...");delay(3000);ESP.restart();}
  }
}

void pixhawk_USB(){
  if (Serial.available()) {     
      softSerial.write(Serial.read());   
    }

  if (softSerial.available()) {    
    Serial.write(softSerial.read());   
  }
 }

void RFD900_Pixhawk(){
  if (softSerial.available()) {     
      Serial2.write(softSerial.read());   
    }

  if (Serial2.available()) {    
    softSerial.write(Serial2.read());   
  }
}

void RFD900_Jetson(){
  if (Serial.available()) {     
    Serial2.write(Serial.read());   
  }

  if (Serial2.available()) {    
    Serial.write(Serial2.read());   
  }
}

void RFD900_OPENHD_Pixhawk(){
  if (softSerial.available()) {   
      byte SERIAL_DATA = softSerial.read();
      Serial2.write(SERIAL_DATA);
      if (softSerial1.available()) {
        softSerial1.write(SERIAL_DATA);
      }
  }

  if (Serial2.available()) {    
    softSerial.write(Serial2.read());   
  }
}
