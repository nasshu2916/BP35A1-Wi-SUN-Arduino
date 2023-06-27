#include "bp35a1_UDP_Response.h"

Coefficient::Coefficient(std::string data)
{
  _coefficient = strtol(data.c_str(), NULL, 16);
}

TotalPower::TotalPower(std::string data)
{
  _totalPower = strtol(data.c_str(), NULL, 16);
}

PowerUnit::PowerUnit(std::string data)
{
  _powerUnit = convertPowerUnit(data.substr(0, 2));
}

float PowerUnit::convertPowerUnit(std::string stringUnit)
{
  if (stringUnit == "00")
  {
    return 1.0f;
  }
  else if (stringUnit == "01")
  {
    return 0.1f;
  }
  else if (stringUnit == "02")
  {
    return 0.01f;
  }
  else if (stringUnit == "03")
  {
    return 0.001f;
  }
  else if (stringUnit == "04")
  {
    return 0.0001f;
  }
  else if (stringUnit == "0A")
  {
    return 10.0f;
  }
  else if (stringUnit == "0B")
  {
    return 100.0f;
  }
  else if (stringUnit == "0C")
  {
    return 1000.0f;
  }
  else if (stringUnit == "0D")
  {
    return 10000.0f;
  }
  else
  {
    return 0.0f;
  }
}

TotalPowerHistories::TotalPowerHistories(std::string data)
{
  std::string days = data.substr(0, 4);
  _day = strtol(days.c_str(), NULL, 16);
  std::string stringPowers = data.substr(4, 384);
  std::string power;
  for (int i = 0; i < 48; i++)
  {
    power = stringPowers.substr(i * 8, 8);
    _powers[i] = strtol(power.c_str(), NULL, 16);
  }
}

CollectionDay::CollectionDay(std::string data)
{
  _day = strtol(data.c_str(), NULL, 16);
}

InstantaneousPower::InstantaneousPower(std::string data)
{
  _power = strtol(data.c_str(), NULL, 16);
}

InstantaneousAmperage::InstantaneousAmperage(std::string data)
{
  _amperageR = strtol(data.substr(0, 4).c_str(), NULL, 16);
  _amperageT = strtol(data.substr(4, 4).c_str(), NULL, 16);
}

CurrentTotalPower::CurrentTotalPower(std::string data)
{
  std::string date = data.substr(0, 14);
  int year = strtol(date.substr(0, 4).c_str(), NULL, 16);
  int month = strtol(date.substr(4, 2).c_str(), NULL, 16);
  int day = strtol(date.substr(6, 2).c_str(), NULL, 16);
  int hour = strtol(date.substr(8, 2).c_str(), NULL, 16);
  int minute = strtol(date.substr(10, 2).c_str(), NULL, 16);
  int second = strtol(date.substr(12, 2).c_str(), NULL, 16);

  _totalPower = strtol(data.substr(14, 8).c_str(), NULL, 16);
}
