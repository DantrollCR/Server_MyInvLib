cmake_minimum_required(VERSION 3.14)
project(Server_MyInvLib)

set(CMAKE_CXX_STANDARD 14)

add_executable(Server_MyInvLib main.cpp Server.cpp Server.h Raid.cpp Raid.h b64.c base64.cpp base64.h json.h)