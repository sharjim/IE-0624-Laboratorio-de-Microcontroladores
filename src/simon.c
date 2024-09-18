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
=======
{
  // Configuración inicial del sistema
  _delay_ms(5000);

  sei(); // Habilita las interrupciones globales

  DDRB = 0b00111100; // Configuración de los pines B2, B3, B4 y B5 como salidas para los 4 LEDs

  GIMSK |= (1 << INT0) | (1 << INT1);   // Habilita las interrupciones externas en D2 y D3
  GIMSK |= (1 << PCIE0) | (1 << PCIE1); // Habilita las interrupciones de cambio de pin en B0 y A0

  PCMSK |= 0b00000001;  // Habilita la interrupción en el pin B0
  PCMSK1 |= 0b00000001; // Habilita la interrupción en el pin A0

  _delay_ms(1000); // Pausa antes de mostrar las primeras luces del juego

  char state = START;      // Variable para el estado actual
  char next_state = START; // Variable para el siguiente estado

  while (1)
  { // Bucle principal del juego

    Contador = (Contador + 1) % 4;

    state = next_state;

    switch (state)
    {

    // ###################################################################################
    case (START): // Lógica del estado de espera
      /*
       * Lógica: El juego permanece en este estado hasta que se presione un botón.
       * Se inicializan contadores y se prepara la secuencia inicial.
       * Cuando se presiona un botón, se genera la secuencia aleatoria.
       */

      // Usamos el operador módulo para alternar entre los valores de `Count`
      Count = (Count + 2) % 4;

      // Verificación de si se ha presionado algún botón
      if (button0 || button1 || button2 || button3)
      {
        Blink_I();
        button0 = button1 = button2 = button3 = 0; // Reiniciar botones
        next_state = INICIO_SEC;
        Array_R = 1; // Señal de que se debe generar una secuencia aleatoria
      }
      else
      {
        next_state = START; // Permanecer en el estado START
      }

      // Generar la secuencia aleatoria si Array_R está activado
      if (Array_R)
      {
        Array_G[0] = Count;
        Array_G[1] = Contador;

        // Utiliza un switch-case para simplificar la lógica de asignación de Array_G
        switch (Contador)
        {
        case 1:
          Array_G[2] = 3;
          Array_G[3] = 0;
          break;

        case 2:
          Array_G[2] = 1;
          Array_G[3] = 3;
          break;

        case 3:
          Array_G[2] = 0;
          Array_G[3] = 2;
          break;

        case 0:
        default:
          Array_G[2] = 3;
          Array_G[3] = 2;
          break;
        }

        Contador = (Contador + 1) % 4; // Mover el contador dentro del rango adecuado
      }
      break;

    // ###################################################################################
    case (INICIO_SEC): // Lógica de mostrar la secuencia
      /*
       * Lógica: En este estado, se muestra la secuencia de luces que el jugador debe memorizar.
       * La transición a este estado ocurre inmediatamente después de que el jugador presiona un botón.
       * La secuencia se muestra para que el jugador la reproduzca.
       */
      ON_Leds(Array_G);
      next_state = INICIALIZACION_ENT;
      break;

    // ###################################################################################
    case (INICIALIZACION_ENT): // Lógica de entrada del jugador
      /*
       * Lógica: El jugador debe ingresar la secuencia de botones que corresponde a la secuencia de LEDs mostrada.
       * Se verifica si el jugador ha presionado suficientes botones para completar la ronda.
       */

      // Verifica si el jugador ha ingresado la secuencia completa
      if (C_Button == Ronda)
      {
        next_state = VS; // Transición al estado de verificación
        C_Button = 0;    // Reiniciar contador de botones para la próxima ronda
      }
      else
      {
        next_state = INICIALIZACION_ENT; // Permanece en el estado si no ha terminado la secuencia

        // Verifica si alguno de los botones fue presionado
        if (button0 || button1 || button2 || button3)
        {
          // Función para manejar cada botón y su correspondiente LED
          int button_pressed = -1;

          if (button0 == 1)
          {
            button_pressed = 0;
            button0 = 0;
            Blink_B2();
          }
          else if (button1 == 1)
          {
            button_pressed = 1;
            button1 = 0;
            Blink_B3();
          }
          else if (button2 == 1)
          {
            button_pressed = 2;
            button2 = 0;
            Blink_B4();
          }
          else if (button3 == 1)
          {
            button_pressed = 3;
            button3 = 0;
            Blink_B5();
          }

          // Si algún botón fue presionado, actualizar el array de inputs y el contador
          if (button_pressed != -1)
          {
            Array_I[C_Button] = button_pressed; // Almacena el botón presionado en la secuencia de entrada
            C_Button++;                         // Incrementa el contador de botones presionados
          }
        }
      }
      break;

    // ###################################################################################
    case (VS): // Lógica de verificación
      /*
       * Lógica: En este estado, se comparan las entradas del jugador con la secuencia generada.
       * Se asume inicialmente que el jugador ha ingresado la secuencia correcta (Win_R = 1)
       * y luego se realiza la verificación. Si alguna entrada no coincide con la secuencia,
       * se marca Win_R como 0 y se pasa al estado de pérdida.
       */

      // Asumimos inicialmente que el jugador ha acertado la secuencia
      Win_R = 1;

      // Comparamos las entradas del jugador con la secuencia generada
      for (int x = 0; x < Ronda; x++)
      {
        if (Array_G[x] != Array_I[x])
        {
          Win_R = 0; // Si cualquier valor no coincide, se marca como pérdida
          break;     // Termina la comparación una vez que se detecta un error
        }
      }

      // Si la secuencia fue correcta, pasa al estado de victoria; de lo contrario, al de derrota
      next_state = (Win_R == 1) ? WINNER : LOSER;

      // Se resetea la bandera de victoria para la próxima ronda
      Win_R = 0;

      break;

      // ###################################################################################
    case (WINNER): // Lógica de victoria
      /*
       * Lógica: Cuando el jugador acierta la secuencia, se activa una animación de luces para celebrar.
       * Luego, se incrementa la dificultad añadiendo un nuevo valor aleatorio a la secuencia
       * y se avanza a la siguiente ronda.
       */

      // Añadir un nuevo valor aleatorio a la secuencia para la siguiente ronda
      Array_G[Ronda] = Contador;
      Ronda++; // Incrementar el nivel de dificultad (más LEDs en la próxima ronda)

      // Celebración de victoria con luces parpadeando
      _delay_ms(5000);    // Retardo antes de iniciar la animación
      PORTB = 0b00111100; // Enciende los LEDs conectados a B2, B3, B4, y B5
      _delay_ms(10000);   // Mantiene los LEDs encendidos durante 10 segundos
      PORTB = 0b00000000; // Apaga los LEDs
      _delay_ms(10000);   // Mantiene los LEDs apagados durante 10 segundos

      // Volver al estado inicial para iniciar una nueva secuencia
      next_state = INICIO_SEC;
      break;

    // ###################################################################################
    case (LOSER): // Lógica de derrota
      /*
       * Lógica: Si el jugador falla la secuencia, el juego entra en un ciclo infinito
       * donde se indica la derrota parpadeando los LEDs tres veces. El juego no avanza de este estado.
       */
      Blink_F();
      _delay_ms(15000);
      next_state = LOSER;
      break;
    }
  }
}

