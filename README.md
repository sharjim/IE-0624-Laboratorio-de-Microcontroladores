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

_(sharlin completa esta parte)_

