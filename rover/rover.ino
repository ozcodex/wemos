#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>

WiFiServer server(80);
const int MA0 = D5;
const int MA1 = D6;
const int MB0 = D7;
const int MB1 = D8;

unsigned long last_use_millis= 0;

void setup() {
  //Initialize Server
  WiFi.mode(WIFI_STA);
  WiFi.setAutoReconnect(true);
  WiFi.begin("MichosMalucos", "delunoalnueve");
  while (WiFi.status() != WL_CONNECTED){
  blinky(250);
  }
  server.begin();
   
  //Initialize rover
  pinMode(MA0, OUTPUT);
  pinMode(MA1, OUTPUT);
  pinMode(MB0, OUTPUT);
  pinMode(MB1, OUTPUT);

  //Setup auto off mode
  last_use_millis = millis();

  //Setup debug
  pinMode(LED_BUILTIN, OUTPUT);
  blinky(1000);

  //start the rover stopped
  brake();

}

//Individual motor movement

void motor_a_forward(){
  digitalWrite(MA0, HIGH);
  digitalWrite(MA1, LOW);  
}

void motor_b_forward(){
  digitalWrite(MB0, HIGH);
  digitalWrite(MB1, LOW);  
}

void motor_a_backward(){
  digitalWrite(MA0, LOW);
  digitalWrite(MA1, HIGH);  
}

void motor_b_backward(){
  digitalWrite(MB0, LOW);
  digitalWrite(MB1, HIGH);  
}

void motor_a_stop(){
  digitalWrite(MA0, LOW);
  digitalWrite(MA1, LOW);  
}

void motor_b_stop(){
  digitalWrite(MB0, LOW);
  digitalWrite(MB1, LOW);  
}

// Rover movement

void brake(){
  motor_a_stop();
  motor_b_stop();
}

void forward(){
  motor_a_forward();
  motor_b_forward();
}

void backward(){
  motor_a_backward();
  motor_b_backward();
}

void right(){
  motor_a_forward();
  motor_b_backward();
}

void left(){
  motor_a_backward();
  motor_b_forward();
}


void blinky(int duration){ //in milliseconds
    digitalWrite(LED_BUILTIN, LOW); //turn on
    delay(duration);
    digitalWrite(LED_BUILTIN, HIGH); //turn off
    delay(duration);
  }

void loop() {

  //if more than 1 minute whithout client deep sleep
  if( ( millis() - last_use_millis ) > 60000 ) {
    blinky(200);
    blinky(200);
    blinky(200);
    ESP.deepSleep(0);
  }
 
  //Check & wait for client
  WiFiClient client = server.available();
  if (!client) return;
  while(!client.available()) delay(1);

  // Read Request
  String request = client.readStringUntil('\r');
  client.flush();

  if (request.indexOf("/action=forward") != -1) {
    forward();
  } else if (request.indexOf("/action=backward") != -1) {
    backward();
  } else if (request.indexOf("/action=right") != -1) {
    right();
  } else if (request.indexOf("/action=left") != -1) {
    left();
  } else if (request.indexOf("/action=stop") != -1) {
    brake();
  }

  // Send Response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<meta charset=\"UTF-8\"/>");
  
  client.println("<h1>");
  client.print("Oz Rover");
  client.println("<br>");
  client.println("<a href=\"/action=forward\">foward</a><br>");
  client.println("<a href=\"/action=backward\">backward</a><br>");
  client.println("<a href=\"/action=right\">right</a><br>");
  client.println("<a href=\"/action=left\">left</a><br>");
  client.println("<a href=\"/action=stop\">stop</a><br>");
  client.println("</h1>");
  client.println("</html>");

  delay(10);

  //Auto turnoff measure
  last_use_millis = millis();
  
}
