#ifndef ColorSensorTCS_h
#define ColorSensorTCS_h

#include <Arduino.h>

class SensorTCS {
  private:
    // Variabel internal pin
    uint8_t _pinS0, _pinS1, _pinS2, _pinS3, _pinOut, _pinPWM;
    
    // Variabel internal kalibrasi
    int _rawMin, _rawMax;

    // Fungsi internal untuk membaca warna
    int bacaWarna(uint8_t stateS2, uint8_t stateS3);

  public:
    // Konstruktor: Dipanggil saat inisialisasi di main.ino
    SensorTCS(uint8_t s0, uint8_t s1, uint8_t s2, uint8_t s3, uint8_t out, uint8_t pwm);
    
    // Fungsi untuk mengatur nilai kalibrasi
    void setKalibrasi(int rawMin, int rawMax);
    
    // Fungsi setup awal
    void begin();
    
    // Fungsi utama yang dipanggil di loop()
    void jalankanSensor();
};

#endif