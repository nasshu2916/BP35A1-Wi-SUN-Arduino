# ROHM BP35A1 Wi-SUN Arduino Library

ROHM 社製 BP35A1 Wi-SUN モジュールを使って、低圧スマート電力量メーターのデータを取得するための Arduino ライブラリです。

## ハードウェア

- M5StickC Plus
- [M5StickC/Plus用Wi-SUN HATキット](https://www.switch-science.com/products/7612)
- [BP35A1](https://www.rohm.co.jp/products/wireless-communication/specified-low-power-radio-modules/bp35a1-product)

## サンプルコードのセットアップ

Arduino スケッチに Bルートの ID とパスワードを設定してください。

```c++
const char *BID = "YOUR_B_ROUTE_ID";
const char *BPWD = "YOUR_B_ROUTE_PWD";
```
