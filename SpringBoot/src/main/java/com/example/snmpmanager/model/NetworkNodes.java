package com.example.snmpmanager.model;

import jakarta.persistence.*;
import java.time.LocalDateTime;

@Entity
@Table(name = "nodosdered")
public class NetworkNodes {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;
    
    @Column(name = "nombre_nodo", length = 50)
    private String nombreNodo;
    
    @Column(name = "ubicacion_nodo", length = 50)
    private String ubicacionNodo;
    
    @Column(name = "tiempo_up_nodo", length = 50)
    private String tiempoUpNodo;
    
    @Column(name = "descripcion_interfaz", length = 50)
    private String descripcionInterfaz;
    
    @Column(name = "estado_interfaz")
    private Integer estadoInterfaz;
    
    @Column(name = "paquetes_entrantes")
    private Integer paquetesEntrantes;
    
    @Column(name = "fecha_lectura")
    private LocalDateTime fechaLectura;

    // Getters y Setters
    public Integer getId() { return id; }
    public void setId(Integer id) { this.id = id; }
    public String getNombreNodo() { return nombreNodo; }
    public void setNombreNodo(String nombreNodo) { this.nombreNodo = nombreNodo; }
    public String getUbicacionNodo() { return ubicacionNodo; }
    public void setUbicacionNodo(String ubicacionNodo) { this.ubicacionNodo = ubicacionNodo; }
    public String getTiempoUpNodo() { return tiempoUpNodo; }
    public void setTiempoUpNodo(String tiempoUpNodo) { this.tiempoUpNodo = tiempoUpNodo; }
    public String getDescripcionInterfaz() { return descripcionInterfaz; }
    public void setDescripcionInterfaz(String descripcionInterfaz) { this.descripcionInterfaz = descripcionInterfaz; }
    public Integer getEstadoInterfaz() { return estadoInterfaz; }
    public void setEstadoInterfaz(Integer estadoInterfaz) { this.estadoInterfaz = estadoInterfaz; }
    public Integer getPaquetesEntrantes() { return paquetesEntrantes; }
    public void setPaquetesEntrantes(Integer paquetesEntrantes) { this.paquetesEntrantes = paquetesEntrantes; }
    public LocalDateTime getFechaLectura() { return fechaLectura; }
    public void setFechaLectura(LocalDateTime fechaLectura) { this.fechaLectura = fechaLectura; }
}
