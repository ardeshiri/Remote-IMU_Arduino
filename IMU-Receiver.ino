#include <LiquidCrystal_I2C.h>
#include<avr/wdt.h>
#include "RF24Network.h"
#include "RF24.h"
#include "RF24Mesh.h"
#include <SPI.h>
#include <EEPROM.h>
RF24 radio(7,8);  /**** Configure the nrf24l01 CE and CS pins ****/
int counter=0; //int hz=0; long unsigned T = 0;
RF24Network network(radio);
RF24Mesh mesh(radio,network);
int pl = 7;  int C[3];
int a=1;
volatile bool tkn = false;
uint32_t displayTimer = 0;

LiquidCrystal_I2C lcd(0x3F, 20, 4);

void SRL()
{
  for(int a=0;a<30000;++a);
  if(tkn == false)
  tkn = true;
  else
  tkn = false;
}

void setup() {
  
  Serial.begin(115200);
  mesh.setNodeID(0);
 
  mesh.begin();

   //radio.setDataRate(RF24_1MBPS);radio.setPALevel(RF24_PA_HIGH);
 
  pinMode(2,INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(2),SRL,FALLING);
  //Serial.println(mesh.getNodeID());
  //Serial.println(mesh.begin());
  lcd.begin();
  lcd.backlight();
  lcd.clear();
  //     T= millis();
  //wdt_enable(WDTO_1S);
}

void loop() {    

  mesh.update();
  mesh.DHCP();

counter++;
if(a==2)
 lcd.clear();

  if(network.available()){
        if(counter%11==0)
  { 
    RF24NetworkHeader header; 
    uint32_t dat=0;
    network.peek(header);
    switch(header.type){      
      case 'M': 
      network.read(header,&C,sizeof(C));
      //Serial.println("node "+String(mesh.getNodeID(header.from_node))+" M = "+String(C[0]));
      if(a==4){
       lcd.setCursor(7,0);      lcd.print(String(C[0]/10)+"    ");
       lcd.setCursor(7,1);      lcd.print(String(C[1]/10)+"    ");
      lcd.setCursor(7,2);      lcd.print(String(C[2]/10)+"    ");
      counter = 0;}
      break;

      default: 
      network.read(header,0,0); 
     break;
    }}
     a=((a<4)?a+1:a);}

if(tkn==true)
{

if(counter%7==0 && a==4)
{
    Serial.print("Orientation: ");
    Serial.print(C[2]);
    Serial.print(" ");
    Serial.print(C[1]);
    Serial.print(" ");
    Serial.println(C[0]);

}}

if(counter==70)
{counter=0;
 a=((a>1)?a-1:a);}

if(a==1)
{
         lcd.setCursor(4,1);      lcd.print("DISCONNECTED!");
}

if(a==3)
{
  lcd.setCursor(0,0);
  lcd.print("ROLL:");
  lcd.setCursor(0,1);
  lcd.print("PITCH:");
  lcd.setCursor(0,2);
  lcd.print("YAW:");
}



}

