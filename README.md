# Proyecto: Simón Dice

Este proyecto consiste en la implementación del juego "Simón Dice" utilizando un microcontrolador ATtiny4313, donde el jugador debe seguir una secuencia de luces parpadeantes y reproducirla correctamente presionando los botones correspondientes. A medida que el jugador avanza, la secuencia se vuelve más rápida y compleja.

## Organización de Trabajo

Junto con la compañera Sharlin Hernández Sánchez, Deybin David García García desarrolló el archivo `main.c`, incluyendo las variables necesarias y las funciones que se debían implementar. Nos dividimos las tareas tomando en cuenta el tamaño aproximado de cada función.

### Funciones desarrolladas por Deybin David García García:

#### 1. `ISR(PCINT1_vect)`
- **Descripción**: Esta función maneja la interrupción en el pin A0 (PCINT8), detectando un flanco ascendente en el botón conectado a dicho pin. Utiliza la variable `UP` para asegurarse de que solo se detecta el flanco positivo, evitando múltiples detecciones debido a los rebotes del botón.
- **Lógica**: La variable `UP` alterna entre 0 y 1, garantizando que solo se detecte el flanco ascendente y, de esta forma, se marca la pulsación del botón cuando `UP` es 1.

#### 2. `ISR(INT1_vect)`
- **Descripción**: Esta función maneja la interrupción externa en el pin INT1 (D3). Cuando se detecta un cambio en este pin, se eleva una bandera (`button1`) para indicar que el botón conectado ha sido presionado.
- **Lógica**: Similar a la interrupción en INT0, esta función marca que el botón asociado ha sido presionado, permitiendo una respuesta rápida sin interferir en el ciclo principal del programa.

#### 3. `Blink_F()`
- **Descripción**: Parpadea todos los LEDs conectados a los pines B2, B3, B4 y B5 tres veces, con un retardo de 5 segundos entre cada ciclo de encendido y apagado.
- **Lógica**: La función enciende y apaga los LEDs tres veces, creando una señal visual clara con un intervalo de tiempo bien definido.

#### 4. `Blink_B4()`
- **Descripción**: Controla el parpadeo del LED conectado al pin B4, encendiéndolo y apagándolo con un retardo de 6 segundos.
- **Lógica**: Proporciona una señal visual cuando se presiona el botón correspondiente al LED de B4, mostrando que la acción del jugador ha sido detectada.

#### 5. `Blink_B5()`
- **Descripción**: Controla el parpadeo del LED conectado al pin B5, similar a la función `Blink_B4()`, pero para el LED en el pin B5.
- **Lógica**: El LED se enciende y apaga con un retardo definido para indicar la interacción del usuario.

#### 6. `ON_Leds(int *Array_R2)`
- **Descripción**: Enciende una secuencia de LEDs de acuerdo a los valores en un arreglo. Cada valor indica qué LED se debe encender (0 para B2, 1 para B3, 2 para B4, y 3 para B5).
- **Lógica**: La función recorre el arreglo `Array_R2`, encendiendo los LEDs correspondientes en secuencia con un retardo entre cada encendido, proporcionando la retroalimentación visual para la secuencia del juego.

### Funciones desarrolladas por Sharlin Hernández Sánchez:

#### 7. Blink_I()
- **Descripción**: Parpadea los LEDs conectados a los pines B2, B3, B4 y B5 del microcontrolador. El parpadeo ocurre dos veces, con un tiempo de encendido y apagado de 5 segundos para cada ciclo, seguido de una pausa final de 7 segundos.
- **Lógica**: La función utiliza un bucle para encender y apagar los LEDs dos veces. Durante cada iteración, los LEDs se encienden con la instrucción PORTB = 0b00111100 y se apagan con PORTB = 0b00000000, controlando los pines correspondientes. Un retardo de 5000 ms se aplica después de cada encendido y apagado. Al finalizar, se añade una pausa de 7 segundos.

