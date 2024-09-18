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
