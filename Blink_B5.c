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