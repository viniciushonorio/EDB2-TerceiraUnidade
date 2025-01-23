#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <queue>  // Para usar a fila

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

    // Função para percorrer a árvore e gerar o output para Graphviz
    void percorreB(ostream& out, int level);
};

void No::percorreB(ostream& out, int level) {
        out << "Node" << this << " [label=\"";
        for (const auto& chave : chaves) {
            out << chave.id << " ";
        }
        out << "\"];\n";
        
        for (size_t i = 0; i < filhos.size(); ++i) {
            out << "Node" << this << " -> Node" << filhos[i] << ";\n";
            filhos[i]->percorreB(out, level + 1); 
        }
}

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

    /*
        Gerencia os três casos principais de remoção e os ajustes necessários para manter as propriedades da árvore B.
    */
    void removerDeNo(No* no, int id) {
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

    /*
        Localiza o índice em que uma chave pode ser encontrada em um nó de uma árvore B.
    */
    int encontrarIndice(No* no, int id) {
        int index = 0;
        while (index < no->chaves.size() && no->chaves[index].id < id) {
            index++;
        }
        return index;
    }

    // Remove uma chave de um nó não folha
    void removerDeNoNaoFolha(No* no, int index) {
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

    /*
        Procura o maior valor na subárvore esquerda
    */
    Item obterAntecessor(No* no, int index) {
        No* atual = no->filhos[index];
        while (!atual->folha) {
            atual = atual->filhos.back();
        }
        return atual->chaves.back();
    }

    /*
        Procura o menor valor na subárvore direita
    */
    Item obterSucessor(No* no, int index) {
        No* atual = no->filhos[index + 1];
        while (!atual->folha) {
            atual = atual->filhos.front();
        }
        return atual->chaves.front();
    }

    /*
        Combina dois nós filhos adjacentes em um único nó, ajustando o pai para remover a chave e o ponteiro usados na fusão.
    */
    void concatenarFilhos(No* no, int index) {
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

    /*
        Responsável por garantir que um filho de um nó tenha pelo menos t chaves antes de prosseguir com a remoção.
    */
    void ajustarFilho(No* no, int index) {
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

    /*
        Permite que um nó filho receba uma chave do seu irmão anterior quando este possui pelo menos t + 1 chaves.
    */
    void emprestarDoAnterior(No* no, int index) {
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

    /*
        Permite que um nó filho receba uma chave do seu irmão à direita quando este possui pelo menos t + 1 chaves.
    */
    void emprestarDoProximo(No* no, int index) {
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

    /*
        Essa função é responsável por remover um item da árvore B através de seu id. 
        Ela lida com casos em que a raiz precisa ser ajustada ou até mesmo descartada após a remoção.
    */
    void remover(int id) {
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

    // Função para imprimir a árvore por níveis
    void imprimirPorNiveis() {
        if (!raiz) return;

        queue<No*> fila;  // Usar fila para percorrer por níveis
        fila.push(raiz);

        int nivel = 1;

        while (!fila.empty()) {
            int tamanho = fila.size();
            cout << "Nível " << nivel << ": ";

            for (int i = 0; i < tamanho; ++i) {
                No* no = fila.front();
                fila.pop();

                // Imprime as chaves do nó
                cout << "[";
                for (size_t j = 0; j < no->chaves.size(); ++j) {
                    cout << no->chaves[j].id;
                    if (j < no->chaves.size() - 1) cout << ", ";
                }
                cout << "] ";

                // Se não for nó folha, adiciona os filhos na fila
                if (!no->folha) {
                    for (No* filho : no->filhos) {
                        fila.push(filho);
                    }
                }
            }

            cout << endl;
            nivel++;
        }
    }

    void visualiza(const string& nomeArq) {
        ofstream file(nomeArq);
        file << "digraph BTree {\nnode [shape=record];\n";  // Definir o formato do nó no Graphviz

        if (raiz) {
            raiz->percorreB(file, 0);  // Inicia a recursão a partir da raiz
        }

        file << "}\n";  // Fecha a definição do gráfico
        file.close();
        cout << "Arquivo " << nomeArq << " gerado para visualização.\n";
    }
};

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
    cout << "3. Remover item\n";
    cout << "4. Imprimir árvore por níveis\n";
    cout << "5. Gerar arquivo .dot da árvore\n";
    cout << "6. Sair\n"; 
    cout << "Escolha uma opção: ";
}

int main() {
    string caminhoArquivo = "dadosB.txt";
    vector<Item> dados = carregarDados(caminhoArquivo);

    // Cria a árvore B com grau mínimo 3
    ArvoreB arvore(3);

    for (const auto& item : dados) {
        arvore.inserir(item);
    }

    int opcao;
    do {
        exibirMenu();
        cin >> opcao;

        switch (opcao) {
        case 1: {
            int idProcurado;
            cout << "Digite o ID do item que deseja: ";
            cin >> idProcurado;

            Item* resultado = arvore.buscar(idProcurado);
            if (resultado) {
                cout << "Item encontrado: " << resultado->nome << ", Quantidade: " << resultado->quantidade << endl;
            } else {
                cout << "Item com ID " << idProcurado << " não encontrado." << endl;
            }
            break;
        }
        case 2: {
            int id, quantidade;
            string nome;
            cout << "Digite o ID do item: ";
            cin >> id;
            cout << "Digite o nome do item: ";
            cin.ignore();
            getline(cin, nome);
            cout << "Digite a quantidade do item: ";
            cin >> quantidade;

            arvore.inserir({id, nome, quantidade});
            cout << "Item inserido com sucesso.\n";
            break;
        }
        case 3: {
            int idRemover;
            cout << "Digite o ID do item que deseja remover: ";
            cin >> idRemover;

            arvore.remover(idRemover);
            cout << "Item removido com sucesso.\n";
            break;
        }
        case 4:
            arvore.imprimirPorNiveis();
            break;
        case 5: {
            string nomeArquivo;
            cout << "Digite o nome do arquivo .dot (exemplo: arvore.dot): ";
            cin >> nomeArquivo;

            arvore.visualiza(nomeArquivo); 
            break;
        }
        case 6:
            cout << "Saindo...\n";
            break;
        default:
            cout << "Opção inválida. Tente novamente.\n";
            break;
        }
    } while (opcao != 6);

    return 0;
}
