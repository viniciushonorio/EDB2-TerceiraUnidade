#ifndef ARVOREB_H
#define ARVOREB_H

#include "No.h"
#include <queue>
#include <fstream>
#include <string>
#include <vector>

/**
 * @class ArvoreB
 * @brief Representa uma árvore B.
 */
class ArvoreB {
private:
    /**
     * @brief Grau mínimo da árvore B.
     * Determina o número mínimo de filhos que um nó pode ter.
     */
    int grauMinimo;

    /**
     * @brief Raiz da árvore B.
     */
    No* raiz;

    /**
     * @brief Divide o filho de um nó em dois.
     * @param no Ponteiro para o nó onde a divisão ocorrerá.
     * @param indice Índice do filho que será dividido.
     */
    void dividirFilho(No* no, int indice);

    /**
     * @brief Insere um item no nó, caso este não esteja cheio.
     * @param no Ponteiro para o nó onde a inserção ocorrerá.
     * @param chave Item a ser inserido no nó.
     */
    void inserirNaoCheio(No* no, const Item& chave);

    /**
     * @brief Busca um item na árvore.
     * @param no Ponteiro para o nó raiz.
     * @param id ID do item a ser buscado.
     * @return Ponteiro para o item encontrado ou nullptr caso não encontrado.
     */
    Item* buscar(No* no, int id);

    /**
     * @brief Remove um item de um nó específico.
     * @param no Ponteiro para o nó onde o item será removido.
     * @param id ID do item a ser removido.
     */
    void removerDeNo(No* no, int id);

    /**
     * @brief Encontra o índice de um item dentro de um nó.
     * @param no Ponteiro para o nó onde a busca será realizada.
     * @param id ID do item a ser buscado.
     * @return Índice do item encontrado dentro do nó.
     */
    int encontrarIndice(No* no, int id);

    /**
     * @brief Remove um item de um nó não-folha.
     * @param no Ponteiro para o nó onde o item será removido.
     * @param index Índice do item a ser removido no nó.
     */
    void removerDeNoNaoFolha(No* no, int index);

    /**
     * @brief Obtém o antecessor de um item de um nó.
     * @param no Ponteiro para o nó onde o antecessor será obtido.
     * @param index Índice do item cujo antecessor será encontrado.
     * @return Item antecessor encontrado.
     */
    Item obterAntecessor(No* no, int index);

    /**
     * @brief Obtém o sucessor de um item de um nó.
     * @param no Ponteiro para o nó onde o sucessor será obtido.
     * @param index Índice do item cujo sucessor será encontrado.
     * @return Item sucessor encontrado.
     */
    Item obterSucessor(No* no, int index);

    /**
     * @brief Realiza a concatenação de dois filhos de um nó.
     * @param no Ponteiro para o nó onde a concatenação ocorrerá.
     * @param index Índice do filho que será concatenado.
     */
    void concatenarFilhos(No* no, int index);

    /**
     * @brief Ajusta o nó na árvore B, equilibrando os filhos após uma remoção.
     * @param no Ponteiro para o nó que será ajustado.
     * @param index Índice do filho no nó.
     */
    void ajustarFilho(No* no, int index);

    /**
     * @brief Empresta uma chave do nó anterior para balancear a árvore.
     * @param no Ponteiro para o nó atual, que precisa de uma chave emprestada.
     * @param index Índice do filho que precisa de uma chave emprestada.
     */
    void emprestarDoAnterior(No* no, int index);

    /**
     * @brief Empresta uma chave do nó próximo para balancear a árvore.
     * @param no Ponteiro para o nó atual, que precisa de uma chave emprestada.
     * @param index Índice do filho que precisa de uma chave emprestada.
     */
    void emprestarDoProximo(No* no, int index);

public:
    /**
     * @brief Construtor da árvore B.
     * @param grau Grau mínimo da árvore.
     */
    ArvoreB(int grau);

    /**
     * @brief Insere um item na árvore B.
     * @param chave Item a ser inserido na árvore.
     */
    void inserir(const Item& chave);

    /**
     * @brief Busca um item na árvore B.
     * @param id ID do item a ser buscado.
     * @return Ponteiro para o item encontrado ou nullptr caso não encontrado.
     */
    Item* buscar(int id);

    /**
     * @brief Remove um item da árvore B.
     * @param id ID do item a ser removido.
     */
    void remover(int id);

    /**
     * @brief Imprime a árvore B por níveis.
     */
    void imprimirPorNiveis();

    /**
     * @brief Gera um arquivo .dot para visualizar a árvore.
     * @param nomeArq Nome do arquivo de saída para o formato Graphviz.
     */
    void visualiza(const std::string& nomeArq);
};

#endif // ARVOREB_H
