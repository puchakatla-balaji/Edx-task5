#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHTesp.h> //DHT11 Library for ESP
#include <ThingSpeak.h> // Include ThingSpeak library

#include "index.h" //Our HTML webpage contents with javascripts

#define LED 2 //On board LED
#define DHTpin 14 //D5 of NodeMCU is GPIO14

DHTesp dht;

//SSID and Password of your WiFi router
const char* ssid = "Bablu";
const char* password = "123451234";

ESP8266WebServer server(80); //Server on port 80

float humidity, temperature;

// ThingSpeak Channel settings
const char *thingSpeakAddress = "api.thingspeak.com";
const unsigned long channelId = 2506729;
const char *writeAPIKey = "LSBO24ZXQHMRB96R";

WiFiClient client; // Initialize WiFi client for ThingSpeak

void handleRoot() {
  String s = MAIN_page; //Read HTML contents
  server.send(200, "text/html", s); //Send web page
}

void handleADC() {
  int rain = analogRead(A0);
  
  // Read DHT11 sensor data
  humidity = dht.getHumidity();
  temperature = dht.getTemperature();
  
  // Create JSON data
  String data = "{\"Rain\":\"" + String(rain) + "\", \"Temperature\":\"" + String(temperature) + "\", \"Humidity\":\"" + String(humidity) + "\"}";

  // Toggle LED on data request ajax
  digitalWrite(LED, !digitalRead(LED));

  // Send data to ThingSpeak
  ThingSpeak.begin(client); // Initialize ThingSpeak client
  ThingSpeak.setField(1, rain); // Field 1: Rain data
  ThingSpeak.setField(2, temperature); // Field 2: Temperature data
  ThingSpeak.setField(3, humidity); // Field 3: Humidity data
  int writeSuccess = ThingSpeak.writeFields(channelId, writeAPIKey);
  if (writeSuccess == 200) {
    Serial.println("Data sent to ThingSpeak successfully.");
  } else {
    Serial.print("Error sending data to ThingSpeak. HTTP error code: ");
    Serial.println(writeSuccess);
  }

  // Send ADC value, temperature, and humidity JSON to client ajax request
  server.send(200, "text/plain", data);
}

void setup() {
  Serial.begin(9600);
  Serial.println();

  dht.setup(DHTpin, DHTesp::DHT11); //for DHT11 Connect DHT sensor to GPIO 17
  pinMode(LED, OUTPUT);

  WiFi.begin(ssid, password); //Connect to your WiFi router
  Serial.println("");
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); //IP address assigned to your ESP

  server.on("/", handleRoot); //Which routine to handle at root location. This is display page
  server.on("/readADC", handleADC); //This page is called by java Script AJAX

  server.begin(); //Start server
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient(); //Handle client requests
}
