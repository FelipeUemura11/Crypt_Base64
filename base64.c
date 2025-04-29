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

// Função para encontrar o índice de um caractere na tabela Base64
int encontrarIndiceBase64(char c) {
    char base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    for (int i = 0; i < 64; i++) {
        if (base64[i] == c) {
            return i;
        }
    }
    return -1; // Caractere não encontrado
}

// Função para converter 6 bits em um byte
unsigned char bitsParaByte(int bits[8]) {
    unsigned char resultado = 0;
    for (int i = 0; i < 8; i++) {
        resultado = (resultado << 1) | bits[i];
    }
    return resultado;
}

// Função para decodificar Base64
char* decodificarBase64(const char* texto) {
    int len = strlen(texto);
    // Verifica se o comprimento é válido (múltiplo de 4)
    if (len % 4 != 0) {
        printf("Erro: Texto Base64 inválido (comprimento não é múltiplo de 4)\n");
        return NULL;
    }
    
    // Calcula o número de caracteres de preenchimento (=)
    int padding = 0;
    if (len > 0) {
        if (texto[len-1] == '=') padding++; // verifica se o ultimo caracter é um padding
        if (texto[len-2] == '=') padding++; // verifica se o penultimo caracter é um padding
    }
    
    // Calcula o tamanho do resultado decodificado
    int resultado_len = (len / 4) * 3 - padding + 1; // +1 para o caractere nulo
    char* resultado = (char*)malloc(resultado_len);
    if (!resultado) {
        printf("Erro: Falha na alocação de memória\n");
        return NULL;
    }
    
    int pos = 0;
    
    // Processa o texto
    for (int i = 0; i < len; i += 4) {
        // Obtém os valores dos 4 caracteres Base64
        int valores[4];
        for (int j = 0; j < 4; j++) {
            if (texto[i+j] == '=') {
                valores[j] = 0; // Padding
            } else {
                valores[j] = encontrarIndiceBase64(texto[i+j]);
                if (valores[j] == -1) {
                    printf("Erro: Caractere inválido na string Base64\n");
                    free(resultado);
                    return NULL;
                }
            }
        }
        
        // Converte os valores de 6 bits em 3 bytes
        int bits[24];
        int bit_pos = 0;
        
        // Extrai os bits de cada caractere Base64
        for (int j = 0; j < 4; j++) {
            for (int k = 5; k >= 0; k--) {
                bits[bit_pos++] = (valores[j] >> k) & 1;
            }
        }
        
        // Converte os bits em bytes
        for (int j = 0; j < 3; j++) {
            int byte_bits[8];
            for (int k = 0; k < 8; k++) {
                byte_bits[k] = bits[j*8 + k];
            }
            resultado[pos++] = bitsParaByte(byte_bits);
        }
    }
    
    resultado[pos - padding] = '\0';
    return resultado;
}

int main() {
    size_t tamanho = 0;
    char* texto = NULL;
    char* nome_arquivo = NULL;
    int opc = -1;

    while(opc != 0){
        printf("\n========= MENU =========\n");
        printf("1 - Codificar Base64\n");
        printf("2 - Decodificar Base64\n");
        printf("3 - Transmissao p/ JSON\n");
        printf("0 - Sair\n");
        printf("========= MENU =========\n");

        printf("Escolha uma opcao: ");
        scanf("%i", &opc);
        getchar();

        switch(opc){
            case 1:
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
                break;
            case 2:
                printf("Digite o texto para decodificar em Base64: ");
                // Usa getline para ler texto de qualquer tamanho
                bytes_lidos = getline(&texto, &tamanho, stdin);
                if (bytes_lidos > 0 && texto[bytes_lidos-1] == '\n') {
                    texto[bytes_lidos-1] = '\0';
                }
                
                resultado = decodificarBase64(texto);
                if (resultado) {
                    printf("\nTexto decodificado de Base64: %s\n", resultado);
                    free(resultado);
                }
                break;
            case 3: 
                printf("Digite o nome do arquivo para transmissao p/ JSON: ");
                scanf("%s", nome_arquivo);

                // transmissaoJSON(nome_arquivo);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida\n");
                break;
        }
            
    }
    
    free(texto);
    return 0;
}
