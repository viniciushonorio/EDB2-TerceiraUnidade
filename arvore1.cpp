#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

// Estrutura para representar um item de estoque
struct Item {
    int id;               // ID do item
    string nome;          // Nome do item
    int quantidade;       // Quantidade em estoque
};

// Classe que representa um nó da árvore B
class No {
public:
    vector<Item> chaves;        // Lista de chaves (itens)
    vector<No*> filhos;         // Ponteiros para os filhos
    bool folha;                 // Indica se o nó é folha

    No(bool ehFolha) : folha(ehFolha) {}
};

// Classe que representa a árvore B
class ArvoreB {
private:
    int grauMinimo;            // Grau mínimo da árvore
    No* raiz;                  // Raiz da árvore

    // Divide um filho de um nó durante a inserção
    void dividirFilho(No* no, int indice) {
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

    // Insere uma chave em um nó não cheio
    void inserirNaoCheio(No* no, const Item& chave) {
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

    // Pesquisa por uma chave na árvore
    Item* buscar(No* no, int id) {
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

public:
    ArvoreB(int grau) : grauMinimo(grau), raiz(new No(true)) {}

    // Insere uma nova chave na árvore
    void inserir(const Item& chave) {
        if (raiz->chaves.size() == 2 * grauMinimo - 1) {
            No* novoNo = new No(false);
            novoNo->filhos.push_back(raiz);
            dividirFilho(novoNo, 0);
            raiz = novoNo;
        }
        inserirNaoCheio(raiz, chave);
    }

    // Pesquisa por uma chave com base no ID
    Item* buscar(int id) {
        return buscar(raiz, id);
    }
};

// Função para carregar os dados do arquivo
vector<Item> carregarDados(const string& caminhoArquivo) {
    ifstream arquivo(caminhoArquivo);
    vector<Item> dados;
    string linha;

    while (getline(arquivo, linha)) {
        linha.erase(remove(linha.begin(), linha.end(), '{'), linha.end());
        linha.erase(remove(linha.begin(), linha.end(), '}'), linha.end());

        stringstream ss(linha);
        string parte;
        Item item;

        getline(ss, parte, ',');
        item.id = stoi(parte);
        getline(ss, parte, ',');
        item.nome = parte.substr(2, parte.size() - 3);
        getline(ss, parte, ',');
        item.quantidade = stoi(parte);

        dados.push_back(item);
    }
    return dados;
}

// Função para exibir o menu
void exibirMenu() {
    cout << "\n--- MENU ---\n";
    cout << "1. Buscar item\n";
    cout << "2. Inserir item\n";
    cout << "3. Remover item (não implementado)\n";
    cout << "4. Sair\n";
    cout << "Escolha uma opção: ";
}

int main() {
    // Caminho para o arquivo de dados
    string caminhoArquivo = "/home/vinicius/Downloads/pdfs/edb/dadosB.txt";
    vector<Item> dados = carregarDados(caminhoArquivo);

    // Cria a árvore B com grau mínimo 3
    ArvoreB arvore(3);

    // Insere os dados na árvore
    for (const auto& item : dados) {
        arvore.inserir(item);
    }

    // Pesquisa por um item específico
    int idProcurado = 110;
    cout << "Digite o ID do item que deseja: ";
    cin >> idProcurado;
    
    Item* resultado = arvore.buscar(idProcurado);
    if (resultado) {
        cout << "Item encontrado: " << resultado->nome << ", Quantidade: " << resultado->quantidade << endl;
    } else {
        cout << "Item com ID " << idProcurado << " nao encontrado." << endl;
    }

    return 0;
}