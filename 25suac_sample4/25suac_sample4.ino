#include <Adafruit_TinyUSB.h>
#include <MIDI.h>

// USB MIDI object
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);
int last_time = HIGH;

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

  pinMode(26, INPUT);
  pinMode(17, OUTPUT);

  // MIDI setting
  MIDI.turnThruOff();
}

void loop() {
  static bool during_note_on = false;
  int this_time = analogRead(26);

  // MIDI送信 & シリアルモニタ送信
  if ((during_note_on == false) && (this_time > 400)) {
    MIDI.sendNoteOn(60, 100, 1);
    Serial.print("on\n");
    during_note_on = true;
  }
  if ((during_note_on == true) && (this_time <= 250)) {
    MIDI.sendNoteOff(60, 0,  1);
    Serial.print("off\n");
    during_note_on = false;
  }

  // LED点灯
  if (this_time > 400) {
    digitalWrite(17, HIGH);
  } else {
    digitalWrite(17, LOW);
  }

  delay(10);
}
