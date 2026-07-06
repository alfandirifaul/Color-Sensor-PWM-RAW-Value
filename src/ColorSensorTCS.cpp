#include "ColorSensorTCS.h"

// --- KONSTRUKTOR (Menyimpan data pin) ---
SensorTCS::SensorTCS(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t out, uint8_t pwm) {
  _pinS0 = s0; _pinS1 = s1; _pinS2 = s2;
  _pinS3 = s3; _pinOut = out; _pinPWM = pwm;
}

// --- FUNGSI SET KALIBRASI ---
void SensorTCS::setKalibrasi(int rawMin, int rawMax) {
  _rawMin = rawMin;
  _rawMax = rawMax;
}

// --- FUNGSI SETUP ---
void SensorTCS::begin() {
  pinMode(_pinS0, OUTPUT);
  pinMode(_pinS1, OUTPUT);
  pinMode(_pinS2, OUTPUT);
  pinMode(_pinS3, OUTPUT);
  pinMode(_pinOut, INPUT);
  pinMode(_pinPWM, OUTPUT);

  // Set frekuensi 20%
  digitalWrite(_pinS0, HIGH);
  digitalWrite(_pinS1, LOW);
  
  analogWrite(_pinPWM, 0);
}

// --- FUNGSI INTERNAL PEMBACAAN SENSOR ---
int SensorTCS::bacaWarna(uint8_t stateS2, uint8_t stateS3) {
  digitalWrite(_pinS2, stateS2);
  digitalWrite(_pinS3, stateS3);
  int nilai = pulseIn(_pinOut, LOW);
  delay(20);
  return nilai;
}

// --- FUNGSI UTAMA (Menjalankan Sensor & Menampilkan Log) ---
void SensorTCS::jalankanSensor() {
  // 1. Baca Warna
  int nilaiMerah = bacaWarna(LOW, LOW);
  int nilaiHijau = bacaWarna(HIGH, HIGH);
  int nilaiBiru  = bacaWarna(LOW, HIGH);

  // 2. Tampilkan nilai RAW ke Serial Monitor
  Serial.print("RAW -> R: "); Serial.print(nilaiMerah);
  Serial.print(" | G: "); Serial.print(nilaiHijau);
  Serial.print(" | B: "); Serial.print(nilaiBiru);

  // 3. Logika Zona Dinamis, Konversi PWM & Log Output
  if (nilaiMerah > _rawMax && nilaiHijau > _rawMax && nilaiBiru > _rawMax) {
    Serial.println(" -> GELAP (0V)");
    analogWrite(_pinPWM, 0);
  }
  else if (nilaiMerah < nilaiBiru && nilaiMerah < nilaiHijau) {
    int pwmOut = map(nilaiMerah, _rawMax, _rawMin, 25, 76);
    pwmOut = constrain(pwmOut, 25, 76); // Murni tanpa *2
    
    Serial.print(" -> ZONA MERAH | PWM: "); Serial.println(pwmOut);
    analogWrite(_pinPWM, pwmOut);
  }
  else if (nilaiHijau < nilaiMerah && nilaiHijau < nilaiBiru) {
    int pwmOut = map(nilaiHijau, _rawMax, _rawMin, 102, 153);
    pwmOut = constrain(pwmOut, 102, 153); // Murni tanpa *2
    
    Serial.print(" -> ZONA HIJAU | PWM: "); Serial.println(pwmOut);
    analogWrite(_pinPWM, pwmOut);
  }
  else if (nilaiBiru < nilaiMerah && nilaiBiru < nilaiHijau) {
    int pwmOut = map(nilaiBiru, _rawMax, _rawMin, 178, 230);
    pwmOut = constrain(pwmOut, 178, 230); // Murni tanpa *2
    
    Serial.print(" -> ZONA BIRU | PWM: "); Serial.println(pwmOut);
    analogWrite(_pinPWM, pwmOut);
  }
  else {
    Serial.println(" -> TIDAK JELAS (0V)");
    analogWrite(_pinPWM, 0); 
  }
}