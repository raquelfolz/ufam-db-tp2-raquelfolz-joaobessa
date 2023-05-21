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

// Função auxiliar para dividir a string em tokens correspondentes a cada campo
std::vector<std::string> split(const std::string& input, char delimiter) {
  std::vector<std::string> tokens;
  std::string token = "";
  bool ctrl = false; //true se esta atualmente lendo um token
  for (unsigned int i = 0; i < input.size(); i++) {
    if (ctrl) {
      if (input[i] == '\"') {
        //aspas podem ou nao marcar o fim do token
        //se for "", entao eh o fim
        //se nao, adiciona as aspas no token
        if (i < input.size()-1 and input[i+1] == '\"') {
          token += "\"\"";
          i++;
        }
        else ctrl = false;
      }
      else token += input[i]; //outros caracteres sempre fazem parte do token
      continue;
    }
    //se nao esta lendo um token, pode ser inicio de outro token ou NULL ou ;
    //outros casos nao foram tratados porque nao ocorrem no arquivo
    if (input[i] == '\"') ctrl = true;
    else if (input[i] == ';'){
      tokens.push_back(token);
      token = "";
    }
    else if (i < input.size()-3 and input.substr(i, 4) == "NULL") {
      i += 3;
    }
  }
  //depois do for, ainda tem que inserir o ultimo token ja que linha nao termina em ;
  tokens.push_back(token);

  return tokens;
}

// Função para realizar o upload dos dados iniciais
void upload(const std::string& filename) {
  std::ifstream file(filename);
  if (!file) {
    std::cout << "Erro ao abrir o arquivo: " << filename << std::endl;
    return;
  }
  
  //estruturas da biblioteca paddrao para tabela hash e arvore redblack
  //idealmente devem ser trocados por estruturas proprias que permitam
  //trabalhar com memoria secundaria, mas esses funcionam
  std::unordered_map<int, Article> dataHash;
  std::map<int, int> primaryIndex;
  std::map<std::string, int> secondaryIndex;

  std::string line;
  std::string aux;
  while (std::getline(file, line)) {
    while (line.size() == 0 or line.back() != '\r') {
      //tem alguns casos que acontece \n no meio de uma chave, entao
      //so \r pode ser usado para efetivamente determinar o fim de uma entrada
      std::getline(file, aux);
      line += '\n';
      line += aux;
    }

    line.pop_back();
    std::vector<std::string> tokens = split(line, ';');
    if (tokens.size() == 7) {
      Article article;
      if (tokens[0] == "") article.ID = -1;
      else article.ID = std::stoi(tokens[0]);
      article.title = tokens[1];
      if (tokens[2] == "") article.ID = -1;
      else article.year = std::stoi(tokens[2]);
      article.authors = tokens[3];
      if (tokens[4] == "") tokens[4] = -1;
      else article.citations = std::stoi(tokens[4]);
      article.updateDateTime = tokens[5];
      article.snippet = tokens[6];

      dataHash[article.ID] = article;
      primaryIndex[article.ID] = dataHash.size();  // Armazena a posição do registro no arquivo de dados
      secondaryIndex[article.title] = article.ID;  // Mapeia o título para o ID do registro
    }
  }

  // Salvar os dados em arquivos (arquivo de dados organizado por hashing,
  // arquivo de índice primário e arquivo de índice secundário)
  std::ofstream dataFile("data.dat", std::ios::binary);
  for (const auto& pair : dataHash) {
    const Article& article = pair.second;
     dataFile.write(reinterpret_cast<const char*>(&article), sizeof(Article));
  }
  dataFile.close();

  std::ofstream primaryIndexFile("primaryIndex.dat", std::ios::binary);
  for (const auto& pair : primaryIndex) {
    primaryIndexFile.write(reinterpret_cast<const char*>(&pair), sizeof(std::pair<int, int>));
  }
  primaryIndexFile.close();

  std::ofstream secondaryIndexFile("secondaryIndex.dat", std::ios::binary);
  for (const auto& pair : secondaryIndex) {
    const std::string& title = pair.first;
    const int& ID = pair.second;
    secondaryIndexFile.write(title.c_str(), title.size() + 1);
    secondaryIndexFile.write(reinterpret_cast<const char*>(&ID), sizeof(int));
  }
  secondaryIndexFile.close();

  std::cout << "Upload concluído com sucesso!" << std::endl;
}


int main() {
    // Etapa 1: Carregar os dados iniciais usando a função "upload"
    upload("artigo.csv");

    return 0;
}
