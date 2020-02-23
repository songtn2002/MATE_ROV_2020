#include <ArduinoJson.h>

char* command;
int arg0;
double arg1;
double arg2;

DeserializationError err;

//char input[] = "{"cmd":"motor accelerate","arg0":1,"arg1":4.2, "arg2":0.0}";


void setup() {
  serialSetup();//setup the serial port
  pinMode(LED_BUILTIN, OUTPUT);
}

/*
void blnk() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(200);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
}
*/

unsigned long timer_start = 0;
boolean timed(int gap) {//timer function
  unsigned long this_read = millis();
  if (this_read - timer_start >= gap) {
    timer_start = this_read;
    return true;
  } else {
    return false;
  }
}

String output = "";
void loop() {
  if (checkSerial()) { //if there is infomation available at the usb port
    readJson();
    //read the info received into the command, arg0, arg1 and arg2 variables
    if (err) {
      output = err.c_str();
      //if there is an error while parsing the json file, store it in output and prepare it for sending
    } else {
      output = "command received: " + String(command);
      //if there is no error, reply to the computer that the command is received
    }
  }
  if (timed(66)) { // for every 66 milliseconds do this
    if (output != "") {
      Serial.print(output);
      //if there is a response to a command or an error, send it
      output = "";
    } else {
      sendStatus();
      //otherwise send the status of the robot
    }
  }
}
