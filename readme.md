# 2025 12/18 インタラクティブプロダクト演習 サンプルコード

## 25suac_sample1

- pico2ボード上のLEDが点滅するだけの単純なプログラムです
- Arduino IDE上からRaspberry Pi Pico2に正常に書き込めるかを確認します
- 初回のみ、BOOTSELボタンを押しながら USB Cable を指します
- 初回のみ、書き込む時にポート設定を UF2_Board にします
- ２回目以降は、ポート設定を USB シリアルっぽいものに変更します

## 25suac_sample2

- ブレッドボード上に、ボタンとLEDの回路を作成し、pico2と接続します
- USB Stack を Arduino TinyUSB に設定し、プログラムを書き込みます
- PC でソフトシンセを立ち上げて、ボードのスイッチを押すと音が鳴るはずです
