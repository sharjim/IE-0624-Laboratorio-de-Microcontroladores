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