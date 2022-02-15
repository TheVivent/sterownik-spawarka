#include <Arduino.h>

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
  // if (do_weld) {
  //   digitalWrite(RELAY, HIGH);

  // }
  digitalWrite(RELAY, HIGH);
  delay(wait_time);
  digitalWrite(RELAY, LOW);
}

void setup()
{
  pinMode(ROT_CLK, INPUT);
  pinMode(ROT_DT, INPUT);
  pinMode(ROT_SW, INPUT_PULLUP);
  pinMode(RELAY, OUTPUT);
  digitalWrite(RELAY, LOW);
  rotary_encoder_pos = digitalRead(ROT_CLK);
  Serial.begin(9600);
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

  // ----------------- ODCZYT PRZYCISKU -----------------
  // potestować z mniejszym czasem
  // stabilizacja napięcia trwa za długo
  // i nieraz jeżeli czas jest za krótki,
  // to potrafi uruchomić się przy odpuszczeniu
  delay(10);
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