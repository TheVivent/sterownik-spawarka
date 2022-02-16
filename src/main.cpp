#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

// general
int wait_time = 10;
int last_wait_time = 0;

// ROTARY ENCODER
int ROT_CLK = 8;
int ROT_DT = 9;
int ROT_SW = 16;
int rotary_encoder_pos = 0;

// relay
int RELAY = 15;
boolean was_pressed = false;

int detect_rotation_direction()
{
  int val = digitalRead(ROT_CLK);

  if (val == rotary_encoder_pos)
  {
    rotary_encoder_pos = val;
    return 0;
  }

  rotary_encoder_pos = val;
  if (digitalRead(ROT_DT) == val)
    return -1;
  return 1;
}

void weld()
{
  digitalWrite(RELAY, HIGH);
  delay(wait_time);
  digitalWrite(RELAY, LOW);
}

void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
  delay(100);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // Address 0x3D for 128x64
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ; // Don't proceed, loop forever
  }
  delay(2000);

  Serial.println("Writing to display");
  display.clearDisplay();
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.println("Hello, world!");
  display.display();
  delay(2000);
  Serial.println("Done writing to display");

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  // display.display();
  // delay(2000);

  pinMode(ROT_CLK, INPUT);
  pinMode(ROT_DT, INPUT);
  pinMode(ROT_SW, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  rotary_encoder_pos = digitalRead(ROT_CLK);
}

void loop()
{
  // ----------------- ODCZYT CZASU -----------------
  wait_time += detect_rotation_direction();
  if (wait_time > 99)
    wait_time = 10;
  if (wait_time < 10)
    wait_time = 99;
  if (wait_time != last_wait_time)
  {
    Serial.println(wait_time);
  }
  last_wait_time = wait_time;

  // Clear the buffer
  // display.clearDisplay();
  // display.drawRect(10, 10, 10, 10, WHITE);
  // display.display();

  // ----------------- ODCZYT PRZYCISKU -----------------
  delay(2);
  if (digitalRead(ROT_SW) == LOW)
  {
    if (!was_pressed)
    {
      was_pressed = true;
      weld();
    }
  }
  else
  {
    was_pressed = false;
  }
}