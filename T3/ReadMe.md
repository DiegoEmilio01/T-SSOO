# Consideraciones
* La página tarda en cargar
* Habrá que conectar los computadores manualente al wifi deseado
* Hay que setearles las IPs a las laptops manualmente. El enunciado no pide que se setee una contraseña o algo similar para asegurar que por default se haga aquello
* Otras acciones necesarias se encuentran en el .pkt


# Respuesta preguntas
# 3. Simulación
## Parte PDU
### 3.1
32 bits, que coincide con ser IPv4.

### 3.2
192.168.1.3, tiene sentido porque la IP de origen no debería cambiar.

### 3.3
3.3.0.2, que es la IP del server DNS.

### 3.4

| In layers | número layer | out layers |
|-----------|--------------|------------|
| IP header Src. IP: 192.168.1.3,  Dest. IP: 3.3.0.2 ICMP Message type: 8 | 3 (red) | IP header Src. IP: 3.3.0.2,  Dest. IP: 192.168.1.3 ICMP Message type: 0 |
| Ethernet II Header 0007.EC41.51D6 >> 00D0.BA90.3ACE | 2 (enlace) | Ethernet II Header 00D0.BA90.3ACE >> 0007-EC41.51D6 |
| Port: FastEthernet0 | 1 (física) | Port(s): FastEthernet0 |


La capa de red indica origen y destino de IP, en la de enlace se indica el MAC, y la física indica el puerto. In layer corresponde al mensaje que entra al servidor y que partió desde el cliente, y out layer indica la información del paquete que sale del servidor con destino final el cliente.

### 3.5
El cambio ocurre porque el cliente tiene una IP dinámica, por lo que al conectarse el router se requiere de Ethernet para poder enviarlo por streaming, mientras que las otras conexiones son fijas (las entre routers), por lo que se usa el HDLC.

## Parte RuzPedia
### 3.1
105 Bytes, ya que según la capa 4 que dice que la data es de largo 105.

### 3.2
El paquete DNS sirve para llegar a la IP de RuzPedia a partir del dominio ```www.ruzpedia.com```, es decir, traduce el dominio a la IP.

El paquete TCP se usa para transefir información entre cliente y servidor (ambos sentidos) y asegurar integridad de la información, además de establecer un canal de comunicación.

El paquete HTTP se usa para el envío de datos (en ambos sentidos, request-response) de una página web.

### 3.3
Primero se envían paquetes DNS desde la laptop al servidor DNS y luego en el sentido contrario.

Luego, se envían paquetes TCP entre la laptop y ruzrPedia solo pasando por el router en común, primero de la laptop a ruzPedia, y luego en el sentido contrario.

Posteriormente,  se envía HTTP desde la laptop a RuzPedia usando la misma ruta usada en TCP, y en el sentido contrario.

Tanto el envío de TCP como HTTP se repite varias veces, además de que se intercalan entre sí (es decir, TCP y HTTP se pueden estar enviando al mismo tiempo en un sentido cualquiera).


