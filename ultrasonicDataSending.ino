#include <ESP8266WiFi.h>

#define trigPin D5
#define echoPin D1

const char* ssid = "Galaxy ";
const char* password = "Jerome1234567";

WiFiServer server(80);

// Pick a static IP for your ESP (must be free in your network)
IPAddress local_IP(192, 168, 0, 105); 
IPAddress gateway(192, 168, 0, 1);     
IPAddress subnet(255, 255, 255, 0);

float getDistance() {
  float duration, distance;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * 0.034) / 2;
  return distance;
}

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Configure static IP
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("⚠️ Failed to configure static IP");
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Connected to WiFi");
  Serial.print("ESP IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  WiFiClient client = server.available();
  if (!client) return;

  while (!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  float distance = getDistance();


  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println("Connection: close");
  client.println();
  client.println(distance);

  delay(1);
  Serial.println("Client disconnected");
}
