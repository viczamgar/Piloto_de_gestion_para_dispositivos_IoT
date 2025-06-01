

#ifndef AgenteSNMP_h
#define AgenteSNMP_h

#define PUERTO_DEFAULT_SNMP	161
#define SNMP_LONGITUD_MIN_OID	2
#define SNMP_LONGITUD_MAX_OID	200 // 128
#define LONGITUD_MAX_COMUNIDAD	20
#define SNMP_LONGITUD_MAX_INSTANCIA      200  // 128 ??? should limit this
#define SNMP_TAM_MAX_PAQUETE     SNMP_LONGITUD_MAX_INSTANCIA + SNMP_LONGITUD_MAX_OID + 25  //???
#define SNMP_FREE(s)   do { if (s) { free((void *)s); s=NULL; } } while(0)
//Frees a pointer only if it is !NULL and sets its valor to NULL. 


#define DEBUG
#include "Arduino.h"
#include "Udp.h"
#include "MIB.h"
#include "Variable.h"

extern "C" {
    // callback function
    typedef void (*callbackPduRecibida)(void);
}

//typedef long long int64_t;
typedef unsigned long long uint64_t;
//typedef long int32_t;
//typedef unsigned long uint32_t;
//typedef unsigned char uint8_t;
//typedef short int16_t;
//typedef unsigned short uint16_t;

typedef union uint64_u {
    uint64_t uint64;
    byte data[8];
};

typedef union int32_u {
    int32_t int32;
    byte data[4];
};

typedef union uint32_u {
    uint32_t uint32;
    byte data[4];
};

typedef union int16_u {
    int16_t int16;
    byte data[2];
};

//typedef union uint16_u {
//	uint16_t uint16;
//	byte data[2];
//};

typedef enum ASN_BER_TIPOS {
    //   ASN/BER base types
    ASN_BER_TIPOS_UNIVERSAL = 0x0,
    ASN_BER_TIPOS_APPLICATION = 0x40,
    ASN_BER_TIPOS_CONTEXT = 0x80,
    ASN_BER_TIPOS_PUBLIC = 0xC0,
    ASN_BER_TIPOS_PRIMITIVE = 0x0,
    ASN_BER_TIPOS_CONSTRUCTOR = 0x20
};

typedef enum SNMP_TIPOS_PDU {
    // PDU choices
    SNMP_PDU_GET = ASN_BER_TIPOS_CONTEXT | ASN_BER_TIPOS_CONSTRUCTOR | 0,
    SNMP_PDU_GET_NEXT = ASN_BER_TIPOS_CONTEXT | ASN_BER_TIPOS_CONSTRUCTOR | 1,
    SNMP_PDU_RESPONSE = ASN_BER_TIPOS_CONTEXT | ASN_BER_TIPOS_CONSTRUCTOR | 2,
    SNMP_PDU_SET = ASN_BER_TIPOS_CONTEXT | ASN_BER_TIPOS_CONSTRUCTOR | 3,
    SNMP_PDU_TRAP = ASN_BER_TIPOS_CONTEXT | ASN_BER_TIPOS_CONSTRUCTOR | 4
};

typedef enum SNMP_TIPOS_TRAP {
    //   Trap generic types:
    SNMP_TRAP_COLD_START = 0,
    SNMP_TRAP_WARM_START = 1,
    SNMP_TRAP_LINK_DOWN = 2,
    SNMP_TRAP_LINK_UP = 3,
    SNMP_TRAP_AUTHENTICATION_FAIL = 4,
    SNMP_TRAP_EGP_NEIGHBORLOSS = 5,
    SNMP_TRAP_ENTERPRISE_SPECIFIC = 6
};

typedef enum SNMP_CODIGOS_ERROR {
    SNMP_SIN_ERROR = 0,
    SNMP_ERROR_Tam = 1,
    SNMP_ERR_NO_SUCH_NAME = 2,
    SNMP_ERR_BAD_VALUE = 3,
    SNMP_ERR_READ_ONLY = 4,
    SNMP_ERR_GEN_ERROR = 5,
    SNMP_ERR_NO_ACCESS = 6,
    SNMP_ERROR_TIPO = 7,
    SNMP_ERR_WRONG_LENGTH = 8,
    SNMP_ERR_WRONG_ENCODING = 9,
    SNMP_ERR_WRONG_VALUE = 10,
    SNMP_ERR_NO_CREATION = 11,
    SNMP_ERR_INCONSISTANT_VALUE = 12,
    SNMP_ERR_RESOURCE_UNAVAILABLE = 13,
    SNMP_ERR_COMMIT_FAILED = 14,
    SNMP_ERR_UNDO_FAILED = 15,
    SNMP_ERR_AUTHORIZATION_ERROR = 16,
    SNMP_ERR_NOT_WRITABLE = 17,
    SNMP_ERR_INCONSISTEN_NAME = 18
};

