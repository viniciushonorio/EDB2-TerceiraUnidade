#include "ArvoreB.h"
#include <iostream>

ArvoreB::ArvoreB(int grau) : grauMinimo(grau), raiz(new No(true)) {}

void ArvoreB::dividirFilho(No* no, int indice) {
    No* filho = no->filhos[indice];
    No* novoNo = new No(filho->folha);

    // Move metade das chaves para o novo nó
    novoNo->chaves.insert(novoNo->chaves.end(), filho->chaves.begin() + grauMinimo, filho->chaves.end());
    filho->chaves.resize(grauMinimo - 1);

    // Move os filhos, se não for folha
    if (!filho->folha) {
        novoNo->filhos.insert(novoNo->filhos.end(), filho->filhos.begin() + grauMinimo, filho->filhos.end());
        filho->filhos.resize(grauMinimo);
    }

    // Insere a chave mediana no nó pai
    no->chaves.insert(no->chaves.begin() + indice, filho->chaves[grauMinimo - 1]);
    no->filhos.insert(no->filhos.begin() + indice + 1, novoNo);
}

void ArvoreB::inserirNaoCheio(No* no, const Item& chave) {
    int i = no->chaves.size() - 1;

    // Se for folha, insere diretamente
    if (no->folha) {
        while (i >= 0 && chave.id < no->chaves[i].id) {
            i--;
        }
        no->chaves.insert(no->chaves.begin() + i + 1, chave);
    } else {
        // Encontra o filho adequado para descer
        while (i >= 0 && chave.id < no->chaves[i].id) {
            i--;
        }
        i++;
        // Se o filho está cheio, divide-o
        if (no->filhos[i]->chaves.size() == 2 * grauMinimo - 1) {
            dividirFilho(no, i);
            if (chave.id > no->chaves[i].id) {
                i++;
            }
        }
        inserirNaoCheio(no->filhos[i], chave);
    }
}

Item* ArvoreB::buscar(No* no, int id) {
    int i = 0;
    while (i < no->chaves.size() && id > no->chaves[i].id) {
        i++;
    }
    if (i < no->chaves.size() && id == no->chaves[i].id) {
        return &no->chaves[i];
    }
    if (no->folha) {
        return nullptr;
    }
    return buscar(no->filhos[i], id);
}

void ArvoreB::removerDeNo(No* no, int id) {
    int index = encontrarIndice(no, id);

    // Caso 1: A chave está em um nó folha com pelo menos t+1 elementos
    if (no->folha) {
        if (index < no->chaves.size() && no->chaves[index].id == id) {
            no->chaves.erase(no->chaves.begin() + index);
        }
    }
    // Caso 2: A chave está em um nó não folha
    else if (index < no->chaves.size() && no->chaves[index].id == id) {
        removerDeNoNaoFolha(no, index);
    }
    // Caso 3: A chave não está no nó, procure no filho apropriado
    else {
        if (no->filhos[index]->chaves.size() == grauMinimo) {
            ajustarFilho(no, index);
        }
        removerDeNo(no->filhos[index], id);
    }
}

int ArvoreB::encontrarIndice(No* no, int id) {
    int index = 0;
    while (index < no->chaves.size() && no->chaves[index].id < id) {
        index++;
    }
    return index;
}

void ArvoreB::removerDeNoNaoFolha(No* no, int index) {
    Item chave = no->chaves[index];

    // Caso 2a: O filho esquerdo tem pelo menos t+1 chaves
    if (no->filhos[index]->chaves.size() >= grauMinimo + 1) {
        Item antecessor = obterAntecessor(no, index);
        no->chaves[index] = antecessor;
        removerDeNo(no->filhos[index], antecessor.id);
    }
    // Caso 2b: O filho direito tem pelo menos t+1 chaves
    else if (no->filhos[index + 1]->chaves.size() >= grauMinimo + 1) {
        Item sucessor = obterSucessor(no, index);
        no->chaves[index] = sucessor;
        removerDeNo(no->filhos[index + 1], sucessor.id);
    }
    // Caso 2c: Ambos os filhos têm t chaves, então são concatenados
    else {
        concatenarFilhos(no, index);
        removerDeNo(no->filhos[index], chave.id);
    }
}

Item ArvoreB::obterAntecessor(No* no, int index) {
    No* atual = no->filhos[index];
    while (!atual->folha) {
        atual = atual->filhos.back();
    }
    return atual->chaves.back();
}

Item ArvoreB::obterSucessor(No* no, int index) {
    No* atual = no->filhos[index + 1];
    while (!atual->folha) {
        atual = atual->filhos.front();
    }
    return atual->chaves.front();
}

