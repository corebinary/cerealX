#include <Arduino.h>
#include <SoftwareSerial.h>

void diag();
void pixhawk_USB();
void RFD900_Pixhawk();
void RFD900_Jetson();
void RFD900_OPENHD_Pixhawk();
void RFD900_OPENHD_Jetson();

SoftwareSerial softSerial(22, 23); // RX, TX
SoftwareSerial softSerial1(19, 21); // RX, TX

void setup() {
  Serial.begin(57600);  //JETSON - USB
  Serial2.begin(57600); //RFD900 - Tx=GPIO17 Rx=GPIO16 
  softSerial.begin(57600); //PIXHAWK  - Tx=GPIO23 Rx=GPIO22 
  softSerial1.begin(57600); //OPENHD  - Tx=GPIO21 Rx=GPIO19
    
  pinMode(14,OUTPUT); //PIXhawk active
  pinMode(12,OUTPUT); //Jetson Active
  pinMode(36, INPUT); // RFD GPIO 3 
  pinMode(39, INPUT); // RFD GPIO 4
  pinMode(34, INPUT); // RFD GPIO 5
}

void loop() {
  //GET THE GPIO SETTINGS
  int GPIO_STATE = 0;
    
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
      diag(); 
      digitalWrite(14,HIGH);
      digitalWrite(12,HIGH);
      break;
    case 6: //Jetson Mode
      RFD900_Jetson();
      //RFD900_OPENHD_Jetson();
      digitalWrite(14,LOW);
      digitalWrite(12,HIGH);
      break;
    default: //Pixhawk mode
      RFD900_Pixhawk();
      //RFD900_OPENHD_Pixhawk();
      digitalWrite(14,HIGH);
      digitalWrite(12,LOW);
      break;
  }
}

void diag(){
  int incomingByte = 0;
  if (Serial2.available()) {     
    Serial2.println("***DIAGNOSTOC MODE: Press 1 to reboot ESP32:***\n"); 
     incomingByte = Serial2.read();  
     if (incomingByte==49){Serial2.println("REBOOTING in 3 seconds...\n");delay(3000);ESP.restart();}
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

void RFD900_OPENHD_Jetson(){
  if (Serial.available()) {  
    byte SERIAL_DATA = Serial.read();
    Serial2.write(SERIAL_DATA); 
    if (softSerial1.available()) {
        softSerial1.write(SERIAL_DATA);
      } 
  }

  if (Serial2.available()) {    
    Serial.write(Serial2.read());   
  }
}
