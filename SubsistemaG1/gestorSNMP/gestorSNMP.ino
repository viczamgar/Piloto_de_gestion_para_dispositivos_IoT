



#include <WiFi.h>
#include <WiFiUdp.h> 
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <PubSubClient.h>
//#include <Ethernet.h>
#include <Arduino_SNMP_Manager.h>       
#include <WebServer.h>
//#include <CapacitiveSensor.h>
#include <string.h>
#include <WiFiServer.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
// Include the PNG decoder library
#include <PNGdec.h>
#include "panda.h" // Image is stored here in an 8-bit array



WiFiClient wifi;
PubSubClient clientMQTT(wifi);
IPAddress broker(192, 168, 1, 106);


PNG png; // PNG decoder instance

#define MAX_IMAGE_WIDTH 240 // Adjust for your images

int16_t xpos = 0;
int16_t ypos = 0;


#define TFT_GREY 0xC618
#define TFT_DARKGREY 0x7BEF // Un gris más oscuro


TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

WebServer server (80);

unsigned long inicio; //VARIABLE PARA QUE MILLIS EMPIECE A CONTAR DESDE QUE ENTRA EN LOOP


const int sensorPin = A0;

WiFiUDP udp;

// VARIABLES PRUEBA MANAGER //
int octetos[2]={59,50};
int octetos2[5]={206,202,203,204,205};

char resultado[5000];  // Asumiendo un tamaño máximo razonable para la cadena resultante




SNMPManager snmp = SNMPManager("public");
SNMPGet snmpRequest = SNMPGet("public", 1);
int pollInterval = 45000; // delay in milliseconds
unsigned long pollStart = 0;
unsigned long intervalBetweenPolls = 0;

//PC
ValueCallback *callbackSysName1;  // Blank Callback for each OID
ValueCallback *callbackCargaCPU1;
ValueCallback *callbackNombreDiscoDuro1;
ValueCallback *tiempo1;
ValueCallback *TCP1;
ValueCallback *diskResp1;
ValueCallback *diskTamUnits1;


int enviado=0;

//PC
char sysName1[2][50];
char nombreDisco1[2][50];
uint32_t time1[2];
uint32_t tcpcon1[2];
uint32_t carga1[2];
uint32_t tamDisco1[2];
uint32_t tamUnits1[2];

char *nombreDiscResp1[]={nombreDisco1[0], nombreDisco1[1]};
char *sysNameResponse1[]={sysName1[0],sysName1[1]};

//SW
ValueCallback *callbackSysName2;  // Blank Callback for each OID
ValueCallback *callbackLocation;
ValueCallback *callbackDescr;
ValueCallback *encendido;
ValueCallback *operStat;
ValueCallback *packetsIn;



void get1SNMP();

//SWyROUTER
char sysName2[5][50];
char sysLocation2[5][50];
char ifDescr[5][50];
uint32_t time2[5];
uint32_t operStatus[5];
uint32_t inPackets[5];

char *sysNameRouter[]={sysName2[0], sysName2[1],sysName2[2],sysName2[3],sysName2[4]};
char *sysLocationRouter[]={sysLocation2[0],sysLocation2[1],sysLocation2[2],sysLocation2[3],sysLocation2[4]};
char *ifDescrPuertos[]={ifDescr[0], ifDescr[1],ifDescr[2],ifDescr[3],ifDescr[4]};

// Callbacks para las nuevas variables de BGP
ValueCallback *bgpMessagesReceived;
ValueCallback *bgpASNNeighbor;
ValueCallback *bgpUptime;
ValueCallback *bgpUpdatesIN;

// Arreglos para almacenar los valores obtenidos por SNMP
uint32_t bgpMsgsReceived[5];  // Número de mensajes BGP recibidos
uint32_t bgpASN[5];           // ASN del vecino BGP
uint32_t bgpTime[5];          // Tiempo activo de la sesión BGP
uint32_t bgpUpdates[5];      // Número de prefijos recibidos

void get2SNMP();
void pngDraw();



int numAccesos;

int numAccesosAnteriorMuestreo;

char ssid[] = "MOVISTAR_3D9E";
char password[] = "Vicente02";

char* strings[4]; // Array de punteros a caracteres (strings)
int num_strings = 0; // Variable para mantener el número actual de strings en la lista
String readString;
// Crear un servidor en el puerto 80
//EthernetServer server(80);
//WiFiEspServer server(80);
//WiFiServer server(80);









