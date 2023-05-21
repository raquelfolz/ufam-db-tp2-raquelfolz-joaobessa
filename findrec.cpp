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


void findrec(int ID) {
    std::ifstream dataFile("data.dat", std::ios::binary);
    if (!dataFile) {
        std::cout << "Erro ao abrir o arquivo" << std::endl;
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
        std::cout << "Quantidade de blocos lidos: " << fBlocks << std::endl;
       // std::cout << "Quantidade total de blocos no arquivo de dados: " << dataFile.size() << std::endl;
    } else {
        std::cout << "Registro não encontrado." << std::endl;
    }
}

int main(int argc, char* argv[]) {
    int id = std::stoi(argv[1]);
    findrec(id);
    return 0;
}
