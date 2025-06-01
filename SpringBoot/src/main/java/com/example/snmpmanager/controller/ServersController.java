package com.example.snmpmanager.controller;

import com.example.snmpmanager.model.Servers;
import com.example.snmpmanager.repository.ServersRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/VManager/servidores")
public class ServersController {
    
    @Autowired
    private ServersRepository serversRepository;

    // Obtener todos los registros
    @GetMapping
    public List<Servers> getAllServers() {
        return serversRepository.findAll();
    }

    // Obtener el último registro de cada servidor
    @GetMapping("/ultimos")
    public List<Servers> getLatestServers() {
        return serversRepository.findLatestOfEachServer();
    }

    // Obtener historial de un servidor específico ordenado por fecha
    @GetMapping("/historial/{nombreServidor}")
    public List<Servers> getServerHistory(@PathVariable String nombreServidor) {
        return serversRepository.findByNombreServidorOrderByFechaLecturaDesc(nombreServidor);
    }
}
