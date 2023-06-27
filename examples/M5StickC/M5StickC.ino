#include "bp35a1.h"

#include <M5StickCPlus.h>

#define RXD2 26
#define TXD2 0

BP35A1 bp35a1;
uint8_t connectingStep = 0;
const uint8_t MAX_CONNECTING_STEP = 11;

bool connectWiSun(const char *id, const char *password)
{
  connectingStep = 0;
  nextConnectStep("start connection");
  Serial.print("start connect Wi-SUN\n");
  bp35a1.clearBuffer();
  // 以前のPANAセッションを解除
  nextConnectStep("delete session");
  bp35a1.deleteSession();

  // B ルートの PASSWORD を送信
  nextConnectStep("set password");
  if (!bp35a1.setPassword(password))
  {
    Serial.println("BP35A1::set password failed");
    return false;
  }

  // B ルートの ID を送信
  nextConnectStep("set ID");
  if (!bp35a1.setId(id))
  {
    Serial.println("BP35A1::set id failed");
    return false;
  }

  // Wi-SUN チャンネルスキャン
  nextConnectStep("scan channel");
  if (!bp35a1.scanChannel())
  {
    Serial.println("BP35A1::scan channel failed");
    return false;
  }

  bp35a1.clearBuffer();
  // MAC アドレスを IPv6 アドレスに変換
  nextConnectStep("get ipv6 address");
  if (!bp35a1.getIpv6Address())
  {
    Serial.println("BP35A1::get IP v6 failed");
    return false;
  }

  // チャンネル設定
  nextConnectStep("set channel");
  if (!bp35a1.setChannel())
  {
    Serial.println("BP35A1::set channel failed");
    return false;
  }

  // PAN ID 設定
  nextConnectStep("set PAN ID");
  if (!bp35a1.setPanId())
  {
    Serial.println("BP35A1::set PAN ID failed");
    return false;
  }

  // PANA 接続要求
  nextConnectStep("request connection");
  if (!bp35a1.requestAndWaitConnection())
  {
    Serial.println("BP35A1::request and wait connection failed");
    return false;
  }
  nextConnectStep("connected");

  return true;
}

void setup()
{
  M5.begin();
  M5.Lcd.setRotation(3);
  M5.Lcd.setTextSize(2);

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

  M5.Lcd.fillScreen(BLACK);
  pinMode(10, OUTPUT);
}

void loop()
{
  bp35a1.requestCurrentTotalPower();
  Serial.printf("Total Power: %f\n", bp35a1.getCurrentTotalPower());

  updateInstantaneousValue();
  digitalWrite(10, HIGH);
  delay(5000);

  updateInstantaneousValue();
  digitalWrite(10, LOW);
  delay(5000);
}

void nextConnectStep(const String message)
{
  M5.Lcd.fillScreen(BLACK);

  drawProgressiveBar(0, 10, ++connectingStep, 7);

  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(10, 15, 4);
  M5.Lcd.print(message);
}

void drawProgressiveBar(uint16_t y, uint16_t height, int value, int maxValue)
{
  uint16_t width = M5.Lcd.width();
  int barWidth = width * value / maxValue;
  M5.Lcd.fillRect(0, y, barWidth, height, GREEN);
  M5.Lcd.fillRect(barWidth, y, width, height, BLACK);
}

void updateInstantaneousValue()
{
  bp35a1.requestInstantaneousPower();
  int watt = bp35a1.getInstantaneousPower();

  // 瞬間電力量の表示
  M5.Lcd.fillRect(0, 0, M5.Lcd.width(), 60, BLACK);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.setCursor(M5.Lcd.width() / 2 - 80, 10, 7);
  M5.Lcd.printf("|%04d|", watt);

  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setCursor(M5.Lcd.width() / 2 + 80, 10 + 24, 4);
  M5.Lcd.print("W");
}
