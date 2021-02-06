//
// Created by Diego Santos Seabra on 05/02/21.
//

#define GL_SILENCE_DEPRECATION

#include "janela.h"
#include "interface.h"

#ifdef __EMSCRIPTEN__

#include <emscripten.h>
#include <GLES3/gl3.h>

#endif

#include <SDL.h>
#include <imgui.h>
#include <iostream>
#include "imgui_impl_sdl.h"

// Variáveis globais
SDL_Window *window;
SDL_GLContext contextoGL;
ImVec4 corBackground = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
static bool fechaJanela = false;
int larguraJanela;
int alturaJanela;

int Janela::inicializa(int largura, int altura, char *titulo)
{
    larguraJanela = largura;
    alturaJanela = altura;

    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Error: %s\n" << SDL_GetError() << '\n';
        return false;
    }

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Aplica interface no display atual
    SDL_DisplayMode current;
    SDL_GetCurrentDisplayMode(0, &current);

    // Cria janela e habilita o contexto
    window = SDL_CreateWindow(titulo,
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              largura, altura,
                              SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == nullptr)
        return 1;

    contextoGL = SDL_GL_CreateContext(window);

    // Faz o bind com a interface
    ImGui_ImplSdl_Init(window);

    // Faz o bind com a interface
    Interface::inicializa();

    // Carrega as texturas na memória
    Interface::inicializaImagens();

    // Roda o loop principal do programa
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(loopPrincipal, 0, 1);
#else
    while (!fechaJanela)
    {
        loopPrincipal();
    }
#endif

    // Limpa as variaveis utilizadas na memória e fecha o programa
    limpa();

    // Fecha o programa
    return EXIT_SUCCESS;
}

void Janela::loopPrincipal()
{
    // "Escuta" por eventos (mouse e teclado)
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        ImGui_ImplSdl_ProcessEvent(&e);

        // Sai do programa se clicar no botão de fechar
        if (e.type == SDL_QUIT)
            fechaJanela = true;

        // Sai do programa se apertar ESC
        if (e.type == SDL_KEYDOWN && SDL_GetKeyName(SDL_SCANCODE_ESCAPE))
        {
            fechaJanela = true;
        }
    }

    // Cria componentes de interface
    Interface::criaComponentes(window);

    // Prepara para desenhar na tela (independente da resolução ou redimensionamento)
    int w, h;
    SDL_GL_GetDrawableSize(window, &w, &h);
    glViewport(0, 0, w, h);
    glClearColor(corBackground.x, corBackground.y, corBackground.z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, w, h);
    Interface::desenha();

    // Troca o buffer do background com o da frente;
    // ou seja, desenha na tela
    SDL_GL_SwapWindow(window);
}

void Janela::limpa()
{
    Interface::limpa();

    SDL_GL_DeleteContext(contextoGL);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

TamanhoJanela Janela::getTamanhoJanela()
{
    return {larguraJanela, alturaJanela};
}