  // g++ -o cliente.exe cliente.cpp -l pthread

  // LA IP Y EL PUERTO ESTABLECIOS SON LOS DEL SERVIDOR VPS
  // IP = 51.15.220.108
  // PUERTO = 50026

 
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

  using namespace std;

  bool fin = false;

  
  void jugadas(int SocketFD){
    char tablero[30][30] = {'.'};
    char buffer[256];
    bzero(buffer,256);
    string entrada, mensaje;
    int win = 0;
    char miFicha;
    char turno;
    int n;
    char bufferAux[2];

    


    read(SocketFD,buffer,4);
    if(buffer[0] != '-'){
      cout << "REGISTRO EXITOSO - SU FICHA ES : [" << buffer[0] << "]" << endl;
      cout << "LAS JUGADAS SE PONEN SEPARADAS DE UNA COMA y PARA ESCRIBIR UN MENSAJE SEPARARLO CON UN ESPACIO"<< endl << "EJ:  3,3 mensaje" << endl << endl;
      miFicha = buffer[0];
      turno = buffer[1];
      bufferAux[0] = buffer[2];
      bufferAux[1] = buffer[3];
      n = atoi(bufferAux);
    }
    else{
      cout << "YA NO SE PERMITEN MAS JUGADORES" << endl;
      return;
    }


    for (int i = 0; i < n; i++) 
        for (int j = 0; j < n; j++) 
          tablero[i][j] = '.';  

    int pos1, pos2, nChat;
    string mensajeChat;
    char fichaActualizar;
    char indicador;
  
    while(!win){
      
      if(miFicha!=turno){
        cout << " ... Esperando a los demás jugadores ..." << endl;
        bzero(buffer,256);
        read(SocketFD,buffer,10);
        //cout << "RECIBIDO : " << buffer << endl;
        indicador=buffer[0];
        fichaActualizar = buffer[1];
        bufferAux[0] = buffer[2];
        bufferAux[1] = buffer[3];
        pos1= atoi(bufferAux);
        bufferAux[0] = buffer[4];
        bufferAux[1] = buffer[5];
        pos2=atoi(bufferAux);
        bufferAux[0] = buffer[8];
        bufferAux[1] = buffer[9];
        nChat=atoi(bufferAux);
        turno = buffer[7];

        if(nChat > 0){
          bzero(buffer,256);
          read(SocketFD,buffer,nChat);
          mensajeChat = buffer;
          cout << endl << " ---> NUEVO MENSAJE DE [" <<  fichaActualizar <<  "] :   " << mensajeChat << endl;
        }

        tablero[pos1][pos2] = fichaActualizar;
        

      cout << " " << endl;
        for(int i=0; i<n; i++){
          cout  << endl << "|";
          for(int j=0; j<n; j++){
            cout << tablero[i][j] << "|";
          }
        }
        cout << endl << "" << endl;


        if(indicador == 'W'){
          if(fichaActualizar == miFicha){
            cout << " ---- FELICIDADES ERES EL GANADOR ------" << endl;
            win = true;
            fin = true;
            return;
          }
          else{
            cout << " ----- LO SENTIMOS, HAS PERDIDO  ------" << endl;
            win = true;
            fin = true;
            return;
          }
        }

        else if( indicador == 'E'){
          cout << "---- EL JUEGO QUEDO EMPATADO ----- " << endl;
          win = true;
          fin = true;
          return;

        }
      }

      else{
        string jugada;
        int coma,espacio;
        mensaje = miFicha;
        cout << "INGRESA TU JUGADA (X,Y) y MENSAJE (SI QUIERES): ";
        getline(cin,jugada);
        coma = jugada.find(',');
        espacio = jugada.find(' ');
        string aux, aux2, aux3;
        if(espacio >= 0){
          aux = jugada.substr(0,coma).size() != 1 ? jugada.substr(0,coma) : "0"+jugada.substr(0,coma);
          aux2 = jugada.substr(coma+1,espacio-(coma+1)).size() != 1 ? jugada.substr(coma+1,espacio-(coma+1)) : "0"+jugada.substr(coma+1,espacio-(coma+1));
          aux3 = jugada.substr(espacio+1, jugada.size() - (espacio+1)).size() >= 10 ? to_string(jugada.substr(espacio+1, jugada.size() - (espacio+1)).size()) : "0" + to_string(jugada.substr(espacio+1, jugada.size() - (espacio+1)).size());
          mensaje = mensaje + aux + aux2;
          mensaje = mensaje + aux3 + jugada.substr(espacio+1, jugada.size() - (espacio+1));
        }
        else{
          aux = jugada.substr(0,coma).size() != 1 ? jugada.substr(0,coma) : "0"+jugada.substr(0,coma);
          aux2 = jugada.substr(coma+1,espacio-(coma+1)).size() != 1 ? jugada.substr(coma+1,jugada.size()-(coma+1)) : "0"+jugada.substr(coma+1,jugada.size()-(coma+1));
          mensaje = mensaje + aux + aux2;
        }
        turno= '*';
        write(SocketFD,mensaje.c_str(),mensaje.length());
        }
      }
  }


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


    std::thread(jugadas,SocketFD).detach();

    while(!fin){
     fin=false;
    }
 
    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);

    
    return 0;
  }