/*
 * Función: ISR(PCINT0_vect)
 * Descripción: Esta rutina de interrupción maneja los cambios de estado del pin B0 (PCINT0). Cuando se detecta
 * un flanco ascendente, se activa la bandera `button2`. Utiliza una variable `DOWN` para asegurarse de que
 * solo se detecta el flanco ascendente y se evita la detección de rebotes.
 * Lógica: Controla las transiciones de flanco del botón, asegurando que solo se detecta el flanco positivo
 * utilizando una bandera para diferenciar entre flancos altos y bajos.
 */

ISR(PCINT0_vect) // Detecta la interrupción en B0 (PCINT0)
{
  // Alterna la bandera DOWN para controlar el flanco ascendente
  if (DOWN == 0)
  {
    DOWN = 1; // Marca que se ha detectado el flanco ascendente
  }
  else
  {
    button2 = 1; // Se activa la bandera para indicar que el botón fue presionado
    DOWN = 0;    // Restablece la bandera para el próximo ciclo de detección
  }
}

/*
 * Función: ISR(PCINT1_vect)
 * Descripción: Maneja la interrupción en el pin A0 (PCINT8). Detecta un flanco ascendente en el botón conectado
 * al pin A0 y activa la bandera `button3`. Utiliza la variable `UP` para asegurarse de que solo se detecta el flanco positivo,
 * evitando múltiples detecciones debido a rebotes.
 * Lógica: Alterna el valor de `UP` entre 0 y 1 para manejar correctamente la detección de flancos, asegurando que solo
 * se detecta el flanco ascendente y se marca la pulsación del botón.
 */

