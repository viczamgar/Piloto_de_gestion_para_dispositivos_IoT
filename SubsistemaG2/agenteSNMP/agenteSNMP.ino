#include "Streaming.h"
#include <WiFiEsp.h>
#include <WiFiEspUdp.h>
#include <Ethernet.h>
#include "MemoryFree.h"
#include "AgenteSNMP.h"
#include "MIB.h"
#include "Variable.h"
#include <CapacitiveSensor.h>
#include <string.h>
#include <WiFiEspServer.h>
#include <WiFiEspClient.h>
#include <LiquidCrystal.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// --- Pines y Constantes ---
#define ONE_WIRE_BUS 8
const int sensorPin = A0;

// --- Configuración de Red ---
char ssid[] = "MOVISTAR_3D9E";
char password[] = "Vicente02";
static byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF};
static byte ipArduino[] = {192, 168, 1, 177};
static byte gateway[] = {192, 168, 1, 1};
static byte subnet[] = {255, 255, 255, 0};

// --- Objetos de Sensores ---
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
CapacitiveSensor capSensor = CapacitiveSensor(29, 27);

// --- Objetos de Red ---
WiFiEspUDP udp;
//EthernetUDP udp; // Alternativa comentada
WiFiEspServer server(80);
IPAddress address;

// --- Variables de Sensores ---
static int32_t* sensores[8];
float temperatureC;
int numAccesos;
int numAccesosAnteriorMuestreo;

// --- Variables de Temporización y Muestreo ---
static int contador1;
static int contador2;
static int contador3;
static int contador4;
static int contador5;
static int contador6;
static int contador7;
static int contador8;
static int muestreo1Ant = 10;
static int muestreo2Ant = 10;
static int muestreo3Ant = 10;
static int muestreo4Ant = 10;
static int muestreo5Ant = 10;
static int muestreo6Ant = 10;
static int muestreo7Ant = 10;
static int muestreo8Ant = 10;

// --- Variables bandera para Eventos (Traps) ---
static int activada = 0;
static int activada2 = 0;
static int activada3 = 0;
static int activada4 = 0;
static int activada5 = 0;

// --- Variables para Almacenamiento/Web ---
RingBuffer buf(8);
char* strings[4];
int num_strings = 0;
String readString;

int x = 100;

