//blink blink sketch
#define LED 0
void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
}
 
void loop() {
  digitalWrite(0, HIGH);
  delay(50);
  digitalWrite(0, LOW);
  delay(600);
}
