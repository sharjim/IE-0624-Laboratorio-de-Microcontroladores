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