void setup() {
  Serial.begin(115200); // Necesario para la comunicación con el ESP8266
  Serial1.begin(115200); // Necesario para la comunicación con el ESP8266
  Serial2.begin(250000); // Restaurado: Inicialización de Serial2
  Serial2.setTimeout(100); // Restaurado: Timeout para Serial2

  pinMode(9, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(22, OUTPUT);
  digitalWrite(9, LOW);
  digitalWrite(22, HIGH);
  analogWrite(A3, 1024);

  WiFi.init(&Serial1);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("CONECTANDO");
  lcd.setCursor(0, 1);
  lcd.print(ssid);

  Ethernet.begin(mac, ipArduino, gateway, subnet);

  if (WiFi.status() == WL_NO_SHIELD) {
    while (true);
  }

  while (WiFi.status() != WL_CONNECTED) {
    if (WiFi.begin(ssid, password) != WL_CONNECTED) {
      delay(10000);
    }
  }

  lcd.clear();
  lcd.print("CONECTADO!");
  lcd.setCursor(1, 1);
  lcd.print(WiFi.localIP());

  address = WiFi.localIP();

  addString("Trap generado por: Sensor de Temperatura");
  addString("Trap generado por: Sensor Capacitivo");
  addString("Trap generado por: Voltimetro");
  addString("Trap generado por: Numero de Accesos");

  sensores[0] = &sensortemp;
  sensores[1] = &sensorhum;
  sensores[2] = &valorled1;
  sensores[3] = &accesosWeb;
  sensores[4] = &sensorSuelo;
  sensores[5] = &ram;
  sensores[6] = &sensorSonido;
  sensores[7] = &microfono;

  

  contador1 = 1000;
  contador2 = 1000;
  contador3 = 1000;
  contador4 = 1000;
  contador5 = 1000;
  contador6 = 1000;
  contador7 = 1000;
  contador8 = 1000;


  api_status = agente.inicio();
  server.begin();
  agente.onPduRecibida(PduRecibida);

  delay(10);

  Serial.println("Iniciando...");
}

void loop() {
  agente.escucha();  // Función de escucha del agente SNMP
  

// Lectura de variables del ESP32
  if (Serial2.available() > 0) {
    String cadenaRecibida = Serial2.readString();
    asignarValoresDesdeCadena(cadenaRecibida);
  }
// Funciones de lectura de los sensores
  actualiza1();
  actualiza2();
  actualiza3();
  actualiza4();
  actualiza5();
  actualiza6();
  actualiza7();
  actualiza8();
  
// Funciones de comprobación de los eventos
  evento1();
  evento2();
  evento3();
  evento4();
  evento5();

  

// Actualización de los contadores y del tiempo de encendido
  if (millis() - prevMillis > 1000) {
    prevMillis += 1000;
    locUpTime += 100;

    contador1 = contador1 - 100;
    contador2 = contador2 - 100;
    contador3 = contador3 - 100;
    contador4 = contador4 - 100;
    contador5 = contador5 - 100;
    contador6 = contador6 - 100;
    contador7 = contador7 - 100;
    contador8 = contador8 - 100;
  }
}

void actualiza1(void) {
  if (tiempoMuestreo1 != muestreo1Ant) {
    muestreo1Ant = tiempoMuestreo1;
    contador1 = tiempoMuestreo1 * 100;
  }

  if (contador1 == 0) {
    int sensorVal = analogRead(9);
    float voltage = sensorVal * (5.0 / 1023.0);
    sensortemp = ((voltage - 0.5) * 100.0) - 12;
    instanteLectura1 = locUpTime;
    contador1 = tiempoMuestreo1 * 100;
  }
}

void actualiza2(void) {
  if (tiempoMuestreo2 != muestreo2Ant) {
    muestreo2Ant = tiempoMuestreo2;
    contador2 = tiempoMuestreo2 * 100;
  }

  if (contador2 == 0) {
    sensorhum = capSensor.capacitiveSensor(30) / 100;
    instanteLectura2 = locUpTime;
    contador2 = tiempoMuestreo2 * 100;
  }
}


void actualiza3(void) {
  if (tiempoMuestreo3 != muestreo3Ant) {
    muestreo3Ant = tiempoMuestreo3;
    contador3 = tiempoMuestreo3 * 100;
  }

  if (contador3 == 0) {
    valorled1 = analogRead(2) * 5.0 / 1023.0;
    instanteLectura3 = locUpTime;
    contador3 = tiempoMuestreo3 * 100;
  }
}


void actualiza4(void) {
  if (tiempoMuestreo4 != muestreo4Ant) {
    muestreo4Ant = tiempoMuestreo4;
    contador4 = tiempoMuestreo4 * 100;
  }

  if ((contador4 == 0)) {
    numAccesosAnteriorMuestreo = accesosWeb;
    accesosWeb = numAccesos;
    instanteLectura4 = locUpTime;
    contador4 = tiempoMuestreo4 * 100;
  }
}

void actualiza5(void) {
  if (tiempoMuestreo5 != muestreo5Ant) {
    muestreo5Ant = tiempoMuestreo5;
    contador5 = tiempoMuestreo5 * 100;
  }

  if (contador5 == 0) {
    sensorSuelo = map(analogRead(10), 0, 1023, 0, 100);
    instanteLectura5 = locUpTime;
    contador5 = tiempoMuestreo5 * 100;
  }
}

void actualiza6(void) {
  if (tiempoMuestreo6 != muestreo6Ant) {
    muestreo6Ant = tiempoMuestreo6;
    contador6 = tiempoMuestreo6 * 100;
  }

  if (contador6 == 0) {
    ram = freeMemory();
    instanteLectura6 = locUpTime;
    contador6 = tiempoMuestreo6 * 100;
  }
}

void actualiza7(void) {
  if (tiempoMuestreo7 != muestreo7Ant) {
    muestreo7Ant = tiempoMuestreo7;
    contador7 = tiempoMuestreo7 * 100;
  }

  if (contador7 == 0) {
    sensorSonido = analogRead(A13);
    instanteLectura7 = locUpTime;
    contador7 = tiempoMuestreo7 * 100;
  }
}

void actualiza8(void) {
  if (tiempoMuestreo8 != muestreo8Ant) {
    muestreo8Ant = tiempoMuestreo8;
    contador8 = tiempoMuestreo8 * 100;
  }

  if (contador8 == 0) {
    microfono=0;
    microfono = analogRead(A15);
    microfono += analogRead(A15);
    microfono += analogRead(A15);
    microfono += analogRead(A15);
    microfono += analogRead(A15);
    microfono += analogRead(A15);
    microfono += analogRead(A15);
    microfono += analogRead(A15);
    microfono += analogRead(A15);
    microfono += analogRead(A15);
    microfono += analogRead(A15);
    microfono += analogRead(A15);
    microfono += analogRead(A15);
    microfono = microfono/13;
    instanteLectura8 = locUpTime;
    contador8 = tiempoMuestreo8 * 100;
  }
}

void evento1(void) {
  if (Sind1 != 0) {
    if (Sind1 == 4) {
      if (((accesosWeb - numAccesosAnteriorMuestreo) > uS1) || ((accesosWeb - numAccesosAnteriorMuestreo) < uI1)) {
        if (activada == 0) {
          char str1[10] = "";
          char strcad[25] = "";
          sprintf(str1, "%ld", *sensores[Sind1 - 1]);
          strcat(strcad, d1);
          strcat(strcad, " Valor: ");
          strcat(strcad, str1);
          byte trap[] = {192, 168, 1, 59};
          agente.Trap(strcad, trap, locUpTime, "1.3.6.1.4.1.36582.1.1.1.4.1", "1.3.6.1.4.1.36582.1.1.1.4.1");
          digitalWrite(9, HIGH);
          digitalWrite(9, LOW);
          activada = 1;
        }
      } else {
        activada = 0;
      }
    } else {
       
      if ((*sensores[Sind1 - 1] > uS1) || (*sensores[Sind1 - 1] < uI1)) {
        if (activada == 0) {
          digitalWrite(9, HIGH);
          digitalWrite(9, LOW);
          char str1[10] = "";
          char strcad[25] = "";
          sprintf(str1, "%ld", *sensores[Sind1 - 1]);
          strcat(strcad, d1);
          strcat(strcad, " Valor: ");
          strcat(strcad, str1);
          agente.Trap(strcad, ip1, locUpTime, "1.3.6.1.4.1.36582.1.1.1.4.1", "1.3.6.1.4.1.36582.1.1.1.4.1");
          activada = 1;
          Serial.println("TRAP GENERADO");
        }
      } else {
        activada = 0;
      }
    }
  }
}

void evento2(void) {
  if (Sind2 != 0) {
    if (Sind2 == 4) {
      if (((accesosWeb - numAccesosAnteriorMuestreo) > uS2) || ((accesosWeb - numAccesosAnteriorMuestreo) < uI2)) {
        if (activada2 == 0) {
          digitalWrite(9, HIGH);
          digitalWrite(9, LOW);
          char str2[10] = "";
          char strcad2[25] = "";
          sprintf(str2, "%ld", *sensores[Sind2 - 1]);
          strcat(strcad2, d2);
          strcat(strcad2, " Valor: ");
          strcat(strcad2, str2);
          agente.Trap(strcad2, ip2, locUpTime, "1.3.6.1.4.1.36582.1.1.1.4.1", "1.3.6.1.4.1.36582.1.1.1.4.1");
          activada2 = 1;
          
        }
      } else {
        activada2 = 0;
      }
    } else {
      if ((*sensores[Sind2 - 1] > uS2) || (*sensores[Sind2 - 1] < uI2)) {
        if (activada2 == 0) {
          digitalWrite(9, HIGH);
          digitalWrite(9, LOW);
          char str2[10] = "";
          char strcad2[25] = "";
          sprintf(str2, "%ld", *sensores[Sind2 - 1]);
          strcat(strcad2, d2);
          strcat(strcad2, " Valor: ");
          strcat(strcad2, str2);
          agente.Trap(strcad2, ip2, locUpTime, "1.3.6.1.4.1.36582.1.1.1.4.1", "1.3.6.1.4.1.36582.1.1.1.4.1");
          activada2 = 1;
           Serial.println("TRAP GENERADO");
        }
      } else {
        activada2 = 0;
      }
    }
  }
}

void evento3(void) {
  if (Sind3 != 0) {
    if (Sind3 == 4) {
      if (((accesosWeb - numAccesosAnteriorMuestreo) > uS3) || ((accesosWeb - numAccesosAnteriorMuestreo) < uI3)) {
        if (activada3 == 0) {
          digitalWrite(9, HIGH);
          digitalWrite(9, LOW);
          char str3[10] = "";
          char strcad3[25] = "";
          sprintf(str3, "%ld", *sensores[Sind3 - 1]);
          strcat(strcad3, d3);
          strcat(strcad3, " Valor: ");
          strcat(strcad3, str3);
          agente.Trap(strcad3, ip3, locUpTime, "1.3.6.1.4.1.36582.1.1.1.4.1", "1.3.6.1.4.1.36582.1.1.1.4.1");
          activada3 = 1;
        }
      } else {
        activada3 = 0;
      }
    } else {
      if ((*sensores[Sind3 - 1] > uS3) || (*sensores[Sind3 - 1] < uI3)) {
        if (activada3 == 0) {
          digitalWrite(9, HIGH);
          digitalWrite(9, LOW);
          char str3[10] = "";
          char strcad3[25] = "";
          sprintf(str3, "%ld", *sensores[Sind3 - 1]);
          strcat(strcad3, d3);
          strcat(strcad3, " Valor: ");
          strcat(strcad3, str3);
          agente.Trap(strcad3, ip3, locUpTime, "1.3.6.1.4.1.36582.1.1.1.4.1", "1.3.6.1.4.1.36582.1.1.1.4.1");
          activada3 = 1;
        }
      } else {
        activada3 = 0;
      }
    }
  }
}


void evento4(void) {
  if (Sind4 != 0) {
    if (Sind4 == 4) {
      if (((accesosWeb - numAccesosAnteriorMuestreo) > uS4) || ((accesosWeb - numAccesosAnteriorMuestreo) < uI4)) {
        if (activada4 == 0) {
          digitalWrite(9, HIGH);
          digitalWrite(9, LOW);
          char str4[10] = "";
          char strcad4[25] = "";
          sprintf(str4, "%ld", *sensores[Sind4 - 1]);
          strcat(strcad4, d4);
          strcat(strcad4, " Valor: ");
          strcat(strcad4, str4);
          agente.Trap(strcad4, ip4, locUpTime, "1.3.6.1.4.1.36582.1.1.1.4.1", "1.3.6.1.4.1.36582.1.1.1.4.1");
          activada4 = 1;
        }
      } else {
        activada4 = 0;
      }
    } else {
      if ((*sensores[Sind4 - 1] > uS4) || (*sensores[Sind4 - 1] < uI4)) {
        if (activada4 == 0) {
          digitalWrite(9, HIGH);
          digitalWrite(9, LOW);
          char str4[10] = "";
          char strcad4[25] = "";
          sprintf(str4, "%ld", *sensores[Sind4 - 1]);
          strcat(strcad4, d4);
          strcat(strcad4, " Valor: ");
          strcat(strcad4, str4);
          agente.Trap(strcad4, ip4, locUpTime, "1.3.6.1.4.1.36582.1.1.1.4.1", "1.3.6.1.4.1.36582.1.1.1.4.1");
          activada4 = 1;
        }
      } else {
        activada4 = 0;
      }
    }
  }
}

void evento5(void) {
  if (Sind5 != 0) {
    if (Sind5 == 4) {
      if (((accesosWeb - numAccesosAnteriorMuestreo) > uS5) || ((accesosWeb - numAccesosAnteriorMuestreo) < uI5)) {
        if (activada5 == 0) {
          digitalWrite(9, HIGH);
          digitalWrite(9, LOW);
          char str5[10] = "";
          char strcad5[25] = "";
          sprintf(str5, "%ld", *sensores[Sind5 - 1]);
          strcat(strcad5, d5);
          strcat(strcad5, " Valor: ");
          strcat(strcad5, str5);
          agente.Trap(strcad5, ip5, locUpTime, "1.3.6.1.4.1.36582.1.1.1.4.1", "1.3.6.1.4.1.36582.1.1.1.4.1");
          activada5 = 1;
        }
      } else {
        activada5 = 0;
      }
    } else {
      if ((*sensores[Sind5 - 1] > uS5) || (*sensores[Sind5 - 1] < uI5)) {
        if (activada5 == 0) {
          digitalWrite(9, HIGH);
          digitalWrite(9, LOW);
          char str5[10] = "";
          char strcad5[25] = "";
          sprintf(str5, "%ld", *sensores[Sind5 - 1]);
          strcat(strcad5, d5);
          strcat(strcad5, " Valor: ");
          strcat(strcad5, str5);
          agente.Trap(strcad5, ip5, locUpTime, "1.3.6.1.4.1.36582.1.1.1.4.1", "1.3.6.1.4.1.36582.1.1.1.4.1");
          activada5 = 1;
        }
      } else {
        activada5 = 0;
      }
    }
  }
}

void addString(const char* str) {
  if (num_strings < 4) {
    strings[num_strings++] = strdup(str);
  }
}

void asignarValoresDesdeCadena(String cadena) {
  int startIndex = 0;
  int endIndex = 0;

  // Primer conjunto de variables (2 entradas)
  for (int i = 0; i < 2; i++) {
    for (int x = 0; x < 7; x++) {
      endIndex = cadena.indexOf('*', startIndex);
      if (endIndex == -1) endIndex = cadena.length();
      String token = cadena.substring(startIndex, endIndex);

      if (token.length() > 0) {
        switch (x) {
          case 0: token.toCharArray(sysName1[i], sizeof(sysName1[i])); break;
          case 1: token.toCharArray(nombreDisco1[i], sizeof(nombreDisco1[i])); break;
          case 2: time1[i] = token.toInt(); break;
          case 3: tcpcon1[i] = token.toInt(); break;
          case 4: carga1[i] = token.toInt(); break;
          case 5: tamDisco1[i] = token.toInt(); break;
          case 6: tamUnits1[i] = token.toInt(); break;
        }
      }
      if (endIndex == cadena.length()) break;
      startIndex = endIndex + 1;
    }
    if (endIndex == cadena.length()) break;
  }

  for (int i = 0; i < 5; i++) {
    for (int x = 0; x < 10; x++) {
      endIndex = cadena.indexOf('*', startIndex);
      if (endIndex == -1) endIndex = cadena.length();
      String token = cadena.substring(startIndex, endIndex);

      if (token.length() > 0) {
        switch (x) {
          case 0: token.toCharArray(sysName2[i], sizeof(sysName2[i])); break;
          case 1: token.toCharArray(sysLocation2[i], sizeof(sysLocation2[i])); break;
          case 2: token.toCharArray(ifDescr[i], sizeof(ifDescr[i])); break;
          case 3: time2[i] = token.toInt(); break;
          case 4: operStatus[i] = token.toInt(); break;
          case 5: inPackets[i] = token.toInt(); break;
          case 6: vecinoASN[i] = token.toInt(); break;
          case 7: tiempoConexion[i] = token.toInt(); break;
          case 8: mensajesUpdate[i] = token.toInt(); break;
          case 9: mensajesTotales[i] = token.toInt(); break;
        }
      }
      if (endIndex == cadena.length()) break;
      startIndex = endIndex + 1;
    }
    if (endIndex == cadena.length()) break;
  }

 // Extraer numAccesos (último valor)
if (startIndex < cadena.length()) {
  String resto = cadena.substring(startIndex);


  if (resto.length() > 0) {
    numAccesos = resto.toInt()-1;
   
  }
} 

}

String convertirGB(uint32_t tam, uint32_t *bytes ) {
  unsigned long gb;
  unsigned long mb;
  unsigned long kb;
  kb = (*bytes / (1024)) * tam;
  mb = kb / 1024;
  gb = mb / 1024;
  *bytes = gb;

  String result = "GB Usados: " + String(gb);
  return result;
}

String convertir(uint32_t timeticks) {
  unsigned long total_seconds;
  unsigned long hours;
  unsigned long minutes;
  unsigned long seconds;

  total_seconds = timeticks / 100;
  hours = total_seconds / 3600;
  total_seconds = total_seconds % 3600;
  minutes = total_seconds / 60;
  seconds = total_seconds % 60;

  String result = "Horas: " + String(hours) + ", Minutos: " + String(minutes) + ", Segundos: " + String(seconds) ;
  return result;
}
