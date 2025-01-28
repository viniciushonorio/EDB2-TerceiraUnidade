#include "ArvoreB.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>

using namespace std;

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

void embaralharDados(vector<Item>& dados) {
    std::random_device rd;
    std::mt19937 g(rd()); 
    std::shuffle(dados.begin(), dados.end(), g); 
}

void exibirMenu() {
    cout << "\n--- MENU ---\n";
    cout << "1. Buscar item\n";
    cout << "2. Inserir item\n";
    cout << "3. Remover item\n";
    cout << "4. Imprimir árvore por níveis\n";
    cout << "5. Gerar arquivo .dot da árvore\n";
    cout << "6. Embaralhar dados de entrada\n";
    cout << "7. Sair\n"; 
    cout << "Escolha uma opção: ";
}

int main() {
    string caminhoArquivo = "./dados/dadosB4.txt";
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
        case 6: {
            embaralharDados(dados);
            ArvoreB novaArvore(3);
            for (const auto& item : dados) {
                novaArvore.inserir(item);
            }
            arvore = novaArvore;
            cout << "Dados embaralhados e reinseridos na árvore.\n";
            break;
        }
        case 7:
            cout << "Saindo...\n";
            break;
        default:
            cout << "Opção inválida. Tente novamente.\n";
            break;
        }
    } while (opcao != 7);

    return 0;
}
