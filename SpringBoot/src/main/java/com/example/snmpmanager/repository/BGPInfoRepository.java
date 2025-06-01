package com.example.snmpmanager.repository;

import com.example.snmpmanager.model.BGPInfo;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface BGPInfoRepository extends JpaRepository<BGPInfo, Integer> {
    
    // Obtener el último registro de cada nodo BGP
    @Query("SELECT b FROM BGPInfo b WHERE b.fechaLectura = (SELECT MAX(b2.fechaLectura) FROM BGPInfo b2 WHERE b2.nombreNodo = b.nombreNodo)")
    List<BGPInfo> findLatestOfEachBGPNode();
    
    // Obtener todos los registros de un nodo BGP específico ordenados por fecha
    List<BGPInfo> findByNombreNodoOrderByFechaLecturaDesc(String nombreNodo);
}
