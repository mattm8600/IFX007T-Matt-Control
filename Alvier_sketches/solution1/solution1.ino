//#include <IFX007T-Motor-Control.h>
#include <IFX007T-Matt-Control.h>

  IFX007TMotorControl MyMotor = IFX007TMotorControl();

void setup() {
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  noInterrupts();

  MyMotor.begin();
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  digitalWrite(2,HIGH);

}

void loop() {
  MyMotor.inverter2(0);
// digitalWrite(10,HIGH);
// digitalWrite(11,LOW);
// delayMicroseconds(10);
// digitalWrite(11,HIGH);
// digitalWrite(10,LOW);
// delayMicroseconds(10);

}
