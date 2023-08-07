    #include <WiFiEsp.h>
      #include <WiFiEspClient.h>
        #include <WiFiEspUdp.h>
      #include <TimeLib.h>  // Library TimeLib untuk mendapatkan waktu
    #include <LiquidCrystal_I2C.h> // Layar LED
// Masuk Di WiFi Kantor Sami
char ssid[] = "SAMI Office2";
char pass[] = "SeCuri!";

// alamat IP dan port API > (Konfirmasi Kholik)
char server[] = "10.62.124.35";
int port = 800;

// Nama pin untuk tombol
const int tombolPin = 2;

// Nama pin untuk LED
const int ledPin = 13;

// Objek LCD
LiquidCrystal_I2C lcd(0x27, 16, 2); // Alamat I2C dan ukuran 16x2

WiFiEspClient client;

void setup() {
  Serial.begin(115200);
  WiFi.init(&Serial);

  // Menunggu koneksi ke WiFi
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Mencoba terhubung ke ");
    Serial.println(ssid);
    WiFi.begin(ssid, pass);
    delay(5000);
  }
  Serial.println("Terhubung ke WiFi!");

  Serial.print("Alamat IP: ");
  Serial.println(WiFi.localIP());

  // Mengatur pin tombol sebagai input
  pinMode(tombolPin, INPUT);

  // Aktifkan resistor pull-up internal pada pin tombol
  digitalWrite(tombolPin, HIGH);

   // Mengatur pin LED sebagai output
  pinMode(ledPin, OUTPUT);

  // Inisialisasi LCD
  lcd.begin(16, 2);
  lcd.print("Tombol: ");
}

void loop() {
  // Baca status tombol (LOW jika ditekan, HIGH jika tidak ditekan)
  int statusTombol = digitalRead(tombolPin);

  // Jika tombol ditekan, kirim data ke API
  if (statusTombol == LOW) {
    Serial.println("Tombol ditekan!");
    
 // Mendapatkan waktu saat tombol ditekan
    char timestamp[20];
    sprintf(timestamp, "%02d:%02d:%02d", hour(), minute(), second());

    // Contoh data yang akan dikirim ke API
    String dataToSend = "Status=1&WaktuMasuk=";
    dataToSend += timestamp;

    // Membuka koneksi TCP ke server API
    if (client.connect(server, port)) {
      Serial.println("Terhubung ke server API");

      // // Mengirimkan data melalui HTTP POST
      client.print("POST /endpoint_api HTTP/1.1\r\n");
      client.print("Host: ");
      client.println(server);
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(dataToSend.length());
      client.println();
      client.println(dataToSend);

       // Matikan LED untuk memberi sinyal data telah dikirim
      digitalWrite(ledPin, LOW);
      delay(500);
      digitalWrite(ledPin, HIGH);

      // Tampilkan pesan di LCD
      lcd.setCursor(0, 1);
      lcd.print("Data terkirim");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print("                ");

    } else {
      Serial.println("Tidak dapat terhubung ke server API");
    }

    // Tunggu beberapa saat untuk menghindari pengiriman berulang saat tombol ditekan
    delay(1000);
  }
}