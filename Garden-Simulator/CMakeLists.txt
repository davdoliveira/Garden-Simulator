cmake_minimum_required(VERSION 3.30)
project(tp)

set(CMAKE_CXX_STANDARD 20)

add_executable(tp src/main.cpp
        src/jardim.cpp
        include/jardim.h
        src/celula.cpp
        include/celula.h
        src/jardineiro.cpp
        include/jardineiro.h
        include/settings.h
        src/comandos.cpp
        include/comandos.h
        src/planta.cpp
        include/planta.h
        src/cacto.cpp
        include/cacto.h
        src/ferramenta.cpp
        include/ferramenta.h
        src/roseira.cpp
        include/roseira.h
        src/ervadaninha.cpp
        include/ervadaninha.h
        src/plantaexotica.cpp
        include/plantaexotica.h
        src/regador.cpp
        include/regador.h
        src/adubo.cpp
        include/adubo.h
        src/tesourapoda.cpp
        include/tesourapoda.h
        src/ferramentaz.cpp
        include/ferramentaz.h)
