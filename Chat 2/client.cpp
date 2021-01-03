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
  #include <thread>

  using namespace std;

  void llegadaMensajes(int SocketFD){
    char buffer[256];
    bzero(buffer,256);
    int n=0, t;
    string mensaje,size;
    
    while(true){
      //////////   READ   ////////////
      bzero(buffer,256); // vaciar buffer
      // read tamaño de cadena
      read(SocketFD,buffer,3);
      t=atoi(buffer);
      //read mensaje completo
      read(SocketFD,buffer,t+3);

      mensaje=buffer;
      if(mensaje=="") continue;
      else
      printf("\n  ||| NUEVO MENSAJE ||| ->  [ %s ]\n",buffer);

    }
 
    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);

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
    stSockAddr.sin_port = htons(1100);
    Res = inet_pton(AF_INET, "127.0.0.1", &stSockAddr.sin_addr);
 
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

    char buffer[256];
    bzero(buffer,256);
    string msgFromChat, size;
    int t;
    thread(llegadaMensajes,SocketFD).detach();

    do{
      
      cout << "\n MENSAJE ->     ";
      getline(cin,msgFromChat);
      //////////   WRITE   ////////////
      t = msgFromChat.size();
      size=std::to_string(t);
      while(size.size() != 3){
        size = "0"+size;
      }
      msgFromChat = size+msgFromChat;
      
      write(SocketFD,msgFromChat.c_str(),msgFromChat.length());

    }while(msgFromChat.compare("chau")!=0);
 
    shutdown(SocketFD, SHUT_RDWR);
    close(SocketFD);

    
    return 0;
  }
