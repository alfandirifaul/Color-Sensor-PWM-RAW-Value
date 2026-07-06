/**
Wiring Diagram

VCC TCS3200 ➔ 5V Arduino Nano
GND TCS3200 ➔ GND Arduino Nano
S0 TCS3200 ➔ Pin D4 Arduino Nano
S1 TCS3200 ➔ Pin D5 Arduino Nano
S2 TCS3200 ➔ Pin D6 Arduino Nano
S3 TCS3200 ➔ Pin D7 Arduino Nano
OUT TCS3200 ➔ Pin D8 Arduino Nano

D9 Arduin Nano ➔ PWM In Module Converter
**/

#include <Arduino.h>

const int pinS0 = 4;
const int pinS1 = 5;
const int pinS2 = 6;
const int pinS3 = 7;
const int pinOut = 8;
const int pinPWM = 9;  // 1 Output PWM ke Modul Converter

#include "ColorSensorTCS.h"

SensorTCS sensorWarna(
  pinS0, pinS1, pinS2, pinS3, pinOut, pinPWM
);

void setup() {
  Serial.begin(9600);
  
  // Masukkan nilai kalibrasi (RAW_MIN, RAW_MAX)
  sensorWarna.setKalibrasi(10, 155);
  
  // Inisialisasi sensor
  sensorWarna.begin();
}

void loop() {
  // Jalankan semua proses pembacaan dan pengiriman ke PLC dengan 1 baris kode
  sensorWarna.jalankanSensor();
  
  delay(100);
}