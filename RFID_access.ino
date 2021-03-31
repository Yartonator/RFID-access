#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <SPI.h>
#include <MFRC522.h>

#define BLYNK_PRINT Serial
#define SS_PIN 21
#define RST_PIN 22
#define SIZE_BUFFER 18
#define MAX_SIZE_BLOCK 16
#define greenPin 32
#define redPin 12

MFRC522 mfrc522(SS_PIN, RST_PIN);

WidgetTerminal terminal(V1);

char auth[] = "J6UGTJH54drXUFLwzZ-hnAEx1GgGhV6N";

char ssid[] = "telenet-BEEC4D1";
char pass [] = "6p6xrEeuwtyk";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SPI.begin();
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(32, OUTPUT);
  mfrc522.PCD_Init();

  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");

  Serial.println("Approach with your card or tag please.");
  Serial.println();
  
  Blynk.begin("J6UGTJH54drXUFLwzZ-hnAEx1GgGhV6N", ssid, pass);
}


void loop() {

  Blynk.run();

  if ( ! mfrc522.PICC_IsNewCardPresent())
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++)
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  if (content.substring(1) == "09 CF 65 E4") //change here the UID of the card/cards that you want to give access
  {
    digitalWrite(greenPin, HIGH);
    Serial.println("Authorized access");
    Serial.println();
    Blynk.virtualWrite(V1, "Authorized user access");
    delay(3000);
    digitalWrite(greenPin, LOW);
  }

  else   {
    digitalWrite(redPin, HIGH);
    Serial.println(" Access denied");
    Serial.println();
    Blynk.virtualWrite(V1, "Unauthorized user trying to access");
    delay(3000);
    digitalWrite(redPin, LOW);
  }
}
