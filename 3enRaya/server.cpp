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
  #include <utility> 

  using namespace std;
 
  bool flag=0;
  char tablero[3][3];
  pair <int, char> j1;
  pair <int, char> j2;
  int win=0;


  int ganador( char ficha){
    if(tablero[0][0] == ficha && tablero[0][1] == ficha && tablero[0][2] == ficha) return 1;
    else if(tablero[1][0] == ficha && tablero[1][1] == ficha && tablero[1][2] == ficha) return 1;
    else if(tablero[2][0] == ficha && tablero[2][1] == ficha && tablero[2][2] == ficha) return 1;
    else if(tablero[0][0] == ficha && tablero[1][0] == ficha && tablero[2][0] == ficha) return 1;
    else if(tablero[0][1] == ficha && tablero[1][1] == ficha && tablero[2][1] == ficha) return 1;
    else if(tablero[0][2] == ficha && tablero[1][2] == ficha && tablero[2][2] == ficha) return 1;
    else if(tablero[0][0] == ficha && tablero[1][1] == ficha && tablero[2][2] == ficha) return 1;
    else if(tablero[2][0] == ficha && tablero[1][1] == ficha && tablero[0][2] == ficha) return 1;

    for(int i=0; i<3; i++)
      for(int j=0; j<3; j++)
        if (tablero[i][j] == '.') return 0;

    return 2;
  }



  void process_client_thread(int socket_client){
    
      char buffer[6];
      int aux;
      char bufferAux[2];
      bzero(buffer,6);
      string entrada;
      string respuesta;

      
      char ficha, ficha2;
      int x,y;

      // ASIGNACION DE FICHAS Y SOCKETS
      if(!flag) {
        j1.first = socket_client;
        j1.second = 'x';
        flag=1;
        respuesta="XP11YR";
        write(socket_client,respuesta.c_str(),respuesta.length());
      }
      else {
        j2.first = socket_client;
        j2.second = 'o';
        respuesta="OP11YR";
        write(socket_client,respuesta.c_str(),respuesta.length());
      }

      // LLENAR TABLERO
      for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          tablero[i][j] = '.';

      do{                 // BUCLE DE JUGADAS
        bzero(buffer,6);
        read(socket_client,buffer,3);
        entrada=buffer;
        ficha=entrada[0];
        bufferAux[0] = entrada[1];
        bufferAux[1] = entrada[2];
        aux=atoi(bufferAux);
        x = aux/10;
        y= aux%10;

        if(ficha=='x') ficha2= 'o';
        else if (ficha == 'o') ficha2='x';

          tablero[x][y] = ficha;
      
          if(win==0)
            win = ganador(ficha);
          
          
          
          if(win==0){       //JUEGO CONTINUA
            respuesta = 'a' + entrada + 't' + ficha2;
            if(ficha=='x')  write(j2.first,respuesta.c_str(),respuesta.length());
            else if (ficha=='o') write(j1.first,respuesta.c_str(),respuesta.length());
          }

          else if(win==1){   //GANADOR
            respuesta = 'w' + entrada + 't' + ficha2;
            if(ficha=='x')  {
              write(j1.first,respuesta.c_str(),respuesta.length());
              respuesta[0] = 'l';
              write(j2.first,respuesta.c_str(),respuesta.length());
            }
            else if (ficha=='o') {      // PERDEDOR
              write(j2.first,respuesta.c_str(),respuesta.length());
              respuesta[0] = 'l';
              write(j1.first,respuesta.c_str(),respuesta.length());
            }

          }

          else if(win==2){    //EMPATE
            respuesta = 'e' + entrada + 't' + ficha2;
            write(j1.first,respuesta.c_str(),respuesta.length());
            write(j2.first,respuesta.c_str(),respuesta.length());
            
          }

      }while(!win);

      
      cout << " JUEGO ACABADO DE USUARIO:  " << socket_client << endl;
      flag=0;
      shutdown(socket_client, SHUT_RDWR);
      close(socket_client);
      return;
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
      
      win=0;  
      std::thread(process_client_thread,ClienteSD).detach();
    }

    
    close(ServerSD);
    exit(EXIT_FAILURE);
 
    return 0;
  }
