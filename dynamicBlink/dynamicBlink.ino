#include <CurieBLE.h>

const int ledPin = 13; // set ledPin to  LED
int ledState = LOW;

BLEPeripheral peripheral; // create peripheral instance
BLEService blinkService("19B10010-E8F2-537E-4F6C-D104768A1214"); // create service


// allow remote device to read and write
BLEIntCharacteristic blinkDelay("19B10011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);
// allow remote device to get notifications
BLEIntCharacteristic notification("19B10012-E8F2-537E-4F6C-D104768A1214", BLERead | BLENotify); // allows remote device to get notifications

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT); // use the LED on pin 13 as an output

  // set the local name peripheral advertises
  peripheral.setLocalName("DynamicBlink");
  // set the UUID for the service this peripheral advertises:
  peripheral.setAdvertisedServiceUuid(blinkService.uuid());

  // add service and characteristics
  peripheral.addAttribute(blinkService);
  peripheral.addAttribute(blinkDelay);
  peripheral.addAttribute(notification);

  blinkDelay.setValue(0);
  notification.setValue(0);

  // advertise the service
  peripheral.begin();

  Serial.println("Bluetooth device active, waiting for connections...");
}

void loop() {
  delay(blinkDelay.value());

  Serial.println(blinkDelay.value());

  // update LED, either central has written to characteristic or button state has changed
  if (ledState == LOW) {
    Serial.println("LED on");
    digitalWrite(ledPin, HIGH);
    ledState = HIGH;
    if ( notification.value() != ledState)
      notification.setValue(ledState);
  } else {
    Serial.println("LED off");
    digitalWrite(ledPin, LOW);
    ledState = LOW;
    if ( notification.value() != ledState)
      notification.setValue(ledState);
  }
}
