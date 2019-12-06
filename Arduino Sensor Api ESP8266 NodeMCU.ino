
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
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

int i = 0;
int sumConnection;
int statusCode;
String pass;
String ssid_;
int ssid;
String BotToken = "882232600:AAFZJ1qLdzNPxp40Csc5YtGDYB4LrICVWE4";
const int id = 787077279;

String st;
String content;

#define CALLBACK_QUERY_DATA  "QueryData"
CTBot botTL;
CTBotReplyKeyboard myKbd;

bool safeModeOn = false;

bool testConnectWifi();
void createAP();
void createWebServer();
void launchWeb();

ESP8266WebServer server(80);

//=========================================================== SETUP ==================================================================== //


void setup() {
  Serial.begin(115200);

  while (!Serial) {
    ;
  }
  delay(3000);

  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D5, INPUT);
  pinMode(D0, OUTPUT);

  WiFi.mode(WIFI_STA);
  Serial.println("Disconnecting");
  WiFi.disconnect();
  delay(100);

  EEPROM.begin(512);

  //  ========================================= READ EEPROM FROM CURRENT SSID AND PASS
  Serial.print("Reading SSID");

  String essid;
  String espass = "";

  for (int i = 0; i < 32; i++) {
    essid += char(EEPROM.read(i));
    Serial.print(".");
    delay(100);
  }
  Serial.println("");
  if (essid != NULL) {
    Serial.print("SSID: ");
    Serial.println(essid);
  } else {
    Serial.println("No such of SSID saved before!");
  }

  Serial.print("Reading PASS");
  for (int i = 32; i < 96; i++) {
    espass += char(EEPROM.read(i));
    Serial.print(".");
    delay(100);
  }
  Serial.println("");
  if (espass != NULL) {
    Serial.print("PASS: ");
    Serial.println(espass);
  } else {
    Serial.println("No such of Pass saved before!");
  }
  WiFi.begin(essid.c_str(), espass.c_str());

  if (testConnectWifi())
  {
    Serial.println("Succesfully Connected!!!");
  }
  else
  {
    Serial.println("Turning the HotSpot On");
    launchWeb();
    createAP();

    Serial.println();
    Serial.println("Waiting.");

    while ((WiFi.status() != WL_CONNECTED)) {
      Serial.println("");
      server.handleClient();
      digitalWrite(D0, HIGH);
      delay(500);
      digitalWrite(D0, LOW);
      delay(1000);
    }
  }


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
      } else if ((msg.text.equalsIgnoreCase("thomiaditya"))) {
        botTL.sendMessage(id, "Password Benar!");
        benar = true;
      }
    }
  }

  myKbd.addButton("Lampu On");
  myKbd.addButton("Lampu Off");
  myKbd.addRow();
  myKbd.addButton("Buzzer On");
  myKbd.addButton("Buzzer Off");
  myKbd.addRow();
  myKbd.addButton("Safe Mode On");
  myKbd.addButton("Safe Mode Off");
  myKbd.addRow();
  myKbd.addButton("Check");

  myKbd.addButton("Hide Keyboard");

  Serial.println("Setup selesai");

}

//=================================================================== END SETUP ==============================================================

/*
   searchWifi() method
   Berguna untuk mencari wifi yang tersedia
*/
void createAP() {

  WiFi.mode(WIFI_STA);
  Serial.println("Disconnecting");
  WiFi.disconnect();
  delay(100);

  Serial.println("Scan start!");
  int n = WiFi.scanNetworks();
  sumConnection = n;
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
  }

  Serial.println("");
  st = "<ol>";
  for (int i = 0; i < n; ++i)
  {
    // Print SSID and RSSI for each network found
    st += "<li>";
    st += WiFi.SSID(i);
    st += " (";
    st += WiFi.RSSI(i);

    st += ")";
    st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
    st += "</li>";
  }
  st += "</ol>";
  delay(100);
  WiFi.softAP("My Home on My Hand", "thomiaditya");
  Serial.println("softap");
  launchWeb();
  Serial.println("over");
}

