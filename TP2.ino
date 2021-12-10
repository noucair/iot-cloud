/* Read RFID Tag with RC522 RFID Reader
 *  Made by miliohm.com
 */
 
#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
WiFiClient client;

const int  RST_PIN = 3;     // Configurable, see typical pin layout above
const int SS_PIN = 4;     // Configurable, see typical pin layout above

String apiKey = "XNH7CYDPLYTZ03QG";  
const char* server = "api.thingspeak.com";

const char *ssid = "INPT-Test_A";
const char *pass = "iinnpptt"; 

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key;

String tag;

void setup() {
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
   delay(500);
   Serial.print(".");              // print … till not connected
  }
  Serial.println("");
  Serial.println("WiFi connected");
  send_data();
  Serial.println("Tag detected");
  Serial.println("Tag id: 4587643159865");

}

void loop() {
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if (rfid.PICC_ReadCardSerial()) {
    Serial.println("Tag detected");
    for (byte i = 0; i < 4; i++) {
      tag += rfid.uid.uidByte[i];
    }
    Serial.println(tag);
    tag = "";
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
  }
}



void send_data(){
if (client.connect(server,80))  
{  

String postStr = apiKey;
postStr +="&field1=";
postStr += String("4587643159865");
postStr += "\r\n\r\n";

Serial.println(postStr);

client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);

Serial.println("%. Send to Thingspeak.");
}   
client.stop();
Serial.println("Waiting...");
// thingspeak needs minimum 15 sec delay between updates
delay(15000);
}
