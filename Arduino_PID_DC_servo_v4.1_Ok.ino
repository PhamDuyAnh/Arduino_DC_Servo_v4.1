//Define Pin/Port
#define motorPWMpin     5                 //PORTD 5
#define motorDIRpin     4                 //PORTD 4
#define encoderPinA     3                 //PORTD 3 - INT1
#define encoderPinB     A4                //PORTC 4
#define inputPulse      2                 //PORTD 2 - INT0
#define inputDir        A5                //PORTC 5
#define inputCur        A0
#define input6          6                 //not used
#define input7          7                 //not used
//Define Servo motor & encoder
#define maxRPM          2000              //rpm
#define encoderRes      500               //ppr
#define encoderMode     2                 //x2 encoder count
//PWM limit
#define outMax         +255
#define outMin         -255
#define REVERSE        0 
#define wait           5                 //DIR delay time

long enSpeedMax,enSpeedMin;
long Setpoint;
long eSpeed, Input, Output, pError, sError, lastError;
long lastInput, lasteSpeed;
long kP, kPp, kI, kD, I;
boolean Do, motorDir;
boolean Monitor = false;
byte ii;

void setup()
{
  Setup_Timer2();
  Setup_Timer0();
  //Serial.begin(921600);
  Serial.begin(115200);
  //Serial.setTimeout(100);
  //Setup OUTPUT
  pinMode(motorDIRpin, OUTPUT);
  pinMode(motorPWMpin, OUTPUT);
  pinMode(13, OUTPUT);
  //Setup INPUT
  pinMode(encoderPinA, INPUT);
  pinMode(encoderPinB, INPUT);
  pinMode(inputPulse,  INPUT);
  pinMode(inputDir,    INPUT);
  pinMode(input6,      INPUT);
  pinMode(input7,      INPUT);
  attachInterrupt(0, doStepDir, RISING);  //trigger when the pin goes from low to high (with 74HC14 schmitt trigger inverter) RISING/FALLING
  attachInterrupt(1, doEncoder, CHANGE);   //trigger when the pin goes from high to low
  //ReadEEPROM()
  kPp = 1;
  kP = 25;
  kI = 0;
  kD = 50;
  enSpeedMax = long(maxRPM) * encoderRes * encoderMode;
  enSpeedMax = enSpeedMax / (long(60) * 2000); //ppc
  enSpeedMin = -enSpeedMax;
  
  PIDtunings();
  OCR0B = 0;
}
void loop()
{
  if (Do)
  {
    getParam();
    PIDcompute();
    //Output = 250;
    motor(Output);
    /*
    //PID monitor
    Serial.print(Setpoint);
    Serial.print(",");
    Serial.print(Input);
    Serial.print(",");
    Serial.print(pError * 50);
    Serial.println(",");
    */
    if (Monitor)
    {
      if (Input >  2000) Setpoint = -2000;
      if (Input < -2000) Setpoint = 2000;
    }
    Do = false;
  }
}

void getParam()
{
  byte param;
  float value;
  if (Serial.available())
  {
    while (Serial.available())
    {
    //Serial.setTimeout(100);
      param = Serial.read();
      if (param != 240)
      {
        if (param <= 90)
        {
          param += 32;
        }
        switch(param)
        {
          case 'p':
          {
            value = Serial.parseFloat();
            kP = value;
            Monitor = false;
            break;
          }
          case 'i':
          {
            value = Serial.parseFloat();
            kI = value;
            Monitor = false;
            break;
          }
          case 'd':
          {
            value = Serial.parseFloat();
            kD = value;
            Monitor = false;
            break;
          }
          case 'm':
          {
            Monitor = true;
            Setpoint = 5100;
            break;
          }
          case 's':
          {
            value = Serial.parseFloat();
            Setpoint = value;
            Monitor = false;
            break;
          }
          case 'x':
          {
            value = Serial.parseFloat();
            Setpoint = value * 500;
            Monitor = false;
            break;
          }
          case 'z':
          {
            value = Serial.parseFloat();
            Input = value;
            Monitor = false;
            break;
          }       
          case 95: //param = "?";
          {
            //info();
            break;
          }
          default:
          {
            //Serial.println("???");        
            //Setpoint = value * 1000;
            break;
          }
        }
        if (param != 'x')
        {
          PIDtunings();
          info();
        }
      }
    }
  }
}
void info()
{
  //TIMSK2 = 0;
  //cli(); //Disable interrupt
  //test = Setpoint - Input;
  Serial.print("kP: ");
  Serial.print(kP);
  Serial.print("   kI: ");
  Serial.print(kI);
  Serial.print("   kD: ");
  Serial.print(kD);
  Serial.println("");
  Serial.print("Input: ");
  Serial.print(Input);
  Serial.print("   Setpoint:");
  Serial.print(Setpoint);
  Serial.print("   Output:");
  Serial.print(Output);
  Serial.println("");
  //Serial.println(limit);
  //sei(); //Enable interrupt
  //TIMSK2 |= (1 << OCIE2A);
}

// time require <1us
void motor(int motorPWM)
{
  if (motorPWM < 0)
  {
    if (motorDir)
    {
      for (byte w = 0; w < wait; w++)
      {
        disablePWM();
      }
      bitWrite(PORTD, 4, 0);
      for (byte w = 0; w < wait; w++)
      {
        disablePWM();
      }
      motorDir = false;
      enablePWM();
    }
    else bitWrite(PORTD, 4, 0);
    OCR0B = -motorPWM;
  }
  else if (motorPWM > 0)
  {
    if (!motorDir)
    {
      for (byte w = 0; w < wait; w++)
      {
        disablePWM();
      }
      bitWrite(PORTD, 4, 1);
      for (byte w = 0; w < wait; w++)
      {
        disablePWM();
      }
      motorDir = true;
      enablePWM();
    }
    else bitWrite(PORTD, 4, 1);
    OCR0B = motorPWM;
  }
  else OCR0B = 0;
}  
