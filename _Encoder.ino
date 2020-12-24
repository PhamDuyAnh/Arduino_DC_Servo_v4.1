// time require <1.8us
boolean A, B, lastA, lastB;
//attachInterrupt(1, doEncoder, CHANGE);
//2x Quadrature Encoder
void doEncoder()
{
  //OCR0B = 0;
  A = bitRead(PIND, 3);
  B = bitRead(PINC, 4);
  //check last change for error
  if ((A != lastA) & (B != lastB))
  {
    if (A ^ B) Input ++;
    else       Input --;
  }
  lastA = A;
  lastB = B;
}
void doStepDir()
{
  if(bitRead(PIND, 2))
  {
    if (bitRead(PINC, 5)) Setpoint --;
    else                  Setpoint ++;
  }
}
