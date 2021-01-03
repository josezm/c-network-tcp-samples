El servidor recibe las consultas del o los clientes, devolviendo los resultados indicando la operación que se realizó en el terminal del cliente.
En el terminal del servidor se imprime un mensaje indicando el exito de la operación enviada.

| COMANDOS |

Compilar archivos:
g++ -o server.exe server.cpp
g++ -o client.exe client.cpp

Ejecutar archivos en distintas terminales
./server.exe
./client.exe


| ACCIONES |

Client.exe
Solicita Operacion Ej : 10 10 +
Devuelve: Su suma es 20

Server.exe
Imprime: La suma ha sido enviada - Resultado 20

