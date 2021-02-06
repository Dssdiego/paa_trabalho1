//
// Created by Diego Santos Seabra on 05/02/21.
//
#define STB_IMAGE_IMPLEMENTATION
#define GL_SILENCE_DEPRECATION

#include <imgui.h>
#include "imgui_impl_sdl.h"

#include "interface.h"
#include "structs.h"
#include "algoritmo.h"
#include "janela.h"

#include <SDL.h>
#include <stb_image/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef __EMSCRIPTEN__
#include <SDL_opengles2.h>
#include <emscripten.h>
#include <GLES3/gl3.h>
#endif

#define ALGO_MEDIA 0
#define ALGO_SAL_PIMENTA 1
#define ALGO_INTERVALO 2

// Variáveis Globais
static int tamanhoMascara = 3;
static int imagemSelecionada = 0;
static int algoritmoSelecionado = 0;
static float corSal[3] = {1.0f, 1.0f, 1.0f};
static float corPimenta[3] = {0.0f, 0.0f, 0.0f};
static float corMin[3] = {0.3f, 0.3f, 0.3f};
static float corMax[3] = {0.35f, 0.35f, 0.35f};
static bool limpaBuffer = true;
const char *imagens[] = {"Estátua", "Pinguim", "Tigre", "Osso", "Mona Lisa", "Menino"};
const char *algoritmos[] = {"Média", "Salt & Pepper", "Intervalo"};

// Criação dos objetos de imagem
Imagem estatua = {(char *) "assets/estatua.png", 0, 0, 0, nullptr};
Imagem pinguim = {(char *) "assets/pinguim.png", 0, 0, 0, nullptr};
Imagem tigre = {(char *) "assets/tigre.png", 0, 0, 0, nullptr};
Imagem osso = {(char *) "assets/osso.png", 0, 0, 0, nullptr};
Imagem monalisa = {(char *) "assets/monalisa.png", 0, 0, 0, nullptr};
Imagem menino = {(char *) "assets/menino.png", 0, 0, 0, nullptr};
Imagem imagemAtual = {};
Imagem imagemResultado = {};

Imagem imgs[] = { estatua, pinguim, tigre, osso, monalisa, menino };

TamanhoJanela tamanhoJanela;

void Interface::inicializa()
{
    // Busca o tamanho da janela (para usos de layout)
    tamanhoJanela = Janela::getTamanhoJanela();

    // Aplica tema escuro à interface
    ImGui::StyleColorsDark();
}

void Interface::inicializaImagens()
{
    for (auto & img : imgs)
    {
        carregaTexturaDoArquivo(img.nomeArquivo, &img.textura, &img.largura, &img.altura);
        img.pixels = stbi_load(img.nomeArquivo, &img.largura, &img.altura, nullptr, 1);
    }
}

