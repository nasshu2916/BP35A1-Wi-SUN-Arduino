#include "bp35a1.h"

#define RXD2 26
#define TXD2 0

const char *BID = "YOUR_B_ROUTE_ID";
const char *BPWD = "YOUR_B_ROUTE_PWD";

BP35A1 bp35a1;

bool connectWiSun(const char *id, const char *password)
{
  Serial.print("start connect Wi-SUN\n");
  bp35a1.clearBuffer();
  // 以前のPANAセッションを解除
  bp35a1.deleteSession();

  // B ルートの PASSWORD を送信
  if (!bp35a1.setPassword(password))
  {
    Serial.println("BP35A1::set password failed");
    return false;
  }

  // B ルートの ID を送信
  if (!bp35a1.setId(id))
  {
    Serial.println("BP35A1::set id failed");
    return false;
  }

  // Wi-SUN チャンネルスキャン
  if (!bp35a1.scanChannel())
  {
    Serial.println("BP35A1::scan channel failed");
    return false;
  }

  bp35a1.clearBuffer();
  // MAC アドレスを IPv6 アドレスに変換
  if (!bp35a1.getIpv6Address())
  {
    Serial.println("BP35A1::get IP v6 failed");
    return false;
  }

  // チャンネル設定
  if (!bp35a1.setChannel())
  {
    Serial.println("BP35A1::set channel failed");
    return false;
  }

  // PAN ID 設定
  if (!bp35a1.setPanId())
  {
    Serial.println("BP35A1::set PAN ID failed");
    return false;
  }

  // PANA 接続要求
  if (!bp35a1.requestAndWaitConnection())
  {
    Serial.println("BP35A1::request and wait connection failed");
    return false;
  }

  return true;
}

void setup()
{
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  bp35a1 = BP35A1(&Serial2);

  int count = 0;
  while (true)
  {
    if (connectWiSun(BID, BPWD))
      break;

    Serial.printf("connect failed. count: %d. retry connect.\n", count++);
    delay(1000);
  }

  Serial.println("Wi-SUN connected!!!");
  bp35a1.requestCoefficient();
  bp35a1.requestPowerUnit();

  pinMode(10, OUTPUT);
}

void loop()
{
  bp35a1.requestCurrentTotalPower();
  Serial.printf("Total Power: %f\n", bp35a1.getCurrentTotalPower());

  bp35a1.requestInstantaneousPower();
  Serial.printf("%d[W] now.\n", bp35a1.getInstantaneousPower());

  digitalWrite(10, HIGH);
  delay(5000);

  bp35a1.requestInstantaneousPower();
  Serial.printf("%d[W] now.\n", bp35a1.getInstantaneousPower());

  digitalWrite(10, LOW);
  delay(5000);
}
