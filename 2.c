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

