#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

const int tam = 8;         // Tamanho do tabuleiro (8x8)
const int MINAS = 10;      // Quantidade total de minas no jogo
char visivel[tam][tam];    // Tabuleiro que o jogador vê (com 'X', números ou minas reveladas)
char real[tam][tam];       // Tabuleiro real com a posição das minas e números ocultos

void inicializarTabuleiros() {
    for (int l = 0; l < tam; l++) {
    for (int c = 0; c < tam; c++) {
            visivel[l][c] = 'X';  // Inicialmente, todas as casas estão ocultas para o jogador
            real[l][c] = '0';     // Começamos com todas as casas sem minas
        }
    }
}

void colocarMinas() { // Posiciona minas aleatoriamente no tabuleiro real
    int colocadas = 0; // Contador de minas já posicionadas

    while (colocadas < MINAS) {
        int lin = rand() % tam; // Sorteia uma linha aleatória
        int col = rand() % tam; // Sorteia uma coluna aleatória

        // Verifica se já há uma mina nessa posição
        if (real[lin][col] != '*') {
            real[lin][col] = '*'; // Coloca uma mina
            colocadas++;          // Incrementa o número de minas colocadas
        }
    }
}

// Calcula a quantidade de minas ao redor de cada casa 
void calcularNumeros() {
    for (int l = 0; l < tam; l++) {
        for (int c = 0; c < tam; c++) {

            if (real[l][c] == '*') continue; // Se a casa atual é uma mina, ignoramos

            int minas = 0;

            for (int dL = -1; dL <= 1; dL++) { // Percorre todas as 8 casas vizinhas
                for (int dC = -1; dC <= 1; dC++) {
                    int vL = l + dL;
                    int vC = c + dC;

                    // Verifica se a posição vizinha está dentro dos limites e contém uma mina
                    if (vL >= 0 && vL < tam && vC >= 0 && vC < tam && real[vL][vC] == '*') {
                        minas++;
                    }
                }
            }
            real[l][c] = minas + '0'; // Armazena o número de minas como caractere 
        }
    }
}
void imprimirTabuleiro(bool mostrarMinas = false) {
    cout << "    ";
    for (int c = 0; c < tam; c++) cout << c << "   ";
    cout << endl;

    for (int i = 0; i < tam; i++) {
        cout << i << " | ";
        for (int j = 0; j < tam; j++) {

            if (visivel[i][j] != 'X') {
                cout << visivel[i][j]; // Mostra casas reveladas
            } else {
                if (mostrarMinas && real[i][j] == '*')
                    cout << "*";       // Revela minas se o jogo terminou
                else
                    cout << "X";       // Casa ainda oculta
            }

            cout << " | ";
        }
        cout << endl;
        cout << "  ";
        for (int c = 0; c < tam; c++) cout << "----";
        cout << endl;
    }
}

// Revela a casa escolhida pelo jogador e propaga se for '0'
void revelarCasa(int l, int c) {
    if (l < 0 || l >= tam || c < 0 || c >= tam) // Verifica se a posição está dentro do tabuleiro
        return;

    if (visivel[l][c] != 'X') // Evita revelar casas já abertas
        return;
        
    visivel[l][c] = real[l][c]; // Revela a casa atual

     if (real[l][c] == '0') { // Se a casa for vazia ('0'), revela automaticamente os vizinhos
        for (int dl = -1; dl <= 1; dl++) {
            for (int dc = -1; dc <= 1; dc++) {
                if (dl != 0 || dc != 0)
                    revelarCasa(l + dl, c + dc);
            }
        }
    }
}

bool verificaVitoria() { // Verifica se o jogador ganhou 
    for (int l = 0; l < tam; l++) {
        for (int c = 0; c < tam; c++) {
            // Se ainda houver casa oculta que não é mina, o jogo não terminou
            if (visivel[l][c] == 'X' && real[l][c] != '*')
                return false;
        }
    }
    return true;
}

int main() {
    srand(time(0)); // Inicializa o gerador de números aleatórios (usado nas minas)
    inicializarTabuleiros();// Funções do código
    colocarMinas();
    calcularNumeros();

    bool gameOver = false;
    int linha, coluna;

    while (!gameOver) { //loop
        imprimirTabuleiro();

        cout << "Escolha uma casa (linha e coluna): ";
        cin >> linha >> coluna;

        // Verifica se a entrada é válida
        if (linha < 0 || linha >= tam || coluna < 0 || coluna >= tam) {
            cout << "Linha ou coluna inválida! Tente novamente.\n";
            continue;
        }
        if (real[linha][coluna] == '*') { // Jogador clicou em uma mina
            cout << "Você perdeu!\n";

            for (int l = 0; l < tam; l++) { // Revela todas as minas no tabuleiro
                for (int c = 0; c < tam; c++) {
                    if (real[l][c] == '*')
                        visivel[l][c] = '*';
                }
            }
            gameOver = true; // Encerra o jogo
        } else {
            revelarCasa(linha, coluna); // Revela a casa e vizinhos

            if (verificaVitoria()) { // Verifica condição de vitória
                cout << "Você venceu!\n";
                gameOver = true;
            }
        }
    }
    imprimirTabuleiro();// Exibe o tabuleiro completo no final
    cout << "FIM DE JOGO!\n";

    return 0;
}
