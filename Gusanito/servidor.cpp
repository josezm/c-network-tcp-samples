  // g++ -o servidor.exe servidor.cpp -l pthread

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
  #include <map>
  #include <vector>
  #include <utility>
  using namespace std;

  bool inicio = true;
  bool win = false;

  int numeroJugadores = 0;
  int contadorJugadores =0;
  int contadorAlfabeto = 0;
  int posicionInicial =0;


  char tablero[20][20];
  

  map<int, pair<char,string>> jugadores;
  vector<int> socketsJugadores;
  string alfabeto = "abcdefghijklmnopqrstuvwxyz";
  
  
  string tableroToString(){
    string tab="";
    for(int i=0; i<20; i++){
      for(int j=0; j<20; j++){
        tab= tab + tablero[i][j];
      }
    }
    return tab;
  }

  int movimiento (char direccion, int posicionesGusano[8], char miFicha){
    if(direccion == 'W' || direccion == 'w')
    {
      if(posicionesGusano[6]>0)
      {
        if(tablero[posicionesGusano[6]-1][posicionesGusano[7]] == miFicha) return 1;
        if(tablero[posicionesGusano[6]-1][posicionesGusano[7]] != '.') return 0;
        tablero[posicionesGusano[0]][posicionesGusano[1]] = '.';
        posicionesGusano[0] = posicionesGusano[2];
        posicionesGusano[1] = posicionesGusano[3];
        posicionesGusano[2] = posicionesGusano[4];
        posicionesGusano[3] = posicionesGusano[5];
        posicionesGusano[4] = posicionesGusano[6];
        posicionesGusano[5] = posicionesGusano[7];
        posicionesGusano[6]--;
      }
      else return 1;
    }
    else if (direccion == 'S'|| direccion == 's')
    {
      if(posicionesGusano[6]<20)
      {
        if(tablero[posicionesGusano[6]+1][posicionesGusano[7]] == miFicha) return 1;
        if(tablero[posicionesGusano[6]+1][posicionesGusano[7]] != '.') return 0;
        tablero[posicionesGusano[0]][posicionesGusano[1]] = '.';
        posicionesGusano[0] = posicionesGusano[2];
        posicionesGusano[1] = posicionesGusano[3];
        posicionesGusano[2] = posicionesGusano[4];
        posicionesGusano[3] = posicionesGusano[5];
        posicionesGusano[4] = posicionesGusano[6];
        posicionesGusano[5] = posicionesGusano[7];
        posicionesGusano[6]++;
      }
      else return 1;
    }
    else if (direccion == 'A' || direccion == 'a')
    {
      if(posicionesGusano[7]>0)
      {
        if(tablero[posicionesGusano[6]][posicionesGusano[7]-1] == miFicha) return 1;
        if(tablero[posicionesGusano[6]][posicionesGusano[7]-1] != '.') return 0;
        tablero[posicionesGusano[0]][posicionesGusano[1]] = '.';
        posicionesGusano[0] = posicionesGusano[2];
        posicionesGusano[1] = posicionesGusano[3];
        posicionesGusano[2] = posicionesGusano[4];
        posicionesGusano[3] = posicionesGusano[5];
        posicionesGusano[4] = posicionesGusano[6];
        posicionesGusano[5] = posicionesGusano[7];
        posicionesGusano[7]--;

      }
      else return 1;
    }
    else if (direccion == 'D' || direccion == 'd')
    {
      if(posicionesGusano[7]<20)
      {
        if(tablero[posicionesGusano[6]][posicionesGusano[7]+1] == miFicha) return 1;
        if(tablero[posicionesGusano[6]][posicionesGusano[7]+1] != '.') return 0;
        tablero[posicionesGusano[0]][posicionesGusano[1]] = '.';
        posicionesGusano[0] = posicionesGusano[2];
        posicionesGusano[1] = posicionesGusano[3];
        posicionesGusano[2] = posicionesGusano[4];
        posicionesGusano[3] = posicionesGusano[5];
        posicionesGusano[4] = posicionesGusano[6];
        posicionesGusano[5] = posicionesGusano[7];
        posicionesGusano[7]++;
      }
      else return 1;
    }

    //cout << endl << posicionesGusano[0] << "," << posicionesGusano[1] << "   "  << posicionesGusano[2] << "," << posicionesGusano[3] << "   " <<  posicionesGusano[4] << "," << posicionesGusano[5] << "   " << posicionesGusano[6] << "," << posicionesGusano[7] << endl;
    return 1;
  }
  
  void printTablero(){
    for(int i=0; i<20; i++){
      cout << endl;
      for(int j=0; j<20; j++){
        cout << tablero[i][j];
      }
    }
  }

  void chat (int socket_client){
    
  } 


  void juego(int socket_client){

    int posicionesGusano[8];
    int miPosicionEnElVector;
    char miFicha;

    char buffer[256];
    char bufferAuxiliar[5];
    bzero(buffer,256);
    bzero(bufferAuxiliar,5);
    string readString;
    string writeString;
    int numeroAuxiliar;
    
    read(socket_client, buffer,3);
    sleep(1/1000);
    miPosicionEnElVector = contadorJugadores;
    contadorJugadores++;
    

    //REGISTRO
    if(buffer[0] == 'R' && contadorJugadores <= numeroJugadores){
      bufferAuxiliar[0] = buffer[1];
      bufferAuxiliar[1] = buffer[2];
      numeroAuxiliar = atoi(bufferAuxiliar);
      bzero(buffer,256);
      read(socket_client, buffer,numeroAuxiliar);
      sleep(1/1000);
      jugadores[socket_client] = make_pair(alfabeto[contadorAlfabeto], buffer);
      socketsJugadores.push_back(socket_client);
      miFicha= alfabeto[contadorAlfabeto];
      contadorAlfabeto++;
      writeString = "C";
      writeString += miFicha;
      writeString += to_string(numeroJugadores);
      //cout << "esto le mandare " << writeString << endl;

      //GUARDAR POSICION INICIAL DEL GUSANO
      posicionesGusano[0] = posicionInicial; posicionesGusano[1] = 0;
      posicionesGusano[2] = posicionInicial; posicionesGusano[3] = 1;
      posicionesGusano[4] = posicionInicial; posicionesGusano[5] = 2;
      posicionesGusano[6] = posicionInicial; posicionesGusano[7] = 3;
      posicionInicial++;
      //PONER GUSANO EN TABLERO
      tablero[posicionesGusano[0]][posicionesGusano[1]] = miFicha;
      tablero[posicionesGusano[2]][posicionesGusano[3]] = miFicha;
      tablero[posicionesGusano[4]][posicionesGusano[5]] = miFicha;
      tablero[posicionesGusano[6]][posicionesGusano[7]] = miFicha;
      //POSICION DE LA CABEZA
      write(socket_client,writeString.c_str(),writeString.length());
    }
    else return;
    
    cout << endl;
    printTablero();

    //JUEGO
    while(!win){
      read(socket_client, buffer,1);
      sleep(1/1000);

      if(buffer[0] == 'M'){ // M: MOVIMIENTO

        read(socket_client, buffer,1);
        sleep(1/1000);
        int resultadoMovimiento = movimiento(buffer[0],posicionesGusano, miFicha);
        //cout << "resultado  " << resultadoMovimiento << endl;

        if ( resultadoMovimiento == 1 ){
          //ACTUALIZAR TABLERO
          tablero[posicionesGusano[0]][posicionesGusano[1]] = miFicha;
          tablero[posicionesGusano[2]][posicionesGusano[3]] = miFicha;
          tablero[posicionesGusano[4]][posicionesGusano[5]] = miFicha;
          tablero[posicionesGusano[6]][posicionesGusano[7]] = miFicha;
          writeString = "";
          writeString = "A";
          writeString += miFicha; 
          writeString += tableroToString();
          //cout << "AAAAAAA   " << writeString << endl;
          for (int i = 0; i < socketsJugadores.size(); i++)
          {
            write(socketsJugadores[i],writeString.c_str(),writeString.length());
          }
          
        }
        else{

          tablero[posicionesGusano[0]][posicionesGusano[1]] = '.';
          tablero[posicionesGusano[2]][posicionesGusano[3]] = '.';
          tablero[posicionesGusano[4]][posicionesGusano[5]] = '.';
          tablero[posicionesGusano[6]][posicionesGusano[7]] = '.';
          writeString = "";
          writeString = "L";
          writeString += miFicha; 
          writeString += tableroToString();

          //cout << "LLLLLLLL   " << writeString << endl;
          

          if(contadorJugadores > 1){
            for (int i = 0; i < socketsJugadores.size(); i++)
            {
              write(socketsJugadores[i],writeString.c_str(),writeString.length());
            }
          }

          contadorJugadores--;
          
          if(contadorJugadores == 1){
            win=true;
          }

        }

        printTablero();
        cout << endl;

      }
      else if( buffer[0] == '-' ){
        //cout << "ENTRE ALOS MENSAJES" << endl;
        read(socket_client, buffer,2);
        sleep(1/1000);
        bufferAuxiliar[0] = buffer[0];
        bufferAuxiliar[1] = buffer[1];
        numeroAuxiliar = atoi(bufferAuxiliar);
        bzero(buffer,256);
        read(socket_client, buffer,numeroAuxiliar);
        sleep(1/1000);
        readString = buffer;

        writeString = "-";
        writeString += jugadores[socket_client].second.size() <10 ?  "0" + to_string(jugadores[socket_client].second.size()) : to_string(jugadores[socket_client].second.size());
        writeString += readString.size() <10 ? "0" + to_string( readString.size()) : to_string( readString.size());
        writeString += jugadores[socket_client].second;
        writeString += readString;

        //cout << "MENSAJE PARA TODOS : " << writeString << endl;
        for (int i = 0; i < socketsJugadores.size(); i++)
        {
          write(socketsJugadores[i],writeString.c_str(),writeString.length());
        }

      }

    }
    
  } 


  int main(void)
  {
    struct sockaddr_in stSockAddr;
    int ServerSD = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    
 
    if(-1 == ServerSD)
    {
      perror("can not create socket");
      exit(EXIT_FAILURE);
    }
 
    memset(&stSockAddr, 0, sizeof(struct sockaddr_in));
 
    stSockAddr.sin_family = AF_INET;
    stSockAddr.sin_port = htons(50026);
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

    char buffer[256];



    for(;;)
    {
      int ClienteSD = accept(ServerSD, NULL, NULL);
      
      if(0 > ClienteSD)
      {
        perror("error accept failed");
        close(ServerSD);
        exit(EXIT_FAILURE);
      }
      
      if(inicio){
        cout << "INGRESA NUMERO DE JUGADORES: ";
        cin >> numeroJugadores;

        
      for(int i=0; i<20; i++){
        for(int j=0; j<20; j++){
          tablero[i][j] = '.';
        }
      }


        inicio=false;
      }

      std::thread(juego,ClienteSD).detach();
      std::thread(chat,ClienteSD).detach();
      
    }
 
    return 0;
  }
