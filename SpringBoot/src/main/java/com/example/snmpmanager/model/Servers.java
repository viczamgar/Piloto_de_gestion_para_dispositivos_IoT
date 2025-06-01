package com.example.snmpmanager.model;

import jakarta.persistence.*;
import java.time.LocalDateTime;

@Entity
@Table(name = "servidores")
public class Servers {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;
    
    @Column(name = "nombre_servidor", length = 50)
    private String nombreServidor;
    
    @Column(name = "tiempo_actividad_servidor", length = 50)
    private String tiempoActividadServidor;
    
    @Column(name = "conexiones_tcp")
    private Integer conexionesTcp;
    
    @Column(name = "carga_cpu")
    private Integer cargaCpu;
    
    @Column(name = "nombre_disco", length = 50)
    private String nombreDisco;
    
    @Column(name = "uso_disco")
    private Integer usoDisco;
    
    @Column(name = "fecha_lectura")
    private LocalDateTime fechaLectura;

    // Getters y Setters
    public Integer getId() { return id; }
    public void setId(Integer id) { this.id = id; }
    public String getNombreServidor() { return nombreServidor; }
    public void setNombreServidor(String nombreServidor) { this.nombreServidor = nombreServidor; }
    public String getTiempoActividadServidor() { return tiempoActividadServidor; }
    public void setTiempoActividadServidor(String tiempoActividadServidor) { this.tiempoActividadServidor = tiempoActividadServidor; }
    public Integer getConexionesTcp() { return conexionesTcp; }
    public void setConexionesTcp(Integer conexionesTcp) { this.conexionesTcp = conexionesTcp; }
    public Integer getCargaCpu() { return cargaCpu; }
    public void setCargaCpu(Integer cargaCpu) { this.cargaCpu = cargaCpu; }
    public String getNombreDisco() { return nombreDisco; }
    public void setNombreDisco(String nombreDisco) { this.nombreDisco = nombreDisco; }
    public Integer getUsoDisco() { return usoDisco; }
    public void setUsoDisco(Integer usoDisco) { this.usoDisco = usoDisco; }
    public LocalDateTime getFechaLectura() { return fechaLectura; }
    public void setFechaLectura(LocalDateTime fechaLectura) { this.fechaLectura = fechaLectura; }
}
