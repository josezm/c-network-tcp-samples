  /* Server code in C */
 
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <netinet/in.h>
  #include <arpa/inet.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include <unistd.h>
  #include <iostream>
  #include <string>
  #include <thread>
  #include <map>
  using namespace std;

  map<string,int> listaClientes;  

  void process_client_thread(int socket_client){
      char buffer[256];
      bzero(buffer,256);
      int n,t;
      string entrada, size;
      bool entrar=true;

      while(entrar){
        bzero(buffer,256);
        //////////   READ   ////////////
        read(socket_client,buffer,3); // READ TAMAÑO DE CADENA
        t= atoi(buffer);
        read(socket_client,buffer,t+3);  // READ MENSAJE COMPLETO 

        entrada=buffer;
        string mynickname,nickname, mensaje;
        int espacio;
        
        espacio= entrada.find(' ');

      if(espacio!=-1){
        
        nickname = entrada.substr(0,espacio);
        mensaje = entrada.substr(espacio+1,entrada.size()-(espacio+1));
        //////////   WRITE   ////////////
        t=mensaje.size();
        size= std::to_string(t);
        while(size.size() != 3){
          size = "0"+size;
        }
        mensaje = size+mensaje;
        write(listaClientes[nickname],mensaje.c_str(),mensaje.length());
        cout << "Usuario " << socket_client << " ha enviado un mensaje a " << nickname <<endl;
      }

      else if(entrada == "lista"){
        string lista;
        cout << "Usuario " << socket_client << " solicito la Lista" <<endl;
        map<string,int>::iterator mapIt = listaClientes.begin();
        lista = "\n --- LISTA DE USUARIOS --- \n";

        for(;mapIt != listaClientes.end(); mapIt++){
          lista = lista + mapIt->first + " -> " + std::to_string(mapIt->second) + " \n";
          //cout << mapIt->first << "," << mapIt->second<< endl;
        }

        lista = lista + "---------------------------";

        //////////   WRITE   ////////////
        t= lista.size();
        size = std::to_string(t);
        while(size.size() != 3){
          size = "0"+size;
        }
        lista = size+lista;
        write(socket_client,lista.c_str(),lista.length());
      }

      else if(entrada=="chau"){
        cout << "Usuario " + socket_client << " se ha desconectado" << endl;
        shutdown(socket_client, SHUT_RDWR);
        close(socket_client);
        entrar=false;
      }
      
      else if(entrada.size()>0){
        mynickname = entrada;
        listaClientes[mynickname] = socket_client;
        cout << mynickname << " Registrado Correctamente" <<endl;
      }

      }

  }


  int main(void)
  {
    struct sockaddr_in stSockAddr;
    int ServerSD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    char buffer[256];
    int n;
 
    if(-1 == ServerSD)
    {
      perror("can not create socket");
      exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(1100);
    stSockAddr.sin_addr.s_addr = INADDR_ANY;
 
    if(-1 == bind(ServerSD,(const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("error bind failed");
      close(ServerSD);
      exit(EXIT_FAILURE);
    }
 
    if(-1 == listen(ServerSD, 10))
    {
      perror("error listen failed");
      close(ServerSD);
      exit(EXIT_FAILURE);
    }
 
    for(;;)
    {
      int ClienteSD = accept(ServerSD, NULL, NULL);
 
      if(0 > ClienteSD)
      {
        perror("error accept failed");
        close(ServerSD);
        exit(EXIT_FAILURE);
      }

      std::thread(process_client_thread,ClienteSD).detach();
      
    }
 
    return 0;
  }
