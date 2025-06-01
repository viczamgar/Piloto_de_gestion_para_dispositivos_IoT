

#include "AgenteSNMP.h"
#include "EthernetUdp.h"
#include <WiFiEspUdp.h>
//#include <WiFiUdp.h>

  



extern WiFiEspUDP udp;

//extern EthernetUDP udp;


SNMP_API_RESULTADO AgenteSNMP::inicio() {
    _getComunidad = "public";
    _setComunidad = "public";
    
    setTam = strlen(_setComunidad);
    getTam = strlen(_getComunidad);
    udp.begin(PUERTO_DEFAULT_SNMP);
    return ESTADO_API_SNMP_EXITO;
}

SNMP_API_RESULTADO AgenteSNMP::inicio(char *getComunidad, char *setComunidad, uint16_t puerto) {
    setTam = strlen(setComunidad);
    getTam = strlen(getComunidad);
    if (setTam > LONGITUD_MAX_COMUNIDAD + 1 || getTam > LONGITUD_MAX_COMUNIDAD + 1) {
        return ESTADO_API_SNMP_LONGITUD_COMUNIDAD;
    }
    //
    _getComunidad = getComunidad;
    _setComunidad = setComunidad;
    //
    if (puerto == NULL || puerto == 0) puerto = PUERTO_DEFAULT_SNMP;
    //
    // init udp socket
    udp.begin(puerto);

    return ESTADO_API_SNMP_EXITO;
}

void AgenteSNMP::freePdu(SNMP_PDU *pdu) {
    //
    memset(pdu->OID.data, 0, SNMP_LONGITUD_MAX_OID);
    memset(pdu->valor.data, 0, SNMP_LONGITUD_MAX_INSTANCIA);
    free((char *) pdu);
    
}

void AgenteSNMP::escucha(void) {
    
    udp.parsePacket();
    if (udp.available() && _callback != NULL) (*_callback)();
}

SNMP_API_RESULTADO AgenteSNMP::PduRecibida(SNMP_PDU *pdu) {
    char *comunidad;
    byte seqTam;
    byte verTam, verFin;
    byte comTam, comFin;
    // pdu
    byte pduTyp, pduTam;
    byte ridTam, ridFin;
    byte errTam, errFin;
    byte eriTam, eriFin;
    byte vblTyp, vblTam;
    byte vbiTyp, vbiTam;
    byte obiTam, obiFin;
    byte valTyp, valTam, valFin;
    byte i;
    
    // Almacena el tamaño del paquete recibido
    _paqueteTam = udp.available();
    
    // Comprobación del tamaño del paquete
    memset(_paquete, 0, SNMP_TAM_MAX_PAQUETE);
    if (_paqueteTam != 0 && _paqueteTam > SNMP_TAM_MAX_PAQUETE) {
      
        return ESTADO_API_SNMP_TAM_PAQUETE;
    }

   
    udp.read(_paquete, _paqueteTam);
   
    if (_paquete[0] != 0x30) {

        return ESTADO_API_PAQUETE_INVALIDO;
    }
    
    // Longitud
    seqTam = _paquete[1];
    // Version
    verTam = _paquete[3];
    verFin = 3 + verTam;
    // Comunidad
    comTam = _paquete[verFin + 2];
    comFin = verFin + 2 + comTam;
    // Resto del paquete
    pduTyp = _paquete[comFin + 1];
    pduTam = _paquete[comFin + 2];
    ridTam = _paquete[comFin + 4];
    ridFin = comFin + 4 + ridTam;
    errTam = _paquete[ridFin + 2];
    errFin = ridFin + 2 + errTam;
    eriTam = _paquete[errFin + 2];
    eriFin = errFin + 2 + eriTam;
    vblTyp = _paquete[eriFin + 1];
    vblTam = _paquete[eriFin + 2];
    vbiTyp = _paquete[eriFin + 3];
    vbiTam = _paquete[eriFin + 4];
    obiTam = _paquete[eriFin + 6];
    obiFin = eriFin + obiTam + 6;
    valTyp = _paquete[obiFin + 1];
    valTam = _paquete[obiFin + 2];
    valFin = obiFin + 2 + valTam;
    
    // Version de SNMP
    pdu->version = 0;
    for (i = 0; i < verTam; i++) {
        pdu->version = (pdu->version << 8) | _paquete[5 + i];
    }
    
    pdu->tipo = (SNMP_TIPOS_PDU) pduTyp;
    _dstType = pdu->tipo;
   
    // Valida la comunidad (tamaño)
    if (comTam > LONGITUD_MAX_COMUNIDAD) {
        pdu->error = SNMP_ERROR_Tam;
        return ESTADO_API_SNMP_LONGITUD_COMUNIDAD;
    }
    //
    //
    // Validad la comunidad (cadena)
    if (pdu->tipo == SNMP_PDU_SET && comTam == setTam) {
        //
        for (i = 0; i < setTam; i++) {
            if (_paquete[verFin + 3 + i] != (byte) _setComunidad[i]) {
         
                pdu->error = SNMP_ERR_NO_SUCH_NAME;
                
                return ESTADO_API_NO_SUCH_NAME;
            }
        }
    } else if (pdu->tipo == SNMP_PDU_GET && comTam == getTam) {
        //
        for (i = 0; i < getTam; i++) {
            if (_paquete[verFin + 3 + i] != (byte) _getComunidad[i]) {
           
                pdu->error = SNMP_ERR_NO_SUCH_NAME;
         
                return ESTADO_API_NO_SUCH_NAME;
            }
        }
    }else if ( pdu->tipo == SNMP_PDU_GET_NEXT && comTam == getTam ) {
   //
    for ( i = 0; i < getTam; i++ ) {
      if( _paquete[verFin + 3 + i] != (byte)_getComunidad[i] ) {
       
        pdu->error = SNMP_ERR_NO_SUCH_NAME;
        
        return ESTADO_API_NO_SUCH_NAME;
      }
    }
    }else {
       
        pdu->error = SNMP_ERR_NO_SUCH_NAME;
        //
        return ESTADO_API_NO_SUCH_NAME;
    }
   
    pdu->IdPeticion = 0;
    for (i = 0; i < ridTam; i++) {
        pdu->IdPeticion = (pdu->IdPeticion << 8) | _paquete[comFin + 5 + i];
    }
    
    pdu->error = SNMP_SIN_ERROR;
    int32_t err = 0;
    for (i = 0; i < errTam; i++) {
        err = (err << 8) | _paquete[ridFin + 3 + i];
    }
    pdu->error = (SNMP_CODIGOS_ERROR) err;
   
    pdu->indiceError = 0;
    for (i = 0; i < eriTam; i++) {
        pdu->indiceError = (pdu->indiceError << 8) | _paquete[errFin + 3 + i];
    }
  
    if (obiTam > SNMP_LONGITUD_MAX_OID) {
        // set pdu error
        pdu->error = SNMP_ERROR_Tam;

        return ESTADO_API_LONGITUD_OID;
    }
  
    memset(pdu->OID.data, 0, SNMP_LONGITUD_MAX_OID);
    pdu->OID.size = obiTam;
    for (i = 0; i < obiTam; i++) {
        pdu->OID.data[i] = _paquete[eriFin + 7 + i];
    }
  
    pdu->valor.syntax = (SNMP_SINTAXIS) valTyp;

    
    if (obiTam > SNMP_LONGITUD_MAX_INSTANCIA) {
        // set pdu error
        pdu->error = SNMP_ERROR_Tam;

        return ESTADO_API_LONGITUD_VALOR;
    }
    
    pdu->valor.size = valTam;
    
    // Se extrae el valor 
    memset(pdu->valor.data, 0, SNMP_LONGITUD_MAX_INSTANCIA);
    for (i = 0; i < valTam; i++) {
        pdu->valor.data[i] = _paquete[obiFin + 3 + i];
    }
    
   
    return ESTADO_API_SNMP_EXITO;
}

