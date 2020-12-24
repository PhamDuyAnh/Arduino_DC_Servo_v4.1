// time require <74.2us
void PIDcompute()
{
  //Position error calculator
  pError = (Input - Setpoint) * kPp;           // E part = set_point - encoder_position
  if      (pError > enSpeedMax) pError = enSpeedMax; 
  else if (pError < enSpeedMin) pError = enSpeedMin;
 
  //Speed error calculator
  eSpeed = lastInput - Input;
  lastInput = Input;
  
  sError = pError - eSpeed;

  I += sError / 10;
  if (I > outMax)      I = outMax; 
  else if (I < outMin) I = outMin;
  
  long D = lasteSpeed - eSpeed;
  lasteSpeed = eSpeed;
  
  Output = kP * sError + kI * I + kD * D;
  if (Output > outMax)      Output = outMax; 
  else if (Output < outMin) Output = outMin;
}
   
void PIDtunings()
{
  if (REVERSE)
  {
    kP = (0 - kP);
    kI = (0 - kI);
    kD = (0 - kD);
  }
}
