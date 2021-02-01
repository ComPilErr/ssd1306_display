#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "pics.h"

#define WIDTH 128
#define HEIGHT 64
#define ADDRESS 0x3C
#define LINES 3
#define PWR_PIN 11
#define GND_PIN 10
Adafruit_SSD1306 display(WIDTH, HEIGHT,&Wire);
bool colour = 1;
uint16_t i = 0;
byte register_to_call = 0;

int voltage=0;
int current=0;
int capasity=0;
int base_capasity=0;
int temp=0;
int cycle=0;

void setup() {

pinMode(GND_PIN, OUTPUT);digitalWrite(GND_PIN,LOW);
pinMode(PWR_PIN, OUTPUT);digitalWrite(PWR_PIN,LOW);
Serial.begin(115200);
if ( !display.begin(SSD1306_SWITCHCAPVCC, ADDRESS) ){Serial.println("Fuck off"); for(;;);}
randomSeed(analogRead(0));
display.clearDisplay();
display.drawBitmap(32,0,apple,64,64,1);
display.display();digitalWrite(PWR_PIN,HIGH);
delay(2000); // Pause for 2 seconds

display.clearDisplay();
//display.invertDisplay(1);
  scan();

  for(int i=0;i<24;i++){
  display.fillCircle(random(WIDTH)/2,random(HEIGHT)/2,i,colour);
  display.fillCircle(WIDTH - random(WIDTH)/2,random(HEIGHT)/2,i,colour);
  display.fillCircle(random(WIDTH)/2,HEIGHT - random(HEIGHT)/2,i,colour);
  display.fillCircle(WIDTH - random(WIDTH)/2,HEIGHT -random(HEIGHT)/2,i,colour);
  display.display();
    }
      display.fillRect(0,0,WIDTH,HEIGHT, colour);  display.display();
}

void loop() {

if(i>23){ //
  processing();
  randomSeed(analogRead(colour)); i = 0; 
  display.fillRect(0,0,WIDTH,HEIGHT, colour);
 // display.display();
//colour = !colour;
 clear();
    
  show();

 display.display();
}//
    i++;
}

void processing()
{
  int 
  x = read(0x08); temp = (x==-1) ? temp:x;
  x = read(0x09);voltage =(x==-1) ? voltage:x;
  x = read(0x0A);current =(x==-1) ? current:x;
  x = read(0x0F);capasity =(x==-1) ? capasity:x;
  x = read(0x10);base_capasity =(x==-1) ? base_capasity:x;
  x = read(0x3c);cycle =  (x==-1) ? cycle:x;

  }

void show(){
  
  display.setTextSize(1); display.setTextColor(2); display.setCursor(80,0);
  display.print("temp:");
  display.setTextSize(1); display.setTextColor(2); display.setCursor(80,10);
  display.print( ( (((float)temp)/10)-273.15 ) );

  display.setTextSize(1); display.setTextColor(2); display.setCursor(0,0);
  display.print("voltage:");
  display.setTextSize(2); display.setTextColor(2); display.setCursor(0,12);
  display.print( (float)voltage/1000 );

  display.setTextSize(1); display.setTextColor(2); display.setCursor(0,34);
  display.print("current:");
  display.setTextSize(2); display.setTextColor(2); display.setCursor(0,46);
  display.print( (float)current/1000 );

  display.setTextSize(1); display.setTextColor(2); display.setCursor(76,34);
  display.print("charge:");
  display.setTextSize(2); display.setTextColor(2); display.setCursor(78,46);
  display.print( (int) (( (float) capasity/base_capasity )*100));
  display.setTextSize(1); display.setTextColor(2); display.setCursor(110,54);
  display.print("%");

  display.setTextSize(1); display.setTextColor(2); display.setCursor(80,24);
  display.print(cycle);



  }


void clear(){
  display.clearDisplay();
  display.setCursor(0,0); 
  }

int read(byte address){

  while (Wire.available()){Wire.read();delay(10);}
  Wire.beginTransmission(0x0B);
  Wire.write(byte(address));
  Wire.endTransmission();  Wire.requestFrom(0x0B, 2);delay(10);
   int k = 0;int i = 0;
   byte b1 = 0;
   byte b2 = 0;

    if (Wire.available()){    
      b1 =  Wire.read();      delay(10);
          if (Wire.available()){
              b2 =  Wire.read(); 
              cli();  k = b2<<8;  k+=b1;  sei(); return k; }
      }
      
  return -1;
  }

void fun()
{//display.setTextSize(2); display.setTextColor(2); display.setCursor(38,24);
//display.println("MeoW");
//display.drawBitmap(0,0,logo,128,64,colour);
display.drawBitmap(32,0,apple,64,64,colour);
display.display();delay(600);
//display.drawBitmap(32,0,apple,64,64,!colour);
//display.setCursor(32,24);//display.setTextColor(2);display.println("GoPro");display.display();delay(500);
}

void scan(){
    int nDevices = 0;
display.setTextSize(1); display.setTextColor(2); display.setCursor(0,0);
  display.println("Scanning...");

  for (byte address = 1; address < 127; ++address) {

    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      display.print("I2C device: 0x");
      if (address < 16) {
        display.print("0");
      }
      display.print(address, HEX);
      display.println();

      ++nDevices;
    } else if (error == 4) {
      display.print("Unknown error: 0x");
      if (address < 16) {
        display.print("0");
      }
      display.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    display.println("No I2C devices found\n");
  } else {
    display.println();
    display.print(nDevices);
    display.println(" devices found");
  }
  display.display();
  delay(1000); // Wait 5 seconds for next scan
  }
