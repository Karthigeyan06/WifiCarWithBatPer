#define BLYNK_TEMPLATE_ID "TMPL34mVzjKAZ"
#define BLYNK_TEMPLATE_NAME "espcar"
#define BLYNK_AUTH_TOKEN "BBpa0fzqD1oN_Dja6fIDf4Qhe4J68pZh"

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


#define ENA D0
#define IN1 D1
#define IN2 D2
#define IN3 D3
#define IN4 D4
#define ENB D5

#define batteryPin A0

bool forward = 0;
bool backward = 0;
bool left = 0;
bool right = 0;
int Speed = 1023; 
char auth[] = "BBpa0fzqD1oN_Dja6fIDf4Qhe4J68pZh";
char ssid[] = "Karthig";
char pass[] = "gkn__f14";

const float maxBatteryVoltage = 7.4; 
const float minBatteryVoltage = 5.0;
const float regulatorVoltage = 3.3; 

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(batteryPin, INPUT);

  Blynk.begin(auth, ssid, pass);
}

BLYNK_WRITE(V0) {
  forward = param.asInt();
}

BLYNK_WRITE(V1) {
  backward = param.asInt();
}

BLYNK_WRITE(V2) {
  left = param.asInt();
}

BLYNK_WRITE(V3) {
  right = param.asInt();
}


void smartcar() {
  if (forward == 1) {
    carforward();
    Serial.println("carforward");
  } else if (backward == 1) {
    carbackward();
    Serial.println("carbackward");
  } else if (left == 1) {
    carturnleft();
    Serial.println("carleft");
  } else if (right == 1) {
    carturnright();
    Serial.println("carright");
  } else if (forward == 0 && backward == 0 && left == 0 && right == 0) {
    carStop();
    Serial.println("carstop");
  }
}

void loop() {
  Blynk.run();
  smartcar();
  checkBattery();
}

void carforward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carbackward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carturnleft() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carturnright() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void checkBattery() {
  int sensorValue = analogRead(batteryPin);
  float inputVoltage = (sensorValue / 1023.0) * regulatorVoltage;
  float batteryVoltage = inputVoltage * (maxBatteryVoltage / regulatorVoltage);
  float batteryPercentage = ((batteryVoltage - minBatteryVoltage) / (maxBatteryVoltage - minBatteryVoltage)) * 100;
  batteryPercentage = constrain(batteryPercentage, 0, 100); 

  Blynk.virtualWrite(V4, batteryPercentage);

  if (batteryPercentage < 20) { 
    Blynk.logEvent("lowbat", "Warning: Low Battery! Please charge your car.");
  }

  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage);
  Serial.print("V, Battery Percentage: ");
  Serial.print(batteryPercentage);
  Serial.println("%");
}
