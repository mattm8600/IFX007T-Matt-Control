#include "avr/pgmspace.h"
#include "math.h"
#include "avr/io.h"
#include "avr/interrupt.h"

#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
volatile uint8_t index = 0;

//Toggle pins 9 and 10 high and low very quickly
// Counter cleared to 0 once it reaches OCR2A
// Use OCR2B to determine when to turn on and off pins

const uint8_t sine16a[] = {
   0,17,32,43,49,49,43,32,17,0,0,0,0,0,0,0
   // 0,0,0,0,0,0,0,0
};

const uint8_t sine16b[] = {
    0,0,0,0,0,0,0,0,17,32,43,49,49,43,32,17
    // 50,50,50,50,50,50,50,50
};

void setup() {
    Serial.begin(115200);
    Serial.println("Avr direct Control Test");
    pinMode(9,OUTPUT); //Enable positive output
    pinMode(10,OUTPUT); //Enable negative output
    ICR1 = 49;
    setup_timer1();
    
    cbi (TIMSK0,TOIE0); // disable Timer0 interrupts to avoid timing distortion
    sbi(TIMSK1, TOIE1); //enable Timer 1 overflow interrupt
}

void loop() {
    while(1) {
    }
}


// Clear OC2A on compare match (with OCR2B)
// Set OC2A at bottom

void setup_timer1() {
  Serial.println("Setup");
    // Timer2 Clock Prescaler to : 1
    sbi(TCCR1B, CS10);
    cbi(TCCR1B, CS11);
    cbi(TCCR1B, CS12);

    // Set Timer to Fast PWM Mode with ICR1 at top
    cbi(TCCR1A, WGM10);  // Mode 14
    sbi(TCCR1A, WGM11);
    sbi(TCCR1B, WGM12);
    sbi(TCCR1B, WGM13);

    // Set non-inverting compare output mode
    cbi(TCCR1A, COM1A0);
    sbi(TCCR1A, COM1A1);
    cbi(TCCR1A, COM1B0);
    sbi(TCCR1A, COM1B1);

    TCNT1=0; //reset the timer
}

//PD3 -> Controlled by OC2B | Pin 3
//PB3 -> Controlled by OC2A | Pin 11
//PB1 -> Controlled by OC1A | Pin 9
//PB2 -> Controlled by OC1B | Pin 10

//TODO: Add in negative portion of duty cycle
ISR(TIMER1_OVF_vect) {   
   // Change to the next index in the sine table
   if(index == 17){
     index = 0;
   }
   else{
     index= index + 1;
   }
   OCR1A = pgm_read_byte_near(sine16a + index);
   OCR1B = pgm_read_byte_near(sine16b + index);
}
