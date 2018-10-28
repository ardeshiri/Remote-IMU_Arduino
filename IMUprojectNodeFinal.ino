/////////////////////
/*
GY955----MINI
VCC----VCC
GND----GND
1:RX---TX,send AA 38 E2 to GY-955
2:TX---RX
3:MINI_TX---FT232_RX
*/
#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <SPI.h>
//#include <EEPROM.h>
#include <Wire.h> 
 
  unsigned char Re_buf[30],counter=0;
  unsigned char sign=0;
  int C[3];
int alfa=0;
volatile bool x = false;
float ca,cb,cc;

RF24 radio(7,8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);
#define nodeID 7

void serialEvent() {
      while (Serial.available()) {   
      Re_buf[counter]=(unsigned char)Serial.read();
      if(counter==0&&Re_buf[0]!=0x5A) return;           
      counter++;       
      if(counter==20)              
      {    
         counter=0;              
         sign=1;
      }      
      }
}
void tisr (){
  for(int a=0;a<30000;++a);
  x = true;
}

void setup() {
  Serial.begin(9600);  
  pinMode(2,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2),tisr,FALLING); 
  delay(1);    
  ca=0;cb=0;cc=0; //correctors
  Serial.write(0XAA); 
  Serial.write(0X38);  
  Serial.write(0XE2);
 mesh.setNodeID(nodeID);
  //Serial.println(F("Connecting to the mesh..."));
  mesh.begin();
 //radio.setDataRate(RF24_1MBPS);radio.setPALevel(RF24_PA_HIGH);
}

void loop() {
  mesh.update();
  unsigned char i=0,sum=0;
   int16_t DATA[7];
   serialEvent();
  if(sign)
  {   
     for(i=0;i<19;i++)
      sum+=Re_buf[i]; 
     if(sum==Re_buf[i] )     
     {  	 
         DATA[0]=(Re_buf[4]<<8)|Re_buf[5];
         DATA[1]=(Re_buf[6]<<8)|Re_buf[7];
         DATA[2]=(Re_buf[8]<<8)|Re_buf[9];
         C[2]= ((uint16_t)DATA[0])/10;//YAW
         C[0]=DATA[1]/10;//ROLL
         C[1]=DATA[2]/10;//PITCH
       sign=0;       
   }
  }

 if(x==true){
  ca=C[0];
  cb=C[1];
  cc=C[2];
  x=false;
 }
   if((alfa)==1200)
{
  alfa =0;
         C[0]=C[0]-ca;
         C[1]=C[1]-cb;
         C[2]=C[2]-cc;
         mesh.write(&C, 'M', sizeof(C));
}

alfa++;
}
