#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

ESP8266WebServer server(80);

const int ledPin = D6;
DHT dht14(D4, DHT11);

void init_wifi(String ssid, String password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup(void) {
  Serial.begin(9600);
  init_wifi("SivannaCL", "sushiluckyto");

  pinMode(ledPin, OUTPUT);

  dht14.begin();

server.on("/", HTTP_GET, []() {
    float temperature = dht14.readTemperature();
    float humidity = dht14.readHumidity();

    String html = "<html><head>";
    html += "<style>";
    html += "body { font-family: cursive; background-color: #f2f2f2; }";
    html += "div { background-color: #fff; border-radius: 5px; padding: 60px; margin: 50px; text-align: center; }";
    html += "h1 { color: #333; font-size: 36px;}";
    html += "p { color: #666; font-size: 28px; font-weight: bold; }";
    html += "input[type='submit'] { background-color: #4CAF50; color: white; padding: 30px 40px; border: none; border-radius: 8px; cursor: pointer; font-size: 16px; width: 150px; margin: 10px auto; }";
    html += "input[type='submit']:hover { background-color: #191970;  }";
    html += "#turnOffButton { background-color: #FF1493; }";
    html += "</style>";
    html += "</head>";
    html += "<body><div>";
    html += "<h1>Web Server</h1>";

html += "<table style='width: 100%;'>";
html += "<tr>";
html += "<th style='width: 40%;'></th>";
html += "<th style='width: 40%;'></th>";
html += "<th style='width: 20%;'></th>";
html += "</tr>";

html += "<tr>";
html += "<td></td>";
html += "<td>";

html += "<p><strong>Temperature:</strong> <span id='temperature'>" + String(temperature, 2) + " C</span></p>";
html += "<p><strong>Humidity:</strong> <span id='humidity'>" + String(humidity, 2) + " %</span></p>";
html += "<p><strong>LED Status:</strong> ";
if (digitalRead(ledPin) == HIGH) {
    html += "<span style='color: blue;'>On</span>";
} else {
    html += "<span style='color: red;'>Off</span>";
}
html += "</p>";

html += "</td>";
html += "<td></td>";
html += "</tr>";
html += "</table>";




    html += "<div style='display: inline-block; margin-top: 2px;'>";
    html += "<form action='/TurnOn' method='get'><input type='submit' value='Turn On'></form>";
    html += "</div>";

    html += "<div style='display: inline-block; margin-top: 2px;'>";
    html += "<form action='/TurnOff' method='get'><input type='submit' id='turnOffButton' value='Turn Off'></form>";
    html += "</div>";


    html += "</div></body></html>";

    html += "<script>";
    html += "function updateData() {";
    html += "  var xhr = new XMLHttpRequest();";
    html += "  xhr.open('GET', '/data', true);";
    html += "  xhr.onreadystatechange = function() {";
    html += "    if (xhr.readyState == 4 && xhr.status == 200) {";
    html += "      var data = JSON.parse(xhr.responseText);";
    html += "      document.getElementById('temperature').textContent = data.temperature.toFixed(2) + ' C';";
    html += "      document.getElementById('humidity').textContent = data.humidity.toFixed(2) + ' %';";
    html += "    }";
    html += "  };";
    html += "  xhr.send();";
    html += "}";
    html += "setInterval(updateData, 500);";
    html += "</script>";

    server.send(200, "text/html", html);
});


  server.on("/TurnOn", HTTP_GET, []() {
    digitalWrite(ledPin, HIGH);
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });

  server.on("/TurnOff", HTTP_GET, []() {
    digitalWrite(ledPin, LOW);
    server.sendHeader("Location", "/");
    server.send(302, "text/plain", "");
  });

  server.on("/data", HTTP_GET, []() {
    float temperature = dht14.readTemperature();
    float humidity = dht14.readHumidity();

    String data = "{\"temperature\": " + String(temperature) + ", \"humidity\": " + String(humidity) + "}";
    server.send(200, "application/json", data);
  });

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
}