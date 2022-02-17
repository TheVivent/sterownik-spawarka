#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/SCL, /* data=*/SDA, /* reset=*/U8X8_PIN_NONE); // OLEDs without Reset of the Display

// U8G2_SSD1306_128X64_NONAME_1_SW_I2C u8g2(U8G2_R0, /* clock=*/SCL, /* data=*/SDA, /* reset=*/U8X8_PIN_NONE); // All Boards without Reset of the Display
// U8G2_SSD1306_128X64_NONAME_2_SW_I2C u8g2(U8G2_R0, /* clock=*/SCL, /* data=*/SDA, /* reset=*/U8X8_PIN_NONE); // All Boards without Reset of the Display
// U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/SCL, /* data=*/SDA, /* reset=*/U8X8_PIN_NONE); // All Boards without Reset of the Display

// general
int start_time = 0;
int wait_time = 10;
int last_wait_time = 0;
char time_str[3];

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
  // UNRIELABLE because, display updates take too long
  // if (start_time != 0)
  // {
  //   digitalWrite(RELAY, HIGH);
  //   if (millis() - start_time > wait_time)
  //   {
  //     digitalWrite(RELAY, LOW);
  //     start_time = 0;
  //   }
  // }

  // RELIABLE, but crashes the display
  digitalWrite(RELAY, HIGH);
  delay(wait_time);
  digitalWrite(RELAY, LOW);
}

void setup()
{
  // ------------- OLED SETUP -------------
  u8x8.begin();
  u8x8.setPowerSave(0);
  // u8x8.setFont(u8x8_font_inb46_4x8_n); // I would like to use this font, but it doesn't work (bottom flickers)
  u8x8.setFont(u8x8_font_inb33_3x6_n);

  // ------------- ROTARY ENCODER SETUP -------------
  pinMode(ROT_CLK, INPUT);
  pinMode(ROT_DT, INPUT);
  pinMode(ROT_SW, INPUT_PULLUP);
  rotary_encoder_pos = digitalRead(ROT_CLK);

  // ------------- RELAY SETUP -------------
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
}

void draw(void)
{
  // draw welding time
  u8x8.drawString(5, 1, time_str);
  // delay(1);

  // u8x8.drawString(0, 1, "Hello World!");
  // u8x8.drawString(0, 0, "Line 0");
  // u8x8.drawString(0, 2, "Line 8");
  // u8x8.drawString(0, 9, "Line 9");
  // delay(2000);
  // u8x8.refreshDisplay(); // only required for SSD1606/7
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
    itoa(wait_time, time_str, 10);
    last_wait_time = wait_time;
  }

  // ----------------- OLED -----------------
  draw();

  // ----------------- ODCZYT PRZYCISKU -----------------
  // delay(2); // needed to avoid reading push on falling edge
  // (but display takes too much time to update, so for now it's not needed)
  if (digitalRead(ROT_SW) == LOW)
  {
    if (!was_pressed)
    {
      was_pressed = true;
      start_time = millis();
      // uncomment when using rielable method
      weld();
    }
  }
  else
  {
    was_pressed = false;
  }

  // uncomment when using unrielable method
  // weld();
}