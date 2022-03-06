// BENGU YURDAKUL 


#define ZAMAN 2500                       // 2.5 sn boyunca mıknatıs kaç kere geçti(tekerlek kaç kere döndü)kontrolü için zaman


unsigned long ilkMillis = 0;             // zaman hesaplamaları için
unsigned long simdikiMillis = 0;         // zaman hesaplamaları için

const byte reedPin = 3;                  // reed pini tanımladık
byte simdikiDurum = 0;                   // reedpin den aldığımız değerlerş karşılaştırmak için    
byte oncekiDurum = 0;                    

int ledKirmizi = 10;                     // kırmızı ledi tanımladık
int ledBeyaz = 13;                       // beyaz ledi tanımladık
int buton = 2;                           // fren butonu tanımladık
int say = 0;                             // sayaç tanımladık
int i;   
                                                                                               
int kontrol = 4;                   // sayacın bir önceki turdaki hali ile şimdiki halini karşılaştırmak için, 4 atamamızın sebebi ise 20 jantlık
                                   // bisiklette yapılan denemeler sonucunda ortalama mıknatısı görme sayısının sayısının 2.5 saniyede 4 kez olması.
                                   // iki mıknatıs var, her mıknatıs geçtiğinde reed switch 1 değeri döndürüyor, 20 jantlık bisiklette, normal hızda
                                   // her 2.5 saniyede 4 kez 1 değerini alıyoruz bu sebepten dolayı 4 ile kontrol etmeye başladık. Sonra tur başına değişti.
                                   
boolean butonKontrol;              // Bu kontrolü yapmadığımda ışık istediğim gibi parlaklığını azalatarak sönmüyordu, ard arda iki kez aynı işlemi 
                                   // yaparak sönüyordu bu problemi çözmek için böyle bir kontrol yaptım.

void setup()
{
  pinMode(reedPin, INPUT_PULLUP);          
  pinMode(ledKirmizi, OUTPUT);            // gerekli pin ve led ayarlarını yaptık 
  pinMode(ledBeyaz, OUTPUT);            
  ilkMillis = millis();                 // başlangıç zamanını kaydettik
  attachInterrupt(digitalPinToInterrupt(buton), butonBasma, CHANGE);            // buton için interrupt tanımladık eğer değer değişirse kesecek
}

void loop()
{
  if (say != 0) {
    digitalWrite(ledBeyaz, HIGH);                     // eğer en az bir kez döndüyse (hareket varsa) beyaz ışığı yaktık (öndeki ışık)
  }
  simdikiMillis = millis();                          // şimdiki zamanı aldık
  oncekiDurum = simdikiDurum;                        // mıknatıs geçmiş mi kontrolü için önceki duruma şimdiki durumu atadık                          
  if (simdikiMillis - ilkMillis <= ZAMAN)            // zamanı aştık mı diye kontrol ettik (aşmadık)
  {
    simdikiDurum = digitalRead(reedPin);                     // eğer zamanı aşmadıysak reedpin deki değeri (0 ya da 1) simdikidurum a atadık
    if (simdikiDurum == LOW and oncekiDurum == HIGH)    // eğer simdikidurum ve öncekidurum farklı ise mıknatıs geçmiş demek
    {
      say++;                                             // say
    }
  }
  else                                                 // zamanı aştık
  {
     if (say == 0) {                                 // eğer hiç dönmediyse beyaz ışığı söndür (öndeki ışık)
        digitalWrite(ledBeyaz, LOW);
      }
     if (kontrol > say) {                           // eğer yavaşlamışsa çakar kırmızı ışığı yak (arkadaki ışık)
        i = 0;
        while (i<4) {
        digitalWrite(ledKirmizi, HIGH);
        delay(500);
        digitalWrite(ledKirmizi, LOW);
        delay(500);
        i = i + 1;
        }
      }

      kontrol = say;                               // kontrol değişkenine bir önceki sayaç değerini atadık
      say = 0;                                     // sayacı sıfırladık
      ilkMillis = millis();                        // karşılaştırmak için yeni zaman aldık
  }
}

void butonBasma()  {                                          // eğer butona (frene) basıldıysa interrupt ile bu fonksiyona geldik
    while(digitalRead(buton) == HIGH) {
      digitalWrite(ledKirmizi, HIGH);                         // butonun basılı olduğu süre boyunca kırmızı ışığı yak (arka ışık)
      butonKontrol = 1;                                       // ışığın düzgünce yanıp sönebilmesi için eklediğim kontrol değişkeni
  }

  if (digitalRead(buton)==LOW and butonKontrol ==1) {         // eğer butona basmayı (frene) bıraktıysak
   for (i = 255; i >= 0; i--){                                // ışığın parlaklığı azalarak sönsün
     analogWrite(ledKirmizi, i);                              // butonkontrol eklemeyince düzgün çalışmadı. !!
     delay(1000);
   }
   butonKontrol = 0;                                     // ışığın düzgünce yanıp sönebilmesi için eklediğim kontrol değişkeni
  }
}
