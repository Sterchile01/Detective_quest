/**
 * DETECTIVE QUEST - Programa Principal
 * Integra todas as estruturas de dados para o jogo completo
 */

#include "detective_quest.h"

/**
 * populaTabelaHash() - Define as associações pista -> suspeito
 * Esta função pré-popula a tabela hash com as relações do jogo
 */
void populaTabelaHash(EntradaHash *tabela) {
    // Pistas apontando para Mordecai
    inserirNaHash(tabela, "Porta principal arrombada - sinal de invasão", "Mordecai");
    inserirNaHash(tabela, "Cofre aberto e documentos espalhados", "Mordecai");
    inserirNaHash(tabela, "Contrato rasgado com nome de um suspeito", "Mordecai");
    
    // Pistas apontando para Isabela
    inserirNaHash(tabela, "Faca sangrenta na pia da cozinha", "Isabela");
    inserirNaHash(tabela, "Pegadas de bota na lama próximo à janela", "Isabela");
    
    // Pistas apontando para Victor
    inserirNaHash(tabela, "Taça de vinho vazia na mesa de centro", "Victor");
    inserirNaHash(tabela, "Joia valiosa encontrada embaixo da cama", "Victor");
    
    // Pistas apontando para Camila
    inserirNaHash(tabela, "Livro de contabilidade com anotações suspeitas", "Camila");
    inserirNaHash(tabela, "Carta não enviada confessando um crime", "Camila");
}

/**
 * main() - Função principal: coordena todo o fluxo do jogo
 */
int main(void) {
    // Limpar buffer
    setbuf(stdout, NULL);
    
    // Inicializar jogo
    JogoDetectiveQuest *jogo = inicializarJogo();
    if (jogo == NULL) {
        fprintf(stderr, "Erro ao inicializar jogo!\n");
        return 1;
    }
    
    // Popular tabela hash com associações pista -> suspeito
    populaTabelaHash(jogo->tabelaHash);
    
    // Exibir menu e instruções
    exibirMenu();
    
    printf("Pressione ENTER para começar...");
    getchar();
    
    // Iniciar exploração da mansão
    printf("\n--- INICIANDO EXPLORAÇÃO ---\n");
    printf("Você entra na mansão escura...\n");
    
    explorarSalas(jogo->raizMansao, jogo);
    
    // Fase final: acusação
    verificarSuspeitoFinal(jogo, jogo->tabelaHash);
    
    // Liberar memória
    liberarJogo(jogo);
    
    printf("\nObrigado por jogar Detective Quest!\n");
    
    return 0;
}