SNMP_API_RESULTADO AgenteSNMP::PduRespuesta(SNMP_PDU *pdu) {
    int32_u u;
    byte i;
    
    // Tamaño del paquete
    _paquetePos = 0; // 23
    _paqueteTam = 25 + sizeof (pdu->IdPeticion) + sizeof (pdu->error) + sizeof (pdu->indiceError) + pdu->OID.size + pdu->valor.size;
    
    memset(_paquete, 0, SNMP_TAM_MAX_PAQUETE);
    
    if (_dstType == SNMP_PDU_SET) {
        _paqueteTam += setTam;
    } else {
        _paqueteTam += getTam;
    }
    //
    _paquete[_paquetePos++] = (byte) SNMP_SINTAXIS_SEQUENCE; 
    _paquete[_paquetePos++] = (byte) _paqueteTam - 2; 
    _paquete[_paquetePos++] = (byte) SNMP_SINTAXIS_INT;
    _paquete[_paquetePos++] = 0x01; 
    _paquete[_paquetePos++] = 0x00; 
    //
    // Comunidad SNMP
    _paquete[_paquetePos++] = (byte) SNMP_SINTAXIS_OCTETS;
    if (_dstType == SNMP_PDU_SET) {
        _paquete[_paquetePos++] = (byte) setTam; 
        for (i = 0; i < setTam; i++) {
            _paquete[_paquetePos++] = (byte) _setComunidad[i];
        }
    } else {
        _paquete[_paquetePos++] = (byte) getTam;
        for (i = 0; i < getTam; i++) {
            _paquete[_paquetePos++] = (byte) _getComunidad[i];
        }
    }
    //
    // SNMP PDU
    _paquete[_paquetePos++] = (byte) pdu->tipo;
    _paquete[_paquetePos++] = (byte) (sizeof (pdu->IdPeticion) + sizeof ((int32_t) pdu->error) + sizeof (pdu->indiceError) + pdu->OID.size + pdu->valor.size + 14);
   
    _paquete[_paquetePos++] = (byte) SNMP_SINTAXIS_INT; 
    _paquete[_paquetePos++] = (byte)sizeof (pdu->IdPeticion);
    u.int32 = pdu->IdPeticion;
    _paquete[_paquetePos++] = u.data[3];
    _paquete[_paquetePos++] = u.data[2];
    _paquete[_paquetePos++] = u.data[1];
    _paquete[_paquetePos++] = u.data[0];
 
    _paquete[_paquetePos++] = (byte) SNMP_SINTAXIS_INT;
    _paquete[_paquetePos++] = (byte)sizeof ((int32_t) pdu->error);
    u.int32 = pdu->error;
    _paquete[_paquetePos++] = u.data[3];
    _paquete[_paquetePos++] = u.data[2];
    _paquete[_paquetePos++] = u.data[1];
    _paquete[_paquetePos++] = u.data[0];

    _paquete[_paquetePos++] = (byte) SNMP_SINTAXIS_INT; 
    _paquete[_paquetePos++] = (byte)sizeof (pdu->indiceError);
    u.int32 = pdu->indiceError;
    _paquete[_paquetePos++] = u.data[3];
    _paquete[_paquetePos++] = u.data[2];
    _paquete[_paquetePos++] = u.data[1];
    _paquete[_paquetePos++] = u.data[0];
    
    // Varbind (Listas)
    _paquete[_paquetePos++] = (byte) SNMP_SINTAXIS_SEQUENCE;
    _paquete[_paquetePos++] = (byte) (pdu->OID.size + pdu->valor.size + 6); //4
    
    // Varbind
    _paquete[_paquetePos++] = (byte) SNMP_SINTAXIS_SEQUENCE; // tipo
    _paquete[_paquetePos++] = (byte) (pdu->OID.size + pdu->valor.size + 4); //2
    
    // OID
    _paquete[_paquetePos++] = (byte) SNMP_SINTAXIS_OID; // tipo
    _paquete[_paquetePos++] = (byte) (pdu->OID.size);
    for (i = 0; i < pdu->OID.size; i++) {
        _paquete[_paquetePos++] = pdu->OID.data[i];
    }
   
    // Valor a enviar
    _paquete[_paquetePos++] = (byte) pdu->valor.syntax; // tipo
    _paquete[_paquetePos++] = (byte) (pdu->valor.size);
    for (i = 0; i < pdu->valor.size; i++) {
        _paquete[_paquetePos++] = pdu->valor.data[i];
    }
    
    // Envio
    udp.beginPacket(udp.remoteIP(), udp.remotePort());
    udp.write(_paquete, _paqueteTam);
    udp.endPacket();
    
    return ESTADO_API_SNMP_EXITO;
}

