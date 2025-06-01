package com.example.snmpmanager.repository;

import com.example.snmpmanager.model.Servers;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface ServersRepository extends JpaRepository<Servers, Integer> {
    
    // Obtener el último registro de cada servidor
    @Query("SELECT s FROM Servers s WHERE s.fechaLectura = (SELECT MAX(s2.fechaLectura) FROM Servers s2 WHERE s2.nombreServidor = s.nombreServidor)")
    List<Servers> findLatestOfEachServer();
    
    // Obtener todos los registros de un servidor específico ordenados por fecha
    List<Servers> findByNombreServidorOrderByFechaLecturaDesc(String nombreServidor);
}
