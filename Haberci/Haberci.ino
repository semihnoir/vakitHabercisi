#include <LiquidCrystal_I2cTUR.h>
#include <Arduino_JSON.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>
#include <WiFiClientSecureBearSSL.h>

const char *ssid = "";     // Wifi SSID
const char *pwd = "";  // Wifi Şifresi
String city = "İstanbul";
String apiUrl = "http://api.aladhan.com/v1/timingsByCity/";
String dateApi = "https://timeapi.io/api/Time/current/zone?timeZone=Europe/Istanbul";

String imsak;
String gunes;
String ogle;
String ikindi;
String aksam;
String yatsi;

String saatDakika;
String yeniSahur;
String yeniIftar;

String saat;
String dakika;

const long utcOffsetInSeconds = 10800;

char daysOfTheWeek[7][12] = { "Pazar", "Pazartesi", "Salı", "Çarşamba", "Perşembe", "Cuma", "Cumartesi" };

// Define NTP Client to get time
WiFiClient wfc;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "1.tr.pool.ntp.org", utcOffsetInSeconds);

LiquidCrystal_I2cTUR lcd(0x27, 16, 2);

#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978
#define REST 0


int tempo = 180;

// change this to whichever pin you want to use
#define buzzer D7

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // Nokia Ringtone
  // Score available at https://musescore.com/user/29944637/scores/5266155

  NOTE_E5,
  8,
  NOTE_D5,
  8,
  NOTE_FS4,
  4,
  NOTE_GS4,
  4,
  NOTE_CS5,
  8,
  NOTE_B4,
  8,
  NOTE_D4,
  4,
  NOTE_E4,
  4,
  NOTE_B4,
  8,
  NOTE_A4,
  8,
  NOTE_CS4,
  4,
  NOTE_E4,
  4,
  NOTE_A4,
  2,
};



int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

void setup() {
  pinMode(D7, OUTPUT);
  Serial.begin(115200);
  Serial.print(ssid);
  Serial.println("Bağlanıyor...");
  Wire.begin(D4, D3);
  lcd.init();
  lcd.backlight();
  WiFi.begin(ssid, pwd);
  timeClient.begin();

  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("Mobility");
  lcd.setCursor(7, 1);
  lcd.print("<3");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bağlanıyor...");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Başarılı.");
  for (int i = 0; i <= 2; i++) {
    tone(D7, NOTE_B7);
    delay(300);
    noTone(D7);
    delay(300);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Başarılı.");
  delay(1100);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IP: ");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(1200);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Veri alınıyor...");

  lcd.clear();
}

void loop() {
  // Saat ve dakikayı güncelle
  timeClient.update();
  saat = String(timeClient.getHours() < 10 ? "0" + String(timeClient.getHours()) : String(timeClient.getHours()));
  dakika = String(timeClient.getMinutes() < 10 ? "0" + String(timeClient.getMinutes()) : String(timeClient.getMinutes()));
  saatDakika = saat + ":" + dakika;

  while (true) {
    timeClient.update();
    saat = String(timeClient.getHours() < 10 ? "0" + String(timeClient.getHours()) : String(timeClient.getHours()));
    dakika = String(timeClient.getMinutes() < 10 ? "0" + String(timeClient.getMinutes()) : String(timeClient.getMinutes()));
    saatDakika = saat + ":" + dakika;

    yeniSahur = GetPrayerTime("Fajr");
    yeniIftar = GetPrayerTime("Maghrib");


    // LCD'ye bilgileri yazdır
    Kontrol();
    Vakitler();
    delay(3000);
    Kontrol();
    Saat();
    delay(2000);
  }
}

