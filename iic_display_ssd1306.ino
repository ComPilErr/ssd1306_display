
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define WIDTH 128
#define HEIGHT 64
#define ADDRESS 0x3C
#define LINES 3
#define REFRESH 16

#define SCL PB6
#define SDA PB7
bool colour = 1;
uint16_t i = 0;

int voltage=0;
int current=0;
int capasity=0;
int base_capasity=0;
int temp=0;
int cycle=0;
int dev;

#define MY_LED_PIN PC13
HardwareSerial My_Serial(PA10, PA9);
TwoWire My_Wire(SDA,SCL);
Adafruit_SSD1306 display(WIDTH, HEIGHT,&My_Wire);

void setup() {
  // initialize digital pin PB2 as an output.
  pinMode(MY_LED_PIN, OUTPUT); // LED connect to pin PB2
  digitalWrite(MY_LED_PIN, HIGH);
  My_Serial.begin(115200);
  if ( !display.begin(SSD1306_SWITCHCAPVCC, ADDRESS) ){My_Serial.println("Fuck off"); for(;;);}
  display.clearDisplay();
  intro(1);  intro(0);
  dev = scan();
  intro(1);  intro(0);
}
void loop() {
if (dev > 1)
{
if( i>REFRESH )
  {
  digitalWrite(MY_LED_PIN, LOW);
  processing();
  digitalWrite(MY_LED_PIN, HIGH);
  display.fillRect(0,0,WIDTH,HEIGHT, colour);
  clear();
  show();
  display.display();
  }
    i++;
    My_Serial.print("Another one bites the dust: ");
    My_Serial.println(i);
}
else {  
  digitalWrite(MY_LED_PIN, LOW);
  delay(100);
  digitalWrite(MY_LED_PIN, HIGH);
  delay(900);
  display.fillCircle(random(WIDTH),random(HEIGHT),random(5),colour);
  display.display();}
}

void processing()
{
  i = 0; 
  int x = read(0x17);

  x = read(0x08); temp = (x==-1) ? temp:x;
  x = read(0x09);voltage =(x==-1) ? voltage:x;
  x = read(0x0A);current =(x==-1) ? current:x;
  x = read(0x0F);capasity =(x==-1) ? capasity:x;
  x = read(0x10);base_capasity =(x==-1) ? base_capasity:x;
  x = read(0x3c);cycle =  (x==-1) ? cycle:x;

  //if(current > 0)  {digitalWrite(PWR_PIN,LOW);delay(10);                    digitalWrite(PWR_PIN,HIGH);
  //                  digitalWrite(PWR_PIN,LOW);delay(10);                    digitalWrite(PWR_PIN,HIGH);}
  }

int read(byte address){
  while (My_Wire.available()){My_Wire.read();delay(10);}
  
  My_Wire.beginTransmission(0x0B);
  My_Wire.write(byte(address));
  My_Wire.endTransmission();  
  
  My_Wire.requestFrom(0x0B, 2);delay(10);
   int k = 0;int i = 0;
   byte b1 = 0;
   byte b2 = 0;

    if (My_Wire.available()){    
      b1 =  My_Wire.read();      delay(10);
          if (My_Wire.available()){
              b2 =  My_Wire.read(); 
              __disable_irq();
              k = b2<<8;  k+=b1;
              __enable_irq();
              return k; }
      }    
  return -1;
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
  display.setTextSize(1); display.setTextColor(2); display.setCursor(114,54);
  display.print("%");

  display.setTextSize(1); display.setTextColor(2); display.setCursor(80,24);
  display.print(cycle);
  }

void clear(){
  display.clearDisplay();
  display.setCursor(0,0); 
  }

void intro(int colour)
{
    for(int i=0;i<24;i++){
    display.fillCircle(random(WIDTH)/2,random(HEIGHT)/2,i,colour);
    display.fillCircle(WIDTH - random(WIDTH)/2,random(HEIGHT)/2,i,colour);
    display.fillCircle(random(WIDTH)/2,HEIGHT - random(HEIGHT)/2,i,colour);
    display.fillCircle(WIDTH - random(WIDTH)/2,HEIGHT -random(HEIGHT)/2,i,colour);
    display.display();
    }
  display.fillRect(0,0,WIDTH,HEIGHT, colour);  display.display();
}

int scan(){
  int nDevices = 0;
  display.setTextSize(1); display.setTextColor(2); display.setCursor(0,0);
  display.println("Scanning...");

  for (byte address = 1; address < 127; ++address) {

    My_Wire.beginTransmission(address);
    byte error = My_Wire.endTransmission();

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
  delay(3000); // Wait 5 seconds for next scan
  return nDevices;
  }
