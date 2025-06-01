package com.example.snmpmanager.controller;

import com.example.snmpmanager.model.BGPInfo;
import com.example.snmpmanager.repository.BGPInfoRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/VManager/infobgp")
public class BGPInfoController {
    
    @Autowired
    private BGPInfoRepository bgpInfoRepository;


    // Obtener el último registro de cada nodo BGP
    @GetMapping("/ultimos")
    public List<BGPInfo> getLatestBGPInfo() {
        return bgpInfoRepository.findLatestOfEachBGPNode();
    }

    // Obtener historial de un nodo BGP específico ordenado por fecha
    @GetMapping("/historial/{nombreNodo}")
    public List<BGPInfo> getBGPHistory(@PathVariable String nombreNodo) {
        return bgpInfoRepository.findByNombreNodoOrderByFechaLecturaDesc(nombreNodo);
    }
}
