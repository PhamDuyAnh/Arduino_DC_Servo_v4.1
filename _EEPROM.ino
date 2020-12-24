#include <EEPROM.h>

byte Param[3];

void ReadEEPROM()
{
  //                kP, kI, kI,
  // Param Address   0,  1,  2,
  for (byte j = 0; j < 3; j++)
  {
    Param[j] = EEPROM.read(j);
  }
}

void SaveEEPROM()
{
  for (byte j = 0; j < 3; j++)
  {
    EEPROM.write(j, Param[j]);
  }
}