ISR(PCINT1_vect) // Detecta la interrupción en A0 (PCINT8)
{
  // Alterna la bandera UP para controlar el flanco ascendente
  if (UP == 0)
  {
    UP = 1; // Marca que se ha detectado el flanco ascendente
  }
  else
  {
    button3 = 1; // Se activa la bandera para indicar que el botón fue presionado
    UP = 0;      // Restablece la bandera para el próximo ciclo de detección
  }
}

/*
 * Función: ISR(INT0_vect)
 * Descripción: Maneja la interrupción externa en INT0. Cuando se detecta un cambio en el pin D2 (INT0),
 * la función eleva una bandera (`button0`) para indicar que el botón correspondiente ha sido presionado.
 * Lógica: Esta rutina de servicio se activa en cada evento de interrupción en INT0. La bandera `button0`
 * se usa para señalar que el botón conectado a D2 fue activado, permitiendo que otras partes del programa
 * respondan a esta acción.
 */

ISR(INT0_vect) // Detecta la interrupción en INT0 (D2)
{
  button0 = 1; // Se eleva la bandera que indica que el botón conectado a INT0 fue presionado
}

/*
 * Función: ISR(INT1_vect)
 * Descripción: Maneja la interrupción externa en INT1. Cuando se detecta un cambio en el pin D3 (INT1),
 * la función eleva una bandera (`button1`) para indicar que el botón correspondiente ha sido presionado.
 * Lógica: Similar a la interrupción en INT0, esta función eleva una bandera para notificar que
 * el botón conectado a INT1 (D3) ha sido activado. Este mecanismo permite la detección rápida de la
 * interacción del usuario sin ocupar el ciclo principal del programa.
 */

ISR(INT1_vect) // Detecta la interrupción en INT1 (D3)
{
  button1 = 1; // Se eleva la bandera que indica que el botón conectado a INT1 fue presionado
}

/*
 * Función: Blink_I()
 * Descripción: Parpadea todas las luces conectadas a los pines B2, B3, B4, y B5 dos veces con un retardo de 5 segundos entre
 * cada encendido y apagado, y una pausa final de 7 segundos.
 * Lógica: Enciende y apaga los LEDs dos veces con un retardo definido para crear una señal visual.
 */

void Blink_I()
{
  int DELAY = 5000; // Tiempo de retardo para cada ciclo de encendido/apagado

  // Bucle para parpadear las luces dos veces
  for (int i = 0; i < 2; i++)
  {
    PORTB = 0b00111100; // Enciende los LEDs conectados a B2, B3, B4, y B5
    _delay_ms(DELAY);   // Mantiene los LEDs encendidos durante el tiempo definido

    PORTB = 0b00000000; // Apaga los LEDs
    _delay_ms(DELAY);   // Mantiene los LEDs apagados durante el tiempo definido
  }

  // Pausa final después del segundo parpadeo
  _delay_ms(7000);
}

/*
 * Función: Blink_F()
 * Descripción: Parpadea todas las luces conectadas a los pines B2, B3, B4, y B5 tres veces con un retardo de 5 segundos
 * entre cada ciclo de encendido y apagado.
 * Lógica: Enciende y apaga los LEDs tres veces con un retardo definido para crear una señal visual clara.
 */

