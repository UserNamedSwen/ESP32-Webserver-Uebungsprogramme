#include <WiFi.h>
#include <WebServer.h>

// Definiere den LED-Pin und den analogen Pin
const int ledPin = 2;        // GPIO2 wird oft für die interne LED des ESP32 verwendet
const int analogPin = 34;    // GPIO34 ist ein analoger Eingang (D34)
int analogValue = 0;
// Erstelle ein WebServer-Objekt auf Port 80
WebServer server(80);

// Variablen zur Überwachung des LED-Status
bool ledStatus = false;

// SSID und Passwort für das Access Point
const char* ssid = "ESP32-LED-AnalogIn";
const char* password = "12345678";

// HTML-Seite mit Graphen für den analogen Wert
const char* htmlPage = R"=====(
  <!DOCTYPE html>
  <html>
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 LED & Analog Monitor</title>
    <style>
      body {
        background-color: green;
        font-family: Arial, sans-serif;
        display: flex;
        flex-direction: column;
        justify-content: center;
        align-items: center;
        height: 100vh;
        margin: 0;
        color: white;
      }
      h1 {
        font-size: 4vw;
      }
      button {
        padding: 15px;
        font-size: 4vw;
        border: none;
        border-radius: 8px;
        background-color: white;
        color: green;
        cursor: pointer;
      }
      button:hover {
        background-color: lightgray;
      }
      canvas {
        background-color: white;
        border-radius: 8px;
        margin-top: 20px;
      }
    </style>
    <script>
      var dataPoints = [];
      var maxDataPoints = 50;  // Maximale Anzahl der Punkte im Graphen

      // Funktion zum Umschalten der LED
      function toggleLED() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/toggleLED", true);
        xhr.send();
      }

      // Funktion zum Abrufen des analogen Werts
      function fetchAnalogValue() {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/analogValue", true);
        xhr.onload = function() {
          if (xhr.status === 200) {
            var analogValue = parseInt(xhr.responseText);
            if (dataPoints.length >= maxDataPoints) {
              dataPoints.shift();  // Entferne den ältesten Punkt, wenn die maximale Anzahl erreicht ist
            }
            dataPoints.push(analogValue);
            drawGraph();
          }
        };
        xhr.send();
      }

      // Funktion zum Zeichnen des Graphen
      function drawGraph() {
        var canvas = document.getElementById('sensorGraph');
        var ctx = canvas.getContext('2d');

        // Canvas reinigen
        ctx.clearRect(0, 0, canvas.width, canvas.height);

        // Einstellungen für die Linie
        ctx.beginPath();
        ctx.lineWidth = 2;
        ctx.strokeStyle = 'red';  // Farbe der Linie auf Rot gesetzt

        // Graph-Größe und Skalierung
        var graphHeight = canvas.height;
        var graphWidth = canvas.width;
        var scaleY = graphHeight / 4095;  // Annahme: 12-Bit-ADC, max. Wert 4095
        var stepX = graphWidth / (maxDataPoints - 1);

        // Zeichne die Datenpunkte
        for (let i = 0; i < dataPoints.length; i++) {
          let x = i * stepX;
          let y = graphHeight - dataPoints[i] * scaleY;
          if (i === 0) {
            ctx.moveTo(x, y);
          } else {
            ctx.lineTo(x, y);
          }
        }

        ctx.stroke();
      }

      // Starte die periodische Aktualisierung des analogen Werts und Zeichnung
      setInterval(fetchAnalogValue, 200);  // Alle 200 Millisekunden den Wert abrufen (1/10 Sekunde)
    </script>
  </head>
  <body>
    <h1>ESP32 LED & Analog Monitor</h1>
    <button onclick="toggleLED()">Toggle LED</button>
    <canvas id="sensorGraph" width="400" height="300"></canvas>
  </body>
  </html>
)=====";

// Funktion, die die Webseite liefert
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

// Funktion zum Umschalten der LED
void handleToggleLED() {
  ledStatus = !ledStatus;         // LED-Status umschalten
  digitalWrite(ledPin, ledStatus); // LED an- oder ausschalten
  server.send(200, "text/plain", ledStatus ? "LED is ON" : "LED is OFF");
}

// Funktion zum Abrufen des analogen Werts
void handleAnalogValue() {
  analogValue = analogRead(analogPin);  // Lese den analogen Wert von Pin D34
  analogValue = min(analogValue, 4095);     // Begrenze den Wert auf 4095
  server.send(200, "text/plain", String(analogValue));  // Sende den Wert zurück
  Serial.print("Messwert: ");
  Serial.println(analogValue);
}

void setup() {
  // Starte den seriellen Monitor
  Serial.begin(115200);

  // Setze den LED-Pin als Ausgang
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // LED standardmäßig aus

  // Initialisiere den Access Point
  WiFi.softAP(ssid, password);
  Serial.println("Access Point gestartet");
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.softAPIP());

  // Routen definieren
  server.on("/", handleRoot);        // Zeige die Webseite
  server.on("/toggleLED", handleToggleLED); // LED umschalten
  server.on("/analogValue", handleAnalogValue); // Sende den analogen Wert

  // Starte den Webserver
  server.begin();
  Serial.println("Webserver gestartet");
}

void loop() {
  // Verarbeite eingehende HTTP-Anfragen
  server.handleClient();
}
