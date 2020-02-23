void serialSetup(){
  Serial.begin(2000000);
  while (!Serial) continue;
}

String readSerStr(){
  int len = Serial.available();
  if (len){
    char btar[len];
    Serial.readBytes(btar, len);
    return String(btar);
  }else{
    return ""; 
  }
}

unsigned long last_read = 0;
int last_len = 0;
boolean checkSerial(){
  unsigned long this_read = micros();
  int this_len = Serial.available();
  unsigned long gap = this_read-last_read;
  if (this_read<last_read){
    last_len = this_len;
    last_read = this_read;
    return false;  
  }
  if (this_len == 0){
    last_len = this_len;
    last_read = this_read;
    return false;  
  }else if (this_len>last_len || this_len<last_len){
    last_len = this_len;
    last_read = this_read;
    return false;
  }else if (gap < 200){
    return false;  
  }else {
    return true;  
  }
}

void sendStatus(){  
  StaticJsonDocument<400> doc;
  doc["row"] = 5.38;
  doc["yaw"] = 4.38;
  doc["pitch"] = 2.38;
  doc["motor1"] = 34;
  doc["motor2"] = 23;
  doc["motor3"] = 45;
  doc["depth"] = 1.1;
  doc["motor4"] = 12;
  doc["motor5"] = 10;
  doc["motor6"] = 58;
  JsonArray errors = doc.createNestedArray("error");
  errors.add ("no error 1");
  errors.add ("no error 2");
  serializeJson(doc, Serial);
}

void readJson(){
  String input = readSerStr();
  StaticJsonDocument<200> json;
  err = deserializeJson(json, input);
  command = json["cmd"];
  arg0 = json["arg0"];
  arg1 = json["arg1"];
  arg2 = json["arg2"];
}
