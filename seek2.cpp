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

void sk2(const std::string& title) {
    std::ifstream secondaryIndexFile("secondaryIndex.dat", std::ios::binary);
    if (!secondaryIndexFile) {
        std::cout << "Erro ao abrir o arquivo de índice secundário." << std::endl;
        return;
    }

    std::map<std::string, int> secondaryIndex;

    std::string indEntryTl;
    int indEntryID;
    while (secondaryIndexFile.peek() != EOF) {
        std::getline(secondaryIndexFile, indEntryTl, '\0');
        secondaryIndexFile.read(reinterpret_cast<char*>(&indEntryID), sizeof(int));
        secondaryIndex[indEntryTl] = indEntryID;  // Título -> ID do registro
    }
    secondaryIndexFile.close();

    auto iter = secondaryIndex.find(title);
    if (iter != secondaryIndex.end()) {
        int ID = iter->second;

        std::ifstream dataFile("data.dat", std::ios::binary);
        if (!dataFile) {
            std::cout << "Erro ao abrir o arquivo de dados." << std::endl;
            return;
        }

        Article article;
        int fBlocks = 0;
        bool found = false;

        while (dataFile.read(reinterpret_cast<char*>(&article), sizeof(Article))) {
            fBlocks++;
            if (article.ID == ID) {
                found = true;
                break;
            }
        }
        dataFile.close();

        if (found) {
            std::cout << "Registro encontrado:" << std::endl;
            std::cout << "ID: " << article.ID << std::endl;
            std::cout << "Título: " << article.title << std::endl;
            std::cout << "Ano: " << article.year << std::endl;
            std::cout << "Autores: " << article.authors << std::endl;
            std::cout << "Citações: " << article.citations << std::endl;
            std::cout << "Data e hora da última atualização: " << article.updateDateTime << std::endl;
            std::cout << "Snippet: " << article.snippet << std::endl;
            std::cout << "Quantidade de blocos lidos no índice secundário: " << secondaryIndex.size() << std::endl;
            std::cout << "Quantidade total de blocos no arquivo de índice secundário: " << secondaryIndex.size() << std::endl;
        } else {
            std::cout << "Registro não encontrado." << std::endl;
        }
    } else {
        std::cout << "Registro não encontrado." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    sk2(argv[1]);
    return 0;
}