void setup() {

    Serial.begin(115200);

    Serial2.begin(250000,SERIAL_8N1,16,17);
    
    Serial.println("Arduino Iniciado, logs preparados para ser servidos :)");
    
    //pinMode(9,OUTPUT);
    //pinMode(12,OUTPUT);
    //pinMode(22,OUTPUT);
    //digitalWrite(9,HIGH);

    clientMQTT.setServer(broker, 1883);

    
   tft.begin();
   tft.fillScreen(TFT_BLACK);

   SPIFFS.begin(true);
 

int16_t rc = png.openFLASH((uint8_t *)panda, sizeof(panda), pngDraw);
  if (rc == PNG_SUCCESS) {
    Serial.println("Successfully opened png file");
    Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
    tft.startWrite();
    uint32_t dt = millis();
    rc = png.decode(NULL, 0);
    Serial.print(millis() - dt); Serial.println("ms");
    tft.endWrite();
    // png.close(); // not needed for memory->memory decode
  }

 delay(5000);
  
 tft.fillScreen(TFT_GREY);
 tft.setTextColor(TFT_BLACK);
 tft.setCursor(50,50);
 tft.setTextSize(2);
 tft.print("Conexion Wifi ");
 tft.setCursor(15,90);
 tft.print(String(ssid));
 
 tft.setTextColor(TFT_WHITE);
 tft.setCursor(52,52);
 tft.print("Conexion Wifi");
 tft.setCursor(17,92);
 tft.print(String(ssid));
 
  int errorn=0;
  int buc=0;
  int salirWifi=0;

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {

  

 for (int i = 0; i < 4; i++) {
    tft.fillCircle(90 + (i * 20), 140, 5, TFT_GREEN);  // Círculos más vistosos
    tft.drawCircle(90 + (i * 20), 140, 7, TFT_BLACK);  // Circunferencia exterior
     if(WiFi.status() == WL_CONNECTED){
        salirWifi=1;
        break;
     }
    delay(1000);
    //PARA QUE LA BARRA DE CARGA SE HAGA VISUALICE DOS VECES PARA CADA INTENTO DE CONEXIÓN
    if(i==3 && buc==0){
      i=-1;
      buc=1;
      tft.fillRect(70, 130, 90, 40, TFT_GREY);  // Borra los puntos
    }

    if(i==3 && buc==1){
      
      buc=0;
    }
    
  }
    if(salirWifi==1){
      break;
    }
    if(WiFi.status() != WL_CONNECTED){
    if (WiFi.begin(ssid, password) != WL_CONNECTED) {
      tft.fillRect(70, 130, 90, 40, TFT_GREY);  // Borra los puntos
     

      
      tft.fillRect(5,180 , 230, 60, TFT_RED);  // Borra los puntos
      tft.drawRect(5, 180, 230, 60, TFT_WHITE); // Dibuja el borde del rectángulo
      tft.setTextColor(TFT_WHITE,TFT_RED);
       tft.setCursor(15,200);
      tft.print("Error al conectar ");
      
      
      tft.setCursor(120,220);
      tft.print(errorn);
      errorn++;
      
      
    }
    }
  }



  server.on("/", inicioWeb);
  server.on("/modificarTm", modificarTm);
  server.on("/obtenerMIB", obtenerMIB);
    
   tft.fillScreen(TFT_GREY);
 tft.setTextColor(TFT_WHITE,TFT_GREY);
 tft.setCursor(5,50);
 tft.setTextSize(2);
 tft.setTextColor(TFT_GREEN);
 tft.println("Conexion Establecida");
 tft.fillRect(15,80 , 210, 220, TFT_DARKGREY);  // Borra los puntos
 tft.drawRect(15, 80, 210, 220, TFT_BLUE); // Dibuja el borde del rectángulo

 //IP,SSID,SERVIDOR HTTP, MAC, GATEWAY
 tft.setTextSize(1);
 tft.setCursor(25,95);
  tft.println("IP");
 tft.setCursor(25,135);
  tft.println("MASK");
 tft.setCursor(25,175);
  tft.println("HTTP/SERVER IP");
 tft.setCursor(25,215);
  tft.println("MAC");
 tft.setCursor(25,255);
  tft.println("GATEWAY");

  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
   tft.setCursor(25,115);
  tft.println(WiFi.localIP());
 tft.setCursor(25,155);
  tft.println(WiFi.subnetMask());
 tft.setCursor(25,195);

 tft.println(WiFi.localIP());
  tft.setCursor(25,235);
 tft.println(WiFi.macAddress());
 tft.setCursor(25,275);
  tft.println(WiFi.gatewayIP());


 delay(5000);

 tft.fillScreen(TFT_GREY);

 
 //tft.print(WiFi.localIP());
  Serial.println(WiFi.localIP());
  
    
    // VARIABLE PARA QUE EMPIECE A CONTAR DESDE AQUÍ
   inicio=millis();
   //MANAGER
   snmp.setUDP(&udp);
  
  //Inicia servidor HTPP
  server.begin();
  
    
   
    
}

