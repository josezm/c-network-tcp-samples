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
  using namespace std;

  bool setN = true;
  int n = 2;
  int nReal=0;
  int contN =0;
  char tablero[30][30] = {'.'};
  map<int,char> listaJugadores;
  int turno = -1;
  string abc = "abcdefghijklmopqrstuvwxyz123456789";
  vector<int> sockets;
  int nWin;
  int it=0;
  
  int checkwin(int x, int y, char ficha) {
    int count = 0;
    int posx = x;
    int posy = y;
        do {
            count++;
            posx--;
        } while (tablero[posx][y] == ficha && posx >= 0);

        posx = x;

        do {
            count++;
            posx++;
        } while (tablero[posx][y] == ficha && posx < n);

        count--;
        posx = x;
        if (count == nWin) return 1;
        else count =1;

        do {
            count++;
            posy++;
        } while (tablero[x][posy] == ficha && posy < n);

        count--;
        posy = y;

        do {
            count++;
            posy--;
        } while (tablero[x][posy] == ficha && posy >= 0);

        count--;
        posy = y;
        if (count == nWin) return 1;
        else count =1;

        do {
            count++;
            posy--;
            posx--;
        } while (tablero[posx][posy] == ficha && posy >= 0 && posx >= 0);

        count--;
        posx = x;
        posy = y;

        do {
            count++;
            posy++;
            posx++;
        } while (tablero[posx][posy] == ficha && posy < n && posx < n);

        count--;
        posx = x;
        posy = y;
        if (count == nWin) return 1;
        else count =1;


        do {
            count++;
            posy++;
            posx--;
        } while (tablero[posx][posy] == ficha && posy < n && posx >= 0);

        count--;
        posx = x;
        posy = y;

        do {
            count++;
            posy--;
            posx++;
        } while (tablero[posx][posy] == ficha && posy >= 0 && posx < n);

        count--;
        posx = x;
        posy = y;

      cout << " " << endl;
      for (int i = 0; i < n; i++) {
            cout << endl;
            for (int j = 0; j < n; j++) {
                cout << tablero[i][j];
            }
      }
      cout << " " << endl;

        if (count == nWin) return 1;
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (tablero[i][j] == '.') return 0;
            }
        }

    return 2;
}

  void process_client_thread(int socket_client){
      int win=0;
      char buffer[256];
      bzero(buffer,256);
      string mensaje="";
      string aux = to_string(n);
      

      if(contN<nReal){
        listaJugadores[socket_client] = abc[contN];
        sockets.push_back(socket_client);
        mensaje = abc[contN];
        mensaje = mensaje + "a";
        aux = aux.size() == 1 ? "0"+aux : aux;
        mensaje = mensaje + aux;
        write(socket_client,mensaje.c_str(),mensaje.length()); // mensaje de confirmación de registro
        contN++;
      }
      else{
        mensaje = "-1";
        write(socket_client,mensaje.c_str(),mensaje.length());
        return;
      }

      
      char ficha;
      int pos1, pos2,nChat;
      string mensajeChat;
      char bufferAux[2];


      while(!win){
        if(sockets[it%nReal] != socket_client) continue;
        bzero(buffer,256);
        read(sockets[it%nReal],buffer,7);
        ficha = buffer[0];
        bufferAux[0] = buffer[1];
        bufferAux[1] = buffer[2];
        pos1= atoi(bufferAux);
        bufferAux[0] = buffer[3];
        bufferAux[1] = buffer[4];
        pos2=atoi(bufferAux);
        bufferAux[0] = buffer[5];
        bufferAux[1] = buffer[6];
        nChat=atoi(bufferAux);

        mensaje = "A";
        mensaje = mensaje + buffer[0];
        mensaje = mensaje + buffer[1] + buffer[2] + buffer[3] + buffer[4] ;
        mensaje = mensaje + "T" ;
        mensaje = mensaje + listaJugadores[sockets[(it+1)%nReal]];
        mensaje = mensaje + buffer[5] + buffer[6];
        

        if (nChat > 0){
          bzero(buffer,256);
          read(sockets[it%nReal],buffer,nChat);
          mensajeChat=buffer;
          mensaje = mensaje + mensajeChat;
        }

        tablero[pos1][pos2] = ficha;
        

        win=checkwin(pos1,pos2,ficha);

        if(win==0){
          for(int i=0;i<n;i++){
             write(sockets[i],mensaje.c_str(),mensaje.length());
          }
        }

        else if(win==1){
          mensaje[0] = 'W';
          for(int i=0;i<n;i++){
             write(sockets[i],mensaje.c_str(),mensaje.length());
          }
        }

        else if(win==2){
          mensaje[0] = 'E';
          for(int i=0;i<n;i++){
             write(sockets[i],mensaje.c_str(),mensaje.length());
          }
        }
        it++;  
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
      
     if(setN){
       cout << "Numero de Jugadores: ";
       cin >> n;
       nReal=n;
       n = n+1;
       setN=false;
       nWin= (n/2) +1;
       nWin = nWin > 3 ? nWin : 3;

       for (int i = 0; i < n; i++) 
        for (int j = 0; j < n; j++) 
          tablero[i][j] = '.';
     }
     
      std::thread(process_client_thread,ClienteSD).detach();
      
    }
 
    return 0;
  }
