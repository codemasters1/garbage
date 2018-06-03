#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, 6, NEO_RGB + NEO_KHZ800);

void setup() {
  strip.begin();
}

const uint32_t dt = 5;
uint8_t color[3] = {0};

void transition(uint8_t *to, uint16_t duration)
{
  int16_t delta[3];
  delta[0] = to[0] - color[0];
  delta[1] = to[1] - color[1];
  delta[2] = to[2] - color[2];

  uint16_t n = duration / dt;

  for (uint16_t i = 0; i <= n; i++)
  {
    uint8_t cc[3];
    
    if (i == n)
    { 
      memcpy(cc, to, 3);
      memcpy(color, to, 3);
    }
    else
    {
      cc[0] = color[0] + delta[0] * i / n;
      cc[1] = color[1] + delta[1] * i / n;
      cc[2] = color[2] + delta[2] * i / n;
    }
    
    for (int i = 0; i < 30; i++)
    {
      strip.setPixelColor(i, cc[0], cc[1], cc[2]);
    }
    strip.show();
    
    delay(dt);
  }
}

struct color_t {
  uint8_t color[3];
  uint16_t duration;
};

void loop() {
  color_t colors[] = {
    {{255, 0, 0}, 5000},
    {{0, 255, 0}, 5000},
    {{0, 0, 255}, 5000},
    {{255, 255, 0}, 5000},
    {{0, 255, 255}, 5000},
    {{255, 0, 255}, 5000}
   };
  
  for (int i = 0; i < sizeof(colors); i++)
  {
    
    transition(colors[i].color, colors[i].duration);
  }
}
