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
  bool win = false;
  bool registrado = false;
  bool tipoDeMensaje = 0; //0: JUGADA   1:MENSAJE
  int numeroJugadores = 0;
  char tablero[20][20];
  char miFicha;

  string nickname;

  void actualizarTablero(string tab){
    int it=0;
    for(int i=0; i<20; i++){
      for(int j=0; j<20; j++){
        tablero[i][j] = tab[it];
        it++;
      }
    }
  }
  
  void printTablero(){
    cout << endl << endl;
    for(int i=0; i<20; i++){
      cout << endl;
      for(int j=0; j<20; j++){
        cout << tablero[i][j];
      }
    }
    cout << endl << endl;
  }


  void chat(int SocketFD){
    
  }

  void leerjugada(int SocketFD){
    char readTablero[402];
    bzero(readTablero,402);
    string readString;

    char mensajeChat[200];
    bzero(mensajeChat,200);
    char bufferAuxiliar[2];
    int sizeNickname;
    int sizeMensaje;
    string nickname;
    string mensaje;
    
    while(!registrado){
      win = false;
    }

    while(!win){
      //cout << "entre a read" << endl;
      read(SocketFD, readTablero,1);
      readString = readTablero;
      //cout << "TOKEN LEIDO " << readTablero[0] << endl;

      if(readTablero[0] == 'A'){
        //cout << "entre a A" << endl;
        read(SocketFD, readTablero,401);
        sleep(1/1000);
        readString = readTablero;
        actualizarTablero(readString.substr(1,400));
        printTablero();
        cout << endl;
      }
      else if (readTablero[0] == 'L'){
        //cout << "entre a L" << endl;
        //cout << "ALGUIEN PERDIO   " << readTablero[1] << endl;
        read(SocketFD, readTablero,401);
        sleep(1/1000);
        readString = readTablero;
        actualizarTablero(readString.substr(1,400));
        if(readTablero[0] == miFicha){
          cout << "--- LO SENTIMOS PERDISTTE ---" << endl;
          win=true;
          return;
        }
        numeroJugadores--;
        //cout << numeroJugadores << endl;
        if(numeroJugadores == 1) {
          cout << " --- FELICIDADES GANASTE ---" << endl;
          win=true;
          return;
        }
      }
      else if (readTablero[0] == '-'){
        //cout << "ENTRE A LEER MENSAJES SALDRE?" << endl;
        read(SocketFD, readTablero,4);
        sleep(1/1000);
        
        bufferAuxiliar[0] = readTablero[0];
        bufferAuxiliar[1] = readTablero[1];
        sizeNickname = atoi(bufferAuxiliar);
        bufferAuxiliar[0] = readTablero[2];
        bufferAuxiliar[1] = readTablero[3];
        sizeMensaje = atoi(bufferAuxiliar);

        bzero(readTablero,402);
        read(SocketFD, readTablero,sizeNickname+sizeMensaje);
        sleep(1/1000);
        //cout << "Mensaje leido: " << readTablero;
        readString = readTablero;
        nickname = readString.substr(0,sizeNickname);
        mensaje = readString.substr(sizeNickname,sizeMensaje);

        cout << endl << nickname << ": " << mensaje << endl << endl;

      }

    }  
  }   
  
  
  void juego(int SocketFD){
    
    
    char buffer[256];
    
    char bufferAuxiliar[1];
    bzero(buffer,256);
    bzero(bufferAuxiliar,1);
    string readString;
    string writeString;
    int numeroAuxiliar;
    string mov;

    cout << "INGRESA TU NICKNAME --> ";
    cin >> nickname;
    writeString = "R";
    writeString += nickname.size()<10 ? "0" + to_string(nickname.size()) : to_string(nickname.size());
    writeString += nickname;
    write(SocketFD,writeString.c_str(),writeString.length());

    read(SocketFD, buffer,3);
    sleep(1/1000);
    //cout << "BUFFER RECIBIDO  " << buffer << endl;
    if( buffer[0] == 'C'){
      miFicha = buffer[1];
      bufferAuxiliar[0] = buffer[2];
      numeroJugadores = atoi(bufferAuxiliar);
      //cout << "numero jugadores recibido  " << numeroJugadores << endl;
    }

    registrado=true;
    //cout << "registrado" << endl;
    while(!win){
        getline(cin,mov);
      if(mov[0] != '-'){
        writeString = "M" + mov;
        write(SocketFD,writeString.c_str(),writeString.length());
      }
      else if (mov[0] == '-'){
        //cout << "ENTRE A LOS MENSAJES" << endl;
        writeString = "-";
        writeString +=  (mov.size() -1 )<10 ? "0" + to_string(mov.size()-1) : to_string(mov.size()-1);
        writeString += mov.substr(1,mov.size()-1);
        //cout << "MENSAJE QUE ENVIARE: " << writeString << endl;
        write(SocketFD,writeString.c_str(),writeString.length());
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


    std::thread(juego,SocketFD).detach();
    std::thread(chat,SocketFD).detach();
    std::thread(leerjugada,SocketFD).detach();

    while(!fin){
     fin=false;
    }
 
    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);

    
    return 0;
  }
