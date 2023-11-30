#include <WiFi.h>
#include <WiFiUdp.h>
// #include <Arduino_FreeRTOS.h>
// #include <semphr.h>

#if CONFIG_FREERTOS_UNICORE
#define ARDUINO_RUNNING_CORE 0
#else
#define ARDUINO_RUNNING_CORE 1
#endif

const char* ssid = "Studenci-esp32";
const char* password = "studenci-esp32";

const char * udpAddress = "192.168.162.208";
const int udpPort = 3333;

WiFiUDP udp;
WiFiServer server(80);

void TaskUdpClient( void *pvParameters);
void TaskWebServer( void *pvParameters);
void connectToWifi() ;

void setup() {
  Serial.begin(115200);

  while(!Serial) {
    delay(10);
  }

  connectToWifi();

  xTaskCreatePinnedToCore(
    TaskWebServer
    ,  "WebServer"  // A name just for humans
    ,  4096  // This stack size can be checked & adjusted by reading the Stack Highwater
    ,  NULL //Parameters for the task
    ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
    ,  NULL
    , ARDUINO_RUNNING_CORE); //Task Handle

  // xTaskCreatePinnedToCore(
  //   TaskUdpClient
  //   ,  "UdpClient" // A name just for humans
  //   ,  4096  // Stack size
  //   ,  NULL //Parameters for the task
  //   ,  2  // Priority
  //   ,  NULL 
  //   , ARDUINO_RUNNING_CORE); //Task Handle

}

void loop() {
  // put your main code here, to run repeatedly:

}

void connectToWifi() {
  Serial.println("\n\nConnecting to WiFi:");
  Serial.println("SSID: ");
  Serial.print(ssid);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());  

  return;
}

void TaskWebServer( void *pvParameters) {
  Serial.println("\nStarting web server");

  while(1) {
    WiFiClient client = server.available();   // listen for incoming clients

      if (client) {                             // if you get a client,
        Serial.println("New Client.");           // print a message out the serial port
        String currentLine = "";                // make a String to hold incoming data from the client
        while (client.connected()) {            // loop while the client's connected
          if (client.available()) {             // if there's bytes to read from the client,
            char c = client.read();             // read a byte, then
            Serial.write(c);                    // print it out the serial monitor
            if (c == '\n') {                    // if the byte is a newline character

              // if the current line is blank, you got two newline characters in a row.
              // that's the end of the client HTTP request, so send a response:
              if (currentLine.length() == 0) {
                // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                // and a content-type so the client knows what's coming, then a blank line:
                client.println("HTTP/1.1 200 OK");
                client.println("Content-type:text/html");
                client.println();

                // the content of the HTTP response follows the header:
                client.print("Click <a href=\"/H\">here</a> to turn the LED on pin 5 on.<br>");
                client.print("Click <a href=\"/L\">here</a> to turn the LED on pin 5 off.<br>");

                // The HTTP response ends with another blank line:
                client.println();
                // break out of the while loop:
                break;
              } else {    // if you got a newline, then clear currentLine:
                currentLine = "";
              }
            } else if (c != '\r') {  // if you got anything else but a carriage return character,
              currentLine += c;      // add it to the end of the currentLine
            }

            // Check to see if the client request was "GET /H" or "GET /L":
            if (currentLine.endsWith("GET /H")) {
              digitalWrite(5, HIGH);               // GET /H turns the LED on
            }
            if (currentLine.endsWith("GET /L")) {
              digitalWrite(5, LOW);                // GET /L turns the LED off
            }
          }
        }
        // close the connection:
        client.stop();
        Serial.println("Client Disconnected.");
      }
  }
}

void TaskUdpClient( void *pvParameters) {
  while(1){

    //Send a packet
    udp.beginPacket(udpAddress,udpPort);
    udp.printf("Seconds since boot: %lu", millis()/1000);
    udp.endPacket();

    //Wait for 1 second
    delay(1000);
  }
}
