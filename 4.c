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
