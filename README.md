# VakitHabercisi
 Ramazan ayına özel ESP8266 vakit habercisi.

Kaynaklar:

Referans aldığım kod: https://www.devrelerim.com/2023/01/nodemcu-namaz-vakitlerini-cekme.html
Arduino buzzer melodileri: https://github.com/robsoncouto/arduino-songs
Vakitlerin olduğu API sitesi: http://www.namazvakitleri.site/api/index.php


Gereksinimler: 

ESP8266'ya kod yüklemek için Arduino IDE'ye bu linki yapıştırın
 ve ESP8266 kütüphanesini ekleyin.

https://arduino.esp8266.com/stable/package_esp8266com_index.json


Gerekli kütüphaneleri kurun.

#include <Arduino.h> <zaten yüklü>
#include <Arduino_JSON.h>
#include <ESP8266WiFi.h> <ESP8266 ile birlikte gelir>
#include <ESP8266HTTPClient.h> <ESP8266 ile birlikte gelir>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <slowAES.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h> <ESP8266 ile birlikte gelir>


Kodu geliştirmek için forklamanız yeterli :)