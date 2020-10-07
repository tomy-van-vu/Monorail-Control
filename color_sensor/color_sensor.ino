
//will need to use male to female wires for s1 and s0 

#define S0 4
#define S1 5
#define S2 6
#define S3 7
#define sensorOut 8

int redfrequency = 0;
int greenfrequency = 0;
int bluefrequency = 0;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);
  
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  Serial.begin(9600);
}

void loop() {
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
   redfrequency = pulseIn(sensorOut, LOW);
   Serial.print("Red: ");
   Serial.print(redfrequency);
   delay(100);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  greenfrequency = pulseIn(sensorOut, LOW);
  // Printing the value on the serial monitor
     Serial.print(" Green: ");
   Serial.print(redfrequency);
  delay(100);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  bluefrequency = pulseIn(sensorOut, LOW);
     Serial.print(" Blue: ");
   Serial.print(redfrequency);
  delay(100);


 if (redfrequency>25 && redfrequency< 77) { //values need to be determined
Serial.println(" RED");
} 
else if (bluefrequency>25 && bluefrequency< 77) { //values need to be determined
Serial.println(" BLUE");
} 
else if (greenfrequency>25 && greenfrequency< 77){ //values need to be determined
Serial.println(" GREEN");
}
 else {
Serial.println(" NO COLOUR DETECTION");
}

}
