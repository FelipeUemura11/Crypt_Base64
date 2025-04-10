#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void bitsByteSeparados(unsigned char byte1, unsigned char byte2, unsigned char byte3, int bytes[4][6]) {
    int index = 0; // perorre o array de bits
    int i, j;
    unsigned char bytes_orig[3] = {byte1, byte2, byte3}; // array de bytes originais
    int bits[24]; // armazena os bits de cada byte 3x8 = 24

    // Converte os cada bytes em arrya bits
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 8; j++) {
            bits[index++] = (bytes_orig[i] >> (7-j)) & 1;
        }
    }

    // Reorganiza os bits em grupos de 6
    index = 0;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 6; j++) {
            bytes[i][j] = bits[index++];
        }
    }
}

int calcularSomaByte(int bits[6]) {
    int soma = 0;
    for (int i = 0; i < 6; i++) {
        soma = (soma << 1) | bits[i];
    }
    return soma;
}

char* codificarBase64(const char* texto) {
    int len = strlen(texto);
    // Calcula o tamanho necessário para o resultado (4 caracteres para cada 3 bytes + padding)
    int resultado_len = ((len + 2) / 3) * 4 + 1;  // +1 para o caracter nulo
    char* resultado = (char*)malloc(resultado_len);
    if (!resultado) {
        printf("Erro: Falha na alocação de memória\n");
        return NULL;
    }
    
    int pos = 0;
    char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    
    // Processa o texto em grupos de 3 bytes
    for(int i = 0; i < len; i += 3) {
        int bytes[4][6]; // matriz 4x6(4 grupos de 6 bits)
        unsigned char byte1 = texto[i];
        unsigned char byte2 = (i + 1 < len) ? texto[i + 1] : 0; // byte = 0 caso o grupo tenha numero impar de bytes
        unsigned char byte3 = (i + 2 < len) ? texto[i + 2] : 0; // byte = 0 caso o grupo tenha numero impar de bytes
        
        bitsByteSeparados(byte1, byte2, byte3, bytes);
        
        // Converte cada 4 grupos de 6 bits em um caractere Base64
        for(int j = 0; j < 4; j++) {
            // caso o grupo tenha - que 3 bytes de entrada - que 4 bytes de saida
            if (j == 3 && i + 2 >= len) {
                resultado[pos++] = '=';
            } else if (j == 2 && i + 1 >= len) {
                resultado[pos++] = '=';
            } else {
                int valor = calcularSomaByte(bytes[j]);
                resultado[pos++] = base64[valor];
            }
        }
    }
    resultado[pos] = '\0';
    return resultado;
}

int main() {
    char* texto = NULL;
    size_t tamanho = 0;
    
    printf("Digite o texto para codificar em Base64: ");
    // Usa getline para ler texto de qualquer tamanho
    int bytes_lidos = getline(&texto, &tamanho, stdin); // pega o tamanho do texto
    if (bytes_lidos > 0 && texto[bytes_lidos-1] == '\n') {
        texto[bytes_lidos-1] = '\0';  // Remove o \n do final
    }
    
    char* resultado = codificarBase64(texto);
    if (resultado) {
        printf("\nTexto codificado em Base64: %s\n", resultado);
        free(resultado);
    }
    
    free(texto);
    return 0;
}
