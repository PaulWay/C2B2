#include <SoftwareSerial.h>

int led_1_pin = 0;
int led_2_pin = 1;
int led_1_val = 0;
int led_2_val = 255;

int sig_in_pin = 2;
int sig_out_pin = 3;

SoftwareSerial mySerial(sig_in_pin, sig_out_pin);

void setup() {
  pinMode(led_1_pin, OUTPUT);
  pinMode(led_2_pin, OUTPUT);

  mySerial.begin(9600);
  mySerial.write('1');
}

void loop() {
  char rcv, snd;

  if (mySerial.available()) {
    rcv = mySerial.read();
    if (rcv != -1) {
      snd = rcv == '1' ? '2' : '1';
      mySerial.write(snd);
    }
  }
  switch (rcv) {
  case '1': 
    led_1_val = 0; 
    led_2_val = 255;
    break;
  default:
    led_1_val = 255; 
    led_2_val = 0;
    break;
  }

  digitalWrite(led_1_pin, led_1_val);
  digitalWrite(led_2_pin, led_2_val);
}


