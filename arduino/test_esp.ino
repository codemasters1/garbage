#include <OneWire.h>
#include <DallasTemperature.h>

#define ONEWIRE 14
#define LED 13
#define RELAY 12

OneWire oneWire(ONEWIRE);
DallasTemperature sensors(&oneWire);

void setup() {
  pinMode(ONEWIRE, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  sensors.begin();
}

const float delta = 2.5;
const float threshold = 25.0;

bool isEnabled = false;
float value;

void loop() {
  sensors.requestTemperatures();
  value = sensors.getTempCByIndex(0);
  Serial.println(value);

  const auto currentValue = readValue();
  if (currentValue < threshold - delta) {
    isEnabled = false;
  } else if (currentValue >= threshold) {
    isEnabled = true;
  } else {
    // no changes
  }
  
  digitalWrite(RELAY, isEnabled);

  if (isEnabled) {
    digitalWrite(LED, false);
    delay(1000);
  } else {
    digitalWrite(LED, false);
    delay(100);
    digitalWrite(LED, true);
    delay(900);
  }
}


float readValue() {
  return value;
}

