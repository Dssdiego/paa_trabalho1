//
// Created by Diego Santos Seabra on 05/02/21.
//

#ifndef GUI_TEST_STRUCTS_H
#define GUI_TEST_STRUCTS_H

#include <SDL_opengl.h>

struct Imagem
{
    char *nomeArquivo;
    GLuint textura;
    int largura;
    int altura;
    unsigned char *pixels;
};

struct TamanhoJanela
{
    int x,y;
};

#endif //GUI_TEST_STRUCTS_H
