
//Defining pins
#define COLOUROUT A5
#define COLOURS2 A4
#define COLOURS3 A3

//Colour variables 
int frequency = 0;
int color = 0;


void setup() {
 pinMode(COLOUROUT,INPUT);
 pinMode(COLOURS2,OUTPUT);
 pinMode(COLOURS3,OUTPUT);
 Serial.begin(9600);
}

void loop() {  
  color = readColor();
  delay(200);
  Serial.print(" Color=");
  Serial.print(color);
  Serial.print(" ");
  delay(200);
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
  delay(200);

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
  delay(200);
  
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
  delay(200);


  //need to play around with values
  if(R>5 & R<20 & G>21 & G< 65 & B>20 & B<50) {
    color = 1; // Red
  }

  else if(R>7 & R<40 & G>11 & G<20 & B>7 & B<14){
    color = 2;  //Blue
  }

   else if(R>20 & R<33 & G>11 & G<24 & B>28 & B<45){
    color = 3;  //Green
  }

   else if(R>2 & R<8 & G>5 & G<10 & B>9 & B<16){
    color = 4;  //Yellow
  }
  
  else {
   color = 0;  //No colour detected 
  }
  
  return color;  
}
