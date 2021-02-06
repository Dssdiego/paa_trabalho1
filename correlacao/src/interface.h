//
// Created by Diego Santos Seabra on 05/02/21.
//

#ifndef GUI_TEST_INTERFACE_H
#define GUI_TEST_INTERFACE_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <SDL_opengles2.h>
#endif

#include <SDL.h>

namespace Interface
{
    // Métodos Principais
    void inicializa();
    void inicializaImagens();
    void criaComponentes(SDL_Window *window);
    void desenha();
    void limpa();

    // Métodos de 'Ajuda'
    void mostraTooltipGrayscale();
    bool carregaTexturaDoArquivo(const char *arquivo, uint *texturaSaida, int *larguraSaida, int *alturaSaida);
}

#endif //GUI_TEST_INTERFACE_H
