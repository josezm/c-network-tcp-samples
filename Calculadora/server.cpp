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
  using namespace std;
 

  void process_client_thread(int socket_client){
      char buffer[256];
      bzero(buffer,256);
      int n;
      string entrada;
      do{
        bzero(buffer,256);
        n = read(socket_client,buffer,256);
        if (n < 0) perror("ERROR reading from socket");
        //printf("Mensaje del cliente >>>>> [ %s ]\n",buffer);
        
        entrada=buffer;
        string factor1, factor2, op;
        int f1,f2,res;
        int espacio;

        espacio= entrada.find(' ');

        factor1 = entrada.substr(0,espacio);
        factor2 = entrada.substr(espacio+1,entrada.size()-2-(espacio+1));

        f1=std::stoi(factor1);
        f2=std::stoi(factor2);
        
        if(entrada[entrada.size()-1] == '+'){
          res=f1+f2;
          op="suma";
        }
        else if(entrada[entrada.size()-1] == '*'){
          res=f1*f2;
          op="multiplicacion";
        }
        else if(entrada[entrada.size()-1] == '-'){
          res=f1-f2;
          op="resta";
        }
        else if(entrada[entrada.size()-1] == '/'){
          res=f1/f2;
          op="division";
        }

        cout << "La " + op + " ha sido enviada - Resultado: " + std::to_string(res) << endl;
            
        string respuesta = "Su " + op + " es : " + std::to_string(res);

        n = write(socket_client,respuesta.c_str(),respuesta.length());
        

      if (n < 0) perror("ERROR writing to socket");
    
      }while(entrada.compare("chau")!=0);
 
     /* perform read write operations ... */
      shutdown(socket_client, SHUT_RDWR);
 
      close(socket_client);
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
