#include <CapacitiveSensor.h>

int trigPin = 12;
int echoPin = 13;
int LED = 8;

int pingCounter = 0;

int prevVolInterval = 0;
int volumeInterval;

int muteFlag = 0;
int muteCount = 0;
int toggleFlag = 0;
boolean nextFlag = false;
boolean prevFlag = false;

CapacitiveSensor  toggle  = CapacitiveSensor(5,2);
CapacitiveSensor  next    = CapacitiveSensor(5,3);
CapacitiveSensor  prev    = CapacitiveSensor(5,4);

void setup() {
  toggle.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
  next.set_CS_AutocaL_Millis(0xFFFFFFFF);
  prev.set_CS_AutocaL_Millis(0xFFFFFFFF);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  long volData = getPing();
  if(volData < 50)
    digitalWrite(LED, HIGH);
  else
    digitalWrite(LED, LOW);
  if (volData < 8) {                          //If ping is within mute range
    if (muteFlag == 0) {
      Serial.write(0);                         //COMMAND --- Write mute to system
      muteCount = 0;                           //Set mute count
      muteFlag = 1;                            //Set mute flag
    }
      
  } else {                                     //else... [ping is greater than mute range]
    if(muteCount >= 5) {                       //n reads have occured since last mute
      muteFlag = 0;
      if (pingCounter >= 30) {                 //Analyze ping value every n samples (increased stability)
        if(volData <= 40) {
          volumeInterval = sendVolInterval(volData);
          if(volumeInterval != prevVolInterval && volumeInterval != 0) {      //SERIAL WRITE VOLUME INTERVAL HERE          
            Serial.write(volumeInterval);        //COMMAND --- Write interval for volume changes
            prevVolInterval = volumeInterval;
          }
        }
        else {
//          Serial.println("Volume Unchanged");
        }
//        Serial.println(volData);
        
        pingCounter = 0;
      }
    } else {
      muteCount++;
    }
  }
  
  
  //CAPACITIVE TOUCH SENSRORS
  long toggleVal = toggle.capacitiveSensor(30);
  long nextVal = next.capacitiveSensor(30);
  long prevVal = prev.capacitiveSensor(30);
  
  if (toggleVal > 1000) {                          //Pause/Play
    if (toggleFlag == 0) {
      Serial.write(2);
//      Serial.println("Play/Pause");
      toggleFlag = 1;
    }
  } else if (nextVal > 1000) {                     //Next
    if(nextFlag == 0) {
      Serial.write(3);
//      Serial.println("Next");
      nextFlag = 1;
    }
  } else if (prevVal > 1000) {                     //Prev
    if(prevFlag == 0) {
      Serial.write(1);
//      Serial.println("Prev");
      prevFlag = 1;
    }
  } else {                                         //If none, reset all values
    toggleFlag = 0;                                    
    nextFlag = 0;
    prevFlag = 0;
  }
  
  pingCounter++;
  delay(10);
}

long getPing() {
  long duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  return distance;  
}

//This function will receive a value from the sensor that will be greater than 10 but less than 40
int sendVolInterval(int pingData) {               
  int volInterval;
  
  //Volume set to 2
  if(pingData <= 15)
    volInterval = 11;
  
  //Volume set to 4
  else if(pingData >= 17 && pingData <= 23)
    volInterval = 12;
  
  //Volume set to 6
  else if(pingData >= 25 && pingData <= 31)
    volInterval = 13;
  
  //Volume set to MAX (7)
  else if(pingData >= 33)
    volInterval = 14;
  
  else
    volInterval = 0;
  
  return volInterval;
}