void AgenteSNMP::onPduRecibida(callbackPduRecibida PduRecibida) {
    _callback = PduRecibida;
}




// usage BER encoding
/*

TRAP PDU
  +---------+-----------+-------------------------------------------------------------------------------------------------------+-------------------+
  | SNMP    | tipo      |                                           0x30                                                        |Sequence           |
  | message +-----------+-------------------------------------------------------------------------------------------------------+-------------------+
  |         | Tamgth    |                                           0x82 0x00 0x44                                              |Tamgth: 68         |
  |         +-----------+-------------------------------------------------------------------------------------------------------+-------------------+
  |         | Version   |                                           0x02                                                        |Integer            |
  |         |           |                                           0x01                                                        |Tamgth: 1          |
  |         |           |                                           0x00                                                        |valor: 0           |
  |         +-----------+-------------------------------------------------------------------------------------------------------+-------------------+
  |         | Community |                                           0x04                                                        |Octet String       |
  |         |           |                                           0x06                                                        |Tamgth: 6          |
  |         |           |                                           0x70 0x75 0x62 0x6C 0x69 0x63                               |valor: public      |
  |         +-----------+ -------+------------+---------------------------------------------------------------------------------+-------------------+
  |         | Data      | SNMPv1 | PDU tipo   |                     0xA4                                                        |TRAP PDU           |
  |         |           | PDU    +------------+---------------------------------------------------------------------------------+-------------------+
  |         |           |        | PDU length |                     0x82 0x00 0x35                                              |Tamgth: 53         |
  |         |           |        + -----------+---------------------------------------------------------------------------------+-------------------+
  |         |           |        | Enterprise |                     0x06                                                        |Object identifier  |
  |         |           |        | OID        |                     0x09                                                        |Tamgth: 9          |
  |         |           |        |            |                     0x2B 0x06 0x01 0x04 0x01 0x82 0x99 0x5D 0x00                |valor:             |
  |         |           |        + -----------+---------------------------------------------------------------------------------+-------------------+
  |         |           |        | Agent      |                     0x40                                                        |IpAddress          |
  |         |           |        | Address    |                     0x04                                                        |Tamgth: 4          |
  |         |           |        |            |                     0xC0 0xA8 0x01 0x80                                         |valor:192.168.1.128|
  |         |           |        + -----------+---------------------------------------------------------------------------------+-------------------+
  |         |           |        | Generic    |                     0x02                                                        |Integer            |
  |         |           |        | Trap tipo  |                     0x01                                                        |Tamgth: 1          |
  |         |           |        |            |                     0x06                                                        |valor: 6           |
  |         |           |        + -----------+---------------------------------------------------------------------------------+-------------------+
  |         |           |        | Specific   |                     0x02                                                        |Integer            |
  |         |           |        | Trap       |                     0x01                                                        |Tamgth: 1          |
  |         |           |        | Number     |                     0x01                                                        |valor: 1           |
  |         |           |        + -----------+---------------------------------------------------------------------------------+-------------------+
  |         |           |        | Time       |                     0x43                                                        |TimeTicks          |
  |         |           |        | Stamp      |                     0x03                                                        |Tamgth: 3          |
  |         |           |        |            |                     0x01 0xE2 0x40                                              |valor: 123456      |
  |         |           |        + -----------+---------------------------------------------------------------------------------+-------------------+
  |         |           |        | VarBind    |                     0x30                                                        |Sequence           |
  |         |           |        | List       +-----------+---------------------------------------------------------------------+-------------------+
  |         |           |        |            | Tamgth    |         0x82 0x00 0x15                                              |Tamgth: 21         |
  |         |           |        |            +-----------+---------------------------------------------------------------------+ ------------------+
  |         |           |        |            | VarBind 1 |         0x30                                                        |Sequence           |
  |         |           |        |            |           +-------+-------------------------------------------------------------+-------------------+
  |         |           |        |            |           | Tam 1 | 0x82 0x00 0x11                                              |Tamgth: 17         |
  |         |           |        |            |           +-------+-------------------------------------------------------------+-------------------+
  |         |           |        |            |           | OID 1 | 0x06                                                        |Object identifier  |
  |         |           |        |            |           |       | 0x0C                                                        |Tamgth: 12         |
  |         |           |        |            |           |       | 0x2B 0x06 0x01 0x04 0x01 0x82 0x99 0x5D 0x03 0x01 0x01 0x01 |valor:             |
  |         |           |        |            |           +-------+-------------------------------------------------------------+-------------------+
  |         |           |        |            |           | valor | 0x02                                                        |Integer            |
  |         |           |        |            |           | 1     | 0x01                                                        |Tamgth: 1          |
  |         |           |        |            |           |       | 0x01                                                        |Tamgth: 1          |
  +---------+-----------+--------+------------+-----------+-------+-------------------------------------------------------------+-------------------+


  Data tipo identifier in SNMP
     Data tipo                 Identifier      Note
     Integer                      0x02         Primitive ASN.1 types
     Octet String                 0x04         Primitive ASN.1 types  
     Null                         0x05         Primitive ASN.1 types
     Object identifier            0x06         Primitive ASN.1 types
     Sequence                     0x30         Constructed ASN.1 types
     IpAddress                    0x40         Primitive SNMP application types
     Counter                      0x41         Primitive SNMP application types
     Gauge                        0x42         Primitive SNMP application types
     TimeTicks                    0x43         Primitive SNMP application types 
     Opaque                       0x44         Primitive SNMP application types
     NsapAddress                  0x45         Primitive SNMP application types
     GetRequest PDU               0xA0         Context-specific Constructed SNMP types
     GetNextRequest PDU           0xA1         Context-specific Constructed SNMP types
     GetResponse PDU              0xA2         Context-specific Constructed SNMP types
     SetRequest PDU               0xA3         Context-specific Constructed SNMP types
     Trap PDU                     0xA4         Context-specific Constructed SNMP types

 */