void reconnect() {
  // Loop hasta que se reconecte
  while (!clientMQTT.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Intentar conexión con credenciales (usuario y contraseña)
    if (clientMQTT.connect("arduino")) {
      Serial.println("connected");
     
    } else {
      Serial.print("failed, rc=");
      Serial.print(clientMQTT.state());
      Serial.println(" try again in 5 seconds");
      // Esperar 5 segundos antes de reintentar
      delay(5000);
    }
  }

}

void loop() {
  if (!clientMQTT.connected()) {
    reconnect();
  }

  clientMQTT.loop();
  snmp.loop();
  server.handleClient();
  

  
     
     
 
  intervalBetweenPolls = millis() - pollStart -inicio;
  inicio=0;
  
 
  if (intervalBetweenPolls >= pollInterval)
  {
    //imprimirVariables();
    combinarVariables();
    //Serial.println(resultado);
    pollStart += pollInterval; // this prevents drift in the delays
    
    

    get1SNMP();
    delay(50);
    get2SNMP();
    //delay(100);

    
   
Serial2.print(resultado); 
Serial.println(resultado); 




for (int i = 0; i < 5; i++) {
    // Publicar sysName en un tópico distinto para cada dispositivo
    String sysNameTopic = String("/arduino1/sysName/") + String(i + 1);
    clientMQTT.publish(sysNameTopic.c_str(), sysName2[i]);

    // Luego, publicar las variables usando el sysName en la URL
    clientMQTT.publish((String("/arduino1/") + sysName2[i] + "/sysLocation").c_str(), sysLocation2[i]);
    clientMQTT.publish((String("/arduino1/") + sysName2[i] + "/interfaceDescription").c_str(), ifDescr[i]);
    clientMQTT.publish((String("/arduino1/") + sysName2[i] + "/systemUptime").c_str(), String(time2[i]).c_str());
    clientMQTT.publish((String("/arduino1/") + sysName2[i] + "/interfaceStatus").c_str(), String(operStatus[i]).c_str());
    clientMQTT.publish((String("/arduino1/") + sysName2[i] + "/incomingPackets").c_str(), String(inPackets[i]).c_str());

    // Publicación de valores BGP con sysName en la URL
    clientMQTT.publish((String("/arduino1/") + sysName2[i] + "/bgp/messagesReceived").c_str(), String(bgpMsgsReceived[i]).c_str());
    clientMQTT.publish((String("/arduino1/") + sysName2[i] + "/bgp/ASN").c_str(), String(bgpASN[i]).c_str());
    clientMQTT.publish((String("/arduino1/") + sysName2[i] + "/bgp/activeTime").c_str(), String(bgpTime[i]).c_str());
    clientMQTT.publish((String("/arduino1/") + sysName2[i] + "/bgp/updatesReceived").c_str(), String(bgpUpdates[i]).c_str());
}

for (int i = 0; i < 2; i++) {
    // Publicar sysName en un tópico distinto para cada PC
    String sysNameTopic = String("/arduino1/sysName/") + String(i + 6);  // Empezamos en 6 para no pisar los anteriores
    clientMQTT.publish(sysNameTopic.c_str(), sysName1[i]);

    // Publicación de valores de PC con sysName en la URL
    clientMQTT.publish((String("/arduino1/") + sysName1[i] + "/diskName").c_str(), nombreDisco1[i]);
    clientMQTT.publish((String("/arduino1/") + sysName1[i] + "/sysUpTime").c_str(), String(time1[i]).c_str());
    clientMQTT.publish((String("/arduino1/") + sysName1[i] + "/tcpConnections").c_str(), String(tcpcon1[i]).c_str());
    clientMQTT.publish((String("/arduino1/") + sysName1[i] + "/cpuLoad").c_str(), String(carga1[i]).c_str());
    clientMQTT.publish((String("/arduino1/") + sysName1[i] + "/diskSize").c_str(), String(tamDisco1[i]).c_str());
    clientMQTT.publish((String("/arduino1/") + sysName1[i] + "/unitsSize").c_str(), String(tamUnits1[i]).c_str());
}



  }

tft.setCursor(10, 50); // Posición en la pantalla
tft.setTextColor(TFT_WHITE, TFT_BLACK); // Texto blanco con fondo negro
tft.setTextSize(2); // Tamaño del texto
tft.print("Sondeo SNMP: ");
tft.print(intervalBetweenPolls / 1000); // Convertir a segundos




     


    
}

