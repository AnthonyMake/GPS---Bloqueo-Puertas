#include <TinyGPS.h>

/* 
    GPS based square geofence, enables or disables a Relay based on Fence Position
 */

#define relayPin 5

//===================================
//Fence Position

#define zoneLat -36.7817944   //latitude
#define zoneLon -73.0745099   //longitude
#define zoneTol 0.0005        //+-tolerance

//===================================

TinyGPS gps;
float flat, flon;

void setup(){
  
  Serial.begin(115200); //arduino to pc serial port
  Serial1.begin(9600);  //GPS serial port
  pinMode(relayPin,OUTPUT);

}

void loop(){
  
    gpsRead();
    boolean isInZone=checkZone(flat, flon);

    if(isInZone){
      digitalWrite(relayPin,HIGH);  //Set relay if is in the zone  
    }else{
      digitalWrite(relayPin,LOW);   //turn off relay out of zone  
    }

}

void gpsRead(){
    
    while (Serial1.available()){
      char c = Serial1.read();   // gps to aduino messages 
      if (gps.encode(c)){        // tinygps take the messages 
         
         unsigned long age;
         gps.f_get_position(&flat, &flon, &age);  //getting positions
         
         long speed=gps.f_speed_kmph();
         Serial.print("LAT=");                   //showing the data
         Serial.print(flat, 6);
         Serial.print(" LON=");
         Serial.print(flon, 6);
         Serial.print(" SPEED=");
         Serial.println(speed, 6);
      
      }
    
    }

}

boolean checkZone(float latitude, float longitude){
  
  boolean isIn=(latitude<zoneLat+zoneTol || latitude>zoneLat-zoneTol || longitude<zoneLon+zoneTol || longitude>zoneLon-zoneTol);
  
  return isIn;

}
