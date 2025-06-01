package com.example.snmpmanager.controller;

import com.example.snmpmanager.model.NetworkNodes;
import com.example.snmpmanager.repository.NetworkNodesRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/VManager/nodosdered")
public class NetworkNodesController {
    
    @Autowired
    private NetworkNodesRepository networkNodesRepository;

    // Obtener todos los registros
    @GetMapping
    public List<NetworkNodes> getAllNetworkNodes() {
        return networkNodesRepository.findAll();
    }

    // Obtener el último registro de cada nodo
    @GetMapping("/ultimos")
    public List<NetworkNodes> getLatestNetworkNodes() {
        return networkNodesRepository.findLatestOfEachNode();
    }

    // Obtener historial de un nodo específico ordenado por fecha
    @GetMapping("/historial/{nombreNodo}")
    public List<NetworkNodes> getNodeHistory(@PathVariable String nombreNodo) {
        return networkNodesRepository.findByNombreNodoOrderByFechaLecturaDesc(nombreNodo);
    }
}