void inicioWeb() {

String html = R"=====(<!DOCTYPE html>
<html lang="es">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>Proyecto Gestion de Redes 2024</title>
  <link rel="icon" href="https://web.dev/static/articles/building/an-adaptive-favicon/image/example-dark-theme-favico-6a6f6f67bc3e7.png" type="image/x-icon">
  <style>
    body {
      font-family: Arial, sans-serif;
      background-color: #f4f4f4;
      color: #333;
      margin: 0;
      padding: 0;
      display: flex;
      flex-direction: column;
      min-height: 100vh;
    }
    main {
      flex: 1;
      padding: 50px;
      text-align: center;
    }

    .contenido-flex {
      display: flex;
      justify-content: space-between;
      align-items: flex-start;
      gap: 40px;
      margin-top: 40px;
      flex-wrap: wrap;
    }

    .columnas-tablas {
      flex: 1 1 60%;
    }

    .config-form {
      flex: 1 1 35%;
      margin: 0 auto;
      padding: 20px;
      background: white;
      border-radius: 5px;
      box-shadow: 0 2px 4px rgba(0,0,0,0.1);
      max-width: 400px;
    }

    h1 {
      color: #0056b3;
      font-size: 36px;
      margin-bottom: 10px;
    }
    h2 {
      color: #333;
      font-size: 28px;
      margin-bottom: 20px;
    }
    h3 {
      color: #666;
      font-size: 24px;
      margin-bottom: 30px;
    }
    p {
      font-size: 18px;
      color: #555;
    }
    hr {
      border: 0;
      height: 1px;
      background: #ccc;
      margin: 20px 0;
    }
    table {
      margin: 20px auto;
      border-collapse: collapse;
      width: 100%;
      max-width: 100%;
      background-color: #ffffff;
      box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);
    }
    th, td {
      border: 1px solid #dddddd;
      padding: 12px 15px;
      text-align: left;
    }
    th {
      background-color: #0056b3;
      color: white;
      text-transform: uppercase;
      font-weight: 600;
    }
    tr:nth-child(even) {
      background-color: #f9f9f9;
    }
    footer {
      position: fixed;
      bottom: 0;
      left: 0;
      width: 100%;
      background-color: #f4f4f4;
      padding: 10px 0;
      color: #333;
      text-align: center;
      border-top: 1px solid #ccc;
    }
    .form-group {
      margin-bottom: 15px;
    }
    .form-group label {
      display: block;
      margin-bottom: 5px;
      font-weight: bold;
    }
    .form-group input {
      width: 100%;
      padding: 8px;
      border: 1px solid #ddd;
      border-radius: 4px;
    }
    .form-submit {
      background-color: #0056b3;
      color: white;
      border: none;
      padding: 10px 15px;
      border-radius: 4px;
      cursor: pointer;
      margin-top: 10px;
    }
    .form-submit:hover {
      background-color: #004494;
    }
  </style>
</head>
<body>
<main>
  <h1>Trabajo Fin de Grado VZG</h1>
  <hr />
  <h2>Mota con Protocolos de Gestión</h2>
  <h3>Número de visitas: )=====";

html += String(numAccesos++);
html += R"=====(</h3>

  <div class="contenido-flex">

    <div class="columnas-tablas">
      <h4>Equipos gestionados vía SNMP:</h4>
      <table>
        <tr><th>Nombre del Equipo</th><th>Dirección IP</th></tr>
)=====";

// Equipos
for (int i = 0; i < 2; i++) {
  html += "<tr><td>";
  html += sysName1[i];
  html += "</td><td>192.168.1.";
  html += String(octetos[i]);
  html += "</td></tr>";
}

html += R"=====(</table>

      <h4>Detalles de Interfaces:</h4>
      <table>
        <tr><th>Nombre del Switch/Router</th><th>Localización</th><th>Descripción de la Interfaz</th></tr>
)=====";

// Interfaces
for (int i = 0; i < 5; i++) {
  html += "<tr><td>";
  html += sysName2[i];
  html += "</td><td>";
  html += sysLocation2[i];
  html += "</td><td>";
  html += ifDescr[i];
  html += "</td></tr>";
}

html += R"=====(</table>
    </div>

    <div class="config-form">
      <h3>Configuración del Intervalo SNMP</h3>
      <p>Intervalo actual: )=====";

html += String(pollInterval / 1000);
html += R"=====( segundos</p>

      <form action='/modificarTm' method='POST'>
        <div class='form-group'>
          <label for='interval'>Nuevo intervalo (segundos):</label>
          <input type='number' id='interval' name='interval' min='5' max='300' value=')=====";

html += String(pollInterval / 1000);
html += R"=====(' required>
        </div>
        <input type='submit' class='form-submit' value='Actualizar Intervalo'>
      </form>

      <form action="/obtenerMIB" method="GET">
        <input type="submit" class="form-submit" value="Descargar MIB">
      </form>
    </div>

  </div>
</main>

<footer>
  @Vicente Zamora García. Contacto: <a href="mailto:vicente.zamgar@gmail.com">vicente.zamgar@gmail.com</a>
</footer>
</body>
</html>)=====";




server.send(200, "text/html", html);
  
  }


