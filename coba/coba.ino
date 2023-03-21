const int LEDPIN = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(LEDPIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  // high itu nyala, kalo low itu mati
  digitalWrite(LEDPIN, HIGH);
  delay(250);
  digitalWrite(LEDPIN, LOW);
  delay(500);
}
