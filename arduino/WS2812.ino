#define PIN_R (9)
#define PIN_G (10)
#define PIN_B (11)

typedef struct
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
} RGB;

const RGB none = {0, 0, 0};
const RGB red = {255, 0, 0};
const RGB green = {0, 255, 0};
const RGB blue = {0, 0, 255};

RGB currentColor;

RGB mixColor(RGB first, RGB second, float factor)
{
  RGB color;
  
  color.r = first.r * (1.0f - factor) + second.r * factor;
  color.g = first.g * (1.0f - factor) + second.g * factor;
  color.b = first.b * (1.0f - factor) + second.b * factor;

  return color;
}

void setColor(RGB color)
{
  analogWrite(PIN_R, color.r);
  analogWrite(PIN_G, color.g);
  analogWrite(PIN_B, color.b);

  currentColor = color;
}

void fadeFromTo(RGB from, RGB to, size_t duration)
{
  for (float p = 0.0f; p < 1.0f; p += 0.01f)
  {
    setColor(mixColor(from, to, p));
    delay(duration / 100);
  }
}

void fadeTo(RGB to, size_t duration)
{
    fadeFromTo(currentColor, to, duration);
}

void flash(RGB color, size_t duration)
{
  RGB tmp = currentColor;
  fadeTo(color, duration / 2);
  fadeTo(tmp, duration / 2);
}

void setup()
{
  Serial.begin(115200);
  
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
}

void loop()
{
  fadeTo(green, 3000);
  fadeTo(red, 3000);
  fadeTo(blue, 3000);
}


