/**
 * @file No.h
 * @brief Declaração da classe No usada para representar os nós de uma árvore B.
 */

#ifndef NO_H
#define NO_H

#include "Item.h"
#include <vector>
#include <ostream>

/**
 * @class No
 * @brief Representa um nó de uma árvore B.
 */
class No {
public:
    /**
     * @brief Lista de chaves armazenadas no nó.
     */
    std::vector<Item> chaves;

    /**
     * @brief Ponteiros para os filhos do nó.
     */
    std::vector<No*> filhos;

    /**
     * @brief Indica se o nó é uma folha.
     */
    bool folha;

    /**
     * @brief Construtor para criar um nó.
     * @param ehFolha Indica se o nó criado será uma folha.
     */
    No(bool ehFolha);

    /**
     * @brief Percorre a árvore e gera a saida no formato Graphviz.
     * @param out Stream de saída para escrever.
     * @param level Nível atual do nó na árvore.
     */
    void percorreB(std::ostream& out, int level);
};

#endif // NO_H
