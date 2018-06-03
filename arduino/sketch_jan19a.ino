#include <OneWire.h>
#include <DallasTemperature.h>


#define ONE_WIRE_BUS A0
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


void setup() {
  pinMode(ONE_WIRE_BUS, OUTPUT);
  pinMode(10, OUTPUT);
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
  
  digitalWrite(10, !isEnabled);
  delay(1000);
}


float readValue() {
  return value;
}

