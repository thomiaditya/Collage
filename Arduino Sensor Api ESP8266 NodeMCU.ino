
#include <ESP8266WiFi.h>
#include <CTBot.h>
#include <CTBotDataStructures.h>
#include <CTBotInlineKeyboard.h>
#include <CTBotReplyKeyboard.h>
#include <Utilities.h>
#include <EEPROM.h>


/*
   PENJELASAN PIN
   D0 = Buzzer
   A0 = Sensor Flame
*/


String pass;
String ssid_;
int ssid;
String BotToken = "882232600:AAFZJ1qLdzNPxp40Csc5YtGDYB4LrICVWE4";
const int id = 787077279;

CTBot botTL;

struct Wifi {
  String ssid;
  String pass;
};

void searchWifi() {
  bool finish = false;
  while (!finish) {
    Serial.println("Scan start!");
    int n = WiFi.scanNetworks();

    Serial.println("Scan done!");
    if (n == 0) {
      Serial.println("No networks found!");
    } else {
      Serial.print(n);
      Serial.println(" Networks found!");
      for (int i = 0; i < n; ++i) {
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
        delay(10);
      }
      finish = true;
    }
  }
  Serial.println("");
}


//=========================================================== START ==================================================================== //


void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ;
  }
  delay(3000);

  WiFi.mode(WIFI_STA);
  //  WiFi.disconnect();
  delay(100);

  if (WiFi.status() != WL_CONNECTED && ssid_ == NULL && pass == NULL) {
    searchWifi();
    Serial.println("\n Masukkan ID : ");
    while (Serial.available() == 0) {}
    ssid = Serial.parseInt();
    Serial.println(ssid);

    Serial.println("\n Masukkan Pass : ");
    for ( ; ; ) {
      if (Serial.available()) {
        pass = Serial.readStringUntil('\n');
        Serial.println(pass);

        if (pass != NULL) {
          WiFi.begin(WiFi.SSID(ssid - 1), pass);
          Serial.print("Connecting");
          while (WiFi.status() != WL_CONNECTED) {
            Serial.print(".");
            delay(200);
          }
          Serial.println("");
          Serial.println("WiFi connected");
          Serial.println("IP address: ");
          Serial.println(WiFi.localIP());

          break;
        }
      }
    }
  } else if (ssid_ != NULL && pass != NULL) {
    WiFi.begin(ssid_, pass);
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
      Serial.print(".");
      delay(200);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

  } else {
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
  //  // EEPROM
  //  String ssidname = WiFi.SSID(ssid - 1);
  //
  //  EEPROM.begin(512);
  //  EEPROM.put(0, ssidname);
  //  EEPROM.put(0 + sizeof(ssidname), pass);
  //  char ok[2 + 1] = "OK";
  //  EEPROM.put(0 + sizeof(ssidname) + sizeof(pass), ok);
  //  EEPROM.commit();
  //
  //
  //  EEPROM.get(0, ssidname);
  //  EEPROM.get(0+sizeof(ssidname), pass);
  //  char oki[2+1];
  //  EEPROM.get(0+sizeof(ssidname)+sizeof(pass), ok);
  //  EEPROM.end();
  //  if (String(oki) != String("OK")) {
  //    ssidname[0] = 0;
  //    pass[0] = 0;
  //  }
  //  Serial.println("Recovered credentials:");
  //  Serial.println(ssidname);
  //  Serial.println(strlen(pass)>0?"********":"<no password>");
  //  // ENDEEPROM

  botTL.setTelegramToken(BotToken);
  botTL.sendMessage(id, "Alat telah menyala!");
  Serial.println("Pesan Terkirim");

  botTL.sendMessage(id, "Password : ");

  bool benar = false;
  while (!benar) {
    TBMessage msg;
    if (botTL.getNewMessage(msg)) {
      if (!(msg.text.equalsIgnoreCase("thomiaditya"))) {
        botTL.sendMessage(id, "Password Salah!");
      } else {

        botTL.sendMessage(id, "Password Benar!");
        benar = true;
      }
    }
  }
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D5, INPUT);
  Serial.println("Setup selesai");

}

String checking(int read) {
  if (read == 1) return "Menyala";
  else if (read == 0) return "Mati";
}

int sensorMotion (int read) {
  if (read > 100) return 1;
  else return 0;
}

void safeMode(int sensorMotion, TBMessage msg, CTBot botTL) {
  while (sensorMotion == 1) {
    digitalWrite(D1, HIGH);
    botTL.sendMessage(msg.sender.id, "Ada penyusup!!!");
    delay(5000);
  }
  digitalWrite(D1, LOW);
}
bool safeModeOn = false;
void loop() {
  
  TBMessage msg;

  if (botTL.getNewMessage(msg)) {

    if (msg.text.equalsIgnoreCase("BUZZER ON")) {
      digitalWrite(D1, HIGH);
      botTL.sendMessage(msg.sender.id, "Buzzer Menyala");
    } else if (msg.text.equalsIgnoreCase("BUZZER OFF")) {
      digitalWrite(D1, LOW);
      botTL.sendMessage(msg.sender.id, "Buzzer Mati");
    } else if (msg.text.equalsIgnoreCase("LAMPU ON")) {
      digitalWrite(D2, HIGH);
      botTL.sendMessage(msg.sender.id, "Lamp Menyala");
    } else if (msg.text.equalsIgnoreCase("LAMPU OFF")) {
      digitalWrite(D2, LOW);
      botTL.sendMessage(msg.sender.id, "Lamp Mati");
    } else if (msg.text.equalsIgnoreCase("CHECK")) {
      int sensorFlameAnalog = digitalRead(D5);
      String sensorApi = sensorFlameAnalog == 1 ? "Menyala" : "Mati";

      botTL.sendMessage(msg.sender.id, "Lampu " + checking(digitalRead(D2)) + "\nBuzzer " + checking(digitalRead(D1)) + "\nKompor " + sensorApi );

    } else if (msg.text.equalsIgnoreCase("SAFE MODE ON")) {
      safeModeOn = true;
      Serial.println("Safe Mode Online");
      botTL.sendMessage(msg.sender.id, "Safe Mode Online");
    } else if (msg.text.equalsIgnoreCase("SAFE MODE OFF")) {
      safeModeOn = false;
      Serial.println("Safe Mode Offline");
      botTL.sendMessage(msg.sender.id, "Safe Mode Offline");
    }
    else {
      String reply;
      reply = (String)"Aku tidak mengerti " + msg.sender.username + (String)".";
      botTL.sendMessage(msg.sender.id, reply);
    }

  }

  if(safeModeOn) {
    safeMode(sensorMotion(analogRead(A0)), msg, botTL);
    Serial.println("Hai!!");
  }


  //
  //
  //
  //    Serial.println(analogRead(A0));
  //
  //    if (sensorFlameAnalog < 70) {
  //      Serial.println("Danger, there's Fire around!");
  //      digitalWrite(D1, HIGH);
  //      botTL.sendMessage(msg.sender.id, "Ada api!!");
  //    } else {
  //      digitalWrite(D1, LOW);
  //    }
  //Serial.println(sensorMotion(analogRead(A0)));

}
