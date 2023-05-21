#include <bits/stdc++.h>

struct Article {
    int ID;
    std::string title;
    int year;
    std::string authors;
    int citations;
    std::string updateDateTime;
    std::string snippet;
};

void sk1(int ID) {
    std::ifstream pIndexFile("primaryIndex.dat", std::ios::binary);
    if (!pIndexFile) {
        std::cout << "Erro ao abrir o arquivo de índice primário." << std::endl;
        return;
    }

    std::map<int, int> pIndex;

    std::pair<int, int> indEntry;
    while (pIndexFile.read(reinterpret_cast<char*>(&indEntry), sizeof(std::pair<int, int>))) {
        pIndex[indEntry.first] = indEntry.second;  // ID -> posição do registro no arquivo de dados
    }
    pIndexFile.close();

    auto iter = pIndex.find(ID);
    if (iter != pIndex.end()) {
        std::ifstream dataFile("data.dat", std::ios::binary);
        if (!dataFile) {
            std::cout << "Erro ao abrir os dados." << std::endl;
            return;
        }

        int position = iter->second - 1;  // A posição é baseada em 1, subtraímos 1 para obter o índice do registro no arquivo de dados
        dataFile.seekg(position * sizeof(Article));
        Article article;
        dataFile.read(reinterpret_cast<char*>(&article), sizeof(Article));
        dataFile.close();

        std::cout << "Registro encontrado:" << std::endl;
        std::cout << "ID: " << article.ID << std::endl;
        std::cout << "Título: " << article.title << std::endl;
        std::cout << "Ano: " << article.year << std::endl;
        std::cout << "Autores: " << article.authors << std::endl;
        std::cout << "Citações: " << article.citations << std::endl;
        std::cout << "Data e hora da última atualização: " << article.updateDateTime << std::endl;
        std::cout << "Snippet: " << article.snippet << std::endl;
        std::cout << "Quantidade de blocos lidos no índice primário: " << pIndex.size() << std::endl;
        std::cout << "Quantidade total de blocos no arquivo de índice primário: " << pIndex.size() << std::endl;
    } else {
        std::cout << "Registro não encontrado." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    int id = std::stoi(argv[1]);
    sk1(id);
    return 0;
}
