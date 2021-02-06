//
// Created by Diego Santos Seabra on 05/02/21.
//

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "algoritmo.h"
#include <stb_image/stb_image_write.h>

int linhas, colunas;
int soma = 0;
unsigned char *pixels = nullptr;
int corSal, corPimenta, corMin, corMax;
int tamMasc;

void Algoritmo::rodaMedia(Imagem *entrada, int tamanhoMascara, Imagem *resultado)
{
    pixels = entrada->pixels;
    linhas = entrada->largura;
    colunas = entrada->altura;
    tamMasc = tamanhoMascara;

    // Percorre as colunas da imagem
    for (int x = 0; x < colunas; ++x)
    {
        // Percorre as linhas da imagem
        for (int y = 0; y < linhas; ++y)
        {
            // soma os pixels de uma mascara 3x3
            for (int i = x - limiteMascara(); i <= x + limiteMascara(); ++i)
            {
                for (int j = y - limiteMascara(); j <= y + limiteMascara(); ++j)
                {
                    // se indice da mascara é maior que o tamanho das colunas
                    // sai do loop
                    if (i == colunas)
                        break;

                    // se indice da mascara é maior que o tamanho das linhas
                    // sai do loop
                    if (j == linhas)
                        break;

                    soma += pixels[convertePosXY(i, j)];
                }
            }

            // calcula a media
            int media = soma / (tamanhoMascara * tamanhoMascara);

            // preenche o pixel na imagem de
            pixels[convertePosXY(x, y)] = media;

            // reseta a soma (para calcular os outros pixels)
            soma = 0;
        }
    }

    // Preenche o resultado
    resultado->largura = entrada->largura;
    resultado->altura = entrada->altura;
    resultado->pixels = pixels;

    // Salva o resultado em uma imagem
    stbi_write_png("resultado.png", linhas, colunas, 1, resultado->pixels, linhas);
}

void Algoritmo::rodaSalPimenta(Imagem *entrada, int tamanhoMascara, int sal, int pimenta, Imagem *resultado)
{
    pixels = entrada->pixels;
    linhas = entrada->largura;
    colunas = entrada->altura;
    tamMasc = tamanhoMascara;
    corSal = sal;
    corPimenta = pimenta;

    // Percorre as colunas da imagem
    for (int x = 0; x < colunas; ++x)
    {
        // Percorre as linhas da imagem
        for (int y = 0; y < linhas; ++y)
        {
            // Somente executa a correção no pixel que for sal (255) ou pimenta (0)
            if (salOuPimenta(x, y))
            {
                // soma os pixels de uma mascara 3x3
                for (int i = x - limiteMascara(); i <= x + limiteMascara(); ++i)
                {
                    for (int j = y - limiteMascara(); j <= y + limiteMascara(); ++j)
                    {
                        // se indice da mascara é maior que o tamanho das colunas
                        // sai do loop
                        if (i == colunas)
                            break;

                        // se indice da mascara é maior que o tamanho das linhas
                        // sai do loop
                        if (j == linhas)
                            break;

                        soma += pixels[convertePosXY(i, j)];
                    }
                }

                // calcula a media
                int media = soma / (tamanhoMascara * tamanhoMascara);

                // preenche o pixel na imagem de
                pixels[convertePosXY(x, y)] = media;

                // reseta a soma (para calcular os outros pixels)
                soma = 0;
            }
        }
    }

    // Preenche o resultado
    resultado->largura = entrada->largura;
    resultado->altura = entrada->altura;
    resultado->pixels = pixels;

    // Salva o resultado em uma imagem
    stbi_write_png("resultado.png", linhas, colunas, 1, resultado->pixels, linhas);
}

void Algoritmo::rodaIntervalo(Imagem *entrada, int tamanhoMascara, int min, int max, Imagem *resultado)
{
    pixels = entrada->pixels;
    linhas = entrada->largura;
    colunas = entrada->altura;
    tamMasc = tamanhoMascara;
    corMin = min;
    corMax = max;

    // Percorre as colunas da imagem
    for (int x = 0; x < colunas; ++x)
    {
        // Percorre as linhas da imagem
        for (int y = 0; y < linhas; ++y)
        {
            if (pertenceIntervalo(x, y))
            {
                // soma os pixels de uma mascara 3x3
                for (int i = x - limiteMascara(); i <= x + limiteMascara(); ++i)
                {
                    for (int j = y - limiteMascara(); j <= y + limiteMascara(); ++j)
                    {
                        // se indice da mascara é maior que o tamanho das colunas
                        // sai do loop
                        if (i == colunas)
                            break;

                        // se indice da mascara é maior que o tamanho das linhas
                        // sai do loop
                        if (j == linhas)
                            break;

                        soma += pixels[convertePosXY(i, j)];
                    }
                }

                // calcula a media
                int media = soma / (tamanhoMascara * tamanhoMascara);

                // preenche o pixel na imagem de
                pixels[convertePosXY(x, y)] = media;

                // reseta a soma (para calcular os outros pixels)
                soma = 0;
            }
        }
    }

    // Preenche o resultado
    resultado->largura = entrada->largura;
    resultado->altura = entrada->altura;
    resultado->pixels = pixels;

    // Salva o resultado em uma imagem
    stbi_write_png("resultado.png", linhas, colunas, 1, resultado->pixels, linhas);
}

bool Algoritmo::salOuPimenta(int x, int y)
{
    return pixels[convertePosXY(x, y)] == corSal || pixels[convertePosXY(x, y)] == corPimenta;
}

int Algoritmo::convertePosXY(int x, int y)
{
    return linhas * x + y;
}

bool Algoritmo::pertenceIntervalo(int x, int y)
{
    return pixels[convertePosXY(x, y)] >= corMin && pixels[convertePosXY(x, y)] <= corMax;
}

int Algoritmo::limiteMascara()
{
    return (tamMasc - 1) / 2;
}


