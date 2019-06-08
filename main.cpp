#include <stdio.h>
#include <iostream>
#include <string.h> //strlen
#include <stdlib.h>
#include <unistd.h> //close
#include <arpa/inet.h> //close
#include <fcntl.h>

#define TRUE 1
#define FALSE 0
#define PUERTO 6969

using namespace std;

int main(int argc, char *argv[]) {
    auto sockfd = socket(PF_INET, SOCK_STREAM, 0);
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    int opt = TRUE;
    int master_socket, largoDireccion, nuevoSocket, socketCliente[4],
            numMaxClientes = 4, actividad, i, valorLeido, sd;
    int max_sd;
    struct sockaddr_in direccionServidor;
    cout << max_sd << endl;

    char buffer[180000]; //el buffer de datos

    //SET DE SOCKETS (lista de sockets clientes)
    fd_set readfds;

    //mensaje inicial
    //char *mensaje = "Hola cliente \r\n";

    //se inicializan todos los sockets como un 0
    for (i = 0; i < numMaxClientes; i++) {
        socketCliente[i] = 0;
    }

    //Crear un socket general
    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("EL socket falló");
        exit(EXIT_FAILURE);
    }

    //se hace un set del socket master para que reciba todas las conexiones
    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &opt,
                   sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    //tipo de socket creado
    direccionServidor.sin_family = AF_INET;
    direccionServidor.sin_addr.s_addr = inet_addr("127.0.0.1");
    direccionServidor.sin_port = htons(PUERTO);

    cout << direccionServidor.sin_addr.s_addr << endl;

    //COnecta el socket al puerto general 6969
    if (bind(master_socket, (struct sockaddr *) &direccionServidor, sizeof(direccionServidor)) < 0) {
        perror("Error al hacer el bind del socket");
        exit(EXIT_FAILURE);
    }
    printf("Recibiendo en el puerto  %d \n", PUERTO);

    //especificar el maximo de conexiones al socket
    if (listen(master_socket, 3) < 0) {
        perror("escuchando");
        exit(EXIT_FAILURE);
    }

    //Aquì es donde acepta las conexiones
    largoDireccion = sizeof(direccionServidor);
    puts("Esperando conexiones al server . . . ");

    while (TRUE) {
        //LImpia un set de sockets
        FD_ZERO(&readfds);

        //agrega el socket master al set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        //Agrega los socket CLIENTE al set
        for (i = 0; i < numMaxClientes; i++) {
            //descripcion del socket
            sd = socketCliente[i];

            //si el descriptor del socket es válido agregar a la lista de lectura
            if (sd > 0)
                FD_SET(sd, &readfds);

            //el numero mas alto del file descriptor, sirve para la funcion de select
            if (sd > max_sd)
                max_sd = sd;
        }

        //esperar alguna accion en alguna de los sockets
        //Null en el timeout para que espere indefinidamente
        actividad = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((actividad < 0) && (errno != EINTR)) {
            printf("Error en la funcion de select()");
        }

        //Si algo pasa en el master socket
        //quiere decir que esta recibiendo una conexion
        if (FD_ISSET(master_socket, &readfds)) {
            if ((nuevoSocket = accept(master_socket,
                                      (struct sockaddr *) &direccionServidor, (socklen_t *) &largoDireccion)) < 0) {
                perror("Aceptar");
                exit(EXIT_FAILURE);
            }

            //INformar al usuario el numero de socket - usado en enviar() o recibir()
            printf("Nueva conexion , el socket fd es %d , el IP es : %s , PUERTO : %d\n", nuevoSocket,
                   inet_ntoa(direccionServidor.sin_addr), ntohs
                           (direccionServidor.sin_port));
            cout << inet_ntoa(direccionServidor.sin_addr) << endl;

            //enviar a la nueva conexion un mensaje
            /** if( send(nuevoSocket, mensaje, strlen(mensaje), 0) != strlen(mensaje) )
             {
                 perror("enviar");
             }
             puts("Hola");***/

            //agregar el socket que se acepto a la lista de sockets
            for (i = 0; i < numMaxClientes; i++) {
                //si la posicion està vacìa
                if (socketCliente[i] == 0) {
                    //crea un nuevo socket en el set de sockets de clientes
                    socketCliente[i] = nuevoSocket;
                    printf("Agregando en la lista de sockets en la posicion %d\n", i);

                    break;
                }
            }
        }

        int contador = 0;
        for (i = 0; i < numMaxClientes; i++) {

            //revisa uno por uno si està recibiendo algo
            sd = socketCliente[i];

            if (FD_ISSET(sd, &readfds)) {
                //  Revisar si se cerrò el socket
                //recibir mensaje
                valorLeido = read(sd, buffer, 4096);//el buffer de datos


                if (valorLeido == 0) {
                    //SI ALGUIEN SE DESCONECTA
                    getpeername(sd, (struct sockaddr *) &direccionServidor, \
                        (socklen_t *) &largoDireccion);
                    printf("Cliente desconectado en, ip %s , port %d \n",
                           inet_ntoa(direccionServidor.sin_addr), ntohs(direccionServidor.sin_port));

                    //cerrar el socket de este cliente
                    close(sd);
                    socketCliente[i] = 0;

                }

                    //Devolver el mensaje que se recibio
                else {

                    int ID_cliente = i; //indica el cliente actual
                    const void *cvp = &i;

                    buffer[valorLeido] = '\0';

                    std::cout << "Valor: " << buffer << std::endl;


                    char *mensaje2 = "Mensaje recibido, y es: ";

                    string s = buffer;
                    if (s == "1") {
                        send(sd, mensaje2, strlen(mensaje2), 0);
                    } else {
                        string s = buffer;
                        send(sd, s.c_str(), s.length(), 0);

                    }


                }

            }

        }
    }

}