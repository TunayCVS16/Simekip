# Simekip — GitHub Actions ile otomatik HEX

Bu repo, ATmega16U2 için **Simekip.hex** üretmek üzere hazırlandı.
Kaynaklar LUFA tabanlı HID joystick (64 buton + 2 hat + 8 eksen) örneğidir ve
USB cihaz adı **"Simekip"** olarak ayarlanmıştır.

## Kullanım
1. Bu klasörü GitHub'ta **yeni bir repo** olarak yükleyin.
2. Repo'da **Actions** sekmesini açın, workflow'u **Run workflow** ile çalıştırın.
3. Tamamlandığında **Artifacts** bölümünden `Simekip.hex` dosyasını indirin.
4. Kartı DFU moduna alarak 16U2'ye `Simekip.hex`'i yükleyin (batchisp/FLIP ile).

> Not: Workflow, LUFA-120730 arşivinden sadece `LUFA/` klasörünü indirip `./lufa/` içine kopyalar.
> Makefile içindeki `LUFA_PATH = ./lufa` buna göre ayarlandı.

## Uyarı (VID/PID)
Şu an test amaçlı Atmel/Microchip **VID/PID (0x03EB:0x2043)** kullanılıyor.
Ticari dağıtımda kendi PID alınması gerekir.

## Derleme lokal yapmak isterseniz
- `gcc-avr`, `avr-libc`, `make` kurulu bir Linux/WSL ortamında:
  ```bash
  mkdir -p lufa && curl -L https://github.com/abcminiuser/lufa/archive/refs/tags/120730.zip -o /tmp/lufa.zip
  unzip -q /tmp/lufa.zip -d /tmp && cp -r /tmp/lufa-120730/LUFA ./lufa/
  make clean && make
  ```
  Çıktı: `Simekip.hex`
