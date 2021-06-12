#define BLYNK_PRINT Serial //Blynk Haberleşme
#include <BlynkSimpleEsp8266.h> //Blynk ESP8266 Haberleşme
char auth[] = "XXXXXXXXXXXXXXXX"; //Blynk ID
char ssid[] = "XXXXXXXXXXXXXXXX"; //WiFi Adı
char pass[] = "XXXXXXXXXXXXXXXX"; //WiFi Şifresi
BlynkTimer timer; //Timer tanımlama.

#include "DHT.h" //DHT11 kütüphanesi
#define DHTPIN 4 //DHT pin tanımlama(D2) 
#define DHTTYPE DHT11 //DHT tip belirleme
DHT dht(DHTPIN, DHTTYPE); //DHT tip ve pin bağlantı

int relay = 5; //Pompa pin tanımlama(D1)
int relay1 = 16; //Fan pin tanımlama(D0)
float persen; //Değer atama

void setup() {
  Blynk.begin(auth, ssid, pass); //Blynk kurulum.
  dht.begin(); //DHT11 kurulum.
  pinMode (relay, OUTPUT); //Çıkış tanımlama
  pinMode (relay1, OUTPUT); //Çıkış tanımlama
  timer.setInterval(1000L, analogSense); //AnalogSense kurulum.
  timer.setInterval(1000L, th); //th kurulum.
}

void loop() {
  Blynk.run(); //Blynk Çalıştır.
  timer.run(); //Timer Çalıştır.
}
void analogSense(){ //Toprak Nem Sensörü Çalışma Komutu
  float s1 = analogRead(A0); //Veri okuma.
  persen=((1024-s1)/1024)*100; //Okuman veri düzenleme
  Blynk.virtualWrite(V7, persen); //Blynk Toprak Nem bağlantı pin tanımlama.
  if(persen<=40){ //Okunan değer 40'ın altında olursa
    relayON(); //Pompa Çalıştır.
  }
  else if(persen>40){ //Okunan değer 40'ın üstünde olursa
    relayOFF(); //Pompa Kapat.
  }
}
void th(){ //DHT11 Sensörü Çalışma Komutu
  float h = dht.readHumidity(); //Nem değeri okuma
  float t = dht.readTemperature(); //Sıcaklık değeri okuma
  if(t>=27){ //Sıcaklık değeri 27'ye eşit veya üzerinde ise
    relay1ON(); //Fan Çalıştır.
  }
  else if(t<27){ //Sıcaklık değeri 27'den küçük ise 
    relay1OFF(); //Fan Kapat.
  }
  Blynk.virtualWrite(V5, h); //Blynk Nem bağlantı pini
  Blynk.virtualWrite(V6, t); //Blynk Sıcaklık bağlantı pini
}
void relayON(){ //Pompa Açık Komutu
  digitalWrite(relay, LOW);
  WidgetLED relay(V8); //Blynk Pompa bağlantı pini
  relay.on(); //Pompa Açık
}
void relayOFF(){ //Pompa Kapalı Komutu
  digitalWrite(relay, HIGH);
  WidgetLED relay(V8); //Blynk Pompa bağlantı pini
  relay.off(); //Pompa Kapalı
}

void relay1ON(){ //Fan Açık Komutu
  digitalWrite(relay1, LOW);
  WidgetLED relay1(V9); //Blynk Fan bağlantı pini
  relay1.on(); //Fan Açık
}
void relay1OFF(){ //Fan Kapalı Komutu
  digitalWrite(relay1, HIGH);
  WidgetLED relay1(V9); //Blynk Fan bağlantı pini
  relay1.off(); //Fan Kapalı
}
