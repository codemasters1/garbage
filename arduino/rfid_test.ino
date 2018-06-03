#include <SPI.h>
#include <MFRC522.h>

#define PIN_RST 9
#define PIN_SS 10
#define PIN_BEEPER 5
#define PIN_RELAY 8

MFRC522 reader(PIN_SS, PIN_RST);
bool isOpen = false;

void setup()
{
  Serial.begin(115200);
  SPI.begin();
  reader.PCD_Init();
  pinMode(PIN_RELAY, OUTPUT);
  pinMode(PIN_BEEPER, OUTPUT);
}

void beepSuccess()
{
  digitalWrite(PIN_BEEPER, HIGH);
  delay(500);
  digitalWrite(PIN_BEEPER, LOW);
}

void beepFailure()
{
  bool isOn = true;
  for (int i = 0; i <= 4; ++i)
  {
    isOn = !isOn;
    digitalWrite(PIN_BEEPER, isOn);
    delay(75);
  }
}

void loop()
{
  if (!reader.PICC_IsNewCardPresent())
    return;
    
  if (!reader.PICC_ReadCardSerial())
    return;
    
  for (byte i = 0; i < reader.uid.size; i++)
  {
    if (reader.uid.uidByte[i] < 0x10)
      Serial.print('0');
    Serial.print(reader.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  
  reader.PICC_HaltA();
  
  char allowedUid[4] = {0x07, 0x3A, 0xA7, 0xD5};
  
  if (memcmp(&reader.uid.uidByte[0], &allowedUid[0], 4) == 0)
  {
    isOpen = !isOpen;
    digitalWrite(PIN_RELAY, isOpen);
    
    beepSuccess();
  }
  else
  {
    beepFailure();
  }
}