### 8. ISR(PCINT1_vect)
-**Descripción:** Esta función maneja la interrupción del pin A0 (PCINT8), detectando un flanco ascendente del botón conectado a ese pin. Cuando se detecta este flanco, se activa la bandera button3. La variable UP se utiliza para asegurar que solo se detecta el flanco positivo, evitando múltiples detecciones causadas por rebotes del botón.
-**Lógica:** La función alterna el valor de la variable UP entre 0 y 1 para distinguir correctamente entre los estados de reposo y de activación del botón. Cuando UP está en 0, indica que se ha detectado un flanco ascendente y se marca como 1. Luego, cuando el valor de UP cambia a 1, la bandera button3 se activa, indicando que el botón ha sido presionado, y UP se reinicia a 0 para el próximo ciclo de detección.

### 9. ISR(PCINT1_vect)
-**Descripción:** Esta función maneja la interrupción en el pin A0 (PCINT8) del microcontrolador. Detecta un flanco ascendente en el botón conectado al pin A0 y, cuando esto ocurre, activa la bandera button3. La función utiliza la variable UP para asegurarse de que solo se detecta el flanco positivo, evitando que el rebote del botón genere múltiples interrupciones.
-**Lógica:** La función alterna el valor de la variable UP entre 0 y 1 para detectar el flanco ascendente. Cuando UP es igual a 0, se detecta el flanco positivo y se establece UP = 1. Si UP es igual a 1, se activa la bandera button3 indicando que el botón fue presionado, y UP se reinicia a 0 para permitir la detección del siguiente flanco ascendente.

### 10. Blink_I()
-**Descripción:** Esta función controla el parpadeo de los LEDs conectados a los pines B2, B3, B4 y B5 del microcontrolador. Los LEDs parpadean dos veces, con un tiempo de encendido y apagado de 5 segundos en cada ciclo. Al finalizar el segundo parpadeo, hay una pausa adicional de 7 segundos.
-**Lógica:** La función utiliza un bucle para encender y apagar los LEDs dos veces. Durante cada iteración del bucle, los LEDs se encienden configurando los bits correspondientes de PORTB a 1 (0b00111100) para activar los pines B2, B3, B4 y B5. Luego, los LEDs se apagan configurando PORTB a 0 (0b00000000). Un retardo de 5000 ms se introduce después de cada encendido y apagado, y una pausa final de 7000 ms se aplica después del último ciclo de parpadeo.

### 11. Blink_F()
-**Descripción:** Esta función parpadea los LEDs conectados a los pines B2, B3, B4 y B5 del microcontrolador tres veces, con un retardo de 5 segundos entre cada encendido y apagado. El propósito es proporcionar una señal visual clara mediante un patrón de parpadeo.
-**Lógica:** La función utiliza un bucle para encender y apagar los LEDs tres veces. Durante cada iteración del bucle, los pines B2, B3, B4 y B5 se activan al establecer los bits correspondientes en PORTB a 1 (0b00111100), lo que enciende los LEDs. Luego, los LEDs se apagan configurando PORTB a 0 (0b00000000). Después de cada encendido y apagado, se introduce un retardo de 5000 ms (5 segundos) para mantener los LEDs encendidos y apagados por igual tiempo. Finalmente, hay una pausa opcional de 5 segundos después del tercer ciclo de parpadeo.

### 12. ON_Leds(int *Array_R2)
**-Descripción:** Esta función enciende una secuencia de LEDs de acuerdo con los valores en un arreglo (Array_R2). Cada valor en el arreglo indica qué LED se debe encender, donde 0 corresponde al LED en B2, 1 al LED en B3, 2 al LED en B4, y 3 al LED en B5.
**-Lógica:** La función recorre el arreglo Array_R2, el cual contiene una secuencia de valores que determinan qué LED se debe encender en cada paso. Utiliza una estructura switch para determinar qué LED encender en función del valor actual del arreglo. Después de encender el LED correspondiente (llamando a una función específica como Blink_B2(), Blink_B3(), etc.), se introduce un retardo de 2000 ms entre el encendido de cada LED, proporcionando una secuencia clara y visible de luces

