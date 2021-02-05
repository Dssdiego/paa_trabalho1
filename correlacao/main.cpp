// Implementações da Biblioteca de Leitura/Escrita de Imagens
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

// Definições Internas
#define COMPONENTE_IMAGEM 1 // Define que o componente a ser utilizado é o grayscale (imagem em tons de cinza)
#define COR_SAL 255
#define COR_PIMENTA 0

// Imports
#include "stb_image.h"
#include "stb_image_write.h"
#include <iostream>

using namespace std;

// Inicialização de Variáveis
int linhas, colunas, cpp; // cpp >> componentes por pixel
int soma = 0;
unsigned char *pixels = nullptr;

// Método de apoio que converte o índice do array em posição (x,y)
// Necessário neste caso pois a leitura da imagem é feita em um array (não em uma matriz),
// portanto este método faz a conversão de um ponto (x,y) da matriz para o índice de um
// array
int convertePosXY(int x, int y)
{
    return linhas * x + y;
}

// Booleano que verifica se um pixel é sal (255) ou pimenta (0)
bool salOuPimenta(int x, int y)
{
    return pixels[convertePosXY(x, y)] == COR_SAL || pixels[convertePosXY(x, y)] == COR_PIMENTA;
}

// Aplica o algoritmo de correção, porém somente para os valores de cor 255 (sal) e 0 (pimenta)
void algo_sal_pimenta(int tamanhoMascara)
{
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
                for (int i = x - 1; i <= x + 1; ++i)
                {
                    for (int j = y - 1; j <= y + 1; ++j)
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
}

// Aplica o algoritmo com o tamanho da máscara especificado
void algo_masc_geral(int tamanhoMascara)
{
    // Percorre as colunas da imagem
    for (int x = 0; x < colunas; ++x)
    {
        // Percorre as linhas da imagem
        for (int y = 0; y < linhas; ++y)
        {
            // soma os pixels de uma mascara 3x3
            for (int i = x - 1; i <= x + 1; ++i)
            {
                for (int j = y - 1; j <= y + 1; ++j)
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

int main()
{
    // Faz a leitura dos pixels da imagem
    pixels = stbi_load("tigre_sem_sal.png", &linhas, &colunas, &cpp, COMPONENTE_IMAGEM); // 1 -> imagem em grayscale

    // Aplica o algoritmo com o tamanho da máscara selecionado
//    algo_sal_pimenta(3);
    algo_masc_geral(3);

    // Cria a imagem de resultado
    stbi_write_png("resultado.png", linhas, colunas, COMPONENTE_IMAGEM, pixels, linhas * cpp);

    // Limpa a memória
    stbi_image_free(pixels);

    // Finaliza o programa com código de sucesso
    return 0;
}
