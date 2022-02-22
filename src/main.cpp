#include <Arduino.h>
#include <U8g2lib.h>
#include <EEPROM.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/SCL, /* data=*/SDA, /* reset=*/U8X8_PIN_NONE); // OLEDs without Reset of the Display

// general
int wait_time = 10;
int last_wait_time = 0;
char time_str[3];

// ROTARY ENCODER
int ROT_CLK = 7; // extINT 6
int ROT_DT = 9;
int ROT_SW = 16;
int rotary_encoder_pos = 0;

// BUTTON
int WELD_BUTTON = 5;

// relay
int RELAY = 15;
boolean was_pressed = false;

void detect_rotation_direction()
{
  int val = digitalRead(ROT_CLK);

  if (val == rotary_encoder_pos)
  {
    rotary_encoder_pos = val;
    return;
  }

  rotary_encoder_pos = val;
  if (digitalRead(ROT_DT) == val)
  {

    wait_time--;
    return;
  }
  wait_time++;
  return;
}

void weld()
{
  digitalWrite(RELAY, HIGH);
  delay(wait_time);
  digitalWrite(RELAY, LOW);
  u8x8.begin();
}

void setup()
{
  // ------------- OLED SETUP -------------
  u8x8.begin();
  u8x8.setPowerSave(0);
  // u8x8.setFont(u8x8_font_inb46_4x8_n); // I would like to use this font, but it doesn't work (bottom flickers)
  u8x8.setFont(u8x8_font_inb33_3x6_r);

  // ------------- ROTARY ENCODER SETUP -------------
  pinMode(ROT_CLK, INPUT); // to interrupt
  pinMode(ROT_DT, INPUT);

  attachInterrupt(digitalPinToInterrupt(ROT_CLK), detect_rotation_direction, CHANGE);

  pinMode(ROT_SW, INPUT_PULLUP);
  rotary_encoder_pos = digitalRead(ROT_CLK);
  wait_time = EEPROM.read(0);

  // ------------- WELD BUTTON SETUP -------------
  pinMode(WELD_BUTTON, INPUT_PULLUP);

  // ------------- RELAY SETUP -------------
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
}

void draw(void)
{
  // draw welding time
  u8x8.drawString(5, 1, time_str);
}

void loop()
{
  // ----------------- ODCZYT CZASU -----------------
  if (wait_time > 99)
    wait_time = 10;
  if (wait_time < 10)
    wait_time = 99;
  if (wait_time != last_wait_time)
  {
    itoa(wait_time, time_str, 10);
    last_wait_time = wait_time;
  }

  // ----------------- OLED -----------------
  draw();

  // ----------------- ODCZYT WELD BUTTON -----------------
  if (digitalRead(WELD_BUTTON) == LOW)
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

  // ----------------- ROT_SW zapis do EEPROM -----------------
  if (digitalRead(ROT_SW) == LOW)
  {
    EEPROM.put(0, wait_time);
    u8x8.begin();
    u8x8.drawString(5, 1, "OK");
    u8x8.drawString(5, 1, "OK");
    u8x8.drawString(5, 1, "OK");
    u8x8.drawString(5, 1, "OK");
    u8x8.drawString(5, 1, "OK");
  }
}