/**
 * DETECTIVE QUEST - Header File
 * Sistema completo de exploração de mansão com pistas, suspeitos e acusação lógica
 */

#ifndef DETECTIVE_QUEST_H
#define DETECTIVE_QUEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PISTA_LEN 100
#define SUSPEITO_LEN 50
#define SALA_LEN 50
#define MAX_PISTAS 100
#define HASH_SIZE 50

// ============ ESTRUTURAS DE DADOS ============

/**
 * Nó da árvore binária de pistas (BST)
 * Armazena pistas em ordem alfabética
 */
typedef struct NoPista {
    char pista[PISTA_LEN];
    struct NoPista *esquerda;
    struct NoPista *direita;
} NoPista;

/**
 * Nó da tabela hash para associação pista -> suspeito
 * Permite busca eficiente de suspeitos por pista
 */
typedef struct EntradaHash {
    char pista[PISTA_LEN];
    char suspeito[SUSPEITO_LEN];
    int ocupada;  // Flag para verificar se a posição está ocupada
} EntradaHash;

/**
 * Nó da árvore binária de salas (estrutura da mansão)
 * Cada sala contém um nome, pista e referências para salas adjacentes
 */
typedef struct NoSala {
    char nome[SALA_LEN];
    char pista[PISTA_LEN];
    struct NoSala *esquerda;   // Sala à esquerda
    struct NoSala *direita;    // Sala à direita
} NoSala;

/**
 * Estrutura principal do jogo
 * Contém a mansão, pistas coletadas e tabela de suspeitos
 */
typedef struct {
    NoSala *raizMansao;          // Raiz da árvore de salas
    NoPista *raizPistas;         // Raiz da BST de pistas coletadas
    EntradaHash *tabelaHash;     // Tabela hash pista -> suspeito
    int totalPistas;             // Contador de pistas coletadas
} JogoDetectiveQuest;

// ============ FUNÇÕES DE GERENCIAMENTO DE SALAS ============

/**
 * criarSala() - Cria dinamicamente um novo cômodo na mansão
 * 
 * Aloca memória para um novo nó de sala, inicializa seu nome,
 * associa uma pista específica ao cômodo e configura seus ponteiros
 * para esquerda e direita como NULL.
 *
 * @param nome: Nome identificador do cômodo
 * @param pista: Pista estática encontrada nesse cômodo
 * @return: Ponteiro para o novo nó de sala criado
 */
NoSala *criarSala(const char *nome, const char *pista);

/**
 * construirMansao() - Monta a estrutura fixa da mansão
 * Cria a árvore binária com salas pré-definidas
 * 
 * @return: Ponteiro para a raiz da árvore de salas
 */
NoSala *construirMansao(void);

// ============ FUNÇÕES DE EXPLORAÇÃO ============

/**
 * explorarSalas() - Navega pela árvore e ativa o sistema de pistas
 *
 * Implementa uma exploração interativa do jogo. O jogador navega
 * pelos cômodos escolhendo ir para esquerda (e), direita (d) ou sair (s).
 * Ao visitar uma sala, a pista é exibida e coletada automaticamente.
 * A função usa recursividade para navegar pela árvore binária.
 *
 * @param no: Nó atual da árvore (sala atual)
 * @param jogo: Estrutura do jogo para armazenar pistas coletadas
 * @return: 1 se continuando, 0 se o jogador saiu
 */
int explorarSalas(NoSala *no, JogoDetectiveQuest *jogo);

/**
 * exibirSala() - Exibe informações da sala atual e sua pista
 */
void exibirSala(NoSala *no);

// ============ FUNÇÕES DE BST DE PISTAS ============

/**
 * inserirPista() / adicionarPista() - Insere a pista coletada na árvore de pistas
 *
 * Implementa a inserção em uma Árvore Binária de Busca (BST).
 * A pista é inserida de forma ordenada (alfabeticamente).
 * Se a pista já existe, não insere duplicata.
 * Usa recursividade para encontrar a posição correta.
 *
 * @param raiz: Ponteiro para a raiz da BST de pistas
 * @param pista: Texto da pista a ser inserida
 * @return: Ponteiro para a raiz atualizada da BST
 */
