#include <WiFi.h>

const char* ssid     = "Arun_2.4Ghz"; // Change this to your WiFi SSID
const char* password = "14k81ao361"; // Change this to your WiFi password

WiFiServer server(80);

void setup() {
    Serial.begin(115200);
    pinMode(2, OUTPUT);      // set the LED pin mode

    delay(10);

    WiFi.setSleep(false);

    // We start by connecting to a WiFi network
    Serial.println();
    Serial.println("Connecting to WiFi...");
    
    Serial.print("SSID: ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    server.begin();
}

void loop() {
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
                        client.println("<!DOCTYPE html><html><head><title>ESP32 LED Control</title>");
                        client.println("<style>");
                        client.println("body { font-family: Arial, sans-serif; margin: 0; padding: 0; text-align: center; }");
                        client.println(".button { display: inline-block; background-color: #4CAF50; color: white; padding: 16px 20px; margin: 10px 2px; border: none; cursor: pointer; border-radius: 5px; text-decoration: none; font-size: 16px; }");
                        client.println(".button:hover { background-color: #45a049; }");
                        client.println("</style></head><body>");
                        client.println("<h1>ESP32 LED Control</h1>");
                        client.println("<p>Click the buttons below to control the LED:</p>");
                        client.println("<form action=\"/H\"><button class=\"button\">Turn LED ON</button></form>");
                        client.println("<form action=\"/L\"><button class=\"button\">Turn LED OFF</button></form>");
                        client.println("<form action=\"/T\"><button class=\"button\">Turn LED 2 MIN</button></form>");
                        client.println("<form action=\"/P\"><button class=\"button\">Turn LED 5 MIN</button></form>");
                        client.println("</body></html>");

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
                    digitalWrite(2, HIGH);               // GET /H turns the LED on
                }
                if (currentLine.endsWith("GET /L")) {
                    digitalWrite(2, LOW);                // GET /L turns the LED off
                }
                if (currentLine.endsWith("GET /T")){
                    digitalWrite(2,HIGH);
                    delay(120000);
                    digitalWrite(2,LOW);
                }
                if (currentLine.endsWith("GET /P")){
                    digitalWrite(2,HIGH);
                    delay(300000);
                    digitalWrite(2,LOW);
                }
            }
        }
        // close the connection:
        client.stop();
        Serial.println("Client Disconnected.");
    }
}
