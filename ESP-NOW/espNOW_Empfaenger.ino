// ESP NOW --> Code für den Empfaenger
#include <esp_now.h>
#include <WiFi.h>

// Struktur für eingehende Nachricht
typedef struct struct_message {
    int id;
} struct_message;

struct_message incomingData;

// Rückruffunktion zum Empfangen von Nachrichten
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    memcpy(&incomingData, incomingData, sizeof(incomingData));
    Serial.print("Daten empfangen: ");
    Serial.println(incomingData.id);
}

void setup() {
    Serial.begin(115200);

    // WiFi im Modus "Station" starten
    WiFi.mode(WIFI_STA);

    // ESP-NOW initiieren
    if (esp_now_init() != ESP_OK) {
        Serial.println("Fehler bei ESP-NOW Initialisierung");
        return;
    }

    // Rückruffunktion zum Empfangen von Daten registrieren
    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
    // Nichts zu tun, da Empfang über Rückruffunktion erfolgt
}
