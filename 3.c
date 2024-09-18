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

