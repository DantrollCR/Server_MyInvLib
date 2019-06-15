//
// Created by dantroll on 08/06/19.
//
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
#include "Server.h"

using namespace std;

void Server::init() {

    /* pruebas JSON ************* NO TOCAR PLIS ************************
     * string to JSON
     *      ptree pt;
            istringstream is (g);
            read_json (is, pt);
            string foo = pt.get<string> ("Jugador");
            cout<<foo<<endl;
     JSON to string
     /std::string as_json_string(ptree const& pt) {
    std::ostringstream oss;
    write_json(oss, pt);
    return oss.str();
}/
     /*
    ptree p;
    p.put("Integrante 1", "Me cago en Manuel");
    stringstream ss;
    boost::property_tree::json_parser::write_json(ss,p);
    cout << ss.str()<< endl;
*/

    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);

    // Port and ipAddress, socket hint
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_addr.s_addr = INADDR_ANY;
    hint.sin_addr.s_addr = inet_addr("127.0.0.2");
    hint.sin_port = htons(6967);

    //inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    bind(listening, (sockaddr *) &hint, sizeof(hint));

    //Tell Winsock the socket is for listening
    cout << "Waiting for client" << endl;
    listen(listening, SOMAXCONN);

    fd_set master;
    FD_ZERO(&master); //borra todo
    FD_SET(listening, &master); //metemos el socket del server SOLAMENTE


    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
    int array[10];
    int clientSocket;
    string nombre;


    while (true) {
        fd_set copy = master;
        int numberSocket = select(FD_SETSIZE, &copy, NULL, NULL, NULL);

        for (int i = 0; i < FD_SETSIZE; i++) {
            if (FD_ISSET(i, &copy)) {
                if (i == listening) {
                    clientSocket = accept(listening, (sockaddr *) &client, &clientSize);
                    cout << clientSocket << endl;
                    FD_SET(clientSocket, &master);
                    array[i] = clientSocket;
                    cout << "conexion exitosa" << endl;

                    char buf1[4096];
                    memset(buf1, 0, 4096);
                    int nameReceived = recv(clientSocket, buf1, 4096, 0);
                    nombre = string(buf1, 0, nameReceived);
                    //send(clientSocket, nombre.c_str(), nombre.size() + 1, 0);

                    cout << "Bienvenido: " << nombre << endl;
                    if (nombre != "Daniel") {
                        string strOut = "Probando esta picha a ver que.";
                        send(clientSocket, strOut.c_str(), strOut.size(), 0);
                    } else {
                        string strOut = "Serà que devuelta puede enviar un total de caracteres mayor a 33";
                        send(clientSocket, strOut.c_str(), strOut.size(), 0);
                    }
                } else {
                    char buf[4096];
                    memset(buf, 0, 4096);
                    int bytesReceived = recv(i, buf, 4096, 0);

                    if (bytesReceived == 0) //dont delete it, use 4 debugging
                    {
                        cout << "Client disconnected " << endl;
                        close(i);
                        FD_CLR(i, &master);
                        break;
                    } else {

                        for (int i = 0; i < FD_SETSIZE; i++) {
                            if (FD_ISSET(i, &master)) {
                                string respuesta = string(buf, 0, bytesReceived);
                                //Reading JSON

                                if (1 != listening && 1 != clientSocket) {
                                    string strOut = "Probando esta picha a ver que.";
                                    send(i, strOut.c_str(), strOut.size(), 0);

                                }
                            }
                        }
                    }
                }
            }
        }
    }
    // Close the socket
    close(clientSocket);
}

void Server::createConnection() {
    string valor;

    auto sockfd = socket(PF_INET, SOCK_STREAM, 0);
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    int opt = TRUE;
    int master_socket, largoDireccion, nuevoSocket, socketCliente[4],
            numMaxClientes = 4, actividad, i, valorLeido, sd;
    int max_sd;
    struct sockaddr_in direccionServidor;
    cout << max_sd << endl;

    char buffer[4096]; //el buffer de datos

    //SET DE SOCKETS (lista de sockets clientes)
    fd_set readfds;

    //mensaje inicial

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
        for (i = 0; i < 1; i++) {

            //revisa uno por uno si està recibiendo algo
            sd = socketCliente[i];

            if (FD_ISSET(sd, &readfds)) {

                /**
                 * Aquì ademàs de recibir el string del cliente, tambien verifica si
                 * el socket ya se cerrò, para asi volver a esperar otro cliente, u otra
                 * peticiòn en este caso.
                 */
                valorLeido = read(sd, buffer, 4096);//el buffer de datos


                if (valorLeido == 0) {
                    /**
                     * Cuando alguien se desconecta.
                     */
                    getpeername(sd, (struct sockaddr *) &direccionServidor, \
                        (socklen_t *) &largoDireccion);
                    printf("Cliente desconectado en, ip %s , port %d \n",
                           inet_ntoa(direccionServidor.sin_addr), ntohs(direccionServidor.sin_port));

                    //cerrar el socket de este cliente
                    close(sd);
                    std::cout << "Valor: " << valor << std::endl;
                    valor.clear();
                    socketCliente[i] = 0;

                }

                    /**
                     * En este else se envia al Cliente algun mensaje, en este caso es el mismo que se
                     * recibiò.
                     * *Aqui tambien està el problema que hay un limite de caracteres, 33, y no se porque.
                     *
                     */
                else {

                    int ID_cliente = i;
                    const void *cvp = &i;

                    buffer[valorLeido] = '\0';
                    valor.append(buffer);


                    char *mensaje2 = "OK_";

                    string s = buffer;
                    if (s == "1") {
                        /**
                         * Send recibe:
                         * @param sd : es el socket
                         * @param mensaje2 es lo que se le envia al cliente
                         * @params tercer espacio es el tamaño del mensaje, y el ultimo no sè.
                         */

                    } else {
                        string s = "OK";
                        send(sd, mensaje2, strlen(mensaje2), 0);

                    }


                }

            }

        }
    }
}