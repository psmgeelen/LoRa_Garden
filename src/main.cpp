#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>
// Concept
// We send some value (preferably sensor value) to a database. We need a webservice 
// to accept and push the value from the module to DB as a straight ODBC call it to 
// heavy for an ESP32 module. 

// WiFi credentials 
const char* ssid     = "harseny";
const char* password = "1234abcd";


//Your Domain name with URL path or IP address with path
// const char* serverName = "http://jsonplaceholder.typicode.com/posts/1";
// headers can be a serious issue
const char* serverName = "http://217.160.172.124:3000/humidity_log";



void setup() {
  Serial.begin(9600); // check baudrate for connection, it needs to be insync with other settings like monitoring. 
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());
}

void loop() {
  // Print what are we exactly writing
  Serial.print("Writing: ");
  // If no Wifi signal, try to reconnect it
  if (WiFi.status() == WL_CONNECTED){
    Serial.println("Wifi connected");

  // read sensor value and create timestamp
    int dacoutput = analogRead(32);
    //char created_at[] = "2020-01-01 00:00:00";

  //create and parse json object to pass to HTTP POST request later
    
    StaticJsonBuffer<69> jsonBuffer;

    char json_payload[] = "{'sensor_value': '1111', 'created_at': '2020-01-01 00:00:00' }";
    JsonObject& root = jsonBuffer.parseObject(json_payload);

    if(!root.success()) {
      Serial.println("parseObject() failed");
    } else {
      Serial.println("JSON OK");
    }

    String data;

    root.printTo(data);


    HTTPClient http;
    
    // Your Domain name with URL path or IP address with path
    http.begin(serverName);

    // Specify content-type header
    //http.addHeader("Content-Type", "text/plain");
             
    // Send HTTP POST request
    //int httpResponseCode = http.PUT("PUT hello world");


    // If you need an HTTP request with a content type: application/json, use the following:
    //http.addHeader("Content-Type", "application/json");
    //int httpResponseCode = http.POST("{\"api_key\":\"tPmAT5Ab3j7F9\",\"sensor\":\"BME280\",\"value1\":\"24.25\",\"value2\":\"49.54\",\"value3\":\"1005.14\"}");

    // If you need an HTTP request with a content type: text/plain
    //http.addHeader("Content-Type", "text/plain");
    int httpResponseCode = http.POST(data);
        
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
      
     Serial.print("Sensor Value: ");
    Serial.println(dacoutput);
    // Free resources
    http.end();

  }
  // Write point
  //Wait 10s
  Serial.println("Wait 10s");
  delay(10000);
}