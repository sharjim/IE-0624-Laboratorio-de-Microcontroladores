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