typedef enum SNMP_API_RESULTADO {
    ESTADO_API_SNMP_EXITO = 0,
    ESTADO_API_ERROR_MALLOC = 1,
    ESTADO_API_SNMP_LONGITUD_COMUNIDAD = 2,
    ESTADO_API_LONGITUD_OID = 3,
    ESTADO_API_LONGITUD_VALOR = 4,
    ESTADO_API_PAQUETE_INVALIDO = 5,
    ESTADO_API_SNMP_TAM_PAQUETE = 6,
    ESTADO_API_NO_SUCH_NAME = 7,
};

//
// http://oreilly.com/catalog/esnmp/chapter/ch02.html Table 2-1: SMIv1 Datatypes

typedef enum SNMP_SINTAXIS {
    //   SNMP ObjectSyntax values
    SNMP_SINTAXIS_SEQUENCE = ASN_BER_TIPOS_UNIVERSAL | ASN_BER_TIPOS_CONSTRUCTOR | 0x10,
    //   These values are used in the "syntax" member of VALUEs
    SNMP_SINTAXIS_BOOL = ASN_BER_TIPOS_UNIVERSAL | ASN_BER_TIPOS_PRIMITIVE | 1,
    SNMP_SINTAXIS_INT = ASN_BER_TIPOS_UNIVERSAL | ASN_BER_TIPOS_PRIMITIVE | 2,
    SNMP_SINTAXIS_BITS = ASN_BER_TIPOS_UNIVERSAL | ASN_BER_TIPOS_PRIMITIVE | 3,
    SNMP_SINTAXIS_OCTETS = ASN_BER_TIPOS_UNIVERSAL | ASN_BER_TIPOS_PRIMITIVE | 4,
    SNMP_SINTAXIS_NULL = ASN_BER_TIPOS_UNIVERSAL | ASN_BER_TIPOS_PRIMITIVE | 5,
    SNMP_SINTAXIS_OID = ASN_BER_TIPOS_UNIVERSAL | ASN_BER_TIPOS_PRIMITIVE | 6,
    SNMP_SINTAXIS_INT32 = SNMP_SINTAXIS_INT,
    SNMP_SINTAXIS_IP_ADDRESS = ASN_BER_TIPOS_APPLICATION | ASN_BER_TIPOS_PRIMITIVE | 0,
    SNMP_SINTAXIS_COUNTER = ASN_BER_TIPOS_APPLICATION | ASN_BER_TIPOS_PRIMITIVE | 1,
    SNMP_SINTAXIS_GAUGE = ASN_BER_TIPOS_APPLICATION | ASN_BER_TIPOS_PRIMITIVE | 2,
    SNMP_SINTAXIS_TIME_TICKS = ASN_BER_TIPOS_APPLICATION | ASN_BER_TIPOS_PRIMITIVE | 3,
    SNMP_SINTAXIS_OPAQUE = ASN_BER_TIPOS_APPLICATION | ASN_BER_TIPOS_PRIMITIVE | 4,
    SNMP_SINTAXIS_NSAPADDR = ASN_BER_TIPOS_APPLICATION | ASN_BER_TIPOS_PRIMITIVE | 5,
    SNMP_SINTAXIS_COUNTER64 = ASN_BER_TIPOS_APPLICATION | ASN_BER_TIPOS_PRIMITIVE | 6,
    SNMP_SINTAXIS_UINT32 = ASN_BER_TIPOS_APPLICATION | ASN_BER_TIPOS_PRIMITIVE | 7,
};

