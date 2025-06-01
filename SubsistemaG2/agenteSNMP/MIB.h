#ifndef _MIB_H_
#define _MIB_H_


const char led1set[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.4"; // read-only  (Integer)
const char led2set[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.5"; // read-only  (Integer)





const char sysName[] PROGMEM = "1.3.6.1.4.1.36582.1.1.0"; 
const char sysLocation[] PROGMEM = "1.3.6.1.4.1.36582.1.2.0";
const char ipAddress[] PROGMEM = "1.3.6.1.4.1.36582.1.3.0";
const char sysUpTime[] PROGMEM = "1.3.6.1.4.1.36582.1.4.0"; 
const char sysContact[] PROGMEM = "1.3.6.1.4.1.36582.1.5.0"; 
const char telef[] PROGMEM = "1.3.6.1.4.1.36582.1.6.0";
const char mail[] PROGMEM = "1.3.6.1.4.1.36582.1.7.0";






// VALORES LEIDOS
const char ifIndex1[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.1.1"; // read-only  (Integer)
const char ifIndex2[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.1.2"; // read-only  (Integer)
const char ifIndex3[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.1.3"; // read-only  (Integer)
const char ifIndex4[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.1.4"; // read-only  (Integer)
const char ifIndex5[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.1.5"; // read-only  (Integer)
const char ifIndex6[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.1.6"; // read-only  (Integer)
const char ifIndex7[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.1.7"; // read-only  (Integer)
const char ifIndex8[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.1.8"; // read-only  (Integer)


const char des1[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.2.1"; // read-only  (Integer)
const char des2[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.2.2"; // read-only  (Integer)
const char des3[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.2.3"; // read-only  (Integer)
const char des4[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.2.4"; // read-only  (Integer)
const char des5[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.2.5"; // read-only  (Integer)
const char des6[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.2.6"; // read-only  (Integer)
const char des7[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.2.7"; // read-only  (Integer)
const char des8[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.2.8"; // read-only  (Integer)



const char sensorTemp[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.3.1"; // read-only  (Integer)
const char sensorHum[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.3.2"; // read-only  (Integer)
const char led1[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.3.3"; // read-only  (Integer)
const char tempArd[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.3.4"; // read-only  (Integer)
const char humSuelo[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.3.5"; // read-only  (Integer)
const char ramD[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.3.6"; // read-only  (Integer)
const char freq[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.3.7"; // read-only  (Integer)
const char micro[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.3.8"; // read-only  (Integer)

const char inst1[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.4.1"; // read-only  (Integer)
const char inst2[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.4.2"; // read-only  (Integer)
const char inst3[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.4.3"; // read-only  (Integer)
const char inst4[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.4.4"; // read-only  (Integer)
const char inst5[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.4.5"; // read-only  (Integer)
const char inst6[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.4.6"; // read-only  (Integer)
const char inst7[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.4.7"; // read-only  (Integer)
const char inst8[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.4.8"; // read-only  (Integer)

const char tMuestreo1[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.5.1"; // read-only  (Integer)
const char tMuestreo2[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.5.2"; // read-only  (Integer)
const char tMuestreo3[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.5.3"; // read-only  (Integer)
const char tMuestreo4[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.5.4"; // read-only  (Integer)
const char tMuestreo5[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.5.5"; // read-only  (Integer)
const char tMuestreo6[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.5.6"; // read-only  (Integer)
const char tMuestreo7[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.5.7"; // read-only  (Integer)
const char tMuestreo8[] PROGMEM = "1.3.6.1.4.1.36582.2.1.1.5.8"; // read-only  (Integer)






//EVENTOS 
const char Index1[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.1.1"; // read-only  (Integer)
const char Index2[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.1.2"; // read-only  (Integer)
const char Index3[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.1.3"; // read-only  (Integer)
const char Index4[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.1.4"; // read-only  (Integer)
const char Index5[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.1.5"; // read-only  (Integer)


const char s1[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.2.1"; // read-only  (Integer)
const char s2[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.2.2"; // read-only  (Integer)
const char s3[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.2.3"; // read-only  (Integer)
const char s4[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.2.4"; // read-only  (Integer)
const char s5[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.2.5"; // read-only  (Integer)


const char dd1[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.3.1"; // read-only  (Integer)
const char dd2[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.3.2"; // read-only  (Integer)
const char dd3[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.3.3"; // read-only  (Integer)
const char dd4[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.3.4"; // read-only  (Integer)
const char dd5[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.3.5"; // read-only  (Integer)

const char us1[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.4.1"; // read-only  (Integer)
const char us2[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.4.2"; // read-only  (Integer)
const char us3[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.4.3"; // read-only  (Integer)
const char us4[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.4.4"; // read-only  (Integer)
const char us5[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.4.5"; // read-only  (Integer)

const char is1[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.5.1"; // read-only  (Integer)
const char is2[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.5.2"; // read-only  (Integer)
const char is3[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.5.3"; // read-only  (Integer)
const char is4[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.5.4"; // read-only  (Integer)
const char is5[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.5.5"; // read-only  (Integer)

const char IP1[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.6.1"; // read-only  (Integer)
const char IP2[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.6.2"; // read-only  (Integer)
const char IP3[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.6.3"; // read-only  (Integer)
const char IP4[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.6.4"; // read-only  (Integer)
const char IP5[] PROGMEM = "1.3.6.1.4.1.36582.3.1.1.6.5"; // read-only  (Integer)



//PC


const char indPC1[] PROGMEM = "1.3.6.1.4.1.36582.4.1.1.1.1"; // read-only  (Integer)
const char indPC2[] PROGMEM = "1.3.6.1.4.1.36582.4.1.1.1.2"; // read-only  (Integer)

const char nPC1[] PROGMEM = "1.3.6.1.4.1.36582.4.1.1.2.1"; // read-only  (Integer)
const char nPC2[] PROGMEM = "1.3.6.1.4.1.36582.4.1.1.2.2"; // read-only  (Integer)

const char timePC1[] PROGMEM = "1.3.6.1.4.1.36582.4.1.1.3.1"; // read-only  (Integer)
const char timePC2[] PROGMEM = "1.3.6.1.4.1.36582.4.1.1.3.2"; // read-only  (Integer)

const char cPC1[] PROGMEM = "1.3.6.1.4.1.36582.4.1.1.4.1"; // read-only  (Integer)
const char cPC2[] PROGMEM = "1.3.6.1.4.1.36582.4.1.1.4.2"; // read-only  (Integer)

const char tPC1[] PROGMEM = "1.3.6.1.4.1.36582.4.1.1.5.1"; // read-only  (Integer)
const char tPC2[] PROGMEM = "1.3.6.1.4.1.36582.4.1.1.5.2"; // read-only  (Integer)

const char ndPC1[] PROGMEM = "1.3.6.1.4.1.36582.4.1.1.6.1"; // read-only  (Integer)
const char ndPC2[] PROGMEM = "1.3.6.1.4.1.36582.4.1.1.6.2"; // read-only  (Integer)

const char elPC1[] PROGMEM = "1.3.6.1.4.1.36582.4.1.1.7.1"; // read-only  (Integer)
const char elPC2[] PROGMEM = "1.3.6.1.4.1.36582.4.1.1.7.2"; // read-only  (Integer)


//SW


// R1
const char indSW1[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.1.1"; 
const char nSW1[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.2.1"; 
const char timeSW1[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.3.1"; 
const char locSW1[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.4.1"; 
const char pSW1[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.5.1"; 
const char epSW1[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.6.1";
const char bySW1[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.7.1"; 

// Switch 2
const char indSW2[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.1.2"; // read-only  (Integer)
const char nSW2[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.2.2"; // read-only  (Integer)
const char timeSW2[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.3.2"; // read-only  (Integer)
const char locSW2[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.4.2"; // read-only  (Integer)
const char pSW2[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.5.2"; // read-only  (Integer)
const char epSW2[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.6.2"; // read-only  (Integer)
const char bySW2[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.7.2"; // read-only  (Integer)

// Switch 3
const char indSW3[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.1.3"; // read-only  (Integer)
const char nSW3[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.2.3"; // read-only  (Integer)
const char timeSW3[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.3.3"; // read-only  (Integer)
const char locSW3[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.4.3"; // read-only  (Integer)
const char pSW3[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.5.3"; // read-only  (Integer)
const char epSW3[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.6.3"; // read-only  (Integer)
const char bySW3[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.7.3"; // read-only  (Integer)

// Switch 4
const char indSW4[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.1.4"; // read-only  (Integer)
const char nSW4[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.2.4"; // read-only  (Integer)
const char timeSW4[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.3.4"; // read-only  (Integer)
const char locSW4[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.4.4"; // read-only  (Integer)
const char pSW4[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.5.4"; // read-only  (Integer)
const char epSW4[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.6.4"; // read-only  (Integer)
const char bySW4[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.7.4"; // read-only  (Integer)

// Switch 5
const char indSW5[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.1.5"; // read-only  (Integer)
const char nSW5[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.2.5"; // read-only  (Integer)
const char timeSW5[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.3.5"; // read-only  (Integer)
const char locSW5[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.4.5"; // read-only  (Integer)
const char pSW5[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.5.5"; // read-only  (Integer)
const char epSW5[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.6.5"; // read-only  (Integer)
const char bySW5[] PROGMEM = "1.3.6.1.4.1.36582.5.1.1.7.5"; // read-only  (Integer)


// BGP Tabla - Fila 1
const char indBGP1[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.1.1"; // read-only (Integer)
const char nodoBGP1[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.2.1"; // read-only (DisplayString)
const char vecinoASN1[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.3.1"; // read-only (Integer)
const char tiempoCon1[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.4.1"; // read-only (Gauge32)
const char mensajesUpdate1[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.5.1"; // read-only (Counter32)
const char mensajesTotales1[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.6.1"; // read-only (Counter32)

// BGP Tabla - Fila 2
const char indBGP2[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.1.2"; // read-only (Integer)
const char nodoBGP2[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.2.2"; // read-only (DisplayString)
const char vecinoASN2[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.3.2"; // read-only (Integer)
const char tiempoCon2[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.4.2"; // read-only (Gauge32)
const char mensajesUpdate2[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.5.2"; // read-only (Counter32)
const char mensajesTotales2[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.6.2"; // read-only (Counter32)

// BGP Tabla - Fila 3
const char indBGP3[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.1.3"; // read-only (Integer)
const char nodoBGP3[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.2.3"; // read-only (DisplayString)
const char vecinoASN3[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.3.3"; // read-only (Integer)
const char tiempoCon3[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.4.3"; // read-only (Gauge32)
const char mensajesUpdate3[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.5.3"; // read-only (Counter32)
const char mensajesTotales3[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.6.3"; // read-only (Counter32)

// BGP Tabla - Fila 4
const char indBGP4[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.1.4"; // read-only (Integer)
const char nodoBGP4[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.2.4"; // read-only (DisplayString)
const char vecinoASN4[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.3.4"; // read-only (Integer)
const char tiempoCon4[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.4.4"; // read-only (Gauge32)
const char mensajesUpdate4[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.5.4"; // read-only (Counter32)
const char mensajesTotales4[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.6.4"; // read-only (Counter32)

// BGP Tabla - Fila 5
const char indBGP5[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.1.5"; // read-only (Integer)
const char nodoBGP5[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.2.5"; // read-only (DisplayString)
const char vecinoASN5[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.3.5"; // read-only (Integer)
const char tiempoCon5[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.4.5"; // read-only (Gauge32)
const char mensajesUpdate5[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.5.5"; // read-only (Counter32)
const char mensajesTotales5[] PROGMEM = "1.3.6.1.4.1.36582.6.1.1.6.5"; // read-only (Counter32)




 //VALOR INVENTADO PARA QUE LA TABLA CARGUE BIEN 

const char inventado[] PROGMEM = "1.3.6.1.4.1.36582.7.1.1.1.1"; // read-only  (Integer)




      
#endif
