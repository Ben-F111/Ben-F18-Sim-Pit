

int led1 = 13;
int led2 = 12;
int led3 = 11;
int led4 = 10;


void setup() {
   Serial.begin(9600);

   // Setup the 3 pins as OUTPUT
   pinMode(led1, OUTPUT);
   pinMode(led2, OUTPUT);
   pinMode(led3, OUTPUT);
pinMode(led4, OUTPUT);
}
void loop() {
digitalWrite(led1, HIGH);
delay(250);
digitalWrite(led1, LOW);
   
digitalWrite(led2, HIGH);
delay(250);
digitalWrite(led2, LOW);

digitalWrite(led3, HIGH);
delay(250);
digitalWrite(led3, LOW);
   
digitalWrite(led4, HIGH);
delay(250);
digitalWrite(led4, LOW);
}
