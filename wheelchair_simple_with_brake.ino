//Atmega328p based Arduino code (should work withouth modifications with Atmega168/88), tested on RBBB Arduino clone by Modern Device:
const byte joysticYA = A0; //Analog Jostick Y axis
const byte joysticXA = A1; //Analog Jostick X axis
#define BREAK_DELAY 500

//const byte controllerFA = 10; //PWM FORWARD PIN for OSMC Controller A (left motor)
//const byte controllerRA = 9;  //PWM REVERSE PIN for OSMC Controller A (left motor)
//const byte controllerFB = 6;  //PWM FORWARD PIN for OSMC Controller B (right motor)
//const byte controllerRB = 5;  //PWM REVERSE PIN for OSMC Controller B (right motor)
//const byte disablePin = 2; //OSMC disable, pull LOW to enable motor controller

 #include <SPI.h>
  const int cs1= 10;
  const int cs2= 8;
  int sensorValue = 0;
  int outputValue = 0;
  byte outputValueByte0 = 0;
  byte outputValueByte1 = 0;
  int o1=0;
  const int r = 11;
  const int b=13;

    int breakLeft = 14;                     
  int breakRight = 15;
  int new_breakLeft = 40;
  int new_breakRight = 41;
  int reverseLeft_A = 44;
  int reverseRight_A = 45;  
  int c=0;

int analogTmp = 0; //temporary variable to store 
int throttle, direction = 0; //throttle (Y axis) and direction (X axis) 

int leftMotor,leftMotorScaled = 0; //left Motor helper variables
float leftMotorScale = 0;

int rightMotor,rightMotorScaled = 0; //right Motor helper variables
float rightMotorScale = 0;

float maxMotorScale = 0; //holds the mixed output scaling factor

int deadZone = 20; //jostick dead zone 

void setup()  { 

  //initialization of pins  
  //Serial.begin(19200);
  //pinMode(controllerFA, OUTPUT);
  //pinMode(controllerRA, OUTPUT);
  //pinMode(controllerFB, OUTPUT);
  //pinMode(controllerRB, OUTPUT);


   pinMode(new_breakLeft,OUTPUT);
  pinMode(new_breakRight,OUTPUT);
   pinMode(breakLeft,OUTPUT);
  pinMode(breakRight,OUTPUT);
  pinMode(reverseLeft_A,OUTPUT);
  pinMode(reverseRight_A,OUTPUT);


//  pinMode(disablePin, OUTPUT);
  //digitalWrite(disablePin, LOW);
  //spi initializations
     SPI.begin();
     SPI.setBitOrder(MSBFIRST);
     SPI.setClockDivider(SPI_CLOCK_DIV4);
     pinMode(cs1, OUTPUT);
      pinMode(cs2, OUTPUT);
     digitalWrite(cs1, HIGH);
     digitalWrite(cs2, HIGH);
     pinMode(r, OUTPUT);
     digitalWrite(r, LOW);
      pinMode(b, OUTPUT);
     digitalWrite(b, LOW);
} 
void breakEnable()
{ 
     throttle1(0);
    throttle2(0);
  
  //delay(BREAK_DELAY);
  
  
  digitalWrite(breakLeft,LOW);     //stop     Power Break
  digitalWrite(breakRight,LOW);  
}
void breaklock()
{
  throttle1(0);
    throttle2(0);
    
  digitalWrite(new_breakLeft,HIGH);     //stop     Power Break
  digitalWrite(new_breakRight,HIGH); 
}
void breakDisable()
{
  digitalWrite(new_breakLeft, LOW);   //No break                    Motor break
  digitalWrite(new_breakRight, LOW);
  digitalWrite(breakLeft,HIGH);      // No break because of spot          Power break  
  digitalWrite(breakRight,HIGH);     //No break because of spot
}

void reverse_rightMotor(int dir)
{
  //Don't reverse the motor
  if(dir==1)
  {
    digitalWrite(reverseRight_A, LOW);
   // digitalWrite(reverseRight_B, HIGH);
  }
  //reverse the motor
  else if(dir==0)
  {
    digitalWrite(reverseRight_A, HIGH);
   // digitalWrite(reverseRight_B, LOW);
  } 
}

void reverse_leftMotor(int dir)
{
  //Don't reverse the motor
  if(dir==1)
  {
    digitalWrite(reverseLeft_A, LOW);
    //digitalWrite(reverseLeft_B, HIGH);
  }
  //reverse the motor
  else if(dir==0)
  {
    digitalWrite(reverseLeft_A, HIGH);
    //digitalWrite(reverseLeft_B, LOW);
  }
}

