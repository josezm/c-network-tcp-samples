Chat implementado con threads que registra clientes de diferentes sockets y permite el envio de mensajes a un cliente específico
Detalle:
- El cliente fue implementado con un solo thread, este thread es el encargado de recibir los mensajes y mostrarlos en pantalla utilizando un busywait que espera los mensajes entrantes. No se utilizó un thread para el envio de mensajes debido a que por una razon que desconozco el cliente finalizaba la ejecución. Sin embargo funciona perfectamente con un solo thread.

| COMANDOS |

Compilar archivos:
g++ -o server.exe server.cpp
g++ -o client.exe client.cpp

Ejecutar archivos en distintas terminales
./server.exe
./client.exe


| ACCIONES |

Client.exe
Solicita mensajes:
- Una sola palabra se considera : nickname
- Dos palabras o mas se considera : nickname + mensaje
- La palabra "lista" devuelve:
	LISTA DE PARTICIPANTES
	Juan - 1
	Pedro - 2
	Pepe - 3
	Julio - 4


Server.exe
Imprime acciones del servidor:
	- Pedro envio mensaje a usuario 4
	- Usuario 3 solicitó La Lista
	- Julio registrado correctamente
	- Usuario 5 se ha desconectado
	