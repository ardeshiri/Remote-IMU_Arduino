#include "RF24Network.h"
#include "RF24.h"
#include "RF24Mesh.h"
#include <SPI.h>
#include <EEPROM.h>
#include <DS3231_Simple.h>

RF24 radio(7,8);  /**** Configure the nrf24l01 CE and CS pins ****/
DS3231_Simple Clock;
int counter=0; //int hz=0; long unsigned T = 0;
RF24Network network(radio);
RF24Mesh mesh(radio,network);
int pl = 7;  int C[3];
int a=1;
uint32_t displayTimer = 0;
DateTime MyTimestamp;
const int chipSelect = 4;
String dataString = "";
char ds[35];

void setup() {
  Serial.begin(9600);
  mesh.setNodeID(0);
  Clock.begin();
  mesh.begin();}

void loop() {    
  mesh.update();
  mesh.DHCP();
  
  if(network.available()){
        if(counter%11==0)
  { MyTimestamp = Clock.read();
    RF24NetworkHeader header; 
    uint32_t dat=0;
    network.peek(header);
    switch(header.type){      
      case 'M': 
      network.read(header,&C,sizeof(C));
      if(a==4){
    dataString = String("O:") + C[2]/10+String(",")+C[1]/10+String(",")+C[0]/10+String(" D:") + String(MyTimestamp.Year)+","+String(MyTimestamp.Month)+","+String(MyTimestamp.Day)+String(" T:")+ String(MyTimestamp.Hour)+String(",")+String(MyTimestamp.Minute)+String(",") + String(MyTimestamp.Second);
    dataString.toCharArray(ds,sizeof(ds));
    Serial.write(ds,sizeof(ds));
      counter = 0;}
      break;
      default: 
      network.read(header,0,0); 
     break;
    }}
     a=((a<4)?a+1:a);}
if(counter==70)
{counter=0;
 a=((a>1)?a-1:a);}
if(a==1)
{
         Serial.println("DISCONNECTED!");
}}