void Blink_F()
{
  int DELAY = 5000; // Tiempo de retardo para cada ciclo de encendido/apagado

  // Bucle para parpadear las luces tres veces
  for (int i = 0; i < 3; i++)
  {
    PORTB = 0b00111100; // Enciende los LEDs conectados a B2, B3, B4, y B5
    _delay_ms(DELAY);   // Mantiene los LEDs encendidos durante el tiempo definido

    PORTB = 0b00000000; // Apaga los LEDs
    _delay_ms(DELAY);   // Mantiene los LEDs apagados durante el tiempo definido
  }

  // Pausa final después del tercer parpadeo (opcional, si es necesario)
  _delay_ms(DELAY);
}

/*
 * Función: Blink_B2()
 * Descripción: Esta función controla el LED conectado al pin B2 para que parpadee una vez.
 * Lógica: El LED en B2 se enciende y luego se apaga tras un retardo, indicando la interacción con el botón correspondiente.
 * Esta función se utiliza para proporcionar retroalimentación visual al jugador cuando se presiona el botón asociado al LED en B2.
 */

void Blink_B2()
{
  int DELAY = 6000; // Define el tiempo de retardo para el parpadeo en milisegundos

  // Enciende el LED conectado a B2
  PORTB = 0b00000100;
  _delay_ms(DELAY); // Mantiene el LED encendido por el tiempo definido en DELAY

  // Apaga el LED
  PORTB = 0b00000000;
  _delay_ms(DELAY); // Mantiene el LED apagado por el tiempo definido en DELAY
}

/*
 * Función: Blink_B3()
 * Descripción: Controla el LED conectado al pin B3 para que parpadee una vez.
 * Lógica: El LED en el pin B3 se enciende y se apaga tras un retardo. Esta función proporciona retroalimentación visual
 * cuando se presiona el botón correspondiente al LED de B3, indicándole al jugador que su acción ha sido detectada.
 */

void Blink_B3()
{
  int DELAY = 6000; // Define el tiempo de retardo para el parpadeo en milisegundos

  // Enciende el LED conectado al pin B3
  PORTB = 0b00001000;
  _delay_ms(DELAY); // Mantiene el LED encendido durante el tiempo especificado por DELAY

  // Apaga el LED conectado al pin B3
  PORTB = 0b00000000;
  _delay_ms(DELAY); // Mantiene el LED apagado durante el tiempo especificado por DELAY
}

/*
 * Función: Blink_B4()
 * Descripción: Controla el LED conectado al pin B4 para que parpadee una vez.
 * Lógica: El LED en el pin B4 se enciende y apaga tras un retardo. Esta función sirve como retroalimentación visual
 * cuando el botón asociado al LED de B4 es presionado, indicando que la entrada del jugador ha sido reconocida.
 */

void Blink_B4()
{
  int DELAY = 6000; // Define el tiempo de retardo para el parpadeo en milisegundos

  // Retardo inicial antes de encender el LED
  _delay_ms(DELAY);

  // Enciende el LED conectado al pin B4
  PORTB = 0b00010000;
  _delay_ms(DELAY); // Mantiene el LED encendido durante el tiempo especificado por DELAY

  // Apaga el LED
  PORTB = 0b00000000;
  _delay_ms(DELAY); // Mantiene el LED apagado durante el tiempo especificado por DELAY
}

/*
 * Función: Blink_B5()
 * Descripción: Controla el LED conectado al pin B5 para que parpadee una vez.
 * Lógica: El LED en el pin B5 se enciende y apaga tras un retardo. Esta función proporciona retroalimentación visual
 * cuando se presiona el botón correspondiente al LED de B5, indicando que la acción del jugador ha sido detectada.
 */

