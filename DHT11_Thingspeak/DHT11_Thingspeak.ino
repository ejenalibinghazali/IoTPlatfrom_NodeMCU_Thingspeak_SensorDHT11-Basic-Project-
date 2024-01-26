#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
#include <DHT.h>

char ssid[] = "Rumah Inayah";
char pass[] = "rumah12345";
int keyIndex = 0;
WiFiClient  client;
DHT dht(D2, DHT11); //Pin, Jenis DHT

unsigned long myChannelNumber = 1570837;
const char * myWriteAPIKey = "4YQZFXBJZTU84M01";

void setup() {
  Serial.begin(115200);  // Initialize serial
  dht.begin();
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  // Menghubungkan ke internet
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    //    Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);  //Mencoba untuk menghubungkan ke internet
      Serial.print(".");
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  float suhu = dht.readTemperature();
  float kelembaban = dht.readHumidity();

  // Mengirim data ke thingspeak
  ThingSpeak.setField(1, suhu);
  ThingSpeak.setField(2, kelembaban);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    Serial.println("Berhasl update ke thingspeak");
    Serial.print("Suhu : ");
    Serial.println(suhu);
    Serial.print("Kelembaban : ");
    Serial.println(kelembaban);
  }
  else {
    Serial.println("Gagal update. HTTP error code " + String(x));
  }

  delay(15000); // Tunggu 15 detik untuk update lagi ke Thingspeak
}
