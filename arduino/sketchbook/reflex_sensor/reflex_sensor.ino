#include <ESP8266WiFi.h>
#include <AddrList.h>
 
const char* ssid = "Xanadu";
const char* password = "4615630762602955";
String linkLocal, ipv6, ipv4;
 
int ledPin = D0; // GPIO13
WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(D1, OUTPUT);
  digitalWrite(D1, LOW); // Turn off LED

  pinMode(D2, OUTPUT);
  digitalWrite(D2, LOW);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected IPv4");

 #if LWIP_IPV6
// Warten auf die Globale IPv6
  for (bool configured = false; !configured;) {
    for (auto addr : addrList)
      if (configured = addr.isV6() && !addr.isLocal()) {
        break;
      }
    Serial.print('.');
    delay(500);
  }
  Serial.println("\nconnected IPv6\n");
  for (auto a : addrList) {
    a.isV6() ? a.isLocal() ? linkLocal = a.toString() : ipv6 = a.toString() : ipv4 = a.toString();
  }
//URLs für den Browser ausgeben
  Serial.printf("IPv4 address: http://%s\n", ipv4.c_str());
  Serial.printf("IPv6 local: http://[%s]\n", linkLocal.c_str());
  Serial.printf("IPv6 global: http://[%s]\n", ipv6.c_str());
#else
  Serial.printf("IPV6 ist nicht aktiviert\n");
#endif

  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
  digitalWrite(ledPin, HIGH);
}
 
void loop() {
  long LoopCt = 120000;
  int Val1, Val2;
  
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
    LoopCt--;
    if (LoopCt < 1) {
      Serial.println("Timeout");
      return;
    }
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  digitalWrite(D1, LOW); // Turn off LED
  digitalWrite(D2, HIGH); // Power on photodiode
  delay(1);
  Val1 = analogRead(A0);
  delay(1);

  digitalWrite(D1, HIGH); // Turn on LED
  delay(5);
  Val2 = analogRead(A0);
  delay(1);
  digitalWrite(D1, LOW); // Turn off LED
  digitalWrite(D2, LOW); // Power down photodiode
  

  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Values: ");
  client.print(Val1); client.print(" / "); 
  client.print(Val2); client.println("<br>");

  client.print((Val2*100)/Val1); client.println(" %<br>");

  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>Reload</button></a>");
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disconnected");
  Serial.println("");
 
}
