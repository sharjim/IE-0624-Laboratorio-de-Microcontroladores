# Proyecto Sismógrafo con STM32

Este proyecto implementa un **sismógrafo** utilizando un microcontrolador **STM32**, integrando un giroscopio, pantalla TFT, ADC y USART para monitoreo en tiempo real. A continuación se describe cómo cada componente y las librerías relacionadas contribuyen a esta implementación.

## Descripción General del Proyecto

El sistema mide la aceleración en los ejes **X, Y, Z** utilizando un giroscopio **L3GD20** y muestra los datos en tiempo real en una pantalla TFT. Adicionalmente, el sistema monitorea el voltaje de la batería mediante el **ADC** y lo despliega en la pantalla, activando un LED si el voltaje es inferior a un umbral. También se permite la transmisión de datos por **USART** al presionar un botón.

## Componentes y Librerías Utilizadas

### Giroscopio (`gyr_spi.c` y `gyr_spi.h`)
- **Función:** Controla el giroscopio L3GD20 mediante SPI.
- **Uso:**
  - `gyr_setup()`: Configura el giroscopio.
  - `gyr_readX()`, `gyr_readY()`, `gyr_readZ()`: Lee los valores de los ejes X, Y y Z.

### Pantalla TFT (`lcd-spi.h` y SPI Configuration)
- **Función:** Controla la pantalla TFT a través del bus SPI.
- **Uso:**
  - `lcd_spi_init()`: Inicializa la pantalla.
  - `lcd_show_frame()`: Actualiza el contenido de la pantalla con los datos más recientes.

### Librería Gráfica (`gfx.h`)
- **Función:** Dibuja elementos gráficos en la pantalla.
- **Uso:**
  - `gfx_init()`: Configura las funciones gráficas.
  - `gfx_fillScreen()`, `gfx_setTextSize()`, `gfx_setCursor()`, `gfx_puts()`: Muestran información en pantalla.

### SDRAM (`sdram.h`)
- **Función:** Gestiona la memoria externa SDRAM para almacenamiento temporal de imágenes y datos.
- **Uso:**
  - `sdram_init()`: Inicializa la SDRAM para almacenamiento de buffers de pantalla.

### Comunicación Serial (`console.h` y USART)
- **Función:** Gestiona la comunicación mediante USART para transmisión de datos.
- **Uso:**
  - `usart_setup()`: Configura la USART.
  - `usart_send_string()`: Envía datos por la consola serial.

### GPIO (LEDs y Botón)
- **Función:** Controla los pines GPIO para los LEDs y el botón.
- **Uso:**
  - `gpio_setup_for_ledR()`, `gpio_setup_for_ledG()`: Configuran los LEDs.
  - `is_button_pressed()`: Detecta si se presionó el botón para enviar datos por USART.

### ADC (Monitoreo del Voltaje)
- **Función:** Convierte señales analógicas en digitales para medir el voltaje de la batería.
- **Uso:**
  - `adc_setup()`: Configura el ADC.
  - `read_adc_average()`, `adc_to_voltage()`: Miden el voltaje y lo convierten para su visualización.

## Flujo del Programa

1. **Configuración de periféricos:** Se inicializan la pantalla TFT, el giroscopio, la SDRAM, el ADC y la USART.
2. **Lectura de datos:** Se obtienen las lecturas del giroscopio y se muestran en pantalla.
3. **Monitoreo del voltaje:** El sistema mide el voltaje de la batería y activa un LED si es inferior al umbral.
4. **Transmisión de datos:** Si se presiona un botón, los datos se envían mediante USART.
5. **Actualización de la pantalla:** El contenido se actualiza periódicamente en la pantalla.

## Conclusión

Este proyecto muestra cómo integrar varios periféricos en un sistema embebido usando STM32:
- **Lectura de sensores** con giroscopio.
- **Visualización gráfica** mediante una pantalla TFT.
- **Control y monitoreo** mediante LEDs y botones.
- **Comunicación serial** mediante USART.
- **Gestión de memoria** con SDRAM para buffering.

Este sismógrafo es un ejemplo práctico de cómo utilizar múltiples módulos y periféricos en sistemas embebidos para tareas de monitoreo en tiempo real.
