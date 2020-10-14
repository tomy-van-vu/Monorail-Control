#define COLOUROUT A5
#define COLOURS2 A4
#define COLOURS3 A3

int frequency = 0;
int color;


void setup() {
 pinMode(COLOUROUT,INPUT);
 pinMode(COLOURS2,OUTPUT);
 pinMode(COLOURS3,OUTPUT);
 Serial.begin(9600);
}

void loop() {
  /*
  //Calibration Code
  // Setting red filtered photodiodes to be read
  digitalWrite(COLOURS2, LOW);
  digitalWrite(COLOURS3, LOW);
  // Reading the output frequency
  frequency = pulseIn(COLOUROUT, LOW);
  int R = frequency;
  // Printing the value on the serial monitor
  Serial.print("R= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(100);

    // Setting Green filtered photodiodes to be read
  digitalWrite(COLOURS2, HIGH);
  digitalWrite(COLOURS3, HIGH);
  // Reading the output frequency
  frequency = pulseIn(COLOUROUT, LOW);
  int G = frequency;
  // Printing the value on the serial monitor
  Serial.print("G= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(100);
  
  // Setting Blue filtered photodiodes to be read
  digitalWrite(COLOURS2, LOW);
  digitalWrite(COLOURS3, HIGH);
  // Reading the output frequency
  frequency = pulseIn(COLOUROUT, LOW);
  int B = frequency;
  // Printing the value on the serial monitor
  Serial.print("B= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.println("  ");
  delay(100);
*/
  
  
  color = readColor();
  delay(10);
  
  Serial.print(" Color=");
  Serial.print(color);
  Serial.print(" ");
  delay(50);
}


int readColor() {
  // Setting red filtered photodiodes to be read
  digitalWrite(COLOURS2, LOW);
  digitalWrite(COLOURS3, LOW);
  // Reading the output frequency
  frequency = pulseIn(COLOUROUT, LOW);
  int R = frequency;
  // Printing the value on the serial monitor
  Serial.print("R= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(100);

    // Setting Green filtered photodiodes to be read
  digitalWrite(COLOURS2, HIGH);
  digitalWrite(COLOURS3, HIGH);
  // Reading the output frequency
  frequency = pulseIn(COLOUROUT, LOW);
  int G = frequency;
  // Printing the value on the serial monitor
  Serial.print("G= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(100);
  
  // Setting Blue filtered photodiodes to be read
  digitalWrite(COLOURS2, LOW);
  digitalWrite(COLOURS3, HIGH);
  // Reading the output frequency
  frequency = pulseIn(COLOUROUT, LOW);
  int B = frequency;
  // Printing the value on the serial monitor
  Serial.print("B= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.println("  ");
  delay(100);

  //still need to play around with values
  if(R<19 & R>6 & G<18 & G>6) {
    color = 1; // Red
  }
  if(G<56 & G>9 & B<58 & B>17){
    color = 2; // Green
  }
  if(R<95 & R>35 & G == 35 & B<75 & B>22){
    color = 3; // Blue
  }
  if(R<11 & R>4 & G<12 & G>6 & B<20 & B>12){
    color = 4; // Yellow
  }
    color = 0; 

  return color;  
  
}

