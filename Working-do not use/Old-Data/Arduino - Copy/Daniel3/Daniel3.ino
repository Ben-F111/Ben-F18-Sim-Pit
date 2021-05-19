

int led1 = 13;
int led2 = 12;
int led3 = 11;
int led4 = 10;
int led5 = 9;
int led6 = 8;
int led7 = 7;
int led8 = 6;
int led9 = 5;
int led10 = 4;
int led11 = 3;
int led12 = 2;
int led13 = 1;
void setup() {
   Serial.begin(9600);

   // Setup the 3 pins as OUTPUT
   pinMode(led1, OUTPUT);
   pinMode(led2, OUTPUT);
   pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
   pinMode(led5, OUTPUT);
    pinMode(led6, OUTPUT);
     pinMode(led7, OUTPUT);
      pinMode(led8, OUTPUT);
        pinMode(led9, OUTPUT);
        pinMode(led10, OUTPUT);
           pinMode(led11, OUTPUT);
                  pinMode(led12, OUTPUT);
                    pinMode(led13, OUTPUT);
}
void loop() {
  digitalWrite(led10, HIGH);
   digitalWrite(led11, HIGH); 
//    digitalWrite(led12, HIGH);
//   digitalWrite(led13, HIGH); 


//digitalWrite(led12, HIGH); // RED LAND
//digitalWrite(led13, LOW); // YELLOW LAND
//delay(100);
//digitalWrite(led12, LOW); // RED LAND
//digitalWrite(led13, HIGH); // YELLOW LAND

digitalWrite(led12, HIGH); // RED LAND
digitalWrite(led13, LOW); // YELLOW LAND
delay(100);
digitalWrite(led12, LOW); // RED LAND
digitalWrite(led13, HIGH); // YELLOW LAND

digitalWrite(led1, HIGH);
delay(100);
digitalWrite(led2, HIGH);
delay(100);
digitalWrite(led1, LOW);  

digitalWrite(led2, HIGH);
//digitalWrite(led9, HIGH); //RED FLASH ON
delay(100);
digitalWrite(led3, HIGH);
delay(100);
//digitalWrite(led9, LOW); //RED FLASH OFF
digitalWrite(led2, LOW);

digitalWrite(led3, HIGH);
delay(100);
digitalWrite(led4, HIGH);
delay(100);
digitalWrite(led3, LOW);
   
digitalWrite(led4, HIGH);
delay(100);
digitalWrite(led5, HIGH);
delay(100);
digitalWrite(led4, LOW);

digitalWrite(led5, HIGH);
delay(100);
digitalWrite(led6, HIGH);
delay(100);
digitalWrite(led5, LOW);

digitalWrite(led6, HIGH);
//digitalWrite(led9, HIGH);//RED FLASH ON
delay(100);
;
digitalWrite(led7, HIGH);
delay(100);
//digitalWrite(led9, LOW);// RED FLASH OFF
digitalWrite(led6, LOW);

digitalWrite(led7, HIGH);
delay(100);
digitalWrite(led8, HIGH);
delay(100);
digitalWrite(led7, LOW);

digitalWrite(led8, HIGH);
delay(100);
digitalWrite(led1, HIGH);
delay(100);
digitalWrite(led8, LOW);






}
