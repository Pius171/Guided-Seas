#include <Arduino.h>
#include <Notecard.h>

#define DEBUG 0

#if DEBUG == 0
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugln2Param(x, y) Serial.println(x, y)

#else
#define debug(x)
#define debugln(x)
#endif

// #define usbSerial Serial
#define productUID "com.gmail.onyemandukwu:guided_seas"
#define ATTN_PIN 6
#define FORNT_MOTOR 9
#define BACK_MOTOR 10
#define LEFT_MOTOR 11
#define RIGHT_MOTOR 12
Notecard notecard;

J *req = nullptr;

void setup() {
  // put your setup code here, to run once:
  notecard.begin();
  notecard.setDebugOutputStream(Serial);
  pinMode(LED_BUILTIN,OUTPUT);
  for(int i=9; i<=12; i++){
    pinMode(i,OUTPUT); // set all motor pins as output
  }
  pinMode(ATTN_PIN,INPUT_PULLDOWN);

  req = notecard.newRequest("hub.set");
  JAddStringToObject(req, "product", productUID);
  JAddStringToObject(req, "mode", "continuous");
  JAddNumberToObject(req, "inbound", 1);
 JAddBoolToObject(req, "sync", true);
  notecard.sendRequest(req);

  delay(500);

// first disarm
req = NoteNewRequest("card.attn");
JAddStringToObject(req, "mode", "disarm,-all");
NoteRequest(req);

// then arm
J *req = NoteNewRequest("card.attn");
JAddStringToObject(req, "mode", "arm, files");

J *files = JAddArrayToObject(req, "files");
JAddItemToArray(files, JCreateString("dir.dbs"));

NoteRequest(req);

digitalWrite(LED_BUILTIN,1);
delay(2000);
digitalWrite(LED_BUILTIN,0);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN,digitalRead(ATTN_PIN));

  
  if(digitalRead(ATTN_PIN)==1){
    // note has changed, get the note
     req = notecard.newRequest("note.get");
  if (req != NULL)
  {
    JAddStringToObject(req, "file", "dir.dbs");
    JAddBoolToObject(req, "sync", true);
    J *rsp = notecard.requestAndResponse(req);
    debugln(JConvertToJSONString(rsp));
    J *body = JGetObject(rsp, "body");
    char *direction = JGetString(body, "direction");
    debugln(direction);
    char *speed = JGetString(body, "speed");
    debugln(speed);

 

    if(strcmp(direction,"forward") == 0){
      analogWrite(FORNT_MOTOR,atoi(speed));
      debugln("running front motor");
      delay(2000);
      analogWrite(FORNT_MOTOR,0); // turn of motor
      //arm ATTN pin
      
  //     req = NoteNewRequest("card.attn");
  // JAddStringToObject(req, "mode", "arm");
  // NoteRequest(req);
    }

        else if(strcmp(direction,"back") == 0){
      analogWrite(BACK_MOTOR,atoi(speed));
      debugln("running back motor");
      delay(2000);
      analogWrite(BACK_MOTOR,0); // turn of motor
            //arm ATTN pin
  //     req = NoteNewRequest("card.attn");
  // JAddStringToObject(req, "mode", "arm");
  // NoteRequest(req);
    }

//  else if(direction=="left"){
    else if(strcmp(direction,"left") == 0){
      analogWrite(LEFT_MOTOR,atoi(speed));
      debugln("running left motor");
      delay(2000);
       analogWrite(LEFT_MOTOR,0); // turn of motor
            //arm ATTN pin
  //     req = NoteNewRequest("card.attn");
  // JAddStringToObject(req, "mode", "arm");
  // NoteRequest(req);
    }

    else if(strcmp(direction,"right") == 0){
      analogWrite(RIGHT_MOTOR,atoi(speed));
      // debugln(atoi(speed));
      debugln("running right motor");
      delay(2000);
       analogWrite(RIGHT_MOTOR,0); // turn of motor
            //arm ATTN pin
  //     req = NoteNewRequest("card.attn");
  // JAddStringToObject(req, "mode", "arm");
  // NoteRequest(req);
    }
   //clean classroom
      //arm ATTN pin
      req = NoteNewRequest("card.attn");
  JAddStringToObject(req, "mode", "arm");
  //debugln("led should be on here");
  NoteRequest(req);
  }
 }
}

