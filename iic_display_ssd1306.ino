
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define WIDTH 128
#define HEIGHT 64
#define ADDRESS 0x3C
#define LINES 3
#define REFRESH 16

#define BATTERY 0x0B

#define SCL PB6
#define SDA PB7

#define RX PA10
#define TX PA9

#define MY_LED_PIN PC13

#define I2C_DEVICES_MAX 8

bool colour = 1;

int16_t voltage=0;
int16_t current=0;
int16_t capasity=0;
int16_t base_capasity=0;
int16_t temp=0;
int16_t cycle=0;

HardwareSerial My_Serial(RX, TX);
TwoWire My_Wire(SDA, SCL);
Adafruit_SSD1306 display(WIDTH, HEIGHT,&My_Wire);

byte device_array[I2C_DEVICES_MAX];

void setup() {

  My_Serial.begin(115200);
  My_Serial.println("Starting initialization...");
  memset(device_array,0,sizeof(byte)*I2C_DEVICES_MAX);
  delay(1000);
  pinMode(MY_LED_PIN, OUTPUT);
  digitalWrite(MY_LED_PIN, HIGH);

  if ( !display.begin(SSD1306_SWITCHCAPVCC, ADDRESS) ){My_Serial.println("Error display initialisation"); for(;;);}
  display.clearDisplay();
  intro(1);  intro(0);
  scan();
  intro(1);  intro(0);
}

void loop() {
if (read_device(BATTERY))
{
  digitalWrite(MY_LED_PIN, LOW);
  processing();
  digitalWrite(MY_LED_PIN, HIGH);
  display.fillRect(0,0,WIDTH,HEIGHT, colour);
  clear();
  show();
  display.display();
  My_Serial.println( String((float)voltage/1000) + " | " + String((float) current/1000) + " | " + String((((float) temp)/10)-273.15) + " | " + String((int) (((float) capasity/base_capasity )*100)) + " | " + String(cycle));
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
  int16_t x = read(0x17);

  x = read(0x08); temp = (x==-1) ? temp:x;
  x = read(0x09);voltage =(x==-1) ? voltage:x;
  x = read(0x0A);current =(x==-1) ? current:x;
  x = read(0x0F);capasity =(x==-1) ? capasity:x;
  x = read(0x10);base_capasity =(x==-1) ? base_capasity:x;
  x = read(0x3d);cycle =  (x==-1) ? cycle:x;

  }

int read(byte address){
  while (My_Wire.available()){My_Wire.read();delay(10);}
  
  My_Wire.beginTransmission(0x0B);
  My_Wire.write(byte(address));
  My_Wire.endTransmission();  
  
  My_Wire.requestFrom(0x0B, 2);delay(10);
   int16_t k = 0;
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

int read_device(byte x)
{
for (int i=0; i<I2C_DEVICES_MAX; i++)
  {
    if (device_array[i] == x) return 1;
  }
return 0;
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
      device_array[nDevices] = address;
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
  delay(3000); // Hold on a moment
  return nDevices;
  }