typedef struct SNMP_OID {
    byte data[SNMP_LONGITUD_MAX_OID]; // ushort array insted??
    size_t size;
    //

    byte * StringAOID(const char *buffer) {

        if (buffer[0] == '1' && buffer[1] == '.' && buffer[2] == '3' && buffer[3] == '.') {
            memset(data, 0, SNMP_LONGITUD_MAX_OID);
            data[0] = 0x2B;
            int fs_ilen = strlen(buffer);
            int fs_ic = 0;
            int fs_id = 1;
            char fs_Csl[5];
            memset(fs_Csl, 0, 5);
            for (int fs_i = 4; fs_i < fs_ilen; fs_i++) {
                if (buffer[fs_i] == '.') {
                    word fs_oidw = atol(fs_Csl);
                    if (fs_oidw < 128) {
                        data[fs_id] = fs_oidw;
                    } else if (fs_oidw < 16384) {
                        byte fs_loidb = lowByte(fs_oidw) & 127;
                        word fs_oidwtmp = fs_oidw << 1;
                        byte fs_hoidb = highByte(fs_oidwtmp) | 128;
                        data[fs_id] = fs_hoidb;
                        fs_id++;
                        data[fs_id] = fs_loidb;
                    } else {
                        byte fs_loidb = lowByte(fs_oidw) & 127;
                        word fs_oidwtmp = fs_oidw << 1;
                        byte fs_hoidb = highByte(fs_oidwtmp) | 128;
                        fs_oidwtmp = fs_oidw >> 14;
                        byte fs_hhoidb = (lowByte(fs_oidwtmp) & 3) | 128;
                        data[fs_id] = fs_hhoidb;
                        fs_id++;
                        data[fs_id] = fs_hoidb;
                        fs_id++;
                        data[fs_id] = fs_loidb;
                    }
                    memset(fs_Csl, 0, 5);
                    fs_ic = 0;
                    fs_id++;
                } else {
                    fs_Csl[fs_ic] = buffer[fs_i];
                    fs_ic++;
                }
            }
            word fs_oidw = atoi(fs_Csl);
            if (fs_oidw < 128) {
                data[fs_id] = fs_oidw;
            } else if (fs_oidw < 16384) {
                byte fs_loidb = lowByte(fs_oidw) & 127;
                word fs_oidwtmp = fs_oidw << 1;
                byte fs_hoidb = highByte(fs_oidwtmp) | 128;
                data[fs_id] = fs_hoidb;
                fs_id++;
                data[fs_id] = fs_loidb;
            } else {
                byte fs_loidb = lowByte(fs_oidw) & 127;
                word fs_oidwtmp = fs_oidw << 1;
                byte fs_hoidb = highByte(fs_oidwtmp) | 128;
                fs_oidwtmp = fs_oidw >> 14;
                byte fs_hhoidb = (lowByte(fs_oidwtmp) & 3) | 128;
                data[fs_id] = fs_hhoidb;
                fs_id++;
                data[fs_id] = fs_hoidb;
                fs_id++;
                data[fs_id] = fs_loidb;
            }

            fs_id++;
            size = fs_id;
       
            
            return data;
        }
    }
    //

    void OIDaString(char *buffer) {
        buffer[0] = '1';
        buffer[1] = '.';
        buffer[2] = '3';
        buffer[3] = '\0';
        //
        char buff[16];
        byte hsize = size - 1;
        byte hpos = 1;
        uint16_t subid;
        //
        while (hsize > 0) {
            subid = 0;
            uint16_t b = 0;
            do {
                uint16_t next = data[hpos++];
                b = next & 0xFF;
                subid = (subid << 7) + (b & ~0x80);
                hsize--;
            } while ((hsize > 0) && ((b & 0x80) != 0));
            utoa(subid, buff, 10);
            strcat(buffer, ".");
            strcat(buffer, buff);
        }
    };
};

// union for values?
//

