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