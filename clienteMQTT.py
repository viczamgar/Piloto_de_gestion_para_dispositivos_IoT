import paho.mqtt.client as mqtt
import psycopg2
from datetime import datetime

# Diccionarios de buffers para almacenar datos por sysName hasta que estén completos
sysNames = {}
network_data = {}
server_data = {}
bgp_data = {}

# Variables necesarias por tipo de dispositivo
network_required = {"sysLocation", "interfaceDescription", "systemUptime", "interfaceStatus", "incomingPackets"}
server_required = {"sysUpTime", "tcpConnections", "cpuLoad", "diskName", "diskSize"}
bgp_required = {"ASN", "activeTime", "updatesReceived", "messagesReceived"}

# Conexión a PostgreSQL
conn = psycopg2.connect(
    dbname="snmpManager",
    user="dit",
    password="dit",
    host="localhost",
    port="5432"
)
cursor = conn.cursor()

# Función para insertar datos en PostgreSQL
def insert_if_complete(sysName):
    now = datetime.now()

    # Si es router
    if sysName.startswith("R") and sysName in network_data and network_required.issubset(network_data[sysName].keys()):
        data = network_data[sysName]
        cursor.execute("""
            INSERT INTO NodosDeRed (
                nombre_nodo, ubicacion_nodo, descripcion_interfaz,
                tiempo_up_nodo, estado_interfaz, paquetes_entrantes, fecha_lectura
            )
            VALUES (%s, %s, %s, %s, %s, %s, %s)
        """, (
            sysName,
            data.get("sysLocation"),
            data.get("interfaceDescription"),
            data.get("systemUptime"),
            data.get("interfaceStatus"),
            data.get("incomingPackets"),
            now
        ))
        del network_data[sysName]  # Borrar tras insertar

    # Si tiene datos BGP completos
    if sysName in bgp_data and bgp_required.issubset(bgp_data[sysName].keys()):
        data = bgp_data[sysName]
        cursor.execute("""
            INSERT INTO InfoBGP (
                nombre_nodo, ASN_vecino, tiempo_up_conexion,
                mensajes_update_in, mensajes_totales_in, fecha_lectura
            )
            VALUES (%s, %s, %s, %s, %s, %s)
        """, (
            sysName,
            data.get("ASN"),
            data.get("activeTime"),
            data.get("updatesReceived"),
            data.get("messagesReceived"),
            now
        ))
        del bgp_data[sysName]

    # Si es servidor
    if not sysName.startswith("R")  and server_required.issubset(server_data[sysName].keys()):
        print("✅ SE MEETEEEEEE")
        data = server_data[sysName]
        cursor.execute("""
            INSERT INTO Servidores (
                nombre_servidor, tiempo_actividad_servidor,
                conexiones_tcp, carga_cpu,
                nombre_disco, uso_disco, fecha_lectura
            )
            VALUES (%s, %s, %s, %s, %s, %s, %s)
        """, (
            sysName,
            data.get("sysUpTime"),
            data.get("tcpConnections"),
            data.get("cpuLoad"),
            data.get("diskName"),
            data.get("diskSize"),
            now
        ))
        del server_data[sysName]

    conn.commit()

# Callback cuando se recibe un mensaje
def on_message(client, userdata, message):
    topic = message.topic
    payload = message.payload.decode("utf-8").strip()
    print(f"📩 Mensaje recibido en {topic}: {payload}")

    if topic.startswith("/arduino1/sysName/"):
        index = topic.split("/")[-1]
        if index not in sysNames:
            sysNames[index] = payload
            subscribe_to_topics(client, payload)

    else:
        parts = topic.split("/")
        if len(parts) >= 4:
            sysName = parts[2]
            key = "/".join(parts[3:])

            valor = int(payload) if payload.isdigit() else payload
            print(f"🧪 key recibido: {key}")
            print(f"📋 server_required contiene: {server_required}")

            if key in network_required:
                network_data.setdefault(sysName, {})[key] = valor
            elif key in server_required:
                print("✅ server REQUIERED sI")
                server_data.setdefault(sysName, {})[key] = valor
            elif key.startswith("bgp/"):
                bgp_key = key.split("/")[-1]
                bgp_data.setdefault(sysName, {})[bgp_key] = valor

            insert_if_complete(sysName)

# Callback cuando se conecta
def on_connect(client, userdata, flags, rc):
    if rc == 0:
        print("✅ Conectado al broker MQTT")
        client.subscribe("/arduino1/sysName/+")
        print("🔎 Esperando nombres de dispositivos...")
    else:
        print(f"❌ Error de conexión: {rc}")

# Función para suscribirse a los tópicos de un dispositivo
def subscribe_to_topics(client, sysName):
    topics = [
        f"/arduino1/{sysName}/sysLocation",
        f"/arduino1/{sysName}/interfaceDescription",
        f"/arduino1/{sysName}/systemUptime",
        f"/arduino1/{sysName}/sysUpTime",
        f"/arduino1/{sysName}/interfaceStatus",
        f"/arduino1/{sysName}/incomingPackets",
        f"/arduino1/{sysName}/bgp/messagesReceived",
        f"/arduino1/{sysName}/bgp/ASN",
        f"/arduino1/{sysName}/bgp/activeTime",
        f"/arduino1/{sysName}/bgp/updatesReceived",
        f"/arduino1/{sysName}/diskName",
        f"/arduino1/{sysName}/tcpConnections",
        f"/arduino1/{sysName}/cpuLoad",
        f"/arduino1/{sysName}/diskSize"
    ]
    for topic in topics:
        client.subscribe(topic)
        print(f"📡 Suscrito a: {topic}")

# Configuración del cliente MQTT
client = mqtt.Client()
client.on_connect = on_connect
client.on_message = on_message

# Conexión con el broker
client.connect("localhost", 1883)
client.loop_forever()