void Interface::criaComponentes(SDL_Window *window)
{
    // Inicia um novo frame
    ImGui_ImplSdl_NewFrame(window);

    // Cria a janela principal
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(tamanhoJanela.x / 2, tamanhoJanela.y));
    ImGui::Begin("Algoritmo de Correção de Ruído", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ImGui::PushItemWidth(-1);

    // Imagem
    {
        ImGui::Text("Escolha uma imagem");
        ImGui::Combo("comboImagem", &imagemSelecionada, imagens, IM_ARRAYSIZE(imagens));

        imagemAtual = imgs[imagemSelecionada];
    }

    // Máscara
    {
        ImGui::LabelText("escolhaMascara", "Escolha o tamanho da máscara (NxN)");
        ImGui::InputInt("inputMascara", &tamanhoMascara, 2);

        // Limita para somente números positivos e maiores que 3 (tamanho mínimo de uma máscara)
        if (tamanhoMascara < 3) { tamanhoMascara = 3; }

        // Limita para somente números ímpares
        if (tamanhoMascara % 2 == 0){ tamanhoMascara += 1; }
    }

    // Algoritmo
    {
        ImGui::LabelText("escolheAlgoritmo", "Escolha um algoritmo");
        ImGui::Combo("comboAlgoritmo", &algoritmoSelecionado, algoritmos, IM_ARRAYSIZE(algoritmos));

        switch (algoritmoSelecionado)
        {
            case ALGO_SAL_PIMENTA:
                ImGui::LabelText("labelSal", "Cor do 'Sal'");
                ImGui::ColorEdit3("corSal", corSal, ImGuiColorEditFlags_HSV);
                mostraTooltipGrayscale();

                ImGui::LabelText("labelPimenta", "Cor da 'Pimenta'");
                ImGui::ColorEdit3("corPimenta", corPimenta, ImGuiColorEditFlags_HSV);
                mostraTooltipGrayscale();
                break;
            case ALGO_INTERVALO:
                ImGui::LabelText("labelMin", "Valor Mínimo");
                ImGui::ColorEdit3("corMin", corMin, ImGuiColorEditFlags_HSV);
                mostraTooltipGrayscale();

                ImGui::LabelText("labelPimenta", "Valor Máximo");
                ImGui::ColorEdit3("corMax", corMax, ImGuiColorEditFlags_HSV);
                mostraTooltipGrayscale();
                break;
        }
    }

    // Botão
    {
        ImGui::Checkbox("Limpa Buffer?", &limpaBuffer);
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Esta opção limpa a imagem da memória\n\nEx.: Para aplicar vários filtros na mesma imagem, deixe esta opção desabilitada\n\nDo contrário a imagem será 'resetada'");

        if (ImGui::Button("Rodar Algoritmo"))
        {
            if (limpaBuffer)
                inicializaImagens();

            switch (algoritmoSelecionado)
            {
                case ALGO_MEDIA:
                    Algoritmo::rodaMedia(&imagemAtual, tamanhoMascara, &imagemResultado);
                    carregaTexturaDoArquivo("resultado.png", &imagemResultado.textura, &imagemResultado.largura,
                                            &imagemResultado.altura);
                    break;
                case ALGO_SAL_PIMENTA:
                    Algoritmo::rodaSalPimenta(&imagemAtual, tamanhoMascara, corSal[0] * 255, corPimenta[0] * 255,
                                              &imagemResultado);
                    carregaTexturaDoArquivo("resultado.png", &imagemResultado.textura, &imagemResultado.largura,
                                            &imagemResultado.altura);
                    break;
                case ALGO_INTERVALO:
                    Algoritmo::rodaIntervalo(&imagemAtual, tamanhoMascara, corMin[0] * 255, corMax[0] * 255,
                                             &imagemResultado);
                    carregaTexturaDoArquivo("resultado.png", &imagemResultado.textura, &imagemResultado.largura,
                                            &imagemResultado.altura);
                    break;
            }
        }
    }

    ImGui::PopItemWidth();
    ImGui::End();

    /* Imagem de Entrada */
    {
        ImGui::SetNextWindowPos(ImVec2(tamanhoJanela.x/2, 0));
        ImGui::SetNextWindowSize(ImVec2(tamanhoJanela.x/2, tamanhoJanela.y/2));
        ImGui::Begin("Entrada", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        ImGui::Image((void *) (intptr_t) imagemAtual.textura, ImVec2(imagemAtual.largura, imagemAtual.altura));
        ImGui::End();
    }

    /* Tela Imagem Resultado */
    {
        ImGui::SetNextWindowPos(ImVec2(tamanhoJanela.x/2, tamanhoJanela.y/2));
        ImGui::SetNextWindowSize(ImVec2(tamanhoJanela.x/2, tamanhoJanela.y/2));
        ImGui::Begin("Resultado", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        ImGui::Image((void *) (intptr_t) imagemResultado.textura,
                     ImVec2(imagemResultado.largura, imagemResultado.altura));
        ImGui::End();
    }
}

void Interface::desenha()
{
    // Chama o método de desenho da interface
    ImGui::Render();
}

void Interface::limpa()
{
    // Limpa os dados de pixels das imagens criadas
    for (auto & img : imgs)
    {
        if(img.pixels)
        {
            stbi_image_free(img.pixels);
        }
    }

    // Limpa a interface
    ImGui_ImplSdl_Shutdown();
//    ImGui_ImplOpenGL3_Shutdown();
//    ImGui_ImplSDL2_Shutdown();
//    ImGui::DestroyContext();
}

void Interface::mostraTooltipGrayscale()
{
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Usar somente cores em grayscale\n\nV (0 a 255)");
}

bool Interface::carregaTexturaDoArquivo(const char *arquivo, GLuint *texturaSaida, int *larguraSaida, int *alturaSaida)
{
    // Load from file
    int image_width = 0;
    int image_height = 0;
    unsigned char *image_data = stbi_load(arquivo, &image_width, &image_height, nullptr, 4);
    if (image_data == nullptr)
        return false;

    // Create a OpenGL texture identifier
    GLuint image_texture;
    glGenTextures(1, &image_texture);
    glBindTexture(GL_TEXTURE_2D, image_texture);

    // Setup filtering parameters for display
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

    // Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
    stbi_image_free(image_data);

    *texturaSaida = image_texture;
    *larguraSaida = image_width;
    *alturaSaida = image_height;

    return true;
}
