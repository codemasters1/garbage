#include <IRremote.h>

const int RECV_PIN = 11;

IRrecv irrecv(RECV_PIN);

decode_results results;

struct {uint32_t code; uint8_t key;} codes[] = {
  {0xFF48B7, 0},
  {0xFF906F, 1},
  {0xFFB847, 2},
  {0xFFF807, 3},
  {0xFFB04F, 4},
  {0xFF9867, 5},
  {0xFFD827, 6},
  {0xFF8877, 7},
  {0xFFA857, 8},
  {0xFFE817, 9}
};

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  irrecv.blink13(true);
}

uint8_t decode_key(uint32_t code) {
  uint8_t count_codes = sizeof(codes) / sizeof(codes[0]);
  for (int i = 0; i < count_codes; ++i)
  {
    if (codes[i].code == code)
    {
      return codes[i].key;
    }
  }
  
  return 0xFF;
}

void loop() {
  if (irrecv.decode(&results)) {
    
    uint8_t key = decode_key(results.value);
    
    if (key == 0xFF) {
      //Serial.println(results.value, HEX);
    }
    else {
      Serial.print("klawisz: ");
      Serial.println(key);
    }
    
    irrecv.resume(); // Receive the next value
  }
}
