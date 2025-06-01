package com.example.snmpmanager.model;

import jakarta.persistence.*;
import java.time.LocalDateTime;

@Entity
@Table(name = "infobgp")
public class BGPInfo {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;
    
    @Column(name = "nombre_nodo", length = 50)
    private String nombreNodo;
    
    @Column(name = "asn_vecino")
    private Integer asnVecino;
    
    @Column(name = "tiempo_up_conexion", length = 50)
    private String tiempoUpConexion;
    
    @Column(name = "mensajes_update_in")
    private Integer mensajesUpdateIn;
    
    @Column(name = "mensajes_totales_in")
    private Integer mensajesTotalesIn;
    
    @Column(name = "fecha_lectura")
    private LocalDateTime fechaLectura;

    // Getters y Setters
    public Integer getId() { return id; }
    public void setId(Integer id) { this.id = id; }
    public String getNombreNodo() { return nombreNodo; }
    public void setNombreNodo(String nombreNodo) { this.nombreNodo = nombreNodo; }
    public Integer getAsnVecino() { return asnVecino; }
    public void setAsnVecino(Integer asnVecino) { this.asnVecino = asnVecino; }
    public String getTiempoUpConexion() { return tiempoUpConexion; }
    public void setTiempoUpConexion(String tiempoUpConexion) { this.tiempoUpConexion = tiempoUpConexion; }
    public Integer getMensajesUpdateIn() { return mensajesUpdateIn; }
    public void setMensajesUpdateIn(Integer mensajesUpdateIn) { this.mensajesUpdateIn = mensajesUpdateIn; }
    public Integer getMensajesTotalesIn() { return mensajesTotalesIn; }
    public void setMensajesTotalesIn(Integer mensajesTotalesIn) { this.mensajesTotalesIn = mensajesTotalesIn; }
    public LocalDateTime getFechaLectura() { return fechaLectura; }
    public void setFechaLectura(LocalDateTime fechaLectura) { this.fechaLectura = fechaLectura; }
}
