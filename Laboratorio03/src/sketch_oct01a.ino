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

// Pin del LED de advertencia
const int ledPin = 13;

// Variables para almacenar los valores convertidos
float voltaje1 = 0;
float voltaje2 = 0;
float voltaje3 = 0;
float voltaje4 = 0;

// Variable para almacenar el estado de la señal rectificada
bool isRectified = false;

void setup() {
  Serial.begin(9600);  // Para depuración
  display.begin();
  display.setContrast(50);  // Ajustar el contraste de la pantalla
  display.clearDisplay();   // Limpiar pantalla al iniciar

  // Configurar el pin del LED como salida
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // Leer valores de los pines analógicos (0 a 1023)
  int adcValue1 = analogRead(channel1);
  int adcValue2 = analogRead(channel2);
  int adcValue3 = analogRead(channel3);
  int adcValue4 = analogRead(channel4);
  int rectificationValue = analogRead(rectificationPin);  // Leer el pin de rectificación

  // Convertir de ADC (0 a 1023) a voltaje (0 a 5V)
  float voltageADC1 = adcValue1 * (5.0 / 1023.0);
  float voltageADC2 = adcValue2 * (5.0 / 1023.0);
  float voltageADC3 = adcValue3 * (5.0 / 1023.0);
  float voltageADC4 = adcValue4 * (5.0 / 1023.0);
  float rectifiedVoltage = rectificationValue * (5.0 / 1023.0);

  // Convertir a valores originales de -24 a 24V
  voltaje1 = convertirVoltaje(voltageADC1);
  voltaje2 = convertirVoltaje(voltageADC2);
  voltaje3 = convertirVoltaje(voltageADC3);
  voltaje4 = convertirVoltaje(voltageADC4);

  // Verificar si alguno de los voltajes supera los límites
  verificarLimites(voltaje1, voltaje2, voltaje3, voltaje4);

  // Verificar si el valor en el pin de rectificación es mayor a 1V
  isRectified = rectifiedVoltage > 1.0;

  // Mostrar los valores en la pantalla LCD y en el monitor serial
  mostrarEnPantalla(voltaje1, voltaje2, voltaje3, voltaje4, isRectified);
  mostrarEnSerial(voltaje1, voltaje2, voltaje3, voltaje4, isRectified);

  delay(1000);  // Actualizar cada segundo
}

// Función para convertir el voltaje de 0-5V a -24V a 24V
float convertirVoltaje(float voltageADC) {
  if (voltageADC <= 2.5) {
    // Señal negativa, multiplicamos por -9.6 para obtener el valor correcto
    return (voltageADC * -9.6);  
  } else {
    // Señal positiva
    float positiveVoltage = (voltageADC - 2.5) * 9.6;
    return positiveVoltage; // Retornamos el voltaje positivo ya convertido
  }
}

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