typedef struct SNMP_VALUE {
    byte data[SNMP_LONGITUD_MAX_INSTANCIA];
    size_t size;
    SNMP_SINTAXIS syntax;
    //
    byte i; // for encoding/decoding functions
    //
    // clear's buffer and sets size to 0

    void clear(void) {
        memset(data, 0, SNMP_LONGITUD_MAX_INSTANCIA);
        size = 0;
    }

    //
    //
    // ASN.1 decoding functions
    //
    // decode's an octet string, object-identifier, opaque syntax to string

    SNMP_CODIGOS_ERROR decode(char *valor, size_t max_size) {
        if (syntax == SNMP_SINTAXIS_OCTETS || syntax == SNMP_SINTAXIS_OID
                || syntax == SNMP_SINTAXIS_OPAQUE) {
            if (strlen(valor) - 1 < max_size) {
                if (syntax == SNMP_SINTAXIS_OID) {
                    valor[0] = '1';
                    valor[1] = '.';
                    valor[2] = '3';
                    valor[3] = '\0';
                    //
                    char buff[16];
                    byte hsize = size - 1;
                    byte hpos = 1;
                    uint16_t subid;
                    //
                    while (hsize > 0) {
                        subid = 0;
                        uint16_t b = 0;
                        do {
                            uint16_t next = data[hpos++];
                            b = next & 0xFF;
                            subid = (subid << 7) + (b & ~0x80);
                            hsize--;
                        } while ((hsize > 0) && ((b & 0x80) != 0));
                        utoa(subid, buff, 10);
                        strcat(valor, ".");
                        strcat(valor, buff);
                    }
                } else {
                    for (i = 0; i < size; i++) {
                        valor[i] = (char) data[i];
                    }
                    valor[size] = '\0';
                }
                return SNMP_SIN_ERROR;
            } else {
                clear();
                return SNMP_ERROR_Tam;
            }
        } else {
            clear();
            return SNMP_ERROR_TIPO;
        }
    }
    //
    // decode's an int syntax to int16

    SNMP_CODIGOS_ERROR decode(int16_t *valor) {
        //Serial.println("Entra aquí2");
        if (syntax == SNMP_SINTAXIS_INT) {
            uint8_t *p = (uint8_t*) valor, i;
            memset(valor, 0, sizeof (*valor));
            //Serial.println("LLEGAAQUI");
            for (i = 0; i < size; i++) {
                *p++ = data[size - 1 - i];
            }
            return SNMP_SIN_ERROR;
        } else {
            clear();
            return SNMP_ERROR_TIPO;
        }
    }
    //
    // decode's an int32 syntax to int32

    SNMP_CODIGOS_ERROR decode1(int32_t *valor) {
        if (syntax == SNMP_SINTAXIS_INT32) {
            //Serial.println("LLEGA AQUI");
            //valorled1 = (data[0] != 0);
            //Serial.println(*valor);
            //Serial.println(valorled1);
            return SNMP_SIN_ERROR;
        } else {
            clear();
            return SNMP_ERROR_TIPO;
        }
    }

       SNMP_CODIGOS_ERROR decode2(int32_t *valor) {
        if (syntax == SNMP_SINTAXIS_INT32) {
            //Serial.println("LLEGA AQUI");
            //tiempoMuestreo1 = data[0];
            *valor = data[0];
            //Serial.println(*valor);
            //Serial.println(valorled1);
            return SNMP_SIN_ERROR;
        } else {
            clear();
            return SNMP_ERROR_TIPO;
        }
    }



     SNMP_CODIGOS_ERROR decode3(int32_t *valor) {
        if (syntax == SNMP_SINTAXIS_INT32) {
            //Serial.println("LLEGA AQUI");
            //tiempoMuestreo1 = data[0];
            *valor = data[0];
            //Serial.println(*valor);
            //Serial.println(valorled1);
            return SNMP_SIN_ERROR;
        } else {
            clear();
            return SNMP_ERROR_TIPO;
        }
    }
    //
    // decode's an uint32, counter, time-ticks, gauge syntax to uint32

    SNMP_CODIGOS_ERROR decode(uint32_t *valor) {
        if (syntax == SNMP_SINTAXIS_COUNTER || syntax == SNMP_SINTAXIS_TIME_TICKS
                || syntax == SNMP_SINTAXIS_GAUGE || syntax == SNMP_SINTAXIS_UINT32) {
            uint8_t *p = (uint8_t*) valor, i;
            memset(valor, 0, sizeof (*valor));
            for (i = 0; i < size; i++) {
                *p++ = data[size - 1 - i];
            }
            return SNMP_SIN_ERROR;
        } else {
            clear();
            return SNMP_ERROR_TIPO;
        }
    }
    //
    // decode's an ip-address, NSAP-address syntax to an ip-address byte array 

   SNMP_CODIGOS_ERROR decodeIP(byte *valor) {
    if (syntax == SNMP_SINTAXIS_IP_ADDRESS || syntax == SNMP_SINTAXIS_NSAPADDR) {
        if (size != 4) {
            // El Tam debe ser exactamente 4 bytes para una dirección IP
            clear();
            return SNMP_ERR_WRONG_LENGTH;
        }
        Serial.println((char)(data[0]));
        memcpy(valor, data, 4); // Copiar los 4 bytes de la dirección IP
        clear();
        return SNMP_SIN_ERROR;
    } else {
        clear();
        return SNMP_ERROR_TIPO;
    }
}
    
    //
    // decode's a boolean syntax to boolean

    SNMP_CODIGOS_ERROR decode(bool *valor) {
        if (syntax == SNMP_SINTAXIS_BOOL) {
            *valor = (data[0] != 0);
            return SNMP_SIN_ERROR;
        } else {
            clear();
            return SNMP_ERROR_TIPO;
        }
    }
    //
    //
    // ASN.1 encoding functions
    //
    // encode's a octet string to a string, opaque syntax
    // encode object-identifier here??

    SNMP_CODIGOS_ERROR encode(SNMP_SINTAXIS syn, const char *valor) {
        memset(data, 0, SNMP_LONGITUD_MAX_INSTANCIA);
        if (syn == SNMP_SINTAXIS_OCTETS || syn == SNMP_SINTAXIS_OPAQUE) {
            if (strlen(valor) - 1 < SNMP_LONGITUD_MAX_INSTANCIA) {
                syntax = syn;
                size = strlen(valor);
                for (i = 0; i < size; i++) {
                    data[i] = (byte) valor[i];
                }
                return SNMP_SIN_ERROR;
            } else {
                clear();
                return SNMP_ERROR_Tam;
            }
        } else {
            clear();
            return SNMP_ERROR_TIPO;
        }
    }
    //
    // encode's an int16 to int, opaque  syntax

    SNMP_CODIGOS_ERROR encode(SNMP_SINTAXIS syn, const int16_t valor) {
        memset(data, 0, SNMP_LONGITUD_MAX_INSTANCIA);
        if (syn == SNMP_SINTAXIS_INT || syn == SNMP_SINTAXIS_OPAQUE) {
            int16_u tmp;
            size = 2;
            syntax = syn;
            tmp.int16 = valor;
            data[0] = tmp.data[1];
            data[1] = tmp.data[0];
            return SNMP_SIN_ERROR;
        } else {
            clear();
            return SNMP_ERROR_TIPO;
        }
    }
    //
    // encode's an int32 to int32, opaque  syntax

    SNMP_CODIGOS_ERROR encode(SNMP_SINTAXIS syn, const int32_t valor) {
        memset(data, 0, SNMP_LONGITUD_MAX_INSTANCIA);
        if (syn == SNMP_SINTAXIS_INT32 || syn == SNMP_SINTAXIS_OPAQUE) {
            int32_u tmp;
            size = 4;
            syntax = syn;
            tmp.int32 = valor;
            data[0] = tmp.data[3];
            data[1] = tmp.data[2];
            data[2] = tmp.data[1];
            data[3] = tmp.data[0];
            return SNMP_SIN_ERROR;
        } else {
            clear();
            return SNMP_ERROR_TIPO;
        }
    }
    //
    // encode's an uint32 to uint32, counter, time-ticks, gauge, opaque  syntax

    SNMP_CODIGOS_ERROR encode(SNMP_SINTAXIS syn, const uint32_t valor) {
        memset(data, 0, SNMP_LONGITUD_MAX_INSTANCIA);
        if (syn == SNMP_SINTAXIS_COUNTER || syn == SNMP_SINTAXIS_TIME_TICKS
                || syn == SNMP_SINTAXIS_GAUGE || syn == SNMP_SINTAXIS_UINT32
                || syn == SNMP_SINTAXIS_OPAQUE) {
            uint32_u tmp;
            size = 4;
            syntax = syn;
            tmp.uint32 = valor;
            data[0] = tmp.data[3];
            data[1] = tmp.data[2];
            data[2] = tmp.data[1];
            data[3] = tmp.data[0];
            return SNMP_SIN_ERROR;
        } else {
            clear();
            return SNMP_ERROR_TIPO;
        }
    }
    //
    // encode's an ip-address byte array to ip-address, NSAP-address, opaque  syntax

    SNMP_CODIGOS_ERROR encode(SNMP_SINTAXIS syn, const byte *valor) {
        memset(data, 0, SNMP_LONGITUD_MAX_INSTANCIA);
        if (syn == SNMP_SINTAXIS_IP_ADDRESS || syn == SNMP_SINTAXIS_NSAPADDR
                || syn == SNMP_SINTAXIS_OPAQUE) {
            if (sizeof (valor) > 4) {
                clear();
                return SNMP_ERROR_Tam;
            } else {
                size = 4;
                syntax = syn;
                //Serial.println(valor[3]);
                //Serial.println(valor[2]);
                //Serial.println(valor[1]);
                //Serial.println(valor[0]);
                
                data[0] = valor[0];
                data[1] = valor[1];
                data[2] = valor[2];
                data[3] = valor[3];
                return SNMP_SIN_ERROR;
            }
        } else {
            clear();
            return SNMP_ERROR_TIPO;
        }
    }
    //
    // encode's a boolean to boolean, opaque  syntax

    SNMP_CODIGOS_ERROR encode(SNMP_SINTAXIS syn, const bool valor) {
        memset(data, 0, SNMP_LONGITUD_MAX_INSTANCIA);
        if (syn == SNMP_SINTAXIS_BOOL || syn == SNMP_SINTAXIS_OPAQUE) {
            size = 1;
            syntax = syn;
            data[0] = valor ? 0xff : 0;
            return SNMP_SIN_ERROR;
        } else {
            clear();
            return SNMP_ERROR_TIPO;
        }
    }
    //
    // encode's an uint64 to counter64, opaque  syntax

    SNMP_CODIGOS_ERROR encode(SNMP_SINTAXIS syn, const uint64_t valor) {
        memset(data, 0, SNMP_LONGITUD_MAX_INSTANCIA);
        if (syn == SNMP_SINTAXIS_COUNTER64 || syn == SNMP_SINTAXIS_OPAQUE) {
            uint64_u tmp;
            size = 8;
            syntax = syn;
            tmp.uint64 = valor;
            data[0] = tmp.data[7];
            data[1] = tmp.data[6];
            data[2] = tmp.data[5];
            data[3] = tmp.data[4];
            data[4] = tmp.data[3];
            data[5] = tmp.data[2];
            data[6] = tmp.data[1];
            data[7] = tmp.data[0];
            return SNMP_SIN_ERROR;
        } else {
            clear();
            return SNMP_ERROR_TIPO;
        }
    }
    //
    // encode's a null to null, opaque  syntax

    SNMP_CODIGOS_ERROR encode(SNMP_SINTAXIS syn) {
        clear();
        if (syn == SNMP_SINTAXIS_NULL || syn == SNMP_SINTAXIS_OPAQUE) {
            size = 0;
            syntax = syn;
            return SNMP_SIN_ERROR;
        } else {
            return SNMP_ERROR_TIPO;
        }
    }
};

