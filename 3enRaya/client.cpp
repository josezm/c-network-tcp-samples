 /* Client code in C */
 
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
  using namespace std;


  int main(void)
  {
    struct sockaddr_in stSockAddr;
    int Res;
    int SocketFD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    int n;
 
    if (-1 == SocketFD)
    {
      perror("cannot create socket");
      exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(50026);
    Res = inet_pton(AF_INET, "51.15.220.108", &stSockAddr.sin_addr);
 
    if (0 > Res)
    {
      perror("error: first parameter is not a valid address family");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
    else if (0 == Res)
    {
      perror("char string (second parameter does not contain valid ipaddress");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }
 
    if (-1 == connect(SocketFD, (const struct sockaddr *)&stSockAddr, sizeof(struct sockaddr_in)))
    {
      perror("connect failed");
      close(SocketFD);
      exit(EXIT_FAILURE);
    }

    char buffer[6];
    char bufferAux[2];
    int aux;
    bzero(buffer,6);
    string respuesta,entrada;
    int t;
    char ficha;
    char fichaRival;
    int x,y;
    bool win=0;
    char tablero[3][3];
    int mov;

    for(int i=0; i<3; i++)
        for(int j=0; j<3; j++)
          tablero[i][j] = '.';
    
    read(SocketFD,buffer,6);


    // REPARTICION DE FICHAS

    if (buffer[0] == 'X') {
      cout <<  endl << "TU FICHA ES LA [X]" << endl << endl;
      ficha = 'x';
    }
    else if (buffer[0] == 'O') {
      cout << "TU FICHA ES LA [0]" << endl << endl;
      ficha= 'o';
    }


    if(ficha=='x'){           // PRIMERA JUGADA
      cout << " INGRESA TU JUGADA --->   ";
      cin >> mov;
      tablero[mov/10][mov%10] = ficha;
      respuesta= to_string(mov);
      respuesta = ficha + respuesta;
      write(SocketFD,respuesta.c_str(),respuesta.length());
     }

    do{                     // BUCLE DE JUGADAS
      read(SocketFD,buffer,6);
      entrada = buffer;
      fichaRival = entrada[1];
      bufferAux[0] = entrada[2];
      bufferAux[1] = entrada[3];
      aux=atoi(bufferAux);
      x = aux/10;
      y= aux%10;
    
      if(entrada[0] == 'a'){      // CASO DE ACTUALIZACION
        tablero[x][y] = fichaRival;

        //PRINT TABLERO
        for(int i=0; i<3; i++){
          cout  << endl << "|";
          for(int j=0; j<3; j++){
            cout << tablero[i][j] << "|";
          }
        }
        cout << endl << "" << endl;

        do{ // COMPROBAR QUE SE PONE EN ESPACIO
        cout << " INGRESA TU JUGADA --->   ";
        cin >> mov;
        } while (tablero[mov/10][mov%10] != '.');

        tablero[mov/10][mov%10] = ficha; 
        respuesta= to_string(mov);
        respuesta = ficha + respuesta;
        write(SocketFD,respuesta.c_str(),respuesta.length());
      }

      else if(entrada[0]== 'l') {   // CASO DE PERDEDOR
        
        win = 1;

        //PRINT TABLERO
        for(int i=0; i<3; i++){
          cout  << endl << "|";
          for(int j=0; j<3; j++){
            cout << tablero[i][j] << "|";
          }
        }
        cout << endl << "" << endl;

        cout << " ----- HA PERDIDO EL JUEGO  -----" << endl;

      }
      else if(entrada[0] == 'w'){   // CASO DE GANADOR
        
        win = 1;

        //PRINT TABLERO
        for(int i=0; i<3; i++){
          cout  << endl << "|";
          for(int j=0; j<3; j++){
            cout << tablero[i][j] << "|";
          }
        }
        cout << endl << "" << endl;

        cout << "  ----- HAS GANADO EL JUEGO -----" << endl;

      }

      else if(entrada[0]== 'e'){   // CASO DE EMPATE
        tablero[x][y] = fichaRival;
        win = 1;

        //PRINT TABLERO
        for(int i=0; i<3; i++){
          cout  << endl << "|";
          for(int j=0; j<3; j++){
            cout << tablero[i][j] << "|";
          }
        }
        cout << endl << "" << endl;

        cout << "  ----- EL JUEGO QUEDO EMPATADO  -----" << endl;

      }

    
    }while(!win);
 
    shutdown(SocketFD, SHUT_RDWR);
 
    close(SocketFD);
    return 0;
  }
