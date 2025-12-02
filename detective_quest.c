/**
 * DETECTIVE QUEST - Implementação das Funções
 * Sistema de exploração, coleta de pistas e dedução lógica
 */

#include "detective_quest.h"

// ============ IMPLEMENTAÇÃO: SALAS (ÁRVORE BINÁRIA) ============

/**
 * criarSala() - Cria dinamicamente um novo cômodo
 * Aloca memória e inicializa um nó de sala com seu nome e pista associada
 */
NoSala *criarSala(const char *nome, const char *pista) {
    NoSala *novaSala = (NoSala *)malloc(sizeof(NoSala));
    if (novaSala == NULL) {
        fprintf(stderr, "Erro ao alocar memória para sala!\n");
        return NULL;
    }
    
    strncpy(novaSala->nome, nome, SALA_LEN - 1);
    novaSala->nome[SALA_LEN - 1] = '\0';
    
    strncpy(novaSala->pista, pista, PISTA_LEN - 1);
    novaSala->pista[PISTA_LEN - 1] = '\0';
    
    novaSala->esquerda = NULL;
    novaSala->direita = NULL;
    
    return novaSala;
}

/**
 * construirMansao() - Monta a árvore binária fixa da mansão
 * A mansão é definida manualmente com uma estrutura pré-determinada
 */
NoSala *construirMansao(void) {
    // Criar raiz
    NoSala *raiz = criarSala("Saguao", "Porta principal arrombada - sinal de invasão");
    
    // Nível 1
    raiz->esquerda = criarSala("Escritorio", "Cofre aberto e documentos espalhados");
    raiz->direita = criarSala("Sala_Estar", "Taça de vinho vazia na mesa de centro");
    
    // Nível 2 - Esquerda
    raiz->esquerda->esquerda = criarSala("Biblioteca", "Livro de contabilidade com anotações suspeitas");
    raiz->esquerda->direita = criarSala("Quarto_Principal", "Joia valiosa encontrada embaixo da cama");
    
    // Nível 2 - Direita
    raiz->direita->esquerda = criarSala("Cozinha", "Faca sangrenta na pia da cozinha");
    raiz->direita->direita = criarSala("Jardim", "Pegadas de bota na lama próximo à janela");
    
    // Nível 3
    raiz->esquerda->esquerda->esquerda = criarSala("Arquivos", "Contrato rasgado com nome de um suspeito");
    raiz->esquerda->esquerda->direita = criarSala("Sala_Leitura", "Carta não enviada confessando um crime");
    
    return raiz;
}

/**
 * exibirSala() - Exibe informações da sala atual
 */
void exibirSala(NoSala *no) {
    if (no == NULL) return;
    
    printf("\n========================================\n");
    printf("SALA: %s\n", no->nome);
    printf("========================================\n");
    printf("PISTA ENCONTRADA: %s\n", no->pista);
    printf("========================================\n");
    printf("Opções:\n");
    printf("  [e] - Ir para esquerda\n");
    printf("  [d] - Ir para direita\n");
    printf("  [s] - Sair e fazer acusação\n");
    printf("========================================\n");
}

// ============ IMPLEMENTAÇÃO: BST DE PISTAS ============

/**
 * inserirPista() - Insere pista na BST de forma ordenada
 * Implementa inserção recursiva com verificação de duplicatas
 */
NoPista *inserirPista(NoPista *raiz, const char *pista) {
    if (raiz == NULL) {
        NoPista *noNovo = (NoPista *)malloc(sizeof(NoPista));
        if (noNovo == NULL) {
            fprintf(stderr, "Erro ao alocar memória para pista!\n");
            return NULL;
        }
        strncpy(noNovo->pista, pista, PISTA_LEN - 1);
        noNovo->pista[PISTA_LEN - 1] = '\0';
        noNovo->esquerda = NULL;
        noNovo->direita = NULL;
        return noNovo;
    }
    
    int comparacao = strcmp(pista, raiz->pista);
    
    if (comparacao < 0) {
        // Ir para esquerda
        raiz->esquerda = inserirPista(raiz->esquerda, pista);
    } else if (comparacao > 0) {
        // Ir para direita
        raiz->direita = inserirPista(raiz->direita, pista);
    }
    // Se comparacao == 0, não insere duplicata
    
    return raiz;
}

