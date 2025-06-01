DROP TABLE IF EXISTS InfoBGP CASCADE;
DROP TABLE IF EXISTS Servidores CASCADE;
DROP TABLE IF EXISTS NodosDeRed CASCADE;


CREATE TABLE NodosDeRed (
    id SERIAL PRIMARY KEY,
    nombre_nodo VARCHAR(50),
    ubicacion_nodo VARCHAR(50),
    tiempo_up_nodo VARCHAR(50),
    descripcion_interfaz VARCHAR(50),
    estado_interfaz INTEGER,
    paquetes_entrantes INTEGER,
    fecha_lectura TIMESTAMP
);

CREATE TABLE Servidores (
    id SERIAL PRIMARY KEY,
    nombre_servidor VARCHAR(50),
    tiempo_actividad_servidor VARCHAR(50),
    conexiones_tcp INTEGER,
    carga_cpu INTEGER,
    nombre_disco VARCHAR(50),
    uso_disco INTEGER,
    fecha_lectura TIMESTAMP
);

CREATE TABLE InfoBGP (
    id SERIAL PRIMARY KEY,
    nombre_nodo VARCHAR(50),
    ASN_vecino INTEGER,
    tiempo_up_conexion VARCHAR(50),
    mensajes_update_in INTEGER,
    mensajes_totales_in INTEGER,
    fecha_lectura TIMESTAMP
);
