#include <SoftwareSerial.h>

#define MODEM_RX  (A3)
#define MODEM_TX  (A1)
#define MODEM_RST (A0)
#define SENSOR    (9)
#define SIREN     (10)
#define BUTTON    (11)
#define DOOR      (12)
#define BREAK_TIME (60000)
#define PHONE_NUMBER ("732253178")

SoftwareSerial ModemSerial(MODEM_TX, MODEM_RX); // RX TX for CPU, not modem!

void resetModem(void)
{
  Serial.println("Performing modem reset...");
  digitalWrite(MODEM_RST, LOW);
  delay(500);
  digitalWrite(MODEM_RST, HIGH);
}

bool isSensorDetectingGas(void)
{
  return (digitalRead(SENSOR) == LOW);
}

bool isButtonPressed(void)
{
  return (digitalRead(BUTTON) == HIGH);
}

bool isDoorOpen(void)
{
  return (digitalRead(DOOR) == LOW);
}

void enableSiren(void)
{
  Serial.println("Siren enabled.");
  digitalWrite(SIREN, HIGH);
}

void disableSiren(void)
{
  Serial.println("Siren disabled.");
  digitalWrite(SIREN, LOW);
}

bool wasDoorOpen = false;

void setup()
{
  pinMode(MODEM_RST, OUTPUT);
  pinMode(SENSOR, INPUT);
  pinMode(SIREN, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(DOOR, INPUT);
  
  ModemSerial.begin(9600);
  Serial.begin(9600);

  Serial.println("Device started.");

  resetModem();

  Serial.println("Waiting 10s until sensor's temperarature is stabilized...");
  delay(10000);
  
  Serial.println("Wait until sensor is warmed up...");
  while (isSensorDetectingGas());

  wasDoorOpen = isDoorOpen();
}

bool isAlarmExecuted = false;
bool wasGasDetected = false;



// timeout
bool isTimeoutSet = false;
uint32_t lastDisableTime = 0;

void sendSms(const char *number, const char *text)
{
  char buffer[128] = {0};
  size_t len = sprintf(buffer, "AT+CMGS=\"%s\"\n%s\x1a", number, text);
  ModemSerial.write(buffer, len);  
}

void sendCusd(const char *number)
{
  char buffer[128] = {0};
  size_t len = sprintf(buffer, "AT+CUSD=1,\"%s\"\n", number);
  ModemSerial.write(buffer, len); 
}

void readSms(const char *smsNumber)
{
  char buffer[128] = {0};
  size_t len = sprintf(buffer, "AT+CMGR=%s\n", smsNumber);
  ModemSerial.write(buffer, len); 
}

#define MAX_ARGC 8

void parseResponse(char *response, size_t length, size_t *argcp, char ***argvp)
{
  size_t argc = 0;
  char *argv[MAX_ARGC] = { 0 };

  static char argBuffer[256];
  size_t argBufferLength = 0;

  bool isInString = false;

  for (size_t i = 0; i < length; ++i)
  {
    char c = response[i];

    switch (c)
    {
    case ',':
      if (isInString)
      {
        argBuffer[argBufferLength++] = c;
      }
      else
      {
        argv[argc] = new char[argBufferLength + 1];
        argv[argc][argBufferLength] = '\0';
        memcpy(argv[argc], argBuffer, argBufferLength);
  
        argBufferLength = 0;
  
        ++argc;
      }

      break;
    case ' ':
      if (isInString)
      {
        argBuffer[argBufferLength++] = c;
      }

      break;
    case '"':
      isInString = !isInString;

      break;
    default:
      argBuffer[argBufferLength++] = c;
    }
  }

  if (response[length - 1] == ',' || argBufferLength > 0)
  {
    argv[argc] = new char[argBufferLength + 1];
    argv[argc][argBufferLength] = '\0';
    memcpy(argv[argc], argBuffer, argBufferLength);

    ++argc;
  }

  *argcp = argc;
  *argvp = new char*[argc];
  memcpy(*argvp, &argv, sizeof(void*) * argc);
}

void deleteParsed(size_t argc, char **argv)
{
  for (size_t i = 0; i < argc; ++i)
    delete[] argv[i];

  delete[] argv;
}

bool willReceiveSmsBody = false;
bool waitForCusdResponse = false;
static char smsNumber[16] = {0};

void strtolower(char *s)
{
  for(; *s; ++s)
    *s = tolower(*s);
}

void processModemResponse(char *response, size_t length)
{
  if (willReceiveSmsBody)
  {
    /*strtolower(response);
    if (strncmp(response, "stan", 4) == 0)
    {
      waitForCusdResponse = true;
      
      sendCusd("*101#");
    }
    
    willReceiveSmsBody = false;*/
  }
 
  if (length > 0)
  {
    Serial.print("~ ");
    Serial.println(response);
    
    if (response[0] == '+')
    {
      char *end = strstr(response, ":");
      size_t codeLength = end - response - 1;

      size_t argc = 0;
      char **argv = NULL;

      size_t offset = codeLength + 2;
      
      parseResponse(&response[offset], length - offset, &argc, &argv);

      for (size_t i = 0; i < argc; ++i)
      {
        Serial.println(argv[i]);
      }

      if (strncmp(&response[1], "CMTI", codeLength) == 0)
      {
        readSms(argv[1]);
      }
      else if (strncmp(&response[1], "CMGR", codeLength) == 0)
      {
        willReceiveSmsBody = true;
        
        memcpy(smsNumber, argv[1], strlen(argv[1]));
      }
      else if (strncmp(&response[1], "CUSD", codeLength) == 0)
      {
        if (waitForCusdResponse)
        {
          waitForCusdResponse = false;

          sendSms(smsNumber, argv[1]);
          memset(smsNumber, 0, 16);
        }
      }

      deleteParsed(argc, argv);
    }
  }
}

static char modemRxBuffer[256] = {0};
size_t modemRxBufferLength = 0;

void loop()
{
  uint8_t countModemBytes = ModemSerial.available();
  if (countModemBytes > 0)
  {
    char *buffer = new char[countModemBytes];

    ModemSerial.readBytes(buffer, countModemBytes);
    
    for (size_t i = 0; i < countModemBytes; ++i)
    {
      if (buffer[i] == '\n')
      {
        processModemResponse(modemRxBuffer, modemRxBufferLength);

        memset(modemRxBuffer, 0, 256);
        modemRxBufferLength = 0;
      }
      else
      {
        modemRxBuffer[modemRxBufferLength++] = buffer[i];
      }
    }

    delete[] buffer;
  }

  uint8_t countBytes = Serial.available();
  if (countBytes > 0)
  {
    char *buffer = new char[countBytes];

    Serial.readBytes(buffer, countBytes);
    ModemSerial.write(buffer, countBytes);
    
    delete[] buffer;
  }

  if (isTimeoutSet)
  {
    if (millis() < lastDisableTime)
    {
      Serial.println("Counter is rolled-over. Timeout cleared.");

      isTimeoutSet = false;
    }
    else
    {
      uint32_t elapsedTime = millis() - lastDisableTime;

      if (elapsedTime >= BREAK_TIME)
      {
        Serial.println("Break time elapsed. Timeout cleared.");
      
        isTimeoutSet = false;
      }
    }
  }

  if (!wasGasDetected && isSensorDetectingGas())
  {
    Serial.println("Gas detected!");
    
    if (!isAlarmExecuted)
    {
      if (isTimeoutSet)
      {
        Serial.println("Siren is not enabled due to device's time-break state.");
      }
      else
      {
        enableSiren();
        isAlarmExecuted = true;
        sendSms(PHONE_NUMBER, "Gaz sie ulatnia!");
      }
    }
    else
    {
      Serial.println("Alarm is already executed.");
    }

    wasGasDetected = true;
  }
  else if (wasGasDetected && !isSensorDetectingGas())
  {
    Serial.println("Gas isn't longer detected.");
    
    wasGasDetected = false;
  }

  if (wasDoorOpen && !isDoorOpen())
  {
    Serial.println("Door closed.");

    wasDoorOpen = false;
  }
  else if (!wasDoorOpen && isDoorOpen())
  {
    Serial.println("Door opened.");

    wasDoorOpen = true;
  }
  
  if (isAlarmExecuted && isButtonPressed())
  {
    Serial.println("Button pressed.");
    
    disableSiren();
    isAlarmExecuted = false;

    Serial.println("Entered time-break state.");
    isTimeoutSet = true;
    lastDisableTime = millis();
  }
}