void loop()  { 
  //aquire the analog input for Y  and rescale the 0..1023 range to -255..255 range
  analogTmp = analogRead(joysticYA);
  throttle = (506-analogTmp)/2;

  //delayMicroseconds(100);
  //...and  the same for X axis
  analogTmp = analogRead(joysticXA);
  direction = -(479-analogTmp)/2;

  //mix throttle and direction
  leftMotor = throttle+direction;
  rightMotor = throttle-direction;

  //print the initial mix results
  //Serial.print("LIN:"); Serial.print( leftMotor, DEC);
 // Serial.print(", RIN:"); Serial.print( rightMotor, DEC);

  //calculate the scale of the results in comparision base 8 bit PWM resolution
  leftMotorScale =  leftMotor/255.0;
  leftMotorScale = abs(leftMotorScale);
  rightMotorScale =  rightMotor/255.0;
  rightMotorScale = abs(rightMotorScale);

//  Serial.print("| LSCALE:"); Serial.print( leftMotorScale,2);
  //Serial.print(", RSCALE:"); Serial.print( rightMotorScale,2);

  //choose the max scale value if it is above 1
  maxMotorScale = max(leftMotorScale,rightMotorScale);
  maxMotorScale = max(1,maxMotorScale);

  //and apply it to the mixed values
  leftMotorScaled = constrain(leftMotor/maxMotorScale,-90,90);
  rightMotorScaled = constrain(rightMotor/maxMotorScale,-90,90);

  //Serial.print("| LOUT:"); Serial.print( leftMotorScaled);
  //Serial.print(", ROUT:"); Serial.print( rightMotorScaled);

  //Serial.print(" |");

  //apply the results to appropriate uC PWM outputs for the LEFT motor:
  if(abs(leftMotorScaled)>deadZone)
  {
    c=0;
    if (leftMotorScaled > 0)
    {
    //  Serial.print("F");
      
      //Serial.print(abs(leftMotorScaled),DEC);
      
      reverse_leftMotor(0);
      breakDisable(); 
      //analogWrite(controllerRA,0);
      //analogWrite(controllerFA,abs(leftMotorScaled)); 
      throttle1(abs(leftMotorScaled));         
    }
    else 
    {
      //Serial.print("R");
     // Serial.print(abs(leftMotorScaled),DEC);
      reverse_leftMotor(1); 
       breakDisable(); 
      throttle1(abs(leftMotorScaled));         

     // analogWrite(controllerFA,0);
      //analogWrite(controllerRA,abs(leftMotorScaled));  
    }
  }  
  else 
  {
  //Serial.print("IDLE");
   throttle1(0);
  breakEnable();
   reverse_leftMotor(0);
    c++;
    if(c>10)
    {
        breaklock();
    }
  
  //analogWrite(controllerFA,0);
 // analogWrite(controllerRA,0);
  } 

  //apply the results to appropriate uC PWM outputs for the RIGHT motor:  
  if(abs(rightMotorScaled)>deadZone)
  {
    c=0;
    if (rightMotorScaled > 0)
    {
    //  Serial.print("F");
      //Serial.print(abs(rightMotorScaled),DEC);
      reverse_rightMotor(0);
      breakDisable();
      //analogWrite(controllerRB,0);
      //analogWrite(controllerFB,abs(rightMotorScaled));     
      throttle2(abs(rightMotorScaled));     
    }
    else 
    {
      //Serial.print("R");
      //Serial.print(abs(rightMotorScaled),DEC);
      reverse_rightMotor(1);
      breakDisable();
     // analogWrite(controllerFB,0);
      //analogWrite(controllerRB,abs(rightMotorScaled));  
      throttle2(abs(rightMotorScaled));    
    }
  }  
  else 
  {
  ///Serial.print("IDLE");
   throttle2(0);
  breakEnable();
   reverse_rightMotor(0);
  //analogWrite(controllerFB,0);
  //analogWrite(controllerRB,0);
      c++;
      if(c>10)
    {
        breaklock();
    }
  } 

  //Serial.println("");

  //To do: throttle change limiting, to avoid radical changes of direction for large DC motors

//  delay(10);

}


void throttle1(int s)
{
  outputValue = map(s, 0, 255, 0, 4095);
   //Serial.print(outputValue);
   //Serial.print("\t");
    outputValueByte0 = byte(outputValue);
    outputValue = outputValue >> 8;
    outputValueByte1 = byte(outputValue | 0b00110000);
     digitalWrite(cs1, LOW);
     SPI.transfer(outputValueByte1);
     SPI.transfer(outputValueByte0);
     digitalWrite(cs1, HIGH);
     delay(10);
    
}
void throttle2(int s)
{
  outputValue = map(s, 0, 255, 0, 4095);
   //Serial.println(outputValue);
    outputValueByte0 = byte(outputValue);
    outputValue = outputValue >> 8;
    outputValueByte1 = byte(outputValue | 0b00110000);
     digitalWrite(cs2, LOW);
     SPI.transfer(outputValueByte1);
     SPI.transfer(outputValueByte0);
     digitalWrite(cs2, HIGH);
     delay(10);
    
}
