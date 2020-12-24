void Setup_Timer0()
{
  TCCR0A = 0;  // set entire TCCR2A register to 0
  TCCR0B = 0;  // same for TCCR2B
  TCNT0  = 0;  //initialize counter value to 0
  // set compare match register for 1khz increments
  //OCR2A = 249;  // = (16*10^6) / (1000*64) - 1 (must be <256)
  // turn on Fast PWM mode
  TCCR0A |= (1 << WGM01) | (1 << WGM00);
  //Clear OC0A/B on Compare Match, set OC0A/B at BOTTOM
  TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS00);  
  // enable timer compare interrupt
  //TIMSK2 |= (1 << OCIE2A);
  OCR0B = 0;
}
void enablePWM()
{
  TCCR0A |= (1 << WGM01) | (1 << WGM00);
  TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
}
void disablePWM()
{
  TCCR0A = 0;
}

void Setup_Timer2()
{
  TCCR2A = 0;  // set entire TCCR2A register to 0
  TCCR2B = 0;  // same for TCCR2B
  TCNT2  = 0;  //initialize counter value to 0
  // set compare match register for 2khz increments ~ 500us
  OCR2A = 124;  // = (16*10^6) / (2000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR2B |= (1 << CS22);  
  // Set CS01 and CS00 bits for 256 prescaler
  //TCCR2B |= (1 << CS22) | (1 << CS21);  
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
}

// timer 2 interrupt
ISR(TIMER2_COMPA_vect)
{
  Do = true;
}


