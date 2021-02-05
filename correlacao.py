import numpy as np
import math
import cv2

# Le a imagem
img = cv2.imread('noise_2.png', cv2.IMREAD_GRAYSCALE)

# Inicia a variavel de resultado com os valores da imagem original
saida = img

# Armazena o numero de linhas e colunas
nLinhas = len(img)
nColunas = len(img[0])
# print(nLinhas)
# print(nColunas)

# Cria a mascara
masc = np.zeros((3,3))
masc[0][0] = 1
masc[0][1] = 2
masc[0][2] = 3
masc[1][0] = 4
masc[1][1] = 5
masc[1][2] = 6
masc[2][0] = 7
masc[2][1] = 8
masc[2][2] = 9

# TODO: Para sempre ter um elemento central, aceitar SOMENTE mascaras impares
idxCentro = math.floor(masc.shape[0]/2)
# print(centroMasc)
# print(centroMasc[0]/2)

# Calcula a media da matriz

masc[idxCentro][idxCentro] = 999

print(masc)
print(masc[-1][-1])

# Imprime o tamanho da imagem
# print(img.shape)

# Percorre a imagem
# for i in range(nLinhas):                # Percorre as linhas
#     for j in range(nColunas):           # Percorre as colunas
#         saida[i][j] = 255
        # print(img[i][j])

# Mostra a imagem de resultado
# cv2.imshow('noise', img) 
# cv2.imshow('saida', saida) 
# cv2.waitKey(0)
# cv2.destroyAllWindows()