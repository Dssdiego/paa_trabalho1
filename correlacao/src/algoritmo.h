//
// Created by Diego Santos Seabra on 05/02/21.
//

#ifndef GUI_TEST_ALGORITMO_H
#define GUI_TEST_ALGORITMO_H

#include "structs.h"

namespace Algoritmo
{
    // Métodos Principais
    void rodaMedia(Imagem *entrada, int tamanhoMascara, Imagem *resultado);
    void rodaSalPimenta(Imagem *entrada, int tamanhoMascara, int corSal, int corPimenta, Imagem *resultado);
    void rodaIntervalo(Imagem *entrada, int tamanhoMascara, int min, int max, Imagem *resultado);

    // Métodos de 'Ajuda'
    int convertePosXY(int x, int y);
    int limiteMascara();
    bool salOuPimenta(int x, int y);
    bool pertenceIntervalo(int x, int y);
}

#endif //GUI_TEST_ALGORITMO_H
