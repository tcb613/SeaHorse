/*--------------------------------------
 * 
 * extra comment in here branch?
 * 
 */
 
 /* 
expected blue tooth values:
0 - turn off sea horse
1 - turn on sea horse
int > 1 will play numbered mp3 from /mp3 folder 
--------------------------------------*/
//
//
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
//
SoftwareSerial DFPlayerSerial(10, 11); // RX, TX
SoftwareSerial BTSerial(8, 9); // RX, TX
//
int blueToothVal;      // value sent over via bluetooth
int pSwitch = 12;      // pin that shorts out seahorse switch
int pLight = 13;       // state indicator led
//
int pVolume =15;
//
void setup()
{
  //
  Serial.begin(9600);
  //init serial ports
  DFPlayerSerial.begin(9600);
  BTSerial.begin(9600);
  //
  //init outpot ports
  pinMode(pSwitch, OUTPUT);
  pinMode(pLight, OUTPUT);
  //
  //init DFPlayer
  mp3_set_serial (DFPlayerSerial);  //set softwareSerial for DFPlayer-mini mp3 module
  delay(1);  //wait 1ms for mp3 module to set volume
  mp3_set_volume (pVolume); //(mid volume)
}


void loop()
{
  BTSerial.listen();
  if (BTSerial.available()){
    //if there is data being recieved
    blueToothVal = BTSerial.parseInt(); //read it

    doFlash(blueToothVal); //flash pin 13 as debug
    
    if (blueToothVal == 0) { 
      //digitalWrite(pLight, LOW);            //turn off LED
      fadeOut();
      pushButton(2000); //long press
      //mp3_stop ();
    }
    else if (blueToothVal == 1) { 
      pushButton(10); //short press
    }
    else if(blueToothVal == 2){
      setVolume(-3);
    }
    else if(blueToothVal == 3){
      setVolume(3);
    }
    else {
      mp3_set_volume (pVolume);
      //mp3_random_play ();
      delay(5);
      mp3_stop();
      delay(5);
      mp3_play (blueToothVal);
    }
  }
}
//
//
//--(functions and shit)---------------------------------------------------------------------------------------------
//
void setVolume(int aAmount){
  pVolume = pVolume + aAmount;
  if(pVolume<0){
    pVolume = 0;
  }else if(pVolume>30){
    pVolume = 30;
  }
  mp3_set_volume (pVolume);
}
//
void pushButton(int aLength) {
  //emulate push and push and hold button of seahorse 
  digitalWrite(pSwitch, HIGH);
  delay(aLength);
  digitalWrite(pSwitch, LOW);
}
//
void doFlash(int aNum){
  while(aNum>0){
    digitalWrite(pLight, HIGH);
    delay(200);
    digitalWrite(pLight, LOW);
    delay(200);
    aNum--;
   }
}
//
//
void fadeOut(){
  int tVolume = pVolume;
  while (tVolume >0){
    tVolume--;
    if(tVolume<0){
      tVolume=0;
    }
    delay(10);
    Serial.print("volume");
    Serial.println(tVolume);
    mp3_set_volume (tVolume);
 }
 Serial.println("mp3 stop");      
 delay(5);
 mp3_stop ();
}

