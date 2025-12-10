#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include "peripheral.h"

const size_t MAX_SENS = 6;

// USB MIDI object
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);
int last_time = HIGH;
uint8_t last_at42qt_status = 0;
bool touch_sw[MAX_SENS] = {false};

void setup() {
  // USB MIDI とシリアルを同時に有効にするための裏ワザ
  Serial.begin(115200);

  // Device Discriptor
  TinyUSBDevice.setManufacturerDescriptor("YourName");
  TinyUSBDevice.setProductDescriptor("YourDevice");
  // MIDI Port Name
  usb_midi.setStringDescriptor("PortName");

  // initialize USB MIDI
  usb_midi.begin();
  MIDI.begin(MIDI_CHANNEL_OMNI);

  pinMode(16, INPUT);
  pinMode(17, OUTPUT);

  // MIDI setting
  MIDI.turnThruOff();

  wireBegin();
  AT42QT_init();
}

void loop() {
  int this_time = digitalRead(16);

  // MIDI送信 & シリアルモニタ送信
  if ((last_time == HIGH) && (this_time == LOW)) {
    MIDI.sendNoteOn(60, 100, 1);
    Serial.print("on\n");
  }
  if ((last_time == LOW) && (this_time == HIGH)) {
    MIDI.sendNoteOff(60, 0,  1);
    Serial.print("off\n");
  }

  uint8_t at42qt_status = AT42QT_read_bit();
  for (int i = 0; i < MAX_SENS; i++) {
    bool now = (at42qt_status & (1 << i)) != 0;
    bool last = (last_at42qt_status & (1 << i)) != 0;
    if (now && !last) {
      MIDI.sendNoteOn(61 + i, 100, 1);
    } else if (!now && last) {
      MIDI.sendNoteOff(61 + i, 0, 1);
    }
  }
  last_at42qt_status = at42qt_status;
  bool tsw = at42qt_status != 0;

  // LED点灯
  if ((this_time == LOW) || tsw){
    digitalWrite(17, HIGH);
  } else {
    digitalWrite(17, LOW);
  }

  last_time = this_time;
}
