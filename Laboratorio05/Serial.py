# Lab #5 Sharlin Hernández y Deybin García

# A continuación se desarolla el código que permite la comunicación 
# serial del proyecto.
import serial
import argparse

# Crear un objeto ArgumentParser
parser = argparse.ArgumentParser(
    description='Este script abre la comunicación serial con el microcontrolador para recibir los datos'
)

# Argumentos de la línea de comandos
parser.add_argument(
    '-b', '--baudios',
    type=int, default=115200,
    help="Baudios para la comunicación serial, por defecto es 115200"
)
parser.add_argument(
    '-p', '--puerto',
    type=str, required=True,
    help='Puerto abierto para la comunicación serial, lo que viene después del tty'
)

# Parsear argumentos
args = parser.parse_args()

# Configuración del puerto serial
baud_rate = args.baudios
port_name = '/dev/tty' + args.puerto

try:
    # Abre el puerto serie una vez
    with serial.Serial(port_name, baud_rate) as ser:
        print(f"Conexión abierta en {port_name} a {baud_rate} baudios")
        while True:
            try:
                # Leer e imprimir línea desde el puerto serial
                line = ser.readline().decode().strip()
                print(line)
            except serial.SerialException as e:
                print(f"Error de lectura serial: {e}")
            except UnicodeDecodeError:
                print("Error al decodificar la línea recibida.")
except serial.SerialException as e:
    print(f"No se pudo abrir el puerto serial {port_name}: {e}")
except KeyboardInterrupt:
    print("\nConexión cerrada. Saliendo...")
