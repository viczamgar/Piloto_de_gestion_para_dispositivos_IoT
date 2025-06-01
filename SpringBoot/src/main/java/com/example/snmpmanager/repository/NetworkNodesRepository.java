package com.example.snmpmanager.repository;

import com.example.snmpmanager.model.NetworkNodes;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface NetworkNodesRepository extends JpaRepository<NetworkNodes, Integer> {
    
    // Obtener el último registro de cada nodo
    @Query("SELECT n FROM NetworkNodes n WHERE n.fechaLectura = (SELECT MAX(n2.fechaLectura) FROM NetworkNodes n2 WHERE n2.nombreNodo = n.nombreNodo)")
    List<NetworkNodes> findLatestOfEachNode();
    
    // Obtener todos los registros de un nodo específico ordenados por fecha
    List<NetworkNodes> findByNombreNodoOrderByFechaLecturaDesc(String nombreNodo);
}