void ArvoreB::concatenarFilhos(No* no, int index) {
    No* esquerdo = no->filhos[index];
    No* direito = no->filhos[index + 1];

    // Mover a chave intermediária do pai para o nó esquerdo
    esquerdo->chaves.push_back(no->chaves[index]);
    // Transferir todas as chaves do nó direito para o nó esquerdo
    esquerdo->chaves.insert(esquerdo->chaves.end(), direito->chaves.begin(), direito->chaves.end());

    // Transferir todos os filhos do nó direito para o nó esquerdo (se não for folha)
    if (!esquerdo->folha) {
        esquerdo->filhos.insert(esquerdo->filhos.end(), direito->filhos.begin(), direito->filhos.end());
    }

    no->chaves.erase(no->chaves.begin() + index);
    no->filhos.erase(no->filhos.begin() + index + 1);
    delete direito;
}

void ArvoreB::ajustarFilho(No* no, int index) {
    // O filho à esquerda pode emprestar uma chave
    if (index > 0 && no->filhos[index - 1]->chaves.size() >= grauMinimo + 1) {
        emprestarDoAnterior(no, index);
    }
    // O filho à direita pode emprestar uma chave
    else if (index < no->filhos.size() - 1 && no->filhos[index + 1]->chaves.size() >= grauMinimo + 1) {
        emprestarDoProximo(no, index);
    }
    // Caso contrário, concatena os filhos
    else {
        if (index < no->filhos.size() - 1) {
            concatenarFilhos(no, index);
        } else {
            concatenarFilhos(no, index - 1);
        }
    }
}

void ArvoreB::emprestarDoAnterior(No* no, int index) {
    No* filho = no->filhos[index];
    No* anterior = no->filhos[index - 1];

    filho->chaves.insert(filho->chaves.begin(), no->chaves[index - 1]);
    no->chaves[index - 1] = anterior->chaves.back();
    anterior->chaves.pop_back();

    if (!anterior->folha) {
        filho->filhos.insert(filho->filhos.begin(), anterior->filhos.back());
        anterior->filhos.pop_back();
    }
}


void ArvoreB::emprestarDoProximo(No* no, int index) {
    No* filho = no->filhos[index];
    No* proximo = no->filhos[index + 1];

    filho->chaves.push_back(no->chaves[index]);
    no->chaves[index] = proximo->chaves.front();
    proximo->chaves.erase(proximo->chaves.begin());

    if (!proximo->folha) {
        filho->filhos.push_back(proximo->filhos.front());
        proximo->filhos.erase(proximo->filhos.begin());
    }
}

void ArvoreB::inserir(const Item& chave) {
    if (raiz->chaves.size() == 2 * grauMinimo - 1) {
        No* novoNo = new No(false);
        novoNo->filhos.push_back(raiz);
        dividirFilho(novoNo, 0);
        raiz = novoNo;
    }
    inserirNaoCheio(raiz, chave);
}

Item* ArvoreB::buscar(int id) {
    return buscar(raiz, id);
}

void ArvoreB::remover(int id) {
    if (!raiz) return;

    removerDeNo(raiz, id);

    // Verifica se a raiz ficou vazia após a remoção
    if (raiz->chaves.empty()) {
        No* temp = raiz;
        if (raiz->folha) {
            raiz = nullptr;
        } else {
            raiz = raiz->filhos[0];
        }
        delete temp;
    }
}


void ArvoreB::imprimirPorNiveis() {
    if (!raiz) return;

    std::queue<No*> fila;
    fila.push(raiz);

    int nivel = 1;

    while (!fila.empty()) {
        int tamanho = fila.size();
        std::cout << "Nível " << nivel << ": ";

        for (int i = 0; i < tamanho; ++i) {
            No* no = fila.front();
            fila.pop();

            std::cout << "[";
            for (size_t j = 0; j < no->chaves.size(); ++j) {
                std::cout << no->chaves[j].id;
                if (j < no->chaves.size() - 1) std::cout << ", ";
            }
            std::cout << "] ";

            if (!no->folha) {
                for (No* filho : no->filhos) {
                    fila.push(filho);
                }
            }
        }

        std::cout << std::endl;
        nivel++;
    }
}

void ArvoreB::visualiza(const std::string& nomeArq) {
    std::ofstream file(nomeArq);
    file << "digraph BTree {\nnode [shape=record];\n";

    if (raiz) {
        raiz->percorreB(file, 0);
    }

    file << "}\n";
    file.close();
    std::cout << "Arquivo " << nomeArq << " gerado para visualização.\n";
}