void AgenteSNMP::Trap(char Mensaje[], byte RemIP[4], uint32_t Time, char enterprise_oid[], char oid_[]) {

    byte miIP[4]       = {192, 168, 1, 72};   
    SNMP_OID oid;
    int32_u u;
    byte i;
    _paquetePos = 0;
    _paqueteTam = 0;

    // Limpiar el buffer
    memset(_paquete, 0, SNMP_TAM_MAX_PAQUETE);

    // trap = sequence, leng = pdu leng
    byte Tipo_y_longitud[2] = {48, 0};

    // tipo = integer, leng = 1, valor = 0 - snmp v1
    byte Version[3] = {2, 1, 0}; // Defined versión 1

    // tipo = octet string, leng = , valor = comunity string
    byte Comunity_string[2 + strlen(_getComunidad)];
    Comunity_string[0] = 4;
    Comunity_string[1] = strlen(_getComunidad);

    for (uint8_t i = 2; i <= strlen(_getComunidad) + 1; i++) {
        Comunity_string[i] = (int) _getComunidad[i - 2];
    }

    // Datos de la PDU
    byte PDU_tipo_y_longitud[4] = {164, 130, 0, 52};

    // OID enterprise
    size_t Tam_oid = 8;
    byte *oid_enterprise_dec = oid.StringAOID(enterprise_oid);

    byte OID[2 + Tam_oid];
    OID[0] = 6;
    OID[1] = Tam_oid;

    for (int i = 2; i < Tam_oid + 2; i++) {
        OID[i] = oid_enterprise_dec[i - 2];
    }

    // Dirección IP del agente
    byte IP_agente[2] = {64, 4};

    // Tipo de trap
    byte Tipo_TRAP[3] = {2, 1, 6};

    // OID específico del trap
    byte OID_extra[3] = {2, 1, 1};

    // Tiempo de arranque del sistema
    byte Tipo_time_stick[2] = {67, 4};
    byte Numero_hexadecimal[4] = {0, 0, 0, 0};
    u.int32 = Time;

    for (int j = 0; j < 4; j++) {
        Numero_hexadecimal[j] = u.data[j];
    }

    // VarBind
    byte Var_Bind[4] = {48, 130, 0, 20 + strlen(Mensaje)};
    byte Var_Bind1[4] = {48, 130, 0, 16 + strlen(Mensaje)};

    

    // OID del VarBind
    byte *oid_dec = oid.StringAOID(oid_);

    byte OID1[2 + Tam_oid];
    OID1[0] = 6;
    OID1[1] = Tam_oid;

    for (int i = 2; i < Tam_oid + 2; i++) {
        OID1[i] = oid_dec[i - 2];
    }

    Var_Bind[3] = 6 + strlen(Mensaje) + (sizeof (OID1) / sizeof ( OID1[0]));
    Var_Bind1[3] = 2 + strlen(Mensaje) + (sizeof (OID1) / sizeof ( OID1[0]));

    // Valor del VarBind
    byte Value1[2] = {4, strlen(Mensaje)};


  Tipo_y_longitud[1] = sizeof (Version) / sizeof (Version[0]) + sizeof (Comunity_string) / sizeof (Comunity_string[0])
            + sizeof (PDU_tipo_y_longitud) / sizeof (PDU_tipo_y_longitud[0]) + sizeof (OID) / sizeof (OID[0])
            + sizeof (IP_agente) / sizeof (IP_agente[0]) + sizeof (mi_IP) / sizeof (mi_IP[0])
            + sizeof (Tipo_TRAP) / sizeof ( Tipo_TRAP[0]) + sizeof (OID_extra) / sizeof ( OID_extra[0])
            + sizeof (Tipo_time_stick) / sizeof ( Tipo_time_stick[0]) + sizeof (Numero_hexadecimal) / sizeof ( Numero_hexadecimal[0])
            + sizeof (Var_Bind) / sizeof ( Var_Bind[0]) + sizeof (Var_Bind1) / sizeof ( Var_Bind1[0])
            + sizeof (OID1) / sizeof ( OID1[0]) + sizeof (Value1) / sizeof (Value1[0]) + strlen(Mensaje);

  PDU_tipo_y_longitud[3] = sizeof (OID) / sizeof (OID[0])
            + sizeof (IP_agente) / sizeof (IP_agente[0]) + sizeof (mi_IP) / sizeof (mi_IP[0])
            + sizeof (Tipo_TRAP) / sizeof ( Tipo_TRAP[0]) + sizeof (OID_extra) / sizeof ( OID_extra[0])
            + sizeof (Tipo_time_stick) / sizeof ( Tipo_time_stick[0]) + sizeof (Numero_hexadecimal) / sizeof ( Numero_hexadecimal[0])
            + sizeof (Var_Bind) / sizeof ( Var_Bind[0]) + sizeof (Var_Bind1) / sizeof ( Var_Bind1[0])
            + sizeof (OID1) / sizeof ( OID1[0]) + sizeof (Value1) / sizeof (Value1[0]) + strlen(Mensaje);


           
    // Construcción completa del paquete
    _paquete[_paquetePos++] = Tipo_y_longitud[0];
    _paquete[_paquetePos++] = Tipo_y_longitud[1];
    _paqueteTam += 2;

    memcpy(_paquete + _paquetePos, Version, sizeof(Version));
    _paquetePos += sizeof(Version);
    _paqueteTam += sizeof(Version);

    memcpy(_paquete + _paquetePos, Comunity_string, sizeof(Comunity_string));
    _paquetePos += sizeof(Comunity_string);
    _paqueteTam += sizeof(Comunity_string);

    memcpy(_paquete + _paquetePos, PDU_tipo_y_longitud, sizeof(PDU_tipo_y_longitud));
    _paquetePos += sizeof(PDU_tipo_y_longitud);
    _paqueteTam += sizeof(PDU_tipo_y_longitud);

    memcpy(_paquete + _paquetePos, OID, sizeof(OID));
    _paquetePos += sizeof(OID);
    _paqueteTam += sizeof(OID);

    memcpy(_paquete + _paquetePos, IP_agente, sizeof(IP_agente));
    _paquetePos += sizeof(IP_agente);
    _paqueteTam += sizeof(IP_agente);

    for (int j = 0; j < 4; j++) {
        _paquete[_paquetePos++] = miIP[j];
        _paqueteTam++;
    }

    memcpy(_paquete + _paquetePos, Tipo_TRAP, sizeof(Tipo_TRAP));
    _paquetePos += sizeof(Tipo_TRAP);
    _paqueteTam += sizeof(Tipo_TRAP);

    memcpy(_paquete + _paquetePos, OID_extra, sizeof(OID_extra));
    _paquetePos += sizeof(OID_extra);
    _paqueteTam += sizeof(OID_extra);

    memcpy(_paquete + _paquetePos, Tipo_time_stick, sizeof(Tipo_time_stick));
    _paquetePos += sizeof(Tipo_time_stick);
    _paqueteTam += sizeof(Tipo_time_stick);

    memcpy(_paquete + _paquetePos, Numero_hexadecimal, sizeof(Numero_hexadecimal));
    _paquetePos += sizeof(Numero_hexadecimal);
    _paqueteTam += sizeof(Numero_hexadecimal);

    memcpy(_paquete + _paquetePos, Var_Bind, sizeof(Var_Bind));
    _paquetePos += sizeof(Var_Bind);
    _paqueteTam += sizeof(Var_Bind);

    memcpy(_paquete + _paquetePos, Var_Bind1, sizeof(Var_Bind1));
    _paquetePos += sizeof(Var_Bind1);
    _paqueteTam += sizeof(Var_Bind1);

    memcpy(_paquete + _paquetePos, OID1, sizeof(OID1));
    _paquetePos += sizeof(OID1);
    _paqueteTam += sizeof(OID1);

    memcpy(_paquete + _paquetePos, Value1, sizeof(Value1));
    _paquetePos += sizeof(Value1);
    _paqueteTam += sizeof(Value1);

    memcpy(_paquete + _paquetePos, Mensaje, strlen(Mensaje));
    _paquetePos += strlen(Mensaje);
    _paqueteTam += strlen(Mensaje);

    IPAddress address;

   byte RemIP2[4] = {192, 168, 1, 87};
  
    for (uint8_t i=0;i<=4;i++) {
        address[i] = RemIP2[i];
        if(i!=4){
        Serial.print(address[i]);
        }
        Serial.print(".");
    }
    Serial.println("");

    //Envío de la PDU
    udp.beginPacket(RemIP2, 16200);
    udp.write(_paquete, _paqueteTam);
    udp.endPacket();

    return ESTADO_API_SNMP_EXITO;
}