/*
   Menjalankan Website
*/

void launchWeb()
{
  Serial.println("");
  if (WiFi.status() == WL_CONNECTED)
    Serial.println("WiFi connected");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("SoftAP IP: ");
  Serial.println(WiFi.softAPIP());
  createWebServer();
  server.begin();
  Serial.println("Server started");
}

/*
   testConnectWifi() Method
   Mendeteksi apakah wifi terkoneksi

   Return bool;

*/
bool testConnectWifi() {
  Serial.print("Connecting please wait ");
  for (int i = 0; i < 35; i++) {
    digitalWrite(D0, HIGH);
    if (WiFi.status() == WL_CONNECTED) {
      digitalWrite(D0, LOW);
      delay(100);
      digitalWrite(D1, HIGH);
      delay(1000);
      digitalWrite(D1, LOW);
      return 1;
    }
    delay(200);
    Serial.print(".");
  }
  for (int j = 0; j < 3; j++) {
    digitalWrite(D1, HIGH);
    delay(100);
    digitalWrite(D1, LOW);
    delay(100);
  }
  digitalWrite(D0, LOW);
  Serial.println("");
  Serial.println("Connection failed! (Gagal)");
  return 0;

}

/*
   Membuat Web Server

*/

void createWebServer() {
  {
    server.on("/", []() {

      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      content = "<!DOCTYPE HTML>\r\n<html>Hello from ESP8266 at ";
      content += "<form action=\"/scan\" method=\"POST\"><input type=\"submit\" value=\"scan\"></form>";
      content += ipStr;
      content += "<p>";
      content += st;
      content += "</p><form method='get' action='setting'><label>SSID: </label><input name='ssid' placeholder='Masukkan ID' length=32><br><label>PASS: </label><input name='pass' length=64><br><input type='submit'></form>";
      content += "</html>";
      server.send(200, "text/html", content);
    });
    server.on("/scan", []() {

      Serial.println("Scan start!");
      int n = WiFi.scanNetworks();
      sumConnection = n;
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
      }

      Serial.println("");
      st = "<ol>";
      for (int i = 0; i < n; ++i)
      {

        st += "<li>";
        st += WiFi.SSID(i);
        st += " (";
        st += WiFi.RSSI(i);

        st += ")";
        st += (WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*";
        st += "</li>";
      }
      st += "</ol>";
      delay(100);

      IPAddress ip = WiFi.softAPIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);

      content = "<!DOCTYPE HTML><form action=\"/\" method=\"GET\"><input type=\"submit\" value=\"scan\"></form><html>";
      server.send(200, "text/html", content);
    });

    server.on("/setting", []() {
      String qsid = WiFi.SSID(server.arg("ssid").toInt() - 1);
      String qpass = server.arg("pass");
      if (qsid.length() > 0 && qpass.length() > 0) {
        Serial.println("clearing eeprom");
        for (int i = 0; i < 96; ++i) {
          EEPROM.write(i, 0);
        }
        Serial.println(qsid);
        Serial.println("");
        Serial.println(qpass);
        Serial.println("");

        Serial.println("writing eeprom ssid:");
        for (int i = 0; i < qsid.length(); ++i)
        {
          EEPROM.write(i, qsid[i]);
          Serial.print("Wrote: ");
          Serial.println(qsid[i]);
        }
        Serial.println("writing eeprom pass:");
        for (int i = 0; i < qpass.length(); ++i)
        {
          EEPROM.write(32 + i, qpass[i]);
          Serial.print("Wrote: ");
          Serial.println(qpass[i]);
        }
        EEPROM.commit();

        content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
        statusCode = 200;
        ESP.reset();
      } else {
        content = "{\"Error\":\"404 not found\"}";
        statusCode = 404;
        Serial.println("Sending 404");
      }
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(statusCode, "application/json", content);

    });
  }
}