void Blink_B5()
{
  int DELAY = 6000; // Define el tiempo de retardo para el parpadeo en milisegundos

  // Enciende el LED conectado al pin B5
  PORTB = 0b00100000;
  _delay_ms(DELAY); // Mantiene el LED encendido durante el tiempo especificado por DELAY

  // Apaga el LED
  PORTB = 0b00000000;
  _delay_ms(DELAY); // Mantiene el LED apagado durante el tiempo especificado por DELAY
}

/*
 * Función: ON_Leds(int *Array_R2)
 * Descripción: Enciende una secuencia de LEDs según un arreglo de valores. Cada valor en el arreglo indica
 * qué LED se debe encender (0 para B2, 1 para B3, 2 para B4, y 3 para B5).
 * Lógica: Recorre el arreglo `Array_R2` y llama a la función correspondiente para encender el LED asociado,
 * con un retardo definido entre cada encendido.
 */

void ON_Leds(int *Array_R2)
{
  int DELAY = 2000; // Tiempo de retardo entre el encendido de cada LED

  // Recorre el arreglo Array_R2 que contiene la secuencia de LEDs a encender
  for (int i = 0; i < Ronda; i++)
  {
    _delay_ms(DELAY); // Retardo antes de encender el LED

    // Según el valor de Array_R2[i], se enciende el LED correspondiente
    switch (Array_R2[i])
    {
    case 0:
      Blink_B2(); // Parpadeo del LED conectado a B2
      break;

    case 1:
      Blink_B3(); // Parpadeo del LED conectado a B3
      break;

    case 2:
      Blink_B4(); // Parpadeo del LED conectado a B4
      break;

    case 3:
      Blink_B5(); // Parpadeo del LED conectado a B5
      break;

    default:
      break; // Si el valor no coincide con ninguno, no hace nada
    }
  }
}

/*
 * Función: Settings_Timers()
 * Descripción: Configura los parámetros iniciales del temporizador Timer0 en modo normal.
 * Este temporizador cuenta desde 0 hasta 255, y se utiliza un prescaler de 1024 para dividir la frecuencia de reloj.
 * También habilita las interrupciones asociadas con el Timer0 cuando el conteo desborda.
 * Lógica: La función configura el Timer0 en modo normal y ajusta el prescaler para controlar la velocidad del conteo.
 * Cuando el temporizador alcanza el valor máximo (255), genera una interrupción, lo que permite ejecutar código
 * en intervalos específicos de tiempo.
 */

void Settings_TTimers()
{
  TCCR0A = 0x00; // Configura el Timer0 en modo normal (contando desde 0 hasta 255)

  TCCR0B = 0x00;                       // Inicializa el registro TCCR0B
  TCCR0B |= (1 << CS00) | (1 << CS02); // Configura el prescaler a 1024 (divide la frecuencia de reloj por 1024)

  TCNT0 = 0; // Reinicia el conteo del Timer0 a 0

  TIMSK |= (1 << TOIE0); // Habilita la interrupción por desbordamiento del Timer0 (cuando alcanza 255)
}

/*
 * Función: Timer()
 * Descripción: Utiliza el Timer0 para contar un número específico de desbordamientos, lo que permite generar una
 * pausa temporal. La función cuenta 62 desbordamientos del Timer0, lo que corresponde a un cierto retardo
 * basado en la configuración del temporizador.
 * Lógica: Cada vez que el Timer0 se desborda (al contar de 0 a 255), se detecta levantando la bandera TOV0.
 * La función espera hasta que la bandera se levante, luego la limpia y continúa el conteo de desbordamientos
 * hasta alcanzar el número deseado.
 */

void Timer()
{
  unsigned int i = 0; // Inicializa el contador de desbordamientos
  while (i <= 62)     // Bucle para contar 62 desbordamientos del Timer0
  {
    // Espera a que la bandera de desbordamiento TOV0 se active (indicando que el Timer0 ha contado de 0 a 255)
    while ((TIFR & (1 << TOV0)) == 0)
      ; // Espera mientras la bandera TOV0 no esté levantada

    TIFR |= (1 << TOV0); // Limpia la bandera de desbordamiento TOV0 para el siguiente ciclo
    i++;                 // Incrementa el contador de desbordamientos
  }
}

// ###########################################################################################