void modificarTm() {
     String pollI;
     if(server.method() == HTTP_POST){
        pollI = server.arg("interval");
        pollInterval = pollI.toInt() * 1000; // suponiendo que quieres milisegundos

      }

      server.sendHeader("Location","/");
      server.send(301);
  
  
  }

void obtenerMIB() {

     
   File file = SPIFFS.open("/MIBTFG.mib", "r");
  if (!file) {
    server.send(404, "text/plain", "Archivo MIB no encontrado");
    return;
  }

  server.sendHeader("Content-Type", "application/octet-stream");
  server.sendHeader("Content-Disposition", "attachment; filename=\"MIBTFG.mib\"");
  server.sendHeader("Connection", "close");

  // Leer todo el contenido del archivo
  size_t fileSize = file.size();
  char* buffer = (char*)malloc(fileSize);
  if (buffer) {
    file.readBytes(buffer, fileSize);
    server.send(200, "application/octet-stream", buffer);
    free(buffer);
  } else {
    server.send(500, "text/plain", "Error al reservar memoria");
  }

  file.close();
  
  
  }

void pngDraw(PNGDRAW *pDraw) {
  uint16_t lineBuffer[MAX_IMAGE_WIDTH];
  png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
  tft.pushImage(xpos, ypos + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void imprimirVariables(){

    /*
    for(int i=0;i<2;i++){
      
   
    Serial.print("PC:");
    Serial.println(sysNameResponse1[i]);
    Serial.println(carga1[i]);
    Serial.println(convertir(time1[i]));
    Serial.println(tcpcon1[i]);
    Serial.println(convertirGB(tamUnits1[i],&tamDisco1[i]));
    Serial.println(tamUnits1[i]);
    Serial.println(nombreDiscResp1[i]);
    Serial.println();
   }
   

    //SW
    for(int i=0;i<1;i++){
      
   
    Serial.print("SW:");
    Serial.println(sysNameRouter[i]);
    Serial.println(sysLocationRouter[i]);
    Serial.println(convertir(time2[i]));
    Serial.println(ifDescrPuertos[i]);
    Serial.println(operStatus[i]);
    Serial.println(inPackets[i]);
    Serial.println();
   }
*/
}




void sendHttpResponse(WiFiClient client) {
  client.print("HTTP/1.1 200 OK\r\n"
             "Content-Type: text/html\r\n"
             "Connection: close\r\n"
             "\r\n"
             "<!DOCTYPE html>"
             "<html lang=\"es\">"
             "<head>"
             "<meta charset=\"UTF-8\">"
             "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">"
             "<title>Proyecto Gestion de Redes 2024</title>"
             "<link rel=\"icon\" href=\"https://web.dev/static/articles/building/an-adaptive-favicon/image/example-dark-theme-favico-6a6f6f67bc3e7.png\" type=\"image/x-icon\">\r\n"
             "<style>"
             "body {"
             "  font-family: Arial, sans-serif;"
             "  background-color: #f4f4f4;"
             "  color: #333;"
             "  margin: 0;"
             "  padding: 0;"
             "  display: flex;"
             "  flex-direction: column;"
             "  min-height: 100vh;"
             "}"
             "main {"
             "  flex: 1;"
             "  padding: 50px;"
             "  text-align: center;"
             "}"
             "h1 {"
             "  color: #0056b3;"
             "  font-size: 36px;"
             "  margin-bottom: 10px;"
             "}"
             "h2 {"
             "  color: #333;"
             "  font-size: 28px;"
             "  margin-bottom: 20px;"
             "}"
             "h3 {"
             "  color: #666;"
             "  font-size: 24px;"
             "  margin-bottom: 30px;"
             "}"
             "p {"
             "  font-size: 18px;"
             "  color: #555;"
             "}"
             "hr {"
             "  border: 0;"
             "  height: 1px;"
             "  background: #ccc;"
             "  margin: 20px 0;"
             "}"
             "table {"
             "  margin: 20px auto;"
             "  border-collapse: collapse;"
             "  width: 80%;"
             "  max-width: 600px;"
             "  background-color: #ffffff;"
             "  box-shadow: 0 4px 8px rgba(0, 0, 0, 0.1);"
             "}"
             "th, td {"
             "  border: 1px solid #dddddd;"
             "  padding: 12px 15px;"
             "  text-align: left;"
             "}"
             "th {"
             "  background-color: #0056b3;"
             "  color: white;"
             "  text-transform: uppercase;"
             "  font-weight: 600;"
             "}"
             "tr:nth-child(even) {"
             "  background-color: #f9f9f9;"
             "}"
             "footer {"
             "  position: fixed;"
             "  bottom: 0;"
             "  left: 0;"
             "  width: 100%;"
             "  background-color: #f4f4f4;"
             "  padding: 10px 0;"
             "  color: #333;"
             "  text-align: center;"
             "  border-top: 1px solid #ccc;"
             "}"
             ".config-form {"
             "  margin: 30px auto;"
             "  padding: 20px;"
             "  background: white;"
             "  border-radius: 5px;"
             "  box-shadow: 0 2px 4px rgba(0,0,0,0.1);"
             "  max-width: 400px;"
             "}"
             ".form-group {"
             "  margin-bottom: 15px;"
             "}"
             ".form-group label {"
             "  display: block;"
             "  margin-bottom: 5px;"
             "  font-weight: bold;"
             "}"
             ".form-group input {"
             "  width: 100%;"
             "  padding: 8px;"
             "  border: 1px solid #ddd;"
             "  border-radius: 4px;"
             "}"
             ".form-submit {"
             "  background-color: #0056b3;"
             "  color: white;"
             "  border: none;"
             "  padding: 10px 15px;"
             "  border-radius: 4px;"
             "  cursor: pointer;"
             "}"
             ".form-submit:hover {"
             "  background-color: #004494;"
             "}"
             "</style>"
             "</head>"
             "<body>"
             "<main>"
             "<h1>Trabajo Fin de Grado VZG</h1>"
             "<hr />"
             "<h2>Mota con Protocolos de Gestión</h2>"
             "<h3>Número de visitas: ");
  client.print(numAccesos);
  client.print("</h3>"
             "<div class='config-form'>"
             "<h3>Configuración del Intervalo SNMP</h3>"
             "<p>Intervalo actual: ");
  client.print(pollInterval / 1000);
  client.print(" segundos</p>"
             "<form action='/update' method='POST'>"
             "<div class='form-group'>"
             "<label for='interval'>Nuevo intervalo (segundos):</label>"
             "<input type='number' id='interval' name='interval' min='5' max='300' value='");
  client.print(pollInterval / 1000);
  client.print("' required>"
             "</div>"
             "<input type='submit' class='form-submit' value='Actualizar Intervalo'>"
             "</form>"
             "</div>"
             "<h4>Equipos gestionados vía SNMP:</h4>"
             "<table>"
             "<tr><th>Nombre del Equipo</th><th>Dirección IP</th></tr>");

  // Imprimir cada fila con la dirección IP y el nombre del equipo
  for (int i = 0; i < 2; i++) {
    client.print("<tr><td>");
    client.print(sysName1[i]);
    client.print("</td><td>");
    client.print("192.168.1."+String(octetos[i]));
    client.print("</td></tr>");
  }

  client.print("</table>"
             "<h4>Detalles de Interfaces:</h4>"
             "<table>"
             "<tr><th>Nombre del Switch/Router</th><th>Localización</th><th>Descripción de la Interfaz</th></tr>");

  // Imprimir cada fila con la dirección IP y la descripción de la interfaz
  for (int i = 0; i < 5; i++) {
    client.print("<tr><td>");
    client.print(sysName2[i]);
    client.print("</td><td>");
    client.print(sysLocation2[i]);
    client.print("</td><td>");
    client.print(ifDescr[i]);
    client.print("</td></tr>");
  }

  client.print("</table>"
             "</main>"
             "<footer>"
             "@Vicente Zamora García. Contacto: <a href=\"mailto:vicente.zamgar@gmail.com\">vicente.zamgar@gmail.com</a>"
             "</footer>"
             "</body>"
             "</html>");
}


void handleUpdateInterval(WiFiClient client, String request) {
  // Buscar el valor del intervalo en la solicitud POST
  int intervalIndex = request.indexOf("interval=");
  if (intervalIndex != -1) {
    String intervalStr = request.substring(intervalIndex + 9);
    intervalStr = intervalStr.substring(0, intervalStr.indexOf(' '));
    int newInterval = intervalStr.toInt() * 1000; // Convertir a milisegundos
    
    if (newInterval >= 5000 && newInterval <= 300000) { // Validar rango (5-300 segundos)
      pollInterval = newInterval;
      client.print("HTTP/1.1 200 OK\r\n"
                 "Content-Type: text/html\r\n"
                 "Connection: close\r\n"
                 "\r\n"
                 "<!DOCTYPE html>"
                 "<html><body>"
                 "<h1>Intervalo actualizado</h1>"
                 "<p>Nuevo intervalo: ");
      client.print(pollInterval / 1000);
      client.print(" segundos</p>"
                 "<a href='/'>Volver</a>"
                 "</body></html>");
      
      Serial.print("Intervalo actualizado a: ");
      Serial.print(pollInterval / 1000);
      Serial.println(" segundos");
      return;
    }
  }
  
  // Si hay algún error
  client.print("HTTP/1.1 400 Bad Request\r\n"
             "Content-Type: text/html\r\n"
             "Connection: close\r\n"
             "\r\n"
             "<!DOCTYPE html>"
             "<html><body>"
             "<h1>Error</h1>"
             "<p>Valor no válido. Debe estar entre 5 y 300 segundos.</p>"
             "<a href='/'>Volver</a>"
             "</body></html>");
}

void get1SNMP() {
for (int i=0; i<2;i++){

  
  IPAddress router(192, 168, 1,octetos[i]);
    //PC1
   callbackSysName1 = snmp.addStringHandler(router, ".1.3.6.1.2.1.1.5.0", &sysNameResponse1[i]); 
   tiempo1 = snmp.addTimestampHandler(router, ".1.3.6.1.2.1.1.3.0", &time1[i]); 
   TCP1 = snmp.addGaugeHandler(router, ".1.3.6.1.2.1.6.9.0", &tcpcon1[i]); 
   callbackCargaCPU1= snmp.addIntegerHandler(router, ".1.3.6.1.2.1.25.3.3.1.2.5", &carga1[i]);
   callbackNombreDiscoDuro1 = snmp.addStringHandler(router, ".1.3.6.1.2.1.25.2.3.1.3.1", &nombreDiscResp1[i]); 
   diskResp1 = snmp.addIntegerHandler(router, ".1.3.6.1.2.1.25.2.3.1.6.1", &tamDisco1[i]); 
   diskTamUnits1 = snmp.addIntegerHandler(router, ".1.3.6.1.2.1.25.2.3.1.4.1", &tamUnits1[i]); 
   //

  snmpRequest.addOIDPointer(callbackSysName1);
  
  snmpRequest.addOIDPointer(diskResp1); 
 snmpRequest.addOIDPointer(TCP1);
   snmpRequest.addOIDPointer(callbackCargaCPU1);
  snmpRequest.addOIDPointer(tiempo1);
  snmpRequest.addOIDPointer(diskTamUnits1);
  snmpRequest.addOIDPointer(callbackNombreDiscoDuro1);
  
  snmpRequest.setIP(WiFi.localIP());
  snmpRequest.setUDP(&udp);
  snmpRequest.setRequestID(random()%5555);
  snmpRequest.sendTo(router);
  snmpRequest.clearOIDList();
  



  

}}


void get2SNMP() {
  
for (int i=0; i<5;i++){






  IPAddress sw(192, 168, 1,octetos2[i]);
    //PC1
   callbackSysName2 = snmp.addStringHandler(sw, ".1.3.6.1.2.1.1.5.0", &sysNameRouter[i]); 
   callbackLocation = snmp.addStringHandler(sw, ".1.3.6.1.2.1.1.6.0", &sysLocationRouter[i]); 
   encendido= snmp.addTimestampHandler(sw, ".1.3.6.1.2.1.1.3.0", &time2[i]);

    
    
   
   if(octetos2[i]==206){
   callbackDescr = snmp.addStringHandler(sw, ".1.3.6.1.2.1.2.2.1.2.2", &ifDescrPuertos[i]);
   operStat = snmp.addIntegerHandler(sw, ".1.3.6.1.2.1.2.2.1.8.2", &operStatus[i]); 
   packetsIn = snmp.addCounter32Handler(sw, ".1.3.6.1.2.1.2.2.1.11.2", &inPackets[i]); 
    // Variables BGP
  bgpMessagesReceived = snmp.addCounter32Handler(sw, ".1.3.6.1.2.1.15.3.1.12.172.20.10.2", &bgpMsgsReceived[i]); 
  bgpUpdatesIN = snmp.addCounter32Handler(sw, ".1.3.6.1.2.1.15.3.1.10.172.20.10.2", &bgpUpdates[i]); 
  bgpUptime = snmp.addGaugeHandler(sw, ".1.3.6.1.2.1.15.3.1.16.172.20.10.2", &bgpTime[i]); 
  bgpASNNeighbor = snmp.addIntegerHandler(sw, ".1.3.6.1.2.1.15.3.1.9.172.20.10.2", &bgpASN[i]);
    
   }else{
   callbackDescr = snmp.addStringHandler(sw, ".1.3.6.1.2.1.2.2.1.2.1", &ifDescrPuertos[i]);
   operStat = snmp.addIntegerHandler(sw, ".1.3.6.1.2.1.2.2.1.8.1", &operStatus[i]); 
   packetsIn = snmp.addCounter32Handler(sw, ".1.3.6.1.2.1.2.2.1.11.1", &inPackets[i]); 
    // Variables BGP
  bgpMessagesReceived = snmp.addCounter32Handler(sw, ".1.3.6.1.2.1.15.3.1.12.192.168.1.206", &bgpMsgsReceived[i]); 
  bgpUpdatesIN = snmp.addCounter32Handler(sw, ".1.3.6.1.2.1.15.3.1.10.192.168.1.206", &bgpUpdates[i]); 
  bgpUptime = snmp.addGaugeHandler(sw, ".1.3.6.1.2.1.15.3.1.16.192.168.1.206", &bgpTime[i]); 
  bgpASNNeighbor = snmp.addIntegerHandler(sw, ".1.3.6.1.2.1.15.3.1.9.192.168.1.206", &bgpASN[i]);
   }
   //

  snmpRequest.addOIDPointer(callbackSysName2);
  snmpRequest.addOIDPointer(callbackLocation); 
 snmpRequest.addOIDPointer(encendido);
   snmpRequest.addOIDPointer(callbackDescr);
  snmpRequest.addOIDPointer(operStat);
  snmpRequest.addOIDPointer(packetsIn);

  snmpRequest.addOIDPointer(bgpMessagesReceived);
snmpRequest.addOIDPointer(bgpASNNeighbor);
snmpRequest.addOIDPointer(bgpUptime);
snmpRequest.addOIDPointer(bgpUpdatesIN);
  
  //snmpRequest.addOIDPointer(callbackNombreDiscoDuro1);
  
  snmpRequest.setIP(WiFi.localIP());
  snmpRequest.setUDP(&udp);
  snmpRequest.setRequestID(random()%5555);
  snmpRequest.sendTo(sw);
  snmpRequest.clearOIDList();
  

}}


String convertirGB(uint32_t tam, uint32_t *bytes ) {

  // CONVERSION DE LOS BITES RECIBIDOS A GB, usando hrAllocationUnit y hrStorageUsed recibidos por SNMP, el primero indica el tamaño de bloque, aunque generalmente en Windows y Linux es de 4096 bytes.
  // CÁLCULO ESCALONADO DEBIDO AL GRAN NÚMERO DE CIFRAS SIGNIFICATIVAS (supera el valor 2^32 que es el máximo para unsigned long y uint32_t).
  unsigned long gb;
  unsigned long mb;
  unsigned long kb;
  kb= (*bytes/(1024))*tam;
  mb=kb/1024;
  gb=mb/1024;
  *bytes=gb;
  
  String result = "GB Usados: " + String(gb);
  return result;
}

String convertir(uint32_t timeticks) {
  unsigned long total_seconds;
  unsigned long hours;
  unsigned long minutes;
  unsigned long seconds;
 
  
  // Convertir timeticks a segundos (1 timetick = 1/100 segundos)
  total_seconds = timeticks / 100;


  // Calcular horas, minutos y segundos
  hours = total_seconds / 3600;
  total_seconds = total_seconds % 3600;
  minutes = total_seconds / 60;
  seconds = total_seconds % 60;

  // Crear el string con el formato deseado
  String result = "Horas: " + String(hours) + ", Minutos: " + String(minutes) + ", Segundos: " + String(seconds) ;
  return result;
}



void combinarVariables() {
    // Inicializar la cadena de resultado como vacía
    resultado[0] = '\0';



   //CONCATENA VARIABLES DE PC'S
    for (int i = 0; i < 2; i++) {
        char buffer[10000]; // Buffer temporal para cada conjunto de valores

        // Crear la cadena para las variables actuales
        snprintf(buffer, sizeof(buffer), "%s*%s*%u*%u*%u*%u*%u",
                 sysName1[i], nombreDisco1[i], time1[i], tcpcon1[i], 
                 carga1[i], tamDisco1[i], tamUnits1[1]);

        // Concatenar al resultado final
        strcat(resultado, buffer);

        // Agregar un asterisco al final excepto para el último conjunto de variables
       
            strcat(resultado, "*");
        
    }


//CONCATENA VARIABLES DE SW y/o ROUTERS
for (int i = 0; i < 5; i++) {
    char buffer2[10000]; // Buffer temporal para cada conjunto de valores




    // Crear la cadena para las variables actuales, incluyendo las variables BGP
    snprintf(buffer2, sizeof(buffer2), "%s*%s*%s*%u*%u*%u*%u*%u*%u*%u",
             sysName2[i], sysLocation2[i], ifDescr[i], time2[i], 
             operStatus[i], inPackets[i], bgpASN[i], bgpTime[i], 
             bgpUpdates[i], bgpMsgsReceived[i]);

    // Concatenar al resultado final
    strcat(resultado, buffer2);

    // Agregar un asterisco al final excepto para el último conjunto de variables
    if (i != 4) {  
        strcat(resultado, "*");
    }

        

}

  // Añadir * + valor de numAccesos al final de resultado
      char bufferNumAccesos[20];
      snprintf(bufferNumAccesos, sizeof(bufferNumAccesos), "*%u", numAccesos);
      strcat(resultado, bufferNumAccesos);
}
