#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySerial(2, 3); // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address

const int buzzerPin = 7;


struct Person {
  int id; 
  String name;
  String address;
  String nic;
};


Person people[] = {
  {1, "Isuru Darshana", "31/2 Isuru Hathamuna Pilawala", "20003500387"},
  {2, "Chandu Shaviska", "193/1 Pahalagama Thebuwana", "200111302664"},
  {3, "Ashan Bandara ", "196/1 Udagunnepana Gunnepana", "200033600160"},
  {4, "Arunalu Rathnathunga", "214 Mulana Narawala Poddala", "200116902899"},
   {7, ""}};
const int peopleCount = sizeof(people) / sizeof(Person);

void setup() {
  Serial.begin(9600);
  mySerial.begin(57600);

  pinMode(buzzerPin, OUTPUT);

  lcd.init();
  lcd.backlight();
  displayWelcomeMessage();
}

void loop() {
  int fingerprintID = getFingerprint();
  
  if (fingerprintID != -1) {
    
    for (int i = 0; i < peopleCount; i++) {
      if (people[i].id == fingerprintID) {
        displayPersonInfo(people[i]);
        logData(people[i]);
        break; 
      }
    }
  } else {
    displayInvalidFingerprintMessage();
  }

  delay(2000); // Delay before next read
}

void displayWelcomeMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Scan Your");
  lcd.setCursor(0, 1);
  lcd.print("Fingerprint");
}

void displayPersonInfo(const Person &person) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Verification");
  lcd.setCursor(0, 1);
  lcd.print("Successful!");
  digitalWrite(buzzerPin, HIGH);
  delay(1000); // Sound buzzer for 1 second
  digitalWrite(buzzerPin, LOW);
}

void logData(const Person &person) {
  
  String dataToLog = String(person.id) + "," + person.name + "," + person.address + "," + person.nic;
  
  // Log data to the serial
  Serial.println(dataToLog);
}

void displayInvalidFingerprintMessage() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Invalid Voter");
  delay(3000); 
  displayWelcomeMessage();
}

int getFingerprint() {
  int p = finger.getImage();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK) return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) return -1;

  return finger.fingerID; 
}
