/*
  sPWM on the atMega328 for the arduino Uno, might be compatable with other atmel chips / arduino boards.
  Compare outputs A and B output to PORTB pins 1 and 2 which are pins 9 and 10 respectively on the Uno.
  Also useful to know the led on the Uno is pin 5 on PORTB.
*/

// Look up tables with 200 entries each, normalised to have max value of 1600 which is the period of the PWM loaded into register ICR1.
int lookUp[201] = {0, 12, 25, 37, 50, 62, 74, 87, 99, 111, 124, 136, 148, 160, 172, 184, 196, 208, 220, 232, 244, 256, 268, 279, 291, 302, 314, 325, 336, 348, 359, 370, 381, 391, 402, 413, 423, 434, 444, 454, 464, 474, 484, 494, 504, 513, 522, 532, 541, 550, 559, 567, 576, 584, 593, 601, 609, 617, 624, 632, 639, 646, 653, 660, 667, 674, 680, 686, 692, 698, 704, 709, 715, 720, 725, 730, 735, 739, 743, 747, 751, 755, 759, 762, 765, 768, 771, 774, 776, 778, 780, 782, 784, 785, 786, 788, 788, 789, 790, 790, 790, 790, 790, 789, 788, 788, 786, 785, 784, 782, 780, 778, 776, 774, 771, 768, 765, 762, 759, 755, 751, 747, 743, 739, 735, 730, 725, 720, 715, 709, 704, 698, 692, 686, 680, 674, 667, 660, 653, 646, 639, 632, 624, 617, 609, 601, 593, 584, 576, 567, 559, 550, 541, 532, 522, 513, 504, 494, 484, 474, 464, 454, 444, 434, 423, 413, 402, 391, 381, 370, 359, 348, 336, 325, 314, 302, 291, 279, 268, 256, 244, 232, 220, 208, 196, 184, 172, 160, 148, 136, 124, 111, 99, 87, 74, 62, 50, 37, 25, 12, 0};

void setup()
{
  // Register initilisation, see datasheet for more detail.
  TCCR1A = 0b10000010;  // CTC (Mode 12); OCR1A: 10; OCR1B: 00 (disconnected)
  TCCR1B = 0b00011001;
  TIMSK1 = 0b00000001;
  ICR1 = 800; // Period for 16MHz crystal, for a switching frequency of 20KHz for 400 subdevisions per 50Hz sin wave cycle.
  sei(); // Enable global interrupts.

  DDRB = DDRB | B00100110;  // Set digital pin13 (PB5) to digital pin 8 (PB0) as output. D13 = PB5 (PB7 & PB6 are unusable). PORTB = [XTAL2 XTAL1 D13 D12 D11 D10 D9 D8]
}

void loop()
{
  ; /*Do nothing . . . . forever!*/
}

ISR(TIMER1_OVF_vect) {
  static int num;  //static means it will NOT be reinitialized
  // change duty-cycle every period.
  OCR1A = lookUp[num];
  OCR1B = lookUp[num];
  if (++num >= 200) { // Pre-increment num then check it's below 200.
    num = 0; // Reset num.
    TCCR1A ^= 0b10100000;  // Toggle OCR1A & OCR1B between clear on compare match and disconnected.
  }
}