/**
 * adicionarPista() - Wrapper para inserirPista
 */
NoPista *adicionarPista(NoPista *raiz, const char *pista) {
    return inserirPista(raiz, pista);
}

/**
 * buscarPista() - Verifica se uma pista já foi coletada
 */
int buscarPista(NoPista *raiz, const char *pista) {
    if (raiz == NULL) return 0;
    
    int comparacao = strcmp(pista, raiz->pista);
    
    if (comparacao == 0) return 1;
    if (comparacao < 0) return buscarPista(raiz->esquerda, pista);
    return buscarPista(raiz->direita, pista);
}

/**
 * listarPistas() - Exibe pistas em ordem (inorder traversal)
 * Demonstra propriedade BST: inorder produz ordem alfabética
 */
void listarPistas(NoPista *raiz) {
    if (raiz == NULL) return;
    
    listarPistas(raiz->esquerda);
    printf("  • %s\n", raiz->pista);
    listarPistas(raiz->direita);
}

/**
 * contarPistas() - Conta total de pistas (recursivo)
 */
int contarPistas(NoPista *raiz) {
    if (raiz == NULL) return 0;
    return 1 + contarPistas(raiz->esquerda) + contarPistas(raiz->direita);
}

/**
 * liberarPistas() - Libera memória da BST (postorder)
 */
