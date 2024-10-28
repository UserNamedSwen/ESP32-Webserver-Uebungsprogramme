//ESP NOW--> Programm für den Sender
#include <esp_now.h>
#include <WiFi.h>

// MAC-Adresse des Empfängers eintragen
uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0xAB, 0xCD, 0xEF};

// Struktur der Nachricht
typedef struct struct_message {
    int id;
} struct_message;

struct_message myData;

void setup() {
    Serial.begin(115200);

    // WiFi im Modus "Station" starten
    WiFi.mode(WIFI_STA);

    // ESP-NOW initiieren
    if (esp_now_init() != ESP_OK) {
        Serial.println("Fehler bei ESP-NOW Initialisierung");
        return;
    }

    // Peer registrieren
    esp_now_peer_info_t peerInfo;
    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    if (esp_now_add_peer(&peerInfo) != ESP_OK) {
        Serial.println("Fehler beim Hinzufügen des Peers");
        return;
    }
}

void loop() {
    // Daten festlegen und senden
    myData.id = random(0, 100);  // Beispiel: Zufallszahl als Nachricht

    // Nachricht senden
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

    if (result == ESP_OK) {
        Serial.println("Nachricht erfolgreich gesendet");
    } else {
        Serial.println("Fehler beim Senden");
    }

    delay(2000);  // 2 Sekunden warten
}
