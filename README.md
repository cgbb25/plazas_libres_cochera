# Proyecto: Control de Capacidad de Cochera

Este proyecto consiste en el diseño e implementación de un **sistema de control de plazas libres en una cochera**, mostrando la cantidad disponible en un **display de siete segmentos**. El sistema usa **sensores infrarrojos** para detectar la entrada y salida de vehículos y **servomotores** para controlar las puertas automáticas.  

---

## Características principales

✅ Conteo dinámico de vehículos que ingresan y salen.  
✅ Detección robusta de secuencias para evitar errores en el conteo.  
✅ Protección del vehículo gracias al cierre seguro de puertas.  
✅ Visualización del número de plazas libres en tiempo real.  

---

## Componentes considerados

- **4 sensores infrarrojos FC-51** (2 por puerta)  
- **2 servomotores** para apertura/cierre de puertas  
- **Display de siete segmentos (cátodo común)**  
- **Arduino UNO** o compatible  

---

## Secuencia lógica

### Entrada de vehículos
1. **Sensor externo detecta presencia** y el interno no detecta (vehículo aproximándose).  
2. Ambos sensores detectan simultáneamente (vehículo en medio de la puerta).  
3. Solo el sensor interno detecta (vehículo avanzando al interior).  
4. Ninguno detecta (vehículo totalmente ingresado → el conteo aumenta).  

### Salida de vehículos
1. **Sensor interno detecta presencia** y el externo no detecta (vehículo aproximándose a la salida).  
2. Ambos sensores detectan simultáneamente (vehículo en medio de la puerta).  
3. Solo el sensor externo detecta (vehículo avanzando hacia afuera).  
4. Ninguno detecta (vehículo totalmente fuera → el conteo disminuye).  

---

## Robustez del sistema

El sistema está diseñado para:
- Evitar conteos incorrectos si el vehículo retrocede en medio del proceso.  
- Permitir entrada y salida simultánea sin interferencias.  
- Impedir accesos indebidos por la puerta contraria.  
- Garantizar que las puertas solo cierren cuando el vehículo esté completamente dentro/fuera.  

---

## Estructura del proyecto

```
conteo_vehiculos_cochera/
├── conteo_vehiculo_cochera.ino → Código principal en Arduino IDE
├── Diagrama Esquematico.pdf → Circuito esquemático (Fritzing)
├── Diagrama Placa de Pruebas.pdf → Implementación en protoboard (Fritzing)
├── Diagrama de bloques.pdf → Diagrama explicativo general
├── Esquema cochera.pdf → Gráfico del circuito (Proteus)
├── Esquema cochera.pdsprj → Archivo de simulación (Proteus)
```

> 🔧 **Importante:**  
Antes de simular en Proteus, asegúrate de **cargar el programa `conteo_vehiculo_cochera.ino` en la tarjeta Arduino** dentro del archivo `.pdsprj`.

---

## Notas adicionales

Aunque este proyecto fue desarrollado usando componentes básicos, puede evolucionar y ser escalable hacia un sistema real aplicando protecciones de sistemas de potencia y control industrial, cumpliendo con estándares de seguridad y confiabilidad.

---

## Licencia

Este proyecto es de libre uso para fines educativos y no tiene garantía de funcionamiento en entornos industriales sin las adaptaciones correspondientes.

---

¡Gracias por contribuir y explorar este proyecto! 🚗🔧
Contacto: carlosgbaltodanoing@gmail.com