typedef struct SNMP_PDU {
    SNMP_TIPOS_PDU tipo;
    int32_t version;
    int32_t IdPeticion;
    SNMP_CODIGOS_ERROR error;
    int32_t indiceError;
    SNMP_OID OID;
    SNMP_VALUE valor;
};

class AgenteSNMP {
public:
    // Agent functions
    SNMP_API_RESULTADO inicio();
    SNMP_API_RESULTADO inicio(char *getComunidad, char *setCommName, uint16_t port);
    void escucha(void);
    SNMP_API_RESULTADO PduRecibida(SNMP_PDU *pdu);
    SNMP_API_RESULTADO PduRespuesta(SNMP_PDU *pdu);
    void onPduRecibida(callbackPduRecibida pduReceived);
    void Trap(char Mensaje[], byte RemIP[4], uint32_t Time,
     char enterprise_oid[] = "1.3.6.1.4.1.36061.0", char oid_[] = "1.3.6.1.4.1.36061.3.1.1.1"); // This is the line you must add
    void freePdu(SNMP_PDU *pdu);

    // Helper functions

private:
    byte _paquete[SNMP_TAM_MAX_PAQUETE];
    uint16_t _paqueteTam;
    uint16_t _paquetePos;
    SNMP_TIPOS_PDU _dstType;
    uint8_t _dstIp[4];
    uint16_t _dstPort;
    char *_getComunidad;
    size_t getTam;
    char *_setComunidad;
    size_t setTam;
    callbackPduRecibida _callback;
};

extern AgenteSNMP agente;

// function for pduReceived() function
extern SNMP_API_RESULTADO api_status;
extern SNMP_CODIGOS_ERROR status;
extern char oid[SNMP_LONGITUD_MAX_OID];
extern uint32_t prevMillis;

void PduRecibida(); // function the perform at SNMP Requirements 

#endif
