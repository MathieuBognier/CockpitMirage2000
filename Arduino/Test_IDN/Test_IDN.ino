
const int PinD =  13;
const int PinE =  12;
const int PinA =  11;
const int PinC =  10;
const int PinB =  9;
const int PinF =  8;

void setup() {
  // put your setup code here, to run once:
  pinMode(PinD, OUTPUT);
  pinMode(PinE, OUTPUT);
  pinMode(PinA, OUTPUT);
  pinMode(PinC, OUTPUT);
  pinMode(PinB, OUTPUT);
  pinMode(PinF, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PinD, LOW);
  digitalWrite(PinE, HIGH);
  delay (1000);
  digitalWrite(PinF, LOW);
  digitalWrite(PinD, HIGH);
  delay (1000);
  digitalWrite(PinD, LOW);
  digitalWrite(PinA, HIGH);
  delay (1000);
  digitalWrite(PinC, LOW);
  digitalWrite(PinD, HIGH);
  delay (1000);
  digitalWrite(PinD, LOW);
  digitalWrite(PinB, HIGH);
  delay (1000);
  digitalWrite(PinE, LOW);
  digitalWrite(PinD, HIGH);
  delay (1000);
  digitalWrite(PinD, LOW);
  digitalWrite(PinF, HIGH);
  delay (1000);
  digitalWrite(PinA, LOW);
  digitalWrite(PinD, HIGH);
  delay (1000);
  digitalWrite(PinD, LOW);
  digitalWrite(PinC, HIGH);
  delay (1000);
  digitalWrite(PinB, LOW);
  digitalWrite(PinD, HIGH);
  delay (1000);

}
