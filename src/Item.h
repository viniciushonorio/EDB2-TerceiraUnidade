/**
 * @file Item.h
 * @brief Declaração do tipo Item usado na Arvore B.
 */

#ifndef ITEM_H
#define ITEM_H

#include <string>

/**
 * @struct Item
 * @brief Representa um item com ID, nome e quantidade.
 */
struct Item {
    int id;            //ID do item. 
    std::string nome;  //Nome do item.
    int quantidade;    //Quantidade do item em estoque.
};

#endif // ITEM_H