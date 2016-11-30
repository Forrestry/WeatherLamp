

#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

#define JSON_BUFF_DIMENSION 2500

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "api.openweathermap.org";    // name address for Google (using DNS)

String text;
int endResponse = 0;
boolean startJson = false;

EthernetClient client;


unsigned long lastConnectionTime = 10 * 60 * 1000;     // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 10 * 60 * 1000;  // posting interval of 10 minutes  (10L * 1000L; 10 seconds delay for testing)

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);

  text.reserve(JSON_BUFF_DIMENSION);
  
// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
char server[] = "api.openweathermap.org";    // name address for openweathermap api (using DNS)
}

void loop() {

  // if ten minutes have passed since your last connection,
  // then connect again and send data:
  if (millis() - lastConnectionTime > postingInterval) {
    // note the time that the connection was made:
    lastConnectionTime = millis();

    httpRequest();
  }

  char c = 0;
  if (client.available()) {
    c = client.read();
    // json contains equal number of open and close curly brackets, therefore by counting
    // the open and close occurences, we can determine when a json is completely received
   
    // endResponse == 0 means equal number of open and close curly brackets reached
    if (endResponse == 0 && startJson == true) {
      parseJson(text.c_str());  // parse c string text in parseJson function
      text = "";                // clear text string for the next time
      startJson = false;        // set startJson to false to indicate that a new message has not yet started
    }
    if (c == '{') {
      startJson = true;         // set startJson true to indicate json message has started
      endResponse++;
    }
    if (c == '}') {
      endResponse--;
    }
    if (startJson == true) {
      text += c;
    }
  }
}
void parseJson(const char * jsonString) {
  StaticJsonBuffer<4000> jsonBuffer;

  // FIND FIELDS IN JSON TREE
  JsonObject& root = jsonBuffer.parseObject(jsonString);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  JsonArray& list = root["list"];
  JsonObject& now = list[0];
  JsonObject& later = list[1];

  float tempNow = now["main"]["temp"];
  String weatherNow = now["weather"][0]["description"];

/* printDiffFloat(tempNow, tempLater, "temperature", "*C");
  printDiffString(weatherNow, weatherLater, "rain");
  printDiffString(weatherNow, weatherLater, "snow");
  printDiffString(weatherNow, weatherLater, "hail");
  printDiffString(weatherNow, weatherLater, "clear");
  printDiffFloat(humidityNow, humidityLater, "humidity", "%");
  Serial.println(); */

}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  client.stop();

  // if there's a successful connection:
  if (client.connect(server, 80)) {
    // Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("GET /data/2.5/forecast?q=Lubbock,us&mode=json&units=metric&cnt=2 HTTP/1.1");
    client.println("Host: api.openweathermap.org");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();
  }
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}

/*void printDiffString(String now, String later, String weatherType) {
  int indexNow = now.indexOf(weatherType);
  int indexLater = later.indexOf(weatherType);
  // for all types of weather except for clear skies, if the current weather does not contain the weather type and the later message does, send notification
  if (weatherType != "clear") {
    if (indexNow == -1 && indexLater != -1) {
      Serial.println("Oh no! It is going to " + weatherType + " later! Predicted " + later);
      for (int thisNote = 0; thisNote < 4; thisNote++) {
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(8, negMelody[thisNote], noteDuration);      // play negative melody through piezo
      }
    }
  }
  // for clear skies, if the current weather does not contain the word clear and the later message does, send notification that it will be sunny later
  else {
    if (indexNow == -1 && indexLater != -1) {
      Serial.println("It is going to be sunny later! Predicted " + later);
      for (int thisNote = 0; thisNote < 4; thisNote++) {
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(8, posMelody[thisNote], noteDuration);      // play positive melody through piezo
      }
    }
  }
}

void printDiffFloat(float now, float later, String parameter, String unit) {
  String change;
  if (now > later) {
    change = "drop from ";
  }
  else if (now < later) {
    change = "rise from ";
  }
  else {
    return;
  }
  Serial.print("UPDATE: The " + parameter + " will " + change);
  Serial.print(now);
  Serial.print(unit + " to ");
  Serial.print(later);
  Serial.println(unit + "!");
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm"); 
}*/
