#include <AH_AD9850.h>

//AH_AD9850(int CLK, int FQUP, int BitData, int RESET);
AH_AD9850 AD9850(9, 8, 7, 6);

void setup()
{
  AD9850.reset();
  AD9850.set_frequency(125000);
  
  Serial.begin(9600);
}


void loop()
{
  while(Serial.available() > 0)
  {
     long frequency = Serial.parseInt();
     Serial.print("Change frequency to ");
     Serial.print(frequency);
     Serial.println("Hz");
     AD9850.set_frequency(frequency);
  }
}



