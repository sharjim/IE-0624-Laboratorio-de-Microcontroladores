/*
 * Universidad de Costa Rica
 * Facultad de Ingeniería
 * Escuela de Ingeniería Eléctrica
 * IE-0624 Laboratorio de Microcontroladores
 * Grupo 001 - MSc. Marco Villalta Fallas
 * Laboratorio #2 - GPIOs, Timers y FSM
 *
 * Juego de Memoria: "Simón Dice"
 *
 * Estudiantes:
 * - Sharlin Hernández Sánchez, Carnet B82831
 * - Deybin García García, Carnet B93171
 * 
 * Descripción:
 * Este programa implementa el juego de memoria "Simón Dice" utilizando un microcontrolador ATtiny4313, 
 * LEDs, botones y otros componentes electrónicos. El objetivo del juego es que el jugador memorice y 
 * reproduzca una secuencia de luces que se incrementa en dificultad conforme avanza el juego. 
 * El tiempo de encendido de los LEDs se reduce a medida que la secuencia se hace más larga. 
 * El juego finaliza cuando el jugador comete un error en la secuencia.
 *
 * Funcionalidades:
 * - El juego utiliza interrupciones para la lectura de botones y temporización de LEDs mediante timers.
 * - Al inicio del juego, todos los LEDs parpadean dos veces para indicar el comienzo.
 * - Los LEDs se encienden en una secuencia aleatoria que el jugador debe memorizar y reproducir.
 * - La secuencia comienza con 4 LEDs encendidos durante 2 segundos cada uno, y la duración de los LEDs
 *   se reduce en 200 ms por cada incremento de la secuencia.
 * - Si el jugador falla, el juego indica el error parpadeando todos los LEDs tres veces y luego finaliza.
 *
 * Requisitos del laboratorio:
 * - Utilizar GPIOs, interrupciones, timers y un modelo de máquina de estados (FSM).
 * - La temporización de los LEDs se debe realizar utilizando los temporizadores del microcontrolador.
 * - El juego debe permitir la lectura de botones y el control del temporizador mediante interrupciones.
 *
 * Curso: IE-0624 Laboratorio de Microcontroladores
 * II - Semestre 2024 - RRF
 * Fecha de entrega: 18 de septiembre de 2024, 23:59
 *
 * Instrucciones adicionales:
 * - Utilizar git para registrar el avance del proyecto.
 * - Entregar un archivo comprimido con un informe en PDF y los archivos de código fuente.
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

// Declaración de los estados del juego
#define START 0 // Estado de espera para que el jugador presione un botón e inicie la partida
                // -> El estado siguiente es siempre INICIO_SEC

#define INICIO_SEC 1 // En este estado, se muestra la secuencia de luces que el jugador debe recordar
                     // -> El siguiente estado es siempre INICIALIZACION_ENT

#define INICIALIZACION_ENT 2 // Aquí se detecta qué botones han sido presionados y se cuenta cuántos fueron
                             // -> El estado siguiente es siempre VS

#define VS 3 // Este estado compara las entradas del jugador con la secuencia generada por el juego
             // -> Puede transicionar a WINNER o LOSER según el resultado

#define WINNER 4 // Si el jugador acierta la secuencia, este estado da la señal de victoria
                 // -> Luego del feedback, el estado vuelve a INICIO_SEC

#define LOSER 5 // Si el jugador falla, este estado indica la derrota con un patrón de parpadeo
                // -> El juego se mantiene en este estado parpadeando LEDs hasta que el sistema se apaga

// Declaración de variables necesarias para el funcionamiento del juego
int button0 = 0;
int button1 = 0;
int button2 = 0;
int button3 = 0; // Variables que almacenan el estado de cada botón
int DOWN = 0;
int UP = 0;       // Previene errores de detección al presionar los botones tanto en el flanco alto como en el bajo
int C_Button = 0; // Lleva la cuenta de cuántas veces se han presionado los botones
int Array_R = 0;  // Indicador que se activa cuando se debe rellenar el array con valores aleatorios
int Contador = 2;
int Count = 0;        // Variables usadas para generar números aleatorios
int Win_R = 0;        // Bandera que se activa al ganar una ronda
int Ronda = 4;        // Indica el número de la ronda actual, comenzando en 4 porque se encienden 4 LEDs aleatorios en la primera ronda
int Array_G[50] = {}; // Array para almacenar la secuencia generada por el juego
int Array_I[50] = {}; // Array para almacenar la secuencia ingresada por el jugador
int Cuenta = 0;       // Variable que cuenta los aciertos del jugador

void Timer();
void Settings_Timers();
void ON_Leds(int *Array_R2);
void Blink_B5();
void Blink_B4();
void Blink_B3();
void Blink_B2();
void Blink_F();
void Blink_I();
ISR(INT1_vect);
ISR(INT0_vect);
ISR(PCINT1_vect);
ISR(PCINT0_vect);

int main(void)
{}
