#include <ArduinoJson.h>
#include <Ethernet.h>
#include <SPI.h>
#include <Controllino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// variables will change:



void setup() {
  pinMode(CONTROLLINO_R0, OUTPUT);
  pinMode(CONTROLLINO_R1, OUTPUT);  // note that we are using CONTROLLINO aliases for the digital outputs
  pinMode(CONTROLLINO_R2, OUTPUT);
  //digitalWrite(CONTROLLINO_R0, HIGH);
  //digitalWrite(CONTROLLINO_R1, HIGH); 
  //digitalWrite(CONTROLLINO_R2, HIGH); 
  delay(1500);
  digitalWrite(CONTROLLINO_R0, LOW);
  digitalWrite(CONTROLLINO_R1, LOW); 
  digitalWrite(CONTROLLINO_R2, LOW); 
  // Initialize Serial port
  Serial.begin(9600);
  while (!Serial) continue;
  Serial.println("buscando DHCP Server.... ");
    // Initialize Ethernet library
  byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
  if (!Ethernet.begin(mac)) {
    Serial.println(F("Failed to configure Ethernet"));
    return;
  }
  delay(1000);
  Serial.println("My IP address is: ");    // Print text on the serial monitor
  Serial.println(Ethernet.localIP());
  Serial.println("Conectado a RED");
  delay(1000);
  Serial.println("Descargando parametros....");
  delay(1000);
  
}


void loop() 
{
  Serial.println("*************************");
  delay(5000);
  http_request_1();
  http_request_2();
  http_request_3();
  
}






void http_request_1(){
  IPAddress server(192,168,137,1);
// Connect to HTTP server
  EthernetClient client;
  client.setTimeout(10000);
  if (!client.connect(server, 8091)) {
    Serial.println(F("Connection failed"));
    return;
  }

  Serial.println(F("Connected!"));
  
  // Send HTTP request
  //client.println(F("GET /scriptcase/app/ipsa/parametros/?getParametros HTTP/1.0"));
  //client.print(F("GET /scriptcase/app/ipsa/parametros/?getParametros  "));
  client.print(F("GET /scriptcase/app/ipsa/blank/?codigos=10001 ")); 
  //client.print(barcode);
  client.println(F("HTTP/1.0"));
  //client.println(F("Host: arduinojson.org"));
  //client.println(F("Connection: close"));
  if (client.println() == 0) {
    Serial.println(F("Failed to send request"));
    return;
  }

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
  if (strcmp(status + 9, "200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return;
  }else
        {
          Serial.println(status);
        }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }

StaticJsonDocument<128> doc;

DeserializationError error = deserializeJson(doc, client);

if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  return;
}

const char* Server = doc["Server"]; // "TAS-WEB"

for (JsonObject elem : doc["Parametros"].as<JsonArray>()) {

  const char* inlot1 = elem["inlot"]; // "1", "0"
  const char* condicion1 = elem["condicion"]; // "1", "0"
  const char* codigo1 = elem["codigo"]; // "1001", "1002"
  Serial.println(Server);
  Serial.println(codigo1);
  Serial.println(inlot1);
  Serial.println(condicion1);
  if((String(inlot1).toInt() == 1)and(String(condicion1).toInt() == 1))
      {
        digitalWrite(CONTROLLINO_R1, HIGH);
        digitalWrite(CONTROLLINO_R2, HIGH);
        Serial.println("DETECTADO");
        delay(500);
        digitalWrite(CONTROLLINO_R1, LOW);
        digitalWrite(CONTROLLINO_R2, LOW);
        delay(5000);
        http_UpdateInlot_1();
        delay(250);
      }else {
              Serial.println("NO DETECTADO");
            }
  
  }
  // Disconnect
  client.stop();
  delay(100);
  

}

