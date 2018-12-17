#include <Servo.h>
//declaring servos and other values
Servo myservo; 
Servo myservo2;
Servo myservo3;

int magnetPIN = 8; 
bool magnetState = false;
bool moveMotors = false;
//declaring the array for the serial inputs from Visual Studio
int incomingCommands[3];

void setup() 
  {  
    //set baud rate
    Serial.begin(9600);
    //attaching the servo values to their respective pins
    myservo.attach(11, 500, 2500);
    myservo2.attach(10, 500, 2500);
    myservo3.attach(9, 500, 2500);
    //giving the servos and output
    pinMode(11, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(8, OUTPUT);
  }

void loop() 
{
  // put your main code here, to run repeatedly:
   //ensures values are greater than 0
  if(Serial.available() > 0) 
  {
    //this if statement is to make sure only values it considers are ones I send, so the array must start with the <, this prevents garabage values
    if(Serial.read() == '<'){
      readSerialBytes();
      //if array values are parsed, send the values to the servo motors
      if(moveMotors){
        Serial.println(incomingCommands[0]);
        Serial.println(incomingCommands[1]);
        Serial.println(incomingCommands[2]);
  
        myservo.write(incomingCommands[0]);
        //delay(1000);  
        myservo2.write(incomingCommands[1]);
        //delay(1000);  
        myservo3.write(incomingCommands[2]);              
        //delay(1000);    
      }   
    }
  } 
      
}

void readSerialBytes()
{
  //parsing values
  int parsedValue;
  int ArrayIndex = 0;
  char readByte;
  bool firstChar = true;
  do{
    if(Serial.available() > 0){
      readByte = Serial.read();
      //makes sure the values are integers using ASCII
      //this will only work if the values comes in in this form: <x y z>
      if(readByte > 47 && readByte < 58)
      {
        if(firstChar)
        {
          incomingCommands[ArrayIndex] = 0;
          firstChar = false;
        }
        //parsing values and setting bool value for when the servos need to be written to
        parsedValue = readByte - '0';
         
        incomingCommands[ArrayIndex] = incomingCommands[ArrayIndex]*10 + parsedValue;
        moveMotors = true;
      }
      //if there's a space, the value remains the same
      else if(readByte == ' ')
      {
        if(++ArrayIndex == 3)
        {
          readByte = '>';
        }
        //resets firstChar to restart the process
        firstChar = true;
      }
      //this turns the magnet on and off when <M> is recieved
      else if(readByte == 'M')
      {
        magnetState = !magnetState;
        Serial.println(magnetState);
        digitalWrite(magnetPIN, magnetState);
        readByte = '>';
        moveMotors = false;
      }
    } 
  }while(readByte != '>');
}
  
    
    
    