// Create one global object
AgenteSNMP agente;

// function for pduReceived() function
SNMP_API_RESULTADO api_status;
SNMP_CODIGOS_ERROR status;
char oid[SNMP_LONGITUD_MAX_OID];
uint32_t prevMillis = 0;

/**
 *
 * @info function the perform at SNMP Requirements
 * @param none
 * @return void
 */

 // DEBUG 

void PduRecibida() {
    SNMP_PDU pdu;
    api_status = agente.PduRecibida(&pdu);
    
    if (pdu.tipo == SNMP_PDU_GET || pdu.tipo == SNMP_PDU_GET_NEXT || pdu.tipo == SNMP_PDU_SET
            && pdu.error == SNMP_SIN_ERROR && api_status == ESTADO_API_SNMP_EXITO) {
        pdu.OID.OIDaString(oid);
        
  char *oidL[] = {
    sysName, sysLocation, ipAddress, sysUpTime, sysContact, telef, mail, 
    ifIndex1, ifIndex2, ifIndex3, ifIndex4,   ifIndex5, ifIndex6, ifIndex7, ifIndex8,
    des1, des2, des3, des4,  des5, des6, des7, des8, 
    sensorTemp, sensorHum, led1, tempArd,humSuelo,ramD,freq,micro,
    inst1, inst2, inst3, inst4, inst5, inst6, inst7, inst8, 
    tMuestreo1, tMuestreo2, tMuestreo3, tMuestreo4, tMuestreo5, tMuestreo6, tMuestreo7, tMuestreo8, 
    Index1, Index2, Index3, Index4, Index5, 
    s1, s2, s3, s4, s5, 
    dd1, dd2, dd3, dd4, dd5, 
    us1, us2, us3, us4, us5, 
    is1, is2, is3, is4, is5, 
    IP1, IP2, IP3, IP4, IP5, 
    indPC1,indPC2,nPC1,nPC2,
    timePC1,timePC2,cPC1,cPC2,
    tPC1,tPC2,ndPC1,ndPC2,
    elPC1,elPC2,
    indSW1,indSW2,indSW3,indSW4,indSW5,
    nSW1,nSW2,nSW3,nSW4,nSW5,
    timeSW1,timeSW2,timeSW3,timeSW4,timeSW5,
    locSW1,locSW2,locSW3,locSW4,locSW5,
    pSW1,pSW2,pSW3,pSW4,pSW5,
    epSW1,epSW2,epSW3,epSW4,epSW5,
    bySW1,bySW2,bySW3,bySW4,bySW5,

    indBGP1, indBGP2, indBGP3, indBGP4, indBGP5,
    nodoBGP1, nodoBGP2, nodoBGP3, nodoBGP4, nodoBGP5,
    vecinoASN1, vecinoASN2, vecinoASN3, vecinoASN4, vecinoASN5,
    tiempoCon1, tiempoCon2, tiempoCon3, tiempoCon4, tiempoCon5,
    mensajesUpdate1, mensajesUpdate2, mensajesUpdate3, mensajesUpdate4, mensajesUpdate5,
    mensajesTotales1, mensajesTotales2, mensajesTotales3, mensajesTotales4, mensajesTotales5,
    
    
    
    inventado
};


void* mibV[] = {
    &locName,&locLocation,  &ip,&locUpTime, &locContact,   &phonenumber,&email,
    &index1, &index2, &index3, &index4,&index5, &index6, &index7, &index8,
    &descr1, &descr2, &descr3, &descr4,&descr5, &descr6, &descr7, &descr8,
    &sensortemp, &sensorhum, &valorled1, &accesosWeb,&sensorSuelo, &ram, &sensorSonido, &microfono,
    &instanteLectura1, &instanteLectura2, &instanteLectura3, &instanteLectura4,&instanteLectura5, 
    &instanteLectura6, &instanteLectura7, &instanteLectura8,
    &tiempoMuestreo1, &tiempoMuestreo2, &tiempoMuestreo3, &tiempoMuestreo4, &tiempoMuestreo5,
    &tiempoMuestreo6, &tiempoMuestreo7, &tiempoMuestreo8,
    &ind1, &ind2, &ind3, &ind4, &ind5,
    &Sind1, &Sind2, &Sind3, &Sind4, &Sind5,
    &d1, &d2, &d3, &d4, &d5,
    &uS1, &uS2, &uS3, &uS4, &uS5,
    &uI1, &uI2, &uI3, &uI4, &uI5,
    &ip1, &ip2, &ip3, &ip4, &ip5,
    &ind1, &ind2,&sysName1[0],&sysName1[1],
    &time1[0],&time1[1],&carga1[0],&carga1[1],
    &tcpcon1[0],&tcpcon1[1],&nombreDisco1[0],&nombreDisco1[1],
    &tamDisco1[0],&tamDisco1[1],
    &ind1, &ind2, &ind3, &ind4, &ind5,
    &sysName2[0], &sysName2[1], &sysName2[2], &sysName2[3], &sysName2[4],
    &time2[0], &time2[1], &time2[2], &time2[3], &time2[4],
    &sysLocation2[0], &sysLocation2[1], &sysLocation2[2], &sysLocation2[3], &sysLocation2[4],
    &ifDescr[0], &ifDescr[1], &ifDescr[2], &ifDescr[3], &ifDescr[4],
    &operStatus[0], &operStatus[1], &operStatus[2], &operStatus[3], &operStatus[4],
    &inPackets[0], &inPackets[1], &inPackets[2], &inPackets[3], &inPackets[4],
    &ind1, &ind2, &ind3, &ind4, &ind5,
    &sysName2[0], &sysName2[1], &sysName2[2], &sysName2[3], &sysName2[4],
    &vecinoASN[0], &vecinoASN[1], &vecinoASN[2], &vecinoASN[3], &vecinoASN[4],
    &tiempoConexion[0], &tiempoConexion[1], &tiempoConexion[2], &tiempoConexion[3], &tiempoConexion[4],
    &mensajesUpdate[0], &mensajesUpdate[1], &mensajesUpdate[2], &mensajesUpdate[3], &mensajesUpdate[4],
    &mensajesTotales[0], &mensajesTotales[1], &mensajesTotales[2], &mensajesTotales[3], &mensajesTotales[4],
    
};


//FUNCIONALIDAD GETNEXT INTEGRADA Y OPTIMIZADA (ELIMINADA LAS NUMEROSAS COMPARACIONES)

    

    int oidPosibles = sizeof(oidL) / sizeof(oidL[0]); //NUMERO DE VARIABLES A ENVIAR POR SNMP
    
     if (pdu.tipo == SNMP_PDU_GET_NEXT) {
            char tmpOIDfs[SNMP_LONGITUD_MAX_OID];

            
    for (int i = 0; i < oidPosibles; i++) {
        if (strcmp_P(oid, oidL[i]) == 0) {  //SE COMPARA CADA OID CON EL RECIBIDO EN EL REQUEST
            if (i < oidPosibles - 1) {
              strcpy_P ( oid, oidL[i + 1] ); 
            strcpy_P ( tmpOIDfs, oidL[i + 1] ); //SE ACTUAlIZA OID CON EL SIGUIENTE
            pdu.OID.StringAOID(tmpOIDfs);  
          
            } else {
                // Caso en el que estamos en el último OID
                strcpy_P(oid, "1.0");
            }
            break;
        }
    }
        int ilen = strlen(oid);

// USANDO LA LONGITUD (oidPosibles), ASEGURAMOS QUE LA VARIABLE A ENVIAR ES LA SIGUIENTE
        for (int i = 0; i < oidPosibles; i++) {  
        if (strncmp_P(oid, oidL[i],ilen) == 0)  {
            
              strcpy_P ( oid, oidL[i] ); 
            strcpy_P ( tmpOIDfs, oidL[i] );        
            pdu.OID.StringAOID(tmpOIDfs);  
          
            
            break;
        }
    }
    
}




         for (int i = 0; i < oidPosibles; i++) {
        if (strcmp_P(oid, oidL[i]) == 0) {  //SE COMPARA CADA OID CON EL RECIBIDO EN EL REQUEST
             int x = 4;
             if(i<3){
              
                  if (pdu.tipo == SNMP_PDU_SET) {
                         
                       status = pdu.valor.decode((char*)mibV[i], strlen((char*)mibV[i]));
                       pdu.tipo = SNMP_PDU_RESPONSE;
                       pdu.error = status;
                    } else {
                      
                       status = pdu.valor.encode(SNMP_SINTAXIS_OCTETS, (char*)mibV[i]);
                       pdu.tipo = SNMP_PDU_RESPONSE;
                       pdu.error = status;
                            }
                     }
                     
            if(i==3){
                  if (pdu.tipo == SNMP_PDU_SET) {
                       pdu.tipo = SNMP_PDU_RESPONSE;
                       pdu.error = SNMP_ERR_READ_ONLY;
                   } else {
                       status = pdu.valor.encode(SNMP_SINTAXIS_TIME_TICKS, (uint32_t)locUpTime);
                       pdu.tipo = SNMP_PDU_RESPONSE;
                       pdu.error = status;
                   }
               }


               if((i>3) && (i<7)){
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode((char*)mibV[i], strlen((char*)mibV[i]));
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            status = pdu.valor.encode(SNMP_SINTAXIS_OCTETS, (char*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

              if((i>6) && (i<11+x)){


                        
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode2(*(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            //Serial.println((int32_t*)mibV[i]);
                            
                            
                            status = pdu.valor.encode(SNMP_SINTAXIS_INT, *(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

              


             if((i>10+x) && (i<15+2*x)){
            
                         if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode((char*)mibV[i], strlen((char*)mibV[i]));
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            status = pdu.valor.encode(SNMP_SINTAXIS_OCTETS, (char*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }


             if((i>14+2*x) && (i<19+3*x)){
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode2(*(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            status = pdu.valor.encode(SNMP_SINTAXIS_INT, *(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

             if((i>18+3*x) && (i<23+4*x)){
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode2((uint32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            status = pdu.valor.encode(SNMP_SINTAXIS_TIME_TICKS, *(uint32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }


              if((i>22+4*x) && (i<37+5*x)){
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode2((int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            status = pdu.valor.encode(SNMP_SINTAXIS_INT, *(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

                if((i>36+5*x) && (i<42+5*x)){
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode((char*)mibV[i], strlen((char*)mibV[i]));
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            status = pdu.valor.encode(SNMP_SINTAXIS_OCTETS, (char*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

             if((i>41+5*x) && (i<52+5*x)){
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                             status = pdu.valor.decode2((int32_t*)mibV[i]);
                               
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            status = pdu.valor.encode(SNMP_SINTAXIS_INT, *(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

 





            

               if((i>51+5*x) && (i<57+5*x)){

                  if (pdu.tipo == SNMP_PDU_SET) {
                // response packet from set-request - object is read/write
            
                status = pdu.valor.decodeIP((byte*)mibV[i]);
                status = pdu.valor.encode(SNMP_SINTAXIS_IP_ADDRESS, (byte*)mibV[i]);
                pdu.tipo = SNMP_PDU_RESPONSE;
                pdu.error = status;
            } else {
                // response packet from get-request - locContact
                status = pdu.valor.encode(SNMP_SINTAXIS_IP_ADDRESS, (byte*)mibV[i]);
                pdu.tipo = SNMP_PDU_RESPONSE;
                pdu.error = status;
            }
             }



  if((i>56+5*x) && (i<59+5*x)){


                        
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode2((int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            //Serial.println((int32_t*)mibV[i]);
                            
                            
                            status = pdu.valor.encode(SNMP_SINTAXIS_INT, *(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }



            
                if((i>58+5*x) && (i<61+5*x)){
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode((char*)mibV[i], strlen((char*)mibV[i]));
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            status = pdu.valor.encode(SNMP_SINTAXIS_OCTETS, (char*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

                if((i>60+5*x) && (i<63+5*x)){
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode2((uint32_t)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            status = pdu.valor.encode(SNMP_SINTAXIS_TIME_TICKS, *(uint32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }



  if((i>62+5*x) && (i<67+5*x)){


                        
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode2(*(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            //Serial.println((int32_t*)mibV[i]);
                            
                            
                            status = pdu.valor.encode(SNMP_SINTAXIS_INT, *(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }


if((i>66+5*x) && (i<69+5*x)){
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode((char*)mibV[i], strlen((char*)mibV[i]));
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            status = pdu.valor.encode(SNMP_SINTAXIS_OCTETS, (char*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }


            if((i>68+5*x) && (i<71+5*x)){


                        
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode2(*(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            //Serial.println((int32_t*)mibV[i]);
                            
                            
                            status = pdu.valor.encode(SNMP_SINTAXIS_INT, *(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

             if((i>69+5*x) && (i<76+5*x)){


                        
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode2(*(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            //Serial.println((int32_t*)mibV[i]);
                            
                            
                            status = pdu.valor.encode(SNMP_SINTAXIS_INT, *(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

            if((i>75+5*x) && (i<81+5*x)){
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode((char*)mibV[i], strlen((char*)mibV[i]));
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            status = pdu.valor.encode(SNMP_SINTAXIS_OCTETS, (char*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

              if((i>80+5*x) && (i<86+5*x)){
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode2(*(uint32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            status = pdu.valor.encode(SNMP_SINTAXIS_TIME_TICKS, *(uint32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

            if((i>85+5*x) && (i<96+5*x)){
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode((char*)mibV[i], strlen((char*)mibV[i]));
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            status = pdu.valor.encode(SNMP_SINTAXIS_OCTETS, (char*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

              if((i>95+5*x) && (i<106+5*x)){


                        
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode2(*(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            //Serial.println((int32_t*)mibV[i]);
                            
                           
                            status = pdu.valor.encode(SNMP_SINTAXIS_INT, *(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

            //AQUI COMIENZA BGP

             if((i>105+5*x) && (i<111+5*x)){


                        
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode2(*(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                   
                            // response packet from get-request - locName
                            //Serial.println((int32_t*)mibV[i]);
                            
                            
                            status = pdu.valor.encode(SNMP_SINTAXIS_INT, *(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

            if((i>110+5*x) && (i<116+5*x)){
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode((char*)mibV[i], strlen((char*)mibV[i]));
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            status = pdu.valor.encode(SNMP_SINTAXIS_OCTETS, (char*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }


            if((i>115+5*x) && (i<121+5*x)){


                        
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode2(*(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            //Serial.println((int32_t*)mibV[i]);
                            
                            
                            status = pdu.valor.encode(SNMP_SINTAXIS_INT, *(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

              if((i>120+5*x) && (i<126+5*x)){
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode2(*(uint32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            status = pdu.valor.encode(SNMP_SINTAXIS_GAUGE, *(uint32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

           if((i>125+5*x) && (i<136+5*x)){


                        
            
                            if (pdu.tipo == SNMP_PDU_SET) {
                            // response packet from set-request - object is read/write
                            status = pdu.valor.decode2(*(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        } else {
                            // response packet from get-request - locName
                            //Serial.println((int32_t*)mibV[i]);
                            
                            
                            status = pdu.valor.encode(SNMP_SINTAXIS_INT, *(int32_t*)mibV[i]);
                            pdu.tipo = SNMP_PDU_RESPONSE;
                            pdu.error = status;
                        }

              
            }

             if(i==136+5*x){

                        if (pdu.tipo == SNMP_PDU_SET) {
                          // response packet from set-request - object is read/write
                          
                          pdu.tipo = SNMP_PDU_RESPONSE;
                          pdu.error = status;
                      } else {
                          // response packet from get-request - locLocation
                          status = pdu.valor.encode(SNMP_SINTAXIS_OCTETS, "FINALMIB");
                          pdu.tipo = SNMP_PDU_RESPONSE;
                          pdu.error = status;
                      }
             }



             

        }
    }

        agente.PduRespuesta(&pdu);
       
    }
    
}