NoPista *inserirPista(NoPista *raiz, const char *pista);

/**
 * adicionarPista() - Wrapper para inserirPista, nome alternativo
 */
NoPista *adicionarPista(NoPista *raiz, const char *pista);

/**
 * buscarPista() - Verifica se uma pista já foi coletada
 */
int buscarPista(NoPista *raiz, const char *pista);

/**
 * listarPistas() - Exibe todas as pistas coletadas em ordem alfabética (inorder)
 */
void listarPistas(NoPista *raiz);

/**
 * contarPistas() - Conta o número total de pistas em uma árvore
 */
int contarPistas(NoPista *raiz);

/**
 * liberarPistas() - Libera memória da BST de pistas
 */
void liberarPistas(NoPista *raiz);

// ============ FUNÇÕES DE TABELA HASH ============

/**
 * inicializarHash() - Inicializa a tabela hash
 */
EntradaHash *inicializarHash(void);

/**
 * hashFunction() - Função de hash simples (soma dos caracteres módulo HASH_SIZE)
 */
int hashFunction(const char *chave);

/**
 * inserirNaHash() - Insere associação pista/suspeito na tabela hash
 *
 * Implementa tratamento de colisão por sondagem linear.
 * Armazena a associação pista -> suspeito para consulta posterior.
 * Usa hash com sondagem linear para resolver colisões.
 *
 * @param tabela: Ponteiro para a tabela hash
 * @param pista: Chave (pista encontrada)
 * @param suspeito: Valor (suspeito associado)
 */
void inserirNaHash(EntradaHash *tabela, const char *pista, const char *suspeito);

/**
 * encontrarSuspeito() - Consulta o suspeito correspondente a uma pista
 *
 * Realiza busca na tabela hash usando a função de hash e sondagem linear.
 * Retorna o suspeito associado a uma pista ou NULL se não encontrado.
 * Implementa sondagem linear para resolver colisões.
 *
 * @param tabela: Ponteiro para a tabela hash
 * @param pista: Chave (pista para buscar)
 * @return: String contendo o nome do suspeito ou "DESCONHECIDO"
 */
const char *encontrarSuspeito(EntradaHash *tabela, const char *pista);

/**
 * contarPistasPorSuspeito() - Conta quantas pistas apontam para um suspeito
 */
int contarPistasPorSuspeito(EntradaHash *tabela, NoPista *pistas, const char *suspeito);

/**
 * liberarHash() - Libera memória da tabela hash
 */
void liberarHash(EntradaHash *tabela);

// ============ FUNÇÕES DE JULGAMENTO FINAL ============

/**
 * verificarSuspeitoFinal() - Conduz à fase de julgamento final
 *
 * Solicita ao jogador a acusação de um suspeito.
 * Verifica se há pelo menos 2 pistas apontando para o suspeito acusado.
 * Exibe mensagem de vitória ou derrota conforme o resultado.
 * Realiza contagem recursiva de pistas relacionadas ao suspeito acusado.
 *
 * @param jogo: Estrutura do jogo com pistas e hash
 * @param tabela: Tabela hash para consultas
 * @return: 1 se acusação correta, 0 caso contrário
 */
int verificarSuspeitoFinal(JogoDetectiveQuest *jogo, EntradaHash *tabela);

/**
 * exibirResultadoFinal() - Exibe o resultado da acusação
 */
void exibirResultadoFinal(int acertou, const char *suspeito, int pistasSuspeito);

// ============ FUNÇÕES AUXILIARES ============

/**
 * inicializarJogo() - Inicializa todas as estruturas do jogo
 */
JogoDetectiveQuest *inicializarJogo(void);

/**
 * liberarJogo() - Libera toda a memória alocada
 */
void liberarJogo(JogoDetectiveQuest *jogo);

/**
 * exibirMenu() - Exibe o menu de instrução
 */
void exibirMenu(void);

#endif // DETECTIVE_QUEST_H
