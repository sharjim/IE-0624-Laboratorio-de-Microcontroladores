#include <Adafruit_PCD8544.h>

// Configuración de la pantalla PCD8544
#define PIN_SCLK 5
#define PIN_DIN 6
#define PIN_DC 7
#define PIN_CS 8
#define PIN_RST 9
Adafruit_PCD8544 display = Adafruit_PCD8544(PIN_SCLK, PIN_DIN, PIN_DC, PIN_CS, PIN_RST);

// Pines de las entradas analógicas
const int channel1 = A0;
const int channel2 = A1;
const int channel3 = A2;
const int channel4 = A3;
const int rectificationPin = A4;  // Pin para detectar la señal rectificada

// Función para verificar si alguno de los voltajes supera los límites
void verificarLimites(float v1, float v2, float v3, float v4) {
  if (abs(v1) >= 20 || abs(v2) >= 20 || abs(v3) >= 20 || abs(v4) >= 20) {
    // Si algún valor supera los 20V o -20V, parpadea el LED
    digitalWrite(ledPin, HIGH);
    delay(500); // Retardo de 500 ms con el LED encendido
    digitalWrite(ledPin, LOW);
    delay(500); // Retardo de 500 ms con el LED apagado
  }
}

// Función para mostrar los valores en la pantalla LCD
void mostrarEnPantalla(float v1, float v2, float v3, float v4, bool rectified) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(BLACK);
  
  display.setCursor(0, 0); 
  display.println("4 channel voltmeter");

  display.setCursor(0, 10);
  display.print("V1: "); display.print(v1); 
  display.println(rectified ? "V_rec" : "V");

  display.setCursor(0, 20);
  display.print("V2: "); display.print(v2); 
  display.println(rectified ? "V_rec" : "V");

  display.setCursor(0, 30);
  display.print("V3: "); display.print(v3); 
  display.println(rectified ? "V_rec" : "V");

  display.setCursor(0, 40);
  display.print("V4: "); display.print(v4); 
  display.println(rectified ? "V_rec" : "V");

  display.display(); // Mostrar los datos en la pantalla
}