void http_request_2(){
  IPAddress server(192,168,137,1);
// Connect to HTTP server
  EthernetClient client;
  client.setTimeout(10000);
  if (!client.connect(server, 8091)) {
    Serial.println(F("Connection failed"));
    return;
  }

  Serial.println(F("Connected!"));
  
  // Send HTTP request
  //client.println(F("GET /scriptcase/app/ipsa/parametros/?getParametros HTTP/1.0"));
  //client.print(F("GET /scriptcase/app/ipsa/parametros/?getParametros  "));
  client.print(F("GET /scriptcase/app/ipsa/blank/?codigos=10002 ")); 
  //http://127.0.0.1:8091/scriptcase/app/ipsa/UpdateInlot/?codigos=10003
  //client.print(barcode);
  client.println(F("HTTP/1.0"));
  //client.println(F("Host: arduinojson.org"));
  //client.println(F("Connection: close"));
  if (client.println() == 0) {
    Serial.println(F("Failed to send request"));
    return;
  }

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
  if (strcmp(status + 9, "200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return;
  }else
        {
          Serial.println(status);
        }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }

StaticJsonDocument<128> doc;

DeserializationError error = deserializeJson(doc, client);

if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  return;
}

const char* Server = doc["Server"]; // "TAS-WEB"

for (JsonObject elem : doc["Parametros"].as<JsonArray>()) {

  const char* inlot2 = elem["inlot"]; // "1", "0"
  const char* condicion2 = elem["condicion"]; // "1", "0"
  const char* codigo2 = elem["codigo"]; // "1001", "1002"
  Serial.println(Server);
  Serial.println(codigo2);
  Serial.println(inlot2);
  Serial.println(condicion2);

  if((String(inlot2).toInt() == 1)and(String(condicion2).toInt() == 1))
      {
        digitalWrite(CONTROLLINO_R1, HIGH);
        digitalWrite(CONTROLLINO_R2, HIGH);
        Serial.println("DETECTADO");
        delay(500);
        digitalWrite(CONTROLLINO_R1, LOW);
        digitalWrite(CONTROLLINO_R2, LOW);
        delay(5000);
        http_UpdateInlot_2();
        delay(250);
      }else {
              Serial.println("NO DETECTADO");
            }
  
  }
  // Disconnect
  client.stop();
  delay(100);
  

}

void http_request_3(){
  IPAddress server(192,168,137,1);
// Connect to HTTP server
  EthernetClient client;
  client.setTimeout(10000);
  if (!client.connect(server, 8091)) {
    Serial.println(F("Connection failed"));
    return;
  }

  Serial.println(F("Connected!"));
  
  // Send HTTP request
  //client.println(F("GET /scriptcase/app/ipsa/parametros/?getParametros HTTP/1.0"));
  //client.print(F("GET /scriptcase/app/ipsa/parametros/?getParametros  "));
  client.print(F("GET /scriptcase/app/ipsa/blank/?codigos=10003 ")); 
  //client.print(barcode);
  client.println(F("HTTP/1.0"));
  //client.println(F("Host: arduinojson.org"));
  //client.println(F("Connection: close"));
  if (client.println() == 0) {
    Serial.println(F("Failed to send request"));
    return;
  }

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
  if (strcmp(status + 9, "200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return;
  }else
        {
          Serial.println(status);
        }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }

StaticJsonDocument<128> doc;

DeserializationError error = deserializeJson(doc, client);

if (error) {
  Serial.print(F("deserializeJson() failed: "));
  Serial.println(error.f_str());
  return;
}

const char* Server = doc["Server"]; // "TAS-WEB"

for (JsonObject elem : doc["Parametros"].as<JsonArray>()) {

  const char* inlot3 = elem["inlot"]; // "1", "0"
  const char* condicion3 = elem["condicion"]; // "1", "0"
  const char* codigo3 = elem["codigo"]; // "1001", "1002"
  Serial.println(Server);
  Serial.println(codigo3);
  Serial.println(inlot3);
  Serial.println(condicion3);
   if((String(inlot3).toInt() == 1)and(String(condicion3).toInt() == 1))
      {
        digitalWrite(CONTROLLINO_R0, HIGH);
        Serial.println("DETECTADO");
        delay(500);
        digitalWrite(CONTROLLINO_R0, LOW);
        delay(5000);
        http_UpdateInlot_3();
        delay(250);
      }else {
              Serial.println("NO DETECTADO");
            }
  }
  // Disconnect
  client.stop();
  delay(100);
  
}



