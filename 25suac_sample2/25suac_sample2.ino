#include <Adafruit_TinyUSB.h>
#include <MIDI.h>
#include <RPi_Pico_TimerInterrupt.h>

#define 

// USB MIDI object
Adafruit_USBD_MIDI usb_midi;
MIDI_CREATE_INSTANCE(Adafruit_USBD_MIDI, usb_midi, MIDI);

void setup() {
  // Device Discriptor
  TinyUSBDevice.setManufacturerDescriptor("YourName");
  TinyUSBDevice.setProductDescriptor("YourDevice");
  // MIDI Port Name
  usb_midi.setStringDescriptor("PortName");

  // initialize USB MIDI
  usb_midi.begin();
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // MIDI setting
  MIDI.turnThruOff();
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void send_midi_message(uint8_t status, uint8_t second, uint8_t third) {
  uint8_t channel = (status & 0x0F) + 1;
  switch(status & 0xF0) {
      case 0x90: // Note On
          MIDI.sendNoteOn(second, third, channel);
          break;
      case 0x80: // Note Off
          MIDI.sendNoteOff(second, third, channel);
          break;
      case 0xB0: // Control Change
          //MIDI.sendControlChange(second, third, channel);
          break;
      case 0xC0: // Program Change
          //MIDI.sendProgramChange(second, channel);
          break;
      default:
          break;
  }
}
