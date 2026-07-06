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


const int pinS0 = 4;
const int pinS1 = 5;
const int pinS2 = 6;
const int pinS3 = 7;
const int pinOut = 8;
const int pinPWM = 9;  // 1 Output PWM ke Modul Converter

// --- NILAI KALIBRASI WARNA MENTAH (RAW) ---
const int RAW_MIN = 20;   // Nilai pulseIn saat warna paling PEKAT / dekat dengan sensor
const int RAW_MAX = 100;  // Nilai pulseIn saat warna REDUP / batas sebelum dianggap kosong

int nilaiMerah = 0;
int nilaiHijau = 0;
int nilaiBiru = 0;

void setup() {
  Serial.begin(9600);

  pinMode(pinS0, OUTPUT);
  pinMode(pinS1, OUTPUT);
  pinMode(pinS2, OUTPUT);
  pinMode(pinS3, OUTPUT);
  pinMode(pinOut, INPUT);
  pinMode(pinPWM, OUTPUT);

  // Skala frekuensi 20%
  digitalWrite(pinS0, HIGH);
  digitalWrite(pinS1, LOW);
  
  analogWrite(pinPWM, 0);
}

void loop() {
  // 1. BACA NILAI SENSOR
  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, LOW);
  nilaiMerah = pulseIn(pinOut, LOW);
  delay(20); 

  digitalWrite(pinS2, HIGH);
  digitalWrite(pinS3, HIGH);
  nilaiHijau = pulseIn(pinOut, LOW);
  delay(20);

  digitalWrite(pinS2, LOW);
  digitalWrite(pinS3, HIGH);
  nilaiBiru = pulseIn(pinOut, LOW);
  delay(20);

  Serial.print("RAW -> R: "); Serial.print(nilaiMerah);
  Serial.print(" | G: "); Serial.print(nilaiHijau);
  Serial.print(" | B: "); Serial.print(nilaiBiru);

  // 2. LOGIKA ZONA DINAMIS
  if (nilaiMerah > RAW_MAX && nilaiHijau > RAW_MAX && nilaiBiru > RAW_MAX) {
    Serial.println(" -> GELAP (0V)");
    analogWrite(pinPWM, 0);
  }
  // ZONA MERAH (Dominan Merah)
  else if (nilaiMerah < nilaiBiru && nilaiMerah < nilaiHijau) {
    // Mapping: RAW_MAX(Redup) -> RAW_MIN(Pekat) menjadi PWM 25 -> 76 (~1V hingga 3V)
    int pwmOut = map(nilaiMerah, RAW_MAX, RAW_MIN, 25, 76);
    pwmOut = constrain(pwmOut, 25, 76); // Kunci agar tidak bocor ke zona lain
    
    Serial.print(" -> ZONA MERAH | PWM: "); Serial.println(pwmOut);
    analogWrite(pinPWM, pwmOut);
  }
  // ZONA HIJAU (Dominan Hijau)
  else if (nilaiHijau < nilaiMerah && nilaiHijau < nilaiBiru) {
    // Mapping: RAW_MAX(Redup) -> RAW_MIN(Pekat) menjadi PWM 102 -> 153 (~4V hingga 6V)
    int pwmOut = map(nilaiHijau, RAW_MAX, RAW_MIN, 102, 153);
    pwmOut = constrain(pwmOut, 102, 153);
    
    Serial.print(" -> ZONA HIJAU | PWM: "); Serial.println(pwmOut);
    analogWrite(pinPWM, pwmOut);
  }
  // ZONA BIRU (Dominan Biru)
  else if (nilaiBiru < nilaiMerah && nilaiBiru < nilaiHijau) {
    // Mapping: RAW_MAX(Redup) -> RAW_MIN(Pekat) menjadi PWM 178 -> 230 (~7V hingga 9V)
    int pwmOut = map(nilaiBiru, RAW_MAX, RAW_MIN, 178, 230);
    pwmOut = constrain(pwmOut, 178, 230);
    
    Serial.print(" -> ZONA BIRU | PWM: "); Serial.println(pwmOut);
    analogWrite(pinPWM, pwmOut);
  }
  else {
    Serial.println(" -> TIDAK JELAS (0V)");
    analogWrite(pinPWM, 0); 
  }

  delay(100); 
}