void http_UpdateInlot_1(){
 IPAddress server(192,168,137,1);
// Connect to HTTP server
  EthernetClient client;
  client.setTimeout(10000);
  if (!client.connect(server, 8091)) {
    Serial.println(F("Connection failed"));
    return;
  }

  Serial.println(F("Connected!"));
  
  // Send HTTP request
  //client.println(F("GET /scriptcase/app/ipsa/parametros/?getParametros HTTP/1.0"));
  //client.print(F("GET /scriptcase/app/ipsa/parametros/?getParametros  "));
  client.print(F("GET /scriptcase/app/ipsa/UpdateInlot/?codigos=10001 ")); 
  //client.print(barcode);
  client.println(F("HTTP/1.0"));
  //client.println(F("Host: arduinojson.org"));
  //client.println(F("Connection: close"));
  if (client.println() == 0) {
    Serial.println(F("Failed to send request"));
    return;
  }

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
  if (strcmp(status + 9, "200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return;
  }else
        {
          Serial.println(status);
        }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }

  // Disconnect
  client.stop();
  delay(100);
}

void http_UpdateInlot_2(){
 IPAddress server(192,168,137,1);
// Connect to HTTP server
  EthernetClient client;
  client.setTimeout(10000);
  if (!client.connect(server, 8091)) {
    Serial.println(F("Connection failed"));
    return;
  }

  Serial.println(F("Connected!"));
  
  // Send HTTP request
  //client.println(F("GET /scriptcase/app/ipsa/parametros/?getParametros HTTP/1.0"));
  //client.print(F("GET /scriptcase/app/ipsa/parametros/?getParametros  "));
  client.print(F("GET /scriptcase/app/ipsa/UpdateInlot/?codigos=10002 ")); 
  //client.print(barcode);
  client.println(F("HTTP/1.0"));
  //client.println(F("Host: arduinojson.org"));
  //client.println(F("Connection: close"));
  if (client.println() == 0) {
    Serial.println(F("Failed to send request"));
    return;
  }

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
  if (strcmp(status + 9, "200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return;
  }else
        {
          Serial.println(status);
        }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }

  // Disconnect
  client.stop();
  delay(100);
}

void http_UpdateInlot_3(){
 IPAddress server(192,168,137,1);
// Connect to HTTP server
  EthernetClient client;
  client.setTimeout(10000);
  if (!client.connect(server, 8091)) {
    Serial.println(F("Connection failed"));
    return;
  }

  Serial.println(F("Connected!"));
  
  // Send HTTP request
  //client.println(F("GET /scriptcase/app/ipsa/parametros/?getParametros HTTP/1.0"));
  //client.print(F("GET /scriptcase/app/ipsa/parametros/?getParametros  "));
  client.print(F("GET /scriptcase/app/ipsa/UpdateInlot/?codigos=10003 ")); 
  //client.print(barcode);
  client.println(F("HTTP/1.0"));
  //client.println(F("Host: arduinojson.org"));
  //client.println(F("Connection: close"));
  if (client.println() == 0) {
    Serial.println(F("Failed to send request"));
    return;
  }

  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  // It should be "HTTP/1.0 200 OK" or "HTTP/1.1 200 OK"
  if (strcmp(status + 9, "200 OK") != 0) {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return;
  }else
        {
          Serial.println(status);
        }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }

  // Disconnect
  client.stop();
  delay(100);
}
