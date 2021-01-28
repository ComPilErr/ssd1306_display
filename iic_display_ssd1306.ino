#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define WIDTH 128
#define HEIGHT 64
#define ADDRESS 0x3C
#define LINES 3
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

class Ma_Item
{
  public:
  String ma_name;
  int ma_val;
  Ma_Item(String a, int b){
    ma_name = a;
    ma_val = b;}
  };

static const Ma_Item line[]{Ma_Item("temperature:",0x08),Ma_Item("capasity:",0x0F),Ma_Item("current:",0x0A),Ma_Item("voltage:",0x09)};

static const unsigned char PROGMEM dick[]{
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFC, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x1F, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x0F, 0xC0, 0x00, 0x00,
0x00, 0x00, 0x01, 0xF8, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0x03, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x03, 0xF0, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF0, 0x01, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x01, 0xF0, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF8, 0x05, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x00, 0xFC, 0x0D, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x02, 0x7F, 0xFD, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x02, 0x7F, 0xFD, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x03, 0x3F, 0xFD, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x03, 0x8F, 0xFD, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE3, 0xF1, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x07, 0xE0, 0x03, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0xEC, 0x0F, 0xF6, 0x00, 0x00,
0x00, 0x00, 0x1F, 0xEE, 0x1F, 0xF7, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xEF, 0xBF, 0xF7, 0x80, 0x00,
0x00, 0x00, 0xFF, 0xEF, 0x9F, 0xF7, 0xC0, 0x00, 0x00, 0x01, 0xF8, 0x0F, 0xCC, 0x07, 0xE0, 0x00,
0x00, 0x03, 0xF0, 0x07, 0xE8, 0x03, 0xF0, 0x00, 0x00, 0x03, 0xF0, 0x03, 0xE0, 0x01, 0xF0, 0x00,
0x00, 0x03, 0xE0, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x00, 0x03, 0xE0, 0x03, 0xE0, 0x00, 0xF0, 0x00,
0x00, 0x03, 0xC0, 0x01, 0xF0, 0x00, 0xF0, 0x00, 0x00, 0x03, 0xC0, 0x01, 0xF0, 0x00, 0xF0, 0x00,
0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0xF0, 0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x01, 0xF0, 0x00,
0x00, 0x03, 0xF0, 0x01, 0xF8, 0x01, 0xF0, 0x00, 0x00, 0x01, 0xF0, 0x04, 0xF8, 0x03, 0xF0, 0x00,
0x00, 0x01, 0xFC, 0x0E, 0xFE, 0x0F, 0xE0, 0x00, 0x00, 0x00, 0xFF, 0xFE, 0x7F, 0xFF, 0xC0, 0x00,
0x00, 0x00, 0x7F, 0xFF, 0x3F, 0xFF, 0xC0, 0x00, 0x00, 0x00, 0x3F, 0xFE, 0x1F, 0xFF, 0x00, 0x00,
0x00, 0x00, 0x1F, 0xFC, 0x0F, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0x01, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

static const unsigned char PROGMEM apple[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x07, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x3F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xE0, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x7F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x80, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0x00,
0x00, 0x00, 0x0F, 0x00, 0x00, 0xFE, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xF0, 0x07, 0xFF, 0x00, 0x00,
0x00, 0x01, 0xFF, 0xFE, 0x3F, 0xFF, 0xE0, 0x00, 0x00, 0x07, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00,
0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00,
0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x00,
0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00,
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00,
0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00,
0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00,
0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00,
0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00,
0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00,
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xC0, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x00,
0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x00,
0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC, 0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x00,
0x00, 0x1F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x00, 0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0x00,
0x00, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00,
0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x00,
0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xE0, 0x00,
0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFF, 0xFF, 0x80, 0x00,
0x00, 0x00, 0x1F, 0xFC, 0x07, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xE0, 0x01, 0xFC, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  };

static const unsigned char PROGMEM logo[] =
{  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xC0, 0x18, 0x00,
0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x60, 0x7E, 0x00,
0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x30, 0xDB, 0x00,
0x01, 0x86, 0x0F, 0x0C, 0x0C, 0x3C, 0x37, 0x07, 0x81, 0xC3, 0x19, 0xF8, 0x0C, 0x31, 0x99, 0x80,
0x01, 0x86, 0x19, 0x8E, 0x1C, 0x66, 0x39, 0x8C, 0xC3, 0x63, 0x38, 0x60, 0x0C, 0x31, 0x99, 0x80,
0x01, 0xFE, 0x19, 0x8E, 0x1C, 0x66, 0x31, 0x8C, 0xC0, 0x63, 0x38, 0x60, 0x0C, 0x61, 0x99, 0x80,
0x01, 0x86, 0x1F, 0x8B, 0x2C, 0x66, 0x31, 0x8C, 0xC0, 0xC3, 0x58, 0x60, 0x0F, 0xC1, 0x99, 0x80,
0x01, 0x86, 0x18, 0x0B, 0x2C, 0x66, 0x31, 0x8C, 0xC0, 0x63, 0x58, 0x60, 0x0C, 0x01, 0x99, 0x80,
0x01, 0x86, 0x19, 0x89, 0xCC, 0x66, 0x31, 0x8C, 0xC0, 0x63, 0x98, 0x60, 0x0C, 0x00, 0xDB, 0x00,
0x01, 0x86, 0x19, 0x89, 0xCC, 0x66, 0x39, 0x8C, 0xC3, 0x63, 0x98, 0x61, 0x8C, 0x00, 0x7E, 0x00,
0x01, 0x86, 0x0F, 0x09, 0x8C, 0x3C, 0x37, 0x07, 0x81, 0xC3, 0x18, 0x61, 0x8C, 0x00, 0x18, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void setup() {
Serial.begin(115200);
if ( !display.begin(SSD1306_SWITCHCAPVCC, ADDRESS) ){Serial.println("Fuck off"); for(;;);}
randomSeed(analogRead(0));
display.clearDisplay();
display.drawBitmap(32,0,apple,64,64,1);
display.display();
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

  temp =          read(0x08);
  voltage =       read(0x09);
  current =       read(0x0A);
  capasity =      read(0x0F); 
  base_capasity = read(0x10);
  cycle = read(0x3c);

  }

void show(){
//  if (name == "temperature:") val = val/10 - 273.15;
//  if (name == "voltage:") val = val/1000;  
  
  display.setTextSize(1); display.setTextColor(2); display.setCursor(80,0);
  display.print("temp:");
  display.setTextSize(1); display.setTextColor(2); display.setCursor(80,10);
  display.print( ( (temp/10) - 273.15) );

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

  display.setTextSize(1); display.setTextColor(2); display.setCursor(80,24);
  display.print(cycle);



  }


void clear(){
  display.clearDisplay();
  display.setCursor(0,0); 
  }

int read(byte address){

  Wire.beginTransmission(0x0B);
  Wire.write(byte(address));
  Wire.endTransmission();  Wire.requestFrom(0x0B, 2);delay(60);
   int k = 0;
   byte b1=0;
   byte b2 = 0;

    while (Wire.available()){
    b1 =  Wire.read();
    b2 =  Wire.read();  }
  cli();
  k = b2<<8;  k+=b1;
  sei();
  delay(40);
  while (Wire.available()){Wire.read();}
  return k;

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