void Kontrol() {
  if (saatDakika == yeniIftar) {

    Serial.println("İftar vakti!");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("İftar Vakti!");
    lcd.setCursor(0, 1);
    lcd.print(saatDakika);
    // iterate over the notes of the melody.
    // Remember, the array is twice the number of notes (notes + durations)
    for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {
      // calculates the duration of each note
      divider = melody[thisNote + 1];
      if (divider > 0) {
        // regular note, just proceed
        noteDuration = (wholenote) / divider;
      } else if (divider < 0) {
        // dotted notes are represented with negative durations!!
        noteDuration = (wholenote) / abs(divider);
        noteDuration *= 1.5;  // increases the duration in half for dotted notes
      }

      // we only play the note for 90% of the duration, leaving 10% as a pause
      tone(buzzer, melody[thisNote], noteDuration * 0.9);

      // Wait for the specief duration before playing the next note.
      delay(noteDuration);

      // stop the waveform generation before the next note.
      noTone(buzzer);
    }
  }

  else {
    lcd.clear();
    Vakitler();
    delay(5000);
    Saat();
    delay(5000);
  }
}

void Saat() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(daysOfTheWeek[timeClient.getDay()]);
  lcd.setCursor(0, 1);
  lcd.print(saat);
  lcd.print(":");
  lcd.print(dakika);
}

void Vakitler() {
  Serial.println("Sahur: ");
  Serial.println(yeniSahur);
  // Serial.print("Gunes : "); Serial.println(gunes);
  // Serial.print("Ogle : "); Serial.println(ogle);
  // Serial.print("Ikindi : "); Serial.println(ikindi);
  Serial.print("İftar: ");
  Serial.println(yeniIftar);
  // Serial.print("Yatsi : "); Serial.println(yatsi);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sahur:");
  lcd.print(yeniSahur);
  lcd.setCursor(0, 1);
  lcd.print("İftar:");
  lcd.print(yeniIftar);
}

String GetDate() {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    // Ignore SSL certificate validation
    client->setInsecure();
  HTTPClient https;
  https.begin(*client, dateApi);

  int httpCode = https.GET();

  if (httpCode > 0) {
    String payload = https.getString();
    Serial.println("API Response: " + payload);

    // JSON verisini ayrıştır
    JSONVar jsonData = JSON.parse(payload);

    if (JSON.typeof(jsonData) == "undefined") {
      Serial.println("Json data ayıklanamadı.");
      return "";
    }

    String date = (const char *)(jsonData["date"]);
    return date;
  } else {
    Serial.println("HTTP Bağlantı Hatası");
    return "";
  }

  https.end();
}

String formatDateString(String originalDate) {
  // Ayırıcı ("/") kullanarak tarihi parçala
  int firstSlashIndex = originalDate.indexOf('/');
  int secondSlashIndex = originalDate.indexOf('/', firstSlashIndex + 1);

  // Gün, ay ve yılı ayır
  String day = originalDate.substring(firstSlashIndex + 1, secondSlashIndex);
  String month = originalDate.substring(0, firstSlashIndex);
  String year = originalDate.substring(secondSlashIndex + 1);

  // Yeni formatta tarihi oluştur
  String formattedDate = day + "-" + month + "-" + year;
  Serial.println("Tarih: " + formattedDate);
  return formattedDate;
}

String GetPrayerTime(String prayer) {
  // API'ye istek yap
  HTTPClient http;
  String date = GetDate();
  String fDate = formatDateString(date);
  http.begin(wfc, apiUrl + fDate + "?city=" + city + "&country=Turkey&method=8");

  int httpCode = http.GET();

  if (httpCode > 0) {
    String payload = http.getString();
    Serial.println("API Response: " + payload);

    // JSON verisini ayrıştır
    JSONVar jsonData = JSON.parse(payload);

    if (JSON.typeof(jsonData) == "undefined") {
      Serial.println("Json data ayıklanamadı.");
      return "";
    }

    String prayerTime = (const char *)(jsonData["data"]["timings"][prayer]);
    Serial.println(prayer + " vakti: " + prayerTime);
    return prayerTime;
  } else {
    Serial.println("HTTP Bağlantı Hatası");
    return "";
  }

  http.end();
}
