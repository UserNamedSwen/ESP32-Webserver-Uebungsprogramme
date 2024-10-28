# ESP NOW

**ESP-NOW** ist ein drahtloses Kommunikationsprotokoll, das speziell für ESP32 und ESP8266 Mikrocontroller entwickelt wurde. Es ermöglicht schnelle, direkte Verbindungen zwischen ESP-Geräten ohne die Notwendigkeit eines WLAN-Routers. Dies macht es ideal für IoT-Anwendungen, bei denen man Geräte über kurze Distanzen und mit minimaler Latenz verbinden möchte, ohne auf eine bestehende WLAN-Infrastruktur angewiesen zu sein.

### Eigenschaften von ESP-NOW

- **Direkte Kommunikation:** Geräte können direkt miteinander kommunizieren, ohne einen Access Point.
- **Geringe Latenz:** Die Kommunikation erfolgt nahezu in Echtzeit.
- **Effizienter Energieverbrauch:** Ideal für batteriebetriebene IoT-Projekte.
- **Verschlüsselung möglich:** Sicherere Kommunikation durch WPA2.

### Minimalbeispiel für ESP-NOW

Hier zeigen wir, wie man mit ESP-NOW eine einfache Kommunikation zwischen zwei ESP32-Geräten einrichtet. Das Minimalbeispiel besteht aus einem **Sender** und einem **Empfänger**.

#### Aufbau des Beispiels
In diesem Beispiel sendet der **Sender** ein einfaches Byte (zum Beispiel eine Zahl oder eine Statusinformation) an den **Empfänger**, der die empfangenen Daten ausgibt. 

> **Hinweis:** Damit beide ESP32 miteinander kommunizieren können, ist es notwendig, die MAC-Adresse des Empfängers im Code des Senders anzugeben.

#### 1. MAC-Adresse des ESP32-Geräts herausfinden

Zuerst finden wir die MAC-Adresse jedes ESP32-Geräts heraus, um sie für die ESP-NOW-Kommunikation zu verwenden.

1. Öffne die Arduino IDE.
2. Lade mac_addr.ino auf deinen ESP32 hoch, um die MAC-Adresse auszulesen
3. Öffne den seriellen Monitor. Die MAC-Adresse wird angezeigt (z. B. `24:6F:28:AB:CD:EF`).
4. Notiere die MAC-Adresse für die Konfiguration des Senders.

#### 2. Code für den Sender

Hier ist der Code für den ESP32, der als **Sender** fungiert und eine Nachricht an den Empfänger sendet. Die MAC-Adresse des Empfängers muss im Code eingetragen werden.
#### Erklärung des Codes

1. **Sender-Code:**
   - Die `broadcastAddress` speichert die MAC-Adresse des Empfängers.
   - Die Struktur `struct_message` definiert die Daten, die gesendet werden.
   - Im `loop()`-Abschnitt wird die Nachricht mit `esp_now_send()` an die MAC-Adresse des Empfängers gesendet.

2. **Empfänger-Code:**
   - Auch hier wird eine Struktur `struct_message` definiert, die den Nachrichtentyp (einfacher Integer-Wert `id`) angibt.
   - `OnDataRecv` ist die Rückruffunktion, die ausgeführt wird, sobald eine Nachricht empfangen wird. Diese Funktion liest die Daten aus und zeigt sie auf dem seriellen Monitor an.

---

### Schritte zur Ausführung

1. **MAC-Adresse ermitteln:** Finde die MAC-Adresse des Empfängers (wie oben beschrieben) und trage sie in den Sender-Code ein.
2. **Sender und Empfänger auf ESP32 laden:** Lade den Sender-Code auf einen ESP32 und den Empfänger-Code auf einen anderen ESP32.
3. **Serielle Monitore öffnen:** Überprüfe die seriellen Monitore beider ESP32-Boards.
4. **Ergebnisse beobachten:** Der Sender sollte regelmäßig eine Nachricht senden (z.B. eine Zufallszahl zwischen 0 und 100), und der Empfänger gibt die empfangenen Werte aus.

---

### Anwendungsbeispiele

ESP-NOW ist nützlich für viele IoT-Anwendungen, wie z.B.:
- **Drahtlose Sensornetze:** Ein ESP32 kann als Zentrale fungieren, die Daten von verschiedenen Sensoren empfängt.
- **Smarte Fernbedienungen:** Steuerung von Beleuchtung, Ventilatoren oder Geräten ohne WLAN-Router.
- **Energiesparende Kommunikation:** Ideal für batteriebetriebene Geräte, da die Kommunikation schnell und effizient erfolgt.

Dieses Minimalbeispiel zeigt die grundlegende Struktur der Kommunikation über ESP-NOW und ist ein guter Startpunkt für komplexere Anwendungen.

