#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include <RPi_Pico_TimerInterrupt.h>

// USB MIDI object
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);
int last_time = HIGH;

void setup() {
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
}

void loop() {
  int this_time = digitalRead(16);
  if ((last_time == HIGH) && (this_time == LOW)) {
    MIDI.sendNoteOn(60, 100, 1);
  }
  if ((last_time == LOW) && (this_time == HIGH)) {
    MIDI.sendNoteOff(60, 0,  1);
  }
  if (this_time == LOW) {
    digitalWrite(17, HIGH);
  } else {
    digitalWrite(17, LOW);
  }

  last_time = this_time;
}