//================================== GERAK SENSOR DLL ==================================

String checking(int read) {
  if (read == 1) return "Menyala";
  else if (read == 0) return "Mati";
}

int sensorMotion (int read) {
  if (read > 100) return 1;
  else return 0;
}

void safeMode(int sensorMotion, TBMessage msg, CTBot botTL) {
  if (sensorMotion == 1) {
    digitalWrite(D1, HIGH);
    botTL.sendMessage(msg.sender.id, "Ada penyusup!!!");
    delay(1);
  } else digitalWrite(D1, LOW);
}


//===================================================================================================================== LOOP ==================================================================================================

bool isKeyboardActive = false;

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(D0, HIGH);
    TBMessage msg;

    if (botTL.getNewMessage(msg)) {
      if (msg.text.equalsIgnoreCase("/showkeyboard")) {
        // the user is asking to show the reply keyboard --> show it
        String reply = "Reply Keyboard enable.";
        botTL.sendMessage(msg.sender.id, reply, myKbd);
        isKeyboardActive = true;
      } else if (isKeyboardActive) {
        if (msg.text.equalsIgnoreCase("Hide Keyboard")) {
          // sent the "hide keyboard" message --> hide the reply keyboard
//          botTL.removeReplyKeyboard(msg.sender.id, "Keyboard removed");
          isKeyboardActive = false;
        } else {
         
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
      } else {

        botTL.sendMessage(msg.sender.id, "Try '/showkeyboard'");
      }

      //      if (msg.text.equalsIgnoreCase("BUZZER ON")) {
      //        digitalWrite(D1, HIGH);
      //        botTL.sendMessage(msg.sender.id, "Buzzer Menyala");
      //      } else if (msg.text.equalsIgnoreCase("BUZZER OFF")) {
      //        digitalWrite(D1, LOW);
      //        botTL.sendMessage(msg.sender.id, "Buzzer Mati");
      //      } else if (msg.text.equalsIgnoreCase("LAMPU ON")) {
      //        digitalWrite(D2, HIGH);
      //        botTL.sendMessage(msg.sender.id, "Lamp Menyala");
      //      } else if (msg.text.equalsIgnoreCase("LAMPU OFF")) {
      //        digitalWrite(D2, LOW);
      //        botTL.sendMessage(msg.sender.id, "Lamp Mati");
      //      } else if (msg.text.equalsIgnoreCase("CHECK")) {
      //        int sensorFlameAnalog = digitalRead(D5);
      //        String sensorApi = sensorFlameAnalog == 1 ? "Menyala" : "Mati";
      //
      //        botTL.sendMessage(msg.sender.id, "Lampu " + checking(digitalRead(D2)) + "\nBuzzer " + checking(digitalRead(D1)) + "\nKompor " + sensorApi );
      //
      //      } else if (msg.text.equalsIgnoreCase("SAFE MODE ON")) {
      //        safeModeOn = true;
      //        Serial.println("Safe Mode Online");
      //        botTL.sendMessage(msg.sender.id, "Safe Mode Online");
      //      } else if (msg.text.equalsIgnoreCase("SAFE MODE OFF")) {
      //        safeModeOn = false;
      //        Serial.println("Safe Mode Offline");
      //        botTL.sendMessage(msg.sender.id, "Safe Mode Offline");
      //      }
      //      else {
      //        String reply;
      //        reply = (String)"Aku tidak mengerti " + msg.sender.username + (String)".";
      //        botTL.sendMessage(msg.sender.id, reply);
      //      }

    }

    if (safeModeOn) {
      safeMode(sensorMotion(analogRead(A0)), msg, botTL);
      Serial.println("Hai!!");
    }





  } else {
    digitalWrite(D0, LOW);
    for (int j = 0; j < 3; j++) {
      digitalWrite(D1, HIGH);
      delay(100);
      digitalWrite(D1, LOW);
      delay(100);
    }
    ESP.reset();
  }

}
