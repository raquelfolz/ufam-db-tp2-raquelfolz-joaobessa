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

// Função auxiliar para dividir uma string em tokens com base em um delimitador
std::vector<std::string> split(const std::string& input, char delimiter) {
  std::vector<std::string> tokens;
  //std::stringstream ss(input);
  std::string token;
  /*while (std::getline(ss, token, delimiter)) {
    if (token.size() < 3) tokens.push_back("");
    else tokens.push_back(token.substr(1, token.size()-2));
  }*/
  token = "";
  bool s = false; //true se ja passou uma aspa
  for (int i = 0; i < input.size(); i++) {
    if (s) { //esta lendo um token
      if (input[i] == '\"') {
        //aspas podem ou nao marcar o fim do token
        //se for "", entao eh o fim
        //se nao, adiciona as aspas no token
        if (i < input.size()-1 and input[i+1] == '\"') {
          token += "\"\"";
          i++;
        }
        else s = false;
      }
      else token += input[i]; //outros caracteres sempre fazem parte
      continue;
    }
    //se nao esta lendo um token, pode ser inicio de outro ou separador ou NULL
    if (input[i] == '\"') s = true;
    else if (input[i] == ';'){
      tokens.push_back(token);
      token = "";
    }
    else if (i < input.size()-3 and input.substr(i, 4) == "NULL") {
      tokens.push_back("");
      i += 3;
    }
    else throw std:: invalid_argument(input); //so para testar
  }
  //depois do for, ainda tem que colocar o ultimo token ja que linha nao termina em ;
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

  std::unordered_map<int, Article> dataHash;
  std::map<int, int> primaryIndex;
  std::map<std::string, int> secondaryIndex;

  std::string line;
  std::string aux;
  while (std::getline(file, line)) {
    while (line.size() == 0 or line.back() != '\r') {
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

  // Salvar os dados em arquivos (arquivo de dados organizado por hashing, arquivo de índice primário e arquivo de índice secundário)
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
