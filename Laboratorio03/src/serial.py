#!/usr/bin/python3
import serial

# Configuración para el puerto serie.
# Asegúrate de tener permisos para acceder al puerto.
try:
    ser = serial.Serial(
        port='/tmp/ttyS1',
        baudrate=9600,
        parity=serial.PARITY_NONE,
        stopbits=serial.STOPBITS_ONE,
        bytesize=serial.EIGHTBITS,
        timeout=0
    )
except serial.SerialException as e:
    print(f"Error al conectar con el puerto serie: {e}")
    exit(1)

# Abre el archivo 'datos.csv' en modo escritura.
try:
    with open('datos.csv', 'w') as f:
        print(f"Conectado a: {ser.portstr}")
        
        # Ciclo infinito para leer datos del puerto serie.
        while True:
            try:
                data = ser.read()  # Lee los datos del puerto serie
                if data:  # Si hay datos
                    c = data.decode('utf-8')  # Decodifica el byte a carácter
                    print(c, end="")  # Imprime el carácter en la consola.
                    f.write(c)  # Escribe el carácter en el archivo.
                    f.flush()  # Asegura que los datos se escriban inmediatamente.
            except KeyboardInterrupt:
                print("\nEl usuario ha terminado el programa.")
                break
            except Exception as e:
                print(f"Error al leer datos del puerto serie: {e}")
                break
finally:
    # Cierra las conexiones (puerto serie y archivo) al finalizar.
    if ser.is_open:
        ser.close()
    print("Conexión cerrada.")
