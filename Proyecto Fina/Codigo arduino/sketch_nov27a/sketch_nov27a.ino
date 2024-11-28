#include <PDM.h>
#include <Servo.h>
#include <deybin.garcia-project-1_inferencing.h>

// Definiciones para el control del sistema
#define LED_PIN LED_BUILTIN
#define SERVO_PIN 2
#define CONFIDENCE_THRESHOLD 0.80

// Estados del sistema
enum SystemState {
    STATE_CLOSED,
    STATE_OPEN
};

// Variables de control
SystemState currentState = STATE_OPEN;
unsigned long lastCommandTime = 0;
const unsigned long DEBOUNCE_TIME = 1000; // Tiempo mínimo entre comandos (ms)
const unsigned long SERVO_DELAY_TIME = 3000; // Tiempo para mover el servo completamente (ms)
const unsigned long OPEN_HOLD_TIME = 10000;  // Tiempo mínimo en posición abierta (ms)

// Variables del servomotor
Servo garageServo;
unsigned long servoMoveStartTime = 0;
bool servoMoving = false;

// Variables para el LED
const int LED_CLOSED_BLINK = 1000;  // Parpadeo lento para cerrado
const int LED_OPEN_BLINK = 200;     // Parpadeo rápido para abierto
unsigned long previousMillis = 0;
int ledState = LOW;

// Variables de inferencia
static bool debug_nn = false;
static signed short *sampleBuffer;
static bool record_ready = false;

// Estructura de inferencia
typedef struct {
    signed short *buffers[2];
    unsigned char buf_select;
    unsigned char buf_ready;
    unsigned int buf_count;
    unsigned int n_samples;
} inference_t;

static inference_t inference;

/** Funciones y configuración inicial */
void setup() {
    // Inicializar el LED
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    // Inicializar el servomotor
    garageServo.attach(SERVO_PIN);
    garageServo.write(0); // Inicia en posición cerrada (0°)

    Serial.begin(115200);
    while (!Serial);
    Serial.println("Sistema de Control de Estado por Voz con Servo");
    Serial.println("Estado inicial: ABIERTO");

    run_classifier_init();
    microphone_inference_start(EI_CLASSIFIER_SLICE_SIZE);
}

// Función para actualizar el LED según el estado
void updateLED() {
    unsigned long currentMillis = millis();
    int interval = (currentState == STATE_CLOSED) ? LED_CLOSED_BLINK : LED_OPEN_BLINK;

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState);
    }
}

// Función para procesar los comandos de voz
void processVoiceCommand(const char* command, float confidence) {
    if (millis() - lastCommandTime < DEBOUNCE_TIME || servoMoving) {
        return; // Ignorar si no ha pasado suficiente tiempo o el servo está en movimiento
    }

    if (confidence > CONFIDENCE_THRESHOLD) {
        if (strcmp(command, "cerrar") == 0 && currentState == STATE_OPEN) {
            currentState = STATE_CLOSED;
            Serial.println("Comando reconocido: CERRAR - Cambiando estado a CERRADO");
            stopInference();  // Detenemos la inferencia durante el movimiento
            moveServo(0); // Mover el servo a la posición cerrada
            lastCommandTime = millis();
        }
        else if (strcmp(command, "open") == 0 && currentState == STATE_CLOSED) {
            currentState = STATE_OPEN;
            Serial.println("Comando reconocido: ABRIR - Cambiando estado a ABIERTO");
            stopInference();  // Detenemos la inferencia durante el movimiento
            moveServo(90); // Mover el servo a la posición abierta
            lastCommandTime = millis();
        }
    }
}

// Función para detener la inferencia
void stopInference() {
    microphone_inference_end();
}

// Función para reiniciar la inferencia
void startInference() {
    microphone_inference_start(EI_CLASSIFIER_SLICE_SIZE);
}

// Función para mover el servo de manera lenta a la posición deseada
void moveServo(int targetAngle) {
    int currentAngle = garageServo.read();
    int step = (targetAngle > currentAngle) ? 1 : -1;

    servoMoving = true;
    for (int angle = currentAngle; angle != targetAngle; angle += step) {
        garageServo.write(angle);
        delay(15); // Ajusta este valor para hacer el movimiento más lento o más rápido
    }
    garageServo.write(targetAngle);
    servoMoveStartTime = millis();
}

// Función para verificar si el servo ha terminado de moverse
void checkServoMovement() {
    if (servoMoving && millis() - servoMoveStartTime >= SERVO_DELAY_TIME) {
        servoMoving = false;
        startInference(); // Reiniciamos la inferencia después de que el servo termine de moverse
        if (currentState == STATE_OPEN) {
            lastCommandTime = millis(); // Inicia el tiempo de permanencia en estado abierto
        }
    }
}

void loop() {
    // Si el servo está moviéndose o respetando un tiempo, no hacemos inferencia
    if (servoMoving) {
        checkServoMovement();
        return;
    }

    // Proceso de inferencia y reconocimiento de voz
    bool m = microphone_inference_record();
    if (!m) {
        return;
    }

    signal_t signal;
    signal.total_length = EI_CLASSIFIER_SLICE_SIZE;
    signal.get_data = &microphone_audio_signal_get_data;
    ei_impulse_result_t result = {0};

    EI_IMPULSE_ERROR r = run_classifier_continuous(&signal, &result, debug_nn);
    if (r != EI_IMPULSE_OK) {
        return;
    }

    for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
        if (strcmp(result.classification[ix].label, "Ruido") != 0) {
            processVoiceCommand(result.classification[ix].label, result.classification[ix].value);
        }
    }

    // Verificar movimiento del servo
    checkServoMovement();

    // Actualizar el LED
    updateLED();
}
