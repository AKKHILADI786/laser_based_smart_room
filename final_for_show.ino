
#include <IRremote.h>
#include <SharpIR.h>
#include<Wire.h>
#include<LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);
#include<LowPower.h>
//int led = 7;
int count=0;
#define width 100
#define laser1 2
#define laser2 3
#define i1 8
#define i2 9
#define i3 10
#define i4 11
#define iallon 12
#define ialloff 13



const int RECV_PIN = 7; // for IR remote....
IRrecv irrecv(RECV_PIN);
decode_results results;
unsigned long key_value = 0;

#define trigpin1 5
#define trigpin2 6
#define ir1 15 // A1
#define ir2 16  //A2
#define relay 14
#define model1 1080
#define model2 1080

SharpIR sharpir1(ir1, model1);
SharpIR sharpir2(ir2, model2);


void setup() {
  Serial.begin(9600); 
  lcd.init();
  lcd.backlight();
  lcd.print("Visitor Counter");
   //pinMode(led, OUTPUT);
   pinMode(trigpin1, OUTPUT);
   pinMode(trigpin2, OUTPUT);
  pinMode(laser1, INPUT);
  pinMode(laser2, INPUT);
//  digitalWrite(6,HIGH);
pinMode(i1,OUTPUT);
pinMode(i2,OUTPUT);
pinMode(i3,OUTPUT);
pinMode(i4,OUTPUT);
pinMode(iallon,OUTPUT);
pinMode(ialloff,OUTPUT);
  
  pinMode(relay , INPUT);

    irrecv.enableIRIn();
  irrecv.blink13(true);
  
    lcd.setCursor(0,1);
  lcd.print(count);
  if(digitalRead(laser1))
  Serial.println("Laser one is ON");
  else
  Serial.println("Laser 1 is off---------");
//  attachInterrupt(laser1,in,FALLING);
//  attachInterrupt(laser2,out,FALLING);
}


void displaycount()
{
   lcd.clear();
    lcd.setCursor(0,0);
  if(count<=0)
  {
  lcd.print("Room Empty");
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Switching OFF");
    lcd.setCursor(0,1);
  lcd.print("         Lights");
  
  analogWrite(i1,LOW);
  analogWrite(i2,LOW);
  analogWrite(i3,LOW);
  analogWrite(i4,LOW);

    
  }
  else
  {
    lcd.print("Total Count- ");
      lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(count);
//    Serial.println("Total count-"+count);
              digitalWrite(i1,HIGH);
          digitalWrite(i2,HIGH);
          digitalWrite(i3,HIGH);
          digitalWrite(i4,HIGH);
    
  }
}
/////////////////////////
long measuredis()
{
 long dur1= sharpir1.distance();
long dur2=sharpir2.distance();
if(dur1>100)
    dur1=99999;
  if(dur2>100)
    dur2=99999;
  long fina=width-(dur1+dur2);
//  Serial.println(dur1);
//  Serial.println(dur2);
//  Serial.print("Width of person-");
  //Serial.println(fina);
  
  if(fina>0)
    return fina;
  
  return 0;
} // end of measure distance function
//////////////////////////////////


int checksonar()
{
   long i=0,avg=0;
   long maxval=0,temp;
 
//   Serial.print("False wala ");
long int t1=millis();
  
    while(millis() - t1 <1400 )
    {
      Serial.println("Before measuredistance ");
      temp=measuredis();
//      Serial.println((int)temp);
     if(temp>maxval) 
       maxval=temp;
      if(temp>20)
      {
        avg=((avg*i)+temp)/(i+1);
        i=i+1;
      }
      delay(5);
      
    }
    Serial.println(avg);
    if(maxval>60 && avg>55) //  **********************this is to be modified*********************
      return 2;
    else
      return 1;

}
void out(){
   long val;
   Serial.println("INside out");
     if(!digitalRead(laser2))
  {
    delay(20); //******************************changable, assuming 20 sec for 1 frame ***********8
      val=checksonar();
       count-=val;
      displaycount();
  }
   while((!digitalRead(laser1)) && (!digitalRead(laser2)))
    continue;
}
void in(){
   long val=0;
   Serial.println("INside in");
    if(!digitalRead(laser1))
  {
    delay(20); //******************************changable, assuming 20 sec for 1 frame ***********8
      val=checksonar();
       count+=val;
      displaycount();
  }
   while((!digitalRead(laser1)) && (!digitalRead(laser2)))
    continue;
}
void loop() {
   //LowPower.powerDown(SLEEP_FOREVER , ADC_OFF, BOD_OFF); 
  if(!(digitalRead(laser1)))
  in();
 
   else if(!(digitalRead(laser2)))
    {out();
    }
    delay(50);
//      Serial.println("hello");
 if (irrecv.decode(&results)){
  Serial.println("hello");
 
        if (results.value == 0XFFFFFFFF)
          results.value = key_value;

        switch(results.value){
          case 0xFF30CF:
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("1st appliance OFF");
          digitalWrite(i1,HIGH);
          break;
          case 0xFF18E7:
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("2nd appliance OFF");
          digitalWrite(i2,HIGH);
          break;
          case 0xFF7A85:
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("3rd appliance OFF");
          digitalWrite(i3,HIGH);
          break;
          case 0xFF10EF:
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("4th appliance OFF");
          digitalWrite(i4,HIGH);
          break;
          case 0xFF9867:
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("All switched ON");
          digitalWrite(i1,HIGH);
          digitalWrite(i2,HIGH);
          digitalWrite(i3,HIGH);
          digitalWrite(i4,HIGH);
          break ;  
          case 0xFFB04F:
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("All switched OFF");
          analogWrite(i1,LOW);
          analogWrite(i2,LOW);
          analogWrite(i3,LOW);
          analogWrite(i4,LOW);
          break ;               
         
            
        }
        key_value = results.value;
        irrecv.resume(); 
  }
//Serial.println("end reached");
}
