float d = 0.0;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.printf("%f %f %d\n",sin(d -2),cos(d -2),0);
  d = d + 0.2;
  delay(100);
}