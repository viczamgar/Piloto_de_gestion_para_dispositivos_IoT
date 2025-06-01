#include "Variable.h"

uint32_t locUpTime              = 0;                                           
char locContact[20]             = "Vicente";                      
char locName[20]                = "Prototipo 1";                                
char locLocation[20]            = "Dormitorio";                                
char ip[15]                         = "192.168.1.177";   
char email[30]                      = "trabajogredes24@gmail.com";       
char phonenumber[10]                = "639358137";
byte mi_IP[4]       = {192, 168, 1, 72};                            


int32_t index1              = 1;
int32_t index2             = 2; 
int32_t index3             = 3; 
int32_t index4             = 4; 
int32_t index5             =5;
int32_t index6             = 6; 
int32_t index7             = 7; 
int32_t index8             = 8; 

char descr1[25] = "Sensor de Temperatura";
char descr2[20] = "Sensor Capacitivo";
char descr3[20] = "Voltimetro";
char descr4[20] = "Accesos Web";
char descr5[25] = "Humedad del suelo";
char descr6[20] = "Ram Disponible";
char descr7[20] = "Frecuencia Sonido";
char descr8[20] = "Microfono";

int32_t sensortemp             = 7;  
int32_t sensorhum              = 0;  
int32_t valorled1              ; 
int32_t accesosWeb            = 0;
int32_t sensorSuelo             = 0;  
int32_t ram              = 0;  
int32_t sensorSonido              ; 
int32_t microfono            = 0;


uint32_t instanteLectura1 = 0;
uint32_t instanteLectura2 = 0;
uint32_t instanteLectura3 = 0;
uint32_t instanteLectura4 = 0;
uint32_t instanteLectura5 = 0;
uint32_t instanteLectura6 = 0;
uint32_t instanteLectura7 = 0;
uint32_t instanteLectura8 = 0;

int32_t tiempoMuestreo1 = 10;
int32_t tiempoMuestreo2 = 10;
int32_t tiempoMuestreo3 = 10;
int32_t tiempoMuestreo4 = 10;
int32_t tiempoMuestreo5 = 10;
int32_t tiempoMuestreo6 = 10;
int32_t tiempoMuestreo7 = 3;
int32_t tiempoMuestreo8 = 3;

// EVENTOS

 int32_t ind1 = 1;
 int32_t ind2 = 2;
 int32_t ind3 = 3;
 int32_t ind4 = 4;
 int32_t ind5 = 5;

 int32_t Sind1;
 int32_t Sind2;;
 int32_t Sind3;
 int32_t Sind4;
 int32_t Sind5;

char d1[20] = "mensaje evento1";
char d2[20] = "mensaje evento2";
char d3[20] ="mensaje evento3" ;
char d4[20] ="mensaje evento4";
char d5[20] = "mensaje evento5";

 int32_t uS1;
 int32_t uS2;
 int32_t uS3;
 int32_t uS4;
 int32_t uS5;

 int32_t uI1=0;
 int32_t uI2;
 int32_t uI3;
 int32_t uI4;
 int32_t uI5;

 byte ip1[4];
 byte ip2[4];
 byte ip3[4];
 byte ip4[4];
 byte ip5[4];



// Inicialización de variables a 0 o null

//
char sysName1[2][50] = {{""}, {""}};
char nombreDisco1[2][50] = {{""}, {""}};
uint32_t time1[2] = {0, 0};
int32_t tcpcon1[2] = {0, 0};
int32_t carga1[2] = {0, 0};
uint32_t tamDisco1[2] = {0, 0};
uint32_t tamUnits1[2] = {0, 0};
uint32_t tamTotal1[2] = {0, 0};

// Para R1,R2,R3,R4,R5
char sysName2[5][50] = {{""}, {""}, {""}, {""}, {""}};
uint32_t time2[5] = {0, 0, 0, 0, 0};
char sysLocation2[5][50] = {{'\0'}, {'\0'}, {'\0'}, {'\0'}, {'\0'}};
char ifDescr[5][50] = {{'\0'}, {'\0'}, {'\0'}, {'\0'}, {'\0'}};
int32_t operStatus[5] = {0, 0, 0, 0, 0};
int32_t inPackets[5] = {0, 0, 0, 0, 0};



// Para BGP
char nodoBGP[5][50] = {{"\0"}, {"\0"}, {"\0"}, {"\0"}, {"\0"}};        // Nombre o dirección del nodo origen BGP
int32_t vecinoASN[5] = {0, 0, 0, 0, 0};                                  // ASN (Autonomous System Number) del vecino BGP
uint32_t tiempoConexion[5] = {0, 0, 0, 0, 0};                           // Tiempo de conexión BGP en segundos
uint32_t mensajesUpdate[5] = {0, 0, 0, 0, 0};                           // Número de mensajes BGP Update enviados
uint32_t mensajesTotales[5] = {0, 0, 0, 0, 0};                          // Número total de mensajes BGP enviados
int32_t indiceBGP[5] = {0, 0, 0, 0, 0};                                 // Índice único de la entrada BGP
