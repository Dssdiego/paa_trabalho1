//
// Created by Diego Santos Seabra on 05/02/21.
//

#ifndef GUI_TEST_JANELA_H
#define GUI_TEST_JANELA_H

#include "structs.h"

namespace Janela
{
    // Inicializa a janela
    int inicializa(int largura, int altura, char *titulo);

    // Roda o loop principal
    void loopPrincipal();

    // Limpa os dados da memória
    void limpa();

    TamanhoJanela getTamanhoJanela();
}

#endif //GUI_TEST_JANELA_H