void liberarPistas(NoPista *raiz) {
    if (raiz == NULL) return;
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

// ============ IMPLEMENTAÇÃO: TABELA HASH ============

/**
 * inicializarHash() - Cria e inicializa a tabela hash
 */
EntradaHash *inicializarHash(void) {
    EntradaHash *tabela = (EntradaHash *)malloc(sizeof(EntradaHash) * HASH_SIZE);
    if (tabela == NULL) {
        fprintf(stderr, "Erro ao alocar memória para tabela hash!\n");
        return NULL;
    }
    
    for (int i = 0; i < HASH_SIZE; i++) {
        tabela[i].ocupada = 0;
        tabela[i].pista[0] = '\0';
        tabela[i].suspeito[0] = '\0';
    }
    
    return tabela;
}

/**
 * hashFunction() - Função hash simples: soma dos ASCII dos caracteres
 * Módulo HASH_SIZE para obter índice válido
 */
int hashFunction(const char *chave) {
    int hash = 0;
    for (int i = 0; chave[i] != '\0'; i++) {
        hash += (unsigned char)chave[i];
    }
    return hash % HASH_SIZE;
}

/**
 * inserirNaHash() - Insere pista -> suspeito com sondagem linear
 * Tratamento de colisão: se posição ocupada, tenta próxima
 */
void inserirNaHash(EntradaHash *tabela, const char *pista, const char *suspeito) {
    if (tabela == NULL) return;
    
    int indice = hashFunction(pista);
    int tentativas = 0;
    
    // Sondagem linear para tratar colisões
    while (tabela[indice].ocupada && tentativas < HASH_SIZE) {
        // Verificar se chave já existe (evitar duplicata)
        if (strcmp(tabela[indice].pista, pista) == 0) {
            return;  // Já existe, não insere
        }
        indice = (indice + 1) % HASH_SIZE;
        tentativas++;
    }
    
    if (tentativas < HASH_SIZE) {
        strncpy(tabela[indice].pista, pista, PISTA_LEN - 1);
        tabela[indice].pista[PISTA_LEN - 1] = '\0';
        
        strncpy(tabela[indice].suspeito, suspeito, SUSPEITO_LEN - 1);
        tabela[indice].suspeito[SUSPEITO_LEN - 1] = '\0';
        
        tabela[indice].ocupada = 1;
    }
}

/**
 * encontrarSuspeito() - Busca suspeito por pista na hash
 * Usa sondagem linear para localizar a entrada
 */
const char *encontrarSuspeito(EntradaHash *tabela, const char *pista) {
    if (tabela == NULL) return "DESCONHECIDO";
    
    int indice = hashFunction(pista);
    int tentativas = 0;
    
    while (tentativas < HASH_SIZE) {
        if (tabela[indice].ocupada && strcmp(tabela[indice].pista, pista) == 0) {
            return tabela[indice].suspeito;
        }
        indice = (indice + 1) % HASH_SIZE;
        tentativas++;
    }
    
    return "DESCONHECIDO";
}

/**
 * contarPistasPorSuspeito() - Conta pistas relacionadas a um suspeito
 * Recursivo: percorre toda BST comparando suspeitos via hash
 */
int contarPistasPorSuspeito(EntradaHash *tabela, NoPista *pistas, const char *suspeito) {
    if (pistas == NULL) return 0;
    
    int count = 0;
    
    // Verifica pista atual
    const char *susObject = encontrarSuspeito(tabela, pistas->pista);
    if (strcmp(susObject, suspeito) == 0) {
        count = 1;
    }
    
    // Recursão para subtárvores
    count += contarPistasPorSuspeito(tabela, pistas->esquerda, suspeito);
    count += contarPistasPorSuspeito(tabela, pistas->direita, suspeito);
    
    return count;
}

/**
 * liberarHash() - Libera memória da tabela hash
 */
void liberarHash(EntradaHash *tabela) {
    if (tabela != NULL) {
        free(tabela);
    }
}

// ============ IMPLEMENTAÇÃO: EXPLORAÇÃO ============

/**
 * explorarSalas() - Exploração interativa da mansão
 * Navega a árvore binária com escolhas do jogador (e/d/s)
 * Coleta pistas automaticamente ao visitar salas
 */
int explorarSalas(NoSala *no, JogoDetectiveQuest *jogo) {
    if (no == NULL) {
        printf("\nVocê chegou ao final deste caminho. Não há mais salas!\n");
        return 1;  // Continua no jogo
    }
    
    exibirSala(no);
    
    // Adicionar pista se ainda não coletada
    if (!buscarPista(jogo->raizPistas, no->pista)) {
        printf("[NOVA PISTA COLETADA]\n");
        jogo->raizPistas = inserirPista(jogo->raizPistas, no->pista);
        jogo->totalPistas++;
    } else {
        printf("[Pista já coletada anteriormente]\n");
    }
    
    char opcao;
    printf("Sua escolha: ");
    scanf(" %c", &opcao);
    opcao = tolower(opcao);
    
    switch (opcao) {
        case 'e':
            if (no->esquerda != NULL) {
                printf("\n--- Você se move para a esquerda ---\n");
                return explorarSalas(no->esquerda, jogo);
            } else {
                printf("\nNão há caminho à esquerda!\n");
                return explorarSalas(no, jogo);
            }
        case 'd':
            if (no->direita != NULL) {
                printf("\n--- Você se move para a direita ---\n");
                return explorarSalas(no->direita, jogo);
            } else {
                printf("\nNão há caminho à direita!\n");
                return explorarSalas(no, jogo);
            }
        case 's':
            printf("\n--- Você sai da mansão para fazer sua acusação ---\n");
            return 0;  // Sai do jogo
        default:
            printf("Opção inválida! Tente novamente.\n");
            return explorarSalas(no, jogo);
    }
}

// ============ IMPLEMENTAÇÃO: JULGAMENTO FINAL ============

/**
 * verificarSuspeitoFinal() - Sistema de acusação final
 * Valida se há pistas suficientes (mínimo 2) para o suspeito acusado
 */
int verificarSuspeitoFinal(JogoDetectiveQuest *jogo, EntradaHash *tabela) {
    printf("\n========================================\n");
    printf("PHASE FINAL: ACUSAÇÃO\n");
    printf("========================================\n");
    
    printf("\nPistas coletadas durante exploração:\n");
    listarPistas(jogo->raizPistas);
    
    printf("\n========================================\n");
    printf("Suspeitos disponíveis:\n");
    printf("  1. Mordecai (assistente do dono)\n");
    printf("  2. Isabela (empregada da casa)\n");
    printf("  3. Victor (empresário rival)\n");
    printf("  4. Camila (esposa do dono)\n");
    printf("========================================\n");
    
    char suspeito[SUSPEITO_LEN];
    printf("\nEm quem você acusa? ");
    fgets(suspeito, SUSPEITO_LEN, stdin);
    
    // Remove quebra de linha
    size_t len = strlen(suspeito);
    if (len > 0 && suspeito[len - 1] == '\n') {
        suspeito[len - 1] = '\0';
    }
    
    // Contar pistas relacionadas ao suspeito
    int pistasSuspeito = contarPistasPorSuspeito(tabela, jogo->raizPistas, suspeito);
    
    int acertou = (pistasSuspeito >= 2);
    exibirResultadoFinal(acertou, suspeito, pistasSuspeito);
    
    return acertou;
}

/**
 * exibirResultadoFinal() - Exibe resultado da acusação
 */
void exibirResultadoFinal(int acertou, const char *suspeito, int pistasSuspeito) {
    printf("\n========================================\n");
    
    if (acertou) {
        printf("✓ ACUSAÇÃO CORRETA!\n");
        printf("========================================\n");
        printf("Você acusou: %s\n", suspeito);
        printf("Pistas encontradas: %d\n", pistasSuspeito);
        printf("\nCom as pistas suficientes, o culpado foi levado à justiça!\n");
        printf("PARABÉNS! Você resolveu o crime!\n");
    } else {
        printf("✗ ACUSAÇÃO FALHA!\n");
        printf("========================================\n");
        printf("Você acusou: %s\n", suspeito);
        printf("Pistas relacionadas: %d (necessário: 2 ou mais)\n", pistasSuspeito);
        printf("\nSem pistas suficientes, o culpado escapou...\n");
        printf("FALHA! Você não conseguiu resolver o crime.\n");
    }
    
    printf("========================================\n");
}

// ============ IMPLEMENTAÇÃO: INICIALIZAÇÃO E LIMPEZA ============

/**
 * inicializarJogo() - Prepara todas as estruturas do jogo
 */
JogoDetectiveQuest *inicializarJogo(void) {
    JogoDetectiveQuest *jogo = (JogoDetectiveQuest *)malloc(sizeof(JogoDetectiveQuest));
    if (jogo == NULL) {
        fprintf(stderr, "Erro ao alocar memória para jogo!\n");
        return NULL;
    }
    
    jogo->raizMansao = construirMansao();
    jogo->raizPistas = NULL;
    jogo->tabelaHash = inicializarHash();
    jogo->totalPistas = 0;
    
    return jogo;
}

/**
 * liberarJogo() - Libera toda memória alocada
 */
void liberarJogo(JogoDetectiveQuest *jogo) {
    if (jogo == NULL) return;
    
    // Liberar mansão recursivamente
    if (jogo->raizMansao != NULL) {
        // Função auxiliar necessária (implementada abaixo)
    }
    
    liberarPistas(jogo->raizPistas);
    liberarHash(jogo->tabelaHash);
    free(jogo);
}

/**
 * exibirMenu() - Menu de instrução do jogo
 */
void exibirMenu(void) {
    printf("\n");
    printf("╔════════════════════════════════════════╗\n");
    printf("║      DETECTIVE QUEST - ENIGMA STUDIOS ║\n");
    printf("║        O Mistério da Mansão Escura    ║\n");
    printf("╚════════════════════════════════════════╝\n");
    printf("\n");
    printf("BEM-VINDO, DETETIVE!\n");
    printf("\nUma morte misteriosa aconteceu na mansão do milionário.\n");
    printf("Quatro suspeitos estão sob investigação:\n");
    printf("  • Mordecai (assistente do dono) - tinha acesso total\n");
    printf("  • Isabela (empregada) - viu algo suspeito?\n");
    printf("  • Victor (empresário rival) - interessado na herança?\n");
    printf("  • Camila (esposa) - conhecia todos os segredos\n");
    printf("\n");
    printf("OBJETIVO:\n");
    printf("  1. Explore os cômodos da mansão\n");
    printf("  2. Colete pistas em cada sala (BST ordenada)\n");
    printf("  3. Relate pistas a suspeitos (Tabela Hash)\n");
    printf("  4. Acuse o culpado com pelo menos 2 pistas!\n");
    printf("\n");
    printf("CONTROLES:\n");
    printf("  [e] - Explorar sala à esquerda\n");
    printf("  [d] - Explorar sala à direita\n");
    printf("  [s] - Sair da mansão e fazer acusação\n");
    printf("\n");
    printf("Que comece a investigação!\n");
    printf("════════════════════════════════════════\n\n");
}

// ============ FUNÇÃO AUXILIAR: LIBERAR MANSÃO ============

/**
 * Função auxiliar para liberar recursivamente a árvore de salas
 */
void liberarSalas(NoSala *raiz) {
    if (raiz == NULL) return;
    liberarSalas(raiz->esquerda);
    liberarSalas(raiz->direita);
    free(raiz);
}
