#include  <ESP8266WiFi.h>
#include  "ThingSpeak.h"
#define  BLYNK_PRINT Serial
#include  <ESP8266WiFi.h>
#include  <BlynkSimpleEsp8266.h>

char  auth[]  = ""; // Blynk  uygulaması  tarafından  , mailinize gelen token key 
char  ssid[]  = ""; // Bağlantı  yapacağınız wifi  adı 
char  pass[]  = ""; // Wifi  şifreniz    
unsigned long channelID =;
unsigned  int field_no=;
const char* writeAPIKey = ""; 
const char* readAPIKey  = "";  
/*    Pin Tanımları */
int sicaklikSensor=A0;      //  LM35  Data  ucu A0  pinine  bağlanacak
float sicaklikDegeri;     //  Analog  değeri  dönüştüreceğimiz  sıcaklık  değeri
float olculenDeger;        // Ölçeceğimiz analog  değer
WiFiClient  client;
/*  ESP8266 WiFi  Kurulum Fonksiyonu  */
void  wifiSetup ()
{
    delay (10);
    Serial.println(); Serial.println(ssid);
    Serial.println(F("Kablosuz  Agina Baglaniyor"));
    WiFi.begin(ssid, pass);
    //  WiFi  durum kontrolü
    while (WiFi.status()  !=  WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print(ssid);  Serial.println("Kablosuz  Aga Baglandi");
    Serial.println("IP  adresi: "); Serial.println(WiFi.localIP());
}
void  setup()
{
     Serial.begin(9600);      //  Serihaberleşme  9600  baud  hızında başlatılıyor
     wifiSetup();                    //  Kablosuz  ağ  kurulum fonksiyonu
     Blynk.begin(auth, ssid, pass);
     ThingSpeak.begin(client);                 //  ThingSpeak client sınıfı  başlatılıyor
     pinMode (A0,  INPUT);                // A0  ucu sensör  okumak  için  giriş modunda
     Serial.println(F("Kurulum Hazır"));
}
void  loop() 
{
  Blynk.run();
  delay(5000);
  sicaklik();     
}

void  sicaklik ()
{
      /*  LM35  sensöründen sıcaklık  değeri  okuma işlemi  */
        olculenDeger  = analogRead  (sicaklikSensor);   //  A0  analog  ucundan değer oku
        olculenDeger  = (olculenDeger/1024)*5000;         //  mv'a  dönüşüm işlemi
        sicaklikDegeri  = olculenDeger  /12,0;                 // mV'u  sıcaklığa dönüştür
        sicaklikDegeri  = sicaklikDegeri-10;
        Serial.print("Blynk  Gonderilen  Sicaklik  Değeri: "); Serial.println(sicaklikDegeri);
        Blynk.virtualWrite(V0, sicaklikDegeri);
        Serial.print("ThingSpeak  Gonderilen  Sicaklik  Değeri: "); Serial.println(sicaklikDegeri);

        if(sicaklikDegeri<20)
        {
           digitalWrite(16, HIGH);
           digitalWrite(5, LOW);
           digitalWrite(4, LOW);   
        }
        else if(sicaklikDegeri>35)
        {
          digitalWrite(4, HIGH);
          digitalWrite(16, LOW);   
          digitalWrite(5, LOW);   
        }
        else
        {
          digitalWrite(5, HIGH);   
          digitalWrite(4, LOW);
          digitalWrite(16, LOW);      
        }
      /*  ThingSpeak  Field Yazma İşlemi */
        ThingSpeak.writeField (channelID, field_no, sicaklikDegeri, writeAPIKey);             //  sıcaklık  değerini  gönder
//      ThingSpeak.setField (1, sicaklikDegeri);              //  1 nolu  field ı kur
//      ThingSpeak.writeFields(channelID, writeAPIKey);             //  kurulu  field lere  yaz (çoklu  yazma)
        Serial.println("\n");
}
