import serial
import json
import time
import paho.mqtt.client as mqtt

# Configuración del puerto serial
SERIAL_PORT = 'COM6'
BAUD_RATE = 115200
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

# Configuración de ThingsBoard Cloud
THINGSBOARD_HOST = 'mqtt.thingsboard.cloud'
ACCESS_TOKEN = 'A3RevnUu32MGJxms4PHN'

# Inicializa el cliente MQTT
client = mqtt.Client()
client.username_pw_set(ACCESS_TOKEN)

# Habilita el logger para depuración
client.enable_logger()

# Callback para verificar conexión
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("Conectado exitosamente a ThingsBoard.")
    else:
        print(f"Error de conexión: {rc}")

client.on_connect = on_connect

# Callback para verificar publicación
def on_publish(client, userdata, mid):
    print(f"Datos publicados con ID: {mid}")

client.on_publish = on_publish

# Conecta al servidor MQTT con keepalive
client.connect(THINGSBOARD_HOST, 1883, keepalive=30)

# Inicia el loop del cliente MQTT en segundo plano
client.loop_start()

def enviar_datos(x, y, z, voltage):
    telemetry = {
        "X": x,
        "Y": y,
        "Z": z,
        "voltage": voltage
    }
    result = client.publish('v1/devices/me/telemetry', json.dumps(telemetry), qos=1)
    client.publish('v1/devices/me/attributes', json.dumps({"timestamp": time.time()}), qos=1)
    print(f"Datos enviados: {telemetry}")

try:
    print("Iniciando envío de datos... Presiona Ctrl+C para detener.")

    while True:
        line = ser.readline().decode('utf-8').strip()
        if line:
            try:
                x, y, z, voltage = map(float, line.split('\t'))
                enviar_datos(x, y, z, voltage)
                time.sleep(1)
            except ValueError as e:
                print(f"Error al parsear datos: {e}")

except KeyboardInterrupt:
    print("Programa detenido por el usuario.")

finally:
    client.loop_stop()
    ser.close()
    client.disconnect()
    print("Conexión cerrada.")