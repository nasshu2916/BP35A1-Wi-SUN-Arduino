#ifndef BP35A1_H_
#define BP35A1_H_

#include "Arduino.h"
#include "HardwareSerial.h"

#include "bp35a1_UDP_Response.h"

#include <iomanip>
#include <sstream>
#include <vector>

enum class ResponseType : int
{
  SET = 0x71,
  GET = 0x72
};

enum class CmdType : byte
{
  COEFFICIENT = 0xD3,                   // 積算電力量係数を取得する
  TOTAL_POWER = 0xE0,                   // 積算電力量計測値(kWh)を取得する
  POWER_UNIT = 0xE1,                    // 積算電力量単位を取得する
  TOTAL_POWER_HISTORIES = 0xE2,         // 積算電力量計測値履歴(kWh)を取得する
  TOTAL_HISTORY_COLLECTION_DATE = 0xE5, // 積算履歴収集日を取得/変更する
  INSTANTANEOUS_POWER = 0xE7,           // 瞬時電力計測値(W)を取得する
  INSTANTANEOUS_AMPERAGE = 0xE8,        // 瞬時電流計測値(0.1A)を取得する
  CURRENT_TOTAL_POWER = 0xEA            // 30分毎の積算電力量計測値(kWh)を取得する
};

struct ScanResult
{
  String channel;
  String panId;
  String addr;
};

class BP35A1
{
public:
  BP35A1();
  BP35A1(HardwareSerial *serial);

  void setEchoCallback(bool isEnable); // コマンドエコーバックを変更する
  void deleteSession();                // 以前のPANAセッションを解除する
  bool getVersion();                   // バージョン情報を取得する

  bool setPassword(const char *pass); // B ルートの PASSWORD を設定する
  bool setId(const char *id);         // B ルートの ID を設定する

  bool scanChannel(); // チャンネルスキャンを実行する

  bool getIpv6Address();           // MAC アドレスを IPv6 アドレスに変換
  bool setChannel();               // チャンネルを設定する
  bool setPanId();                 // PAN ID を設定する
  bool requestAndWaitConnection(); // PANA 接続要求を送信し、接続完了を待つ

  bool getProperties(std::vector<CmdType> commands);
  bool setProperties(CmdType command, std::vector<byte> values);
  void clearBuffer();

  bool requestCoefficient();                    // 積算電力量係数を取得する(0xD3)
  bool requestTotalPower();                     // 積算電力量計測値を取得する(0xE0)
  bool requestPowerUnit();                      // 積算電力量単位を取得する(0xE1)
  bool requestCurrentTotalPowerHistories();     // 算電力量計測値履歴を取得する(0xE2)
  bool requestTotalHistoryCollectionDate();     // 積算履歴収集日を取得する(0xE5)
  bool setTotalHistoryCollectionDate(byte day); // 積算履歴収集日を設定する(0xE5)
  bool requestInstantaneousPower();             // 瞬時電力計測値を取得する(0xE7)
  bool requestInstantaneousAmperage();          // 瞬時電流計測値を取得する(0xE8)
  bool requestCurrentTotalPower();              // 30分毎の積算電力量計測値を取得する(0xEA)

  ScanResult getScanResult() { return _scanResult; }
  void setScanResult(ScanResult scanResult) { _scanResult = scanResult; }

  int getCoefficient() { return _coefficient.getCoefficient(); }
  float getTotalPower() { return convertTotalPower(_totalPower.getTotalPower()); }
  float getPowerUnit() { return _powerUnit.getPowerUnit(); }
  TotalPowerHistories getTotalPowerHistories() { return _totalPowerHistories; }
  byte getCollectionDay() { return _collectionDay.getDay(); }
  int getInstantaneousPower() { return _instantaneousPower.getPower(); }
  InstantaneousAmperage getInstantaneousAmperage() { return _instantaneousAmperage; }
  float getCurrentTotalPower() { return convertTotalPower(_currentTotalPower.getTotalPower()); }

private:
  bool waitSuccessResponse();
  bool waitScanResponse(int duration);
  bool waitIpv6AddrResponse();
  bool requestConnection(); // PANN 接続要求を送信する
  bool waitConnection();    // PANA 接続完了を待つ

  bool sendUdp(std::vector<byte> data);
  bool waitUpdResponse(const int timeout = READ_TIMEOUT);
  bool handleUdpResponse(String response);
  bool handleUdpGetResponse(std::string *data);
  bool handleUdpSetResponse(std::string *data);

  template <typename T>
  T readUdpResponse(std::string *data, int offset)
  {
    int length = T::dataLength();
    int removeSize = offset + length;
    std::string str = data->substr(offset, length);

    *data = data->substr(removeSize);
    return T(str);
  }

  template <typename T>
  bool validateDataLength(const std::string *data, int offset)
  {
    int dataSize = data->size();
    return dataSize >= dataLength<T>(offset);
  }

  template <typename T>
  int dataLength(int offset)
  {
    return T::dataLength() + offset;
  }

  String readSerialLine();
  float convertTotalPower(long power); // レスポンスで返ってきた積算電力量を kWh に変換する。未来の時刻の積算電力量は 0 になる

  static String removePrefix(String str, String prefix);
  static bool validateIpv6Format(String addr);

  void debugLog(const char *format, ...) __attribute__((format(printf, 2, 3)));

public:
  static const std::string SMART_METER_ID; // 低圧スマート電力量メータの識別子

private:
  HardwareSerial *_serial;
  ScanResult _scanResult;
  String _ipv6;

  Coefficient _coefficient;                     // 積算電力量の係数
  TotalPower _totalPower;                       // 積算電力量計測値(kWh)
  PowerUnit _powerUnit;                         // 積算電力量の単位
  TotalPowerHistories _totalPowerHistories;     // 積算電力量計測値履歴
  CollectionDay _collectionDay;                 // 積算電力量を取得する日(日前)
  InstantaneousPower _instantaneousPower;       // 瞬時電力計測値
  InstantaneousAmperage _instantaneousAmperage; // 瞬時電流計測値
  CurrentTotalPower _currentTotalPower;         // 最新30分毎の積算電力量計測値(kWh)

  static const int READ_TIMEOUT = 5000;
  static const int READ_INTERVAL = 100;
  static const int CONNECTION_TIMEOUT = 30000;
};

#endif
