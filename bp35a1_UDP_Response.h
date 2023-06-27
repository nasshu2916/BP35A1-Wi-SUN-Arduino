#ifndef BP35A1_UDP_RESPONSE_H_
#define BP35A1_UDP_RESPONSE_H_

#include "Arduino.h"

#include <iomanip>
#include <sstream>

class BP35A1UdpResponse
{
public:
  BP35A1UdpResponse() {}
};

class Coefficient : public BP35A1UdpResponse
{
public:
  Coefficient() {}
  Coefficient(std::string data);

  static int dataLength() { return 8; }

  int getCoefficient() { return _coefficient; }

private:
  int _coefficient;
};

class TotalPower : public BP35A1UdpResponse
{
public:
  TotalPower() {}
  TotalPower(std::string data);

  static int dataLength() { return 8; }

  int getTotalPower() { return _totalPower; }

private:
  int _totalPower;
};

class PowerUnit : public BP35A1UdpResponse
{
public:
  PowerUnit() {}
  PowerUnit(std::string data);

  static int dataLength() { return 2; }

  float getPowerUnit() { return _powerUnit; }

private:
  static float convertPowerUnit(std::string stringUnit);

  float _powerUnit;
};

class TotalPowerHistories : public BP35A1UdpResponse
{
public:
  TotalPowerHistories() {}
  TotalPowerHistories(std::string data);

  static int dataLength() { return 388; }

  int getDay() { return _day; }
  long *getPowers() { return _powers; }

private:
  int _day;
  long _powers[48];
};

class CollectionDay : public BP35A1UdpResponse
{
public:
  CollectionDay() {}
  CollectionDay(std::string data);

  static int dataLength() { return 2; }

  byte getDay() { return _day; }

private:
  byte _day;
};

class InstantaneousPower : public BP35A1UdpResponse
{
public:
  InstantaneousPower() {}
  InstantaneousPower(std::string data);

  static int dataLength() { return 8; }

  int getPower() { return _power; }

private:
  int _power; // 瞬間電力量(W)
};

class InstantaneousAmperage : public BP35A1UdpResponse
{
public:
  InstantaneousAmperage() {}
  InstantaneousAmperage(std::string data);

  static int dataLength() { return 8; }

  int getAmperageR() { return _amperageR; }
  int getAmperageT() { return _amperageT; }
  int getAmperage() { return _amperageR + _amperageT; }

private:
  int _amperageR; // 瞬間電流量(R相)(0.1A)
  int _amperageT; // 瞬間電流量(T相)(0.1A)
};

class CurrentTotalPower : public BP35A1UdpResponse
{
public:
  CurrentTotalPower() {}
  CurrentTotalPower(std::string data);

  static int dataLength() { return 22; }

  long getTotalPower() { return _totalPower; }

private:
  long _totalPower; // 積算電力量
};

#endif
