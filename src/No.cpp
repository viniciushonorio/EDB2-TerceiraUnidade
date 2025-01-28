#include "No.h"
#include <iostream>

No::No(bool ehFolha) : folha(ehFolha) {}

void No::percorreB(std::ostream& out, int level) {
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
