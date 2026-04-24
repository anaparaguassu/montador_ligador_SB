#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

// estrutura com as propriedades da instrução
struct Instruction {
    int opcode;
    int size; // 1 + número de operandos
};

// tabela de símbolos
struct Symbol {
    int adress;
    bool isDef;
};

// conjunto de instruções da máquina hipotética
map<string, Instruction> instructionTable = {
    {"ADD", {1, 2}},
    {"SUB", {2, 2}},
    {"MUL", {3, 2}},
    {"DIV", {4, 2}},
    {"JMP", {5, 2}},
    {"JMPN", {6, 2}},
    {"JMPP", {7, 2}},
    {"JMPZ", {8, 2}},
    {"COPY", {9, 3}},
    {"LOAD", {10, 2}},
    {"STORE", {11, 2}},
    {"INPUT", {12, 2}},
    {"OUTPUT", {13, 2}},
    {"STOP", {14, 1}}
};

string clean(string line){
    // remove comentários
    size_t commentPos = line.find(';');
    if(commentPos != string::npos) line = line.substr(0, commentPos);

    // converte para maiúsculo
    transform(line.begin(), line.end(), line.begin(), ::toupper);

    // remove espaços e tabulações
    size_t first = line.find_first_not_of(" \t\r\n");
    if(first == string::npos) return "";
    size_t last = line.find_last_not_of(" \t\r\n");
    line = line.substr(first, (last - first + 1));

    return line;
}

void runPreprocessor(string filename){
    string inputFile = filename + ".asm";
    string outputFile = filename + ".pre";

    ifstream inFile(inputFile);
    ofstream outFile(outputFile);

    if(!inFile.is_open()){
        cout << "Erro: Não foi possível abrir o arquivo.\n";
        return;
    }

    string line;
    vector<string> cleanedLines;
    cout << "Pre-processamento de " << inputFile << "...\n";

    // leitura inicial e limpeza básica
    while(getline(inFile, line)){
        string cleanLine = clean(line);
        // se a linha não ficou vazia, adicionamos no vetor
        if(!cleanLine.empty()) cleanedLines.push_back(cleanLine);
    }
    inFile.close();

    // tratamento de rótulos isolados e COPY
    vector<string> formattedLines;
    string outLabel = "";
    for(size_t i = 0; i < cleanedLines.size(); i++){
        string curLine = cleanedLines[i];

        // se tem rótulo pendente, junta com a linha atual
        if(!outLabel.empty()){
            curLine = outLabel + " " + curLine;
            outLabel = "";
        }

        // se a linha tem apenas 1 rótulo
        if(curLine.back() == ':'){
            outLabel = curLine;
            continue;
        }

        // formatar o COPY tirando espaços da vírgula
        size_t copyPos = curLine.find("COPY ");
        if(copyPos != string::npos){
            size_t commaPos = curLine.find(',', copyPos);
            if(commaPos != string::npos){
                // remove espaços antes da vírgula
                while(commaPos > 0 && curLine[commaPos-1] == ' '){
                    curLine.erase(commaPos-1, 1);
                    commaPos--;
                }
                // remove espaços depois da vírgula
                while(commaPos+1 < curLine.size() && curLine[commaPos+1] == ' ') curLine.erase(commaPos+1, 1);
            }
        }

        formattedLines.push_back(curLine);
    }

    // processa EQU, IF e separa seções
    map<string, string> equTable;
    vector<string> sectionData, sectionText;
    int curSection = 0; // 0 = TEXT 1 = DATA
    bool skipLine = false;

    for(size_t i = 0; i < formattedLines.size(); i++){
        string curLine = formattedLines[i];

        // lógica do IF => pular linha se condição anterior for falsa
        if(skipLine){
            skipLine = false;
            continue;
        }

        // diretiva EQU (DOIS: EQU 2)
        size_t equPos = curLine.find(" EQU ");
        if(equPos != string::npos){
            string label = curLine.substr(0, equPos);
            if(!label.empty() && label.back() == ':') label.pop_back();
            string val = curLine.substr(equPos + 5);
            equTable[label] = val;
            continue;
        }

        // diretiva IF (IF DOIS)
        if(curLine.find("IF ") == 0){
            string condLabel = curLine.substr(3);
            int condVal = 0;
            // busca valor na tabela EQU
            if(equTable.count(condLabel)) condVal = stoi(equTable[condLabel]);
            else{
                try{ condVal = stoi(condLabel); }
                catch(...){}
            }

            // se for 0, marca para pular a próxima instrução
            if(condVal == 0) skipLine = true;
            continue; // não escrever o IF no .pre
        }

        // substitui valores definidos por EQU no resto do código
        for(auto const& [label, val] : equTable){
            size_t pos = 0;
            while((pos = curLine.find(label, pos)) != string::npos){
                bool start = (pos == 0 || curLine[pos-1] == ' ' || curLine[pos-1] == ',');
                size_t lsize = label.size(); 
                bool end = (pos + lsize == curLine.size() || curLine[pos + lsize] == ' ' || curLine[pos + lsize] == ',');

                if(start && end){
                    curLine.replace(pos, lsize, val);
                    pos += val.size();
                }
                else pos += lsize;
            }
        }

        // identifica as seções
        if(curLine == "SECTION TEXT"){
            curSection = 0;
            sectionText.push_back(curLine);
        } 
        else if (curLine == "SECTION DATA"){
            curSection = 1;
            sectionData.push_back(curLine);
        }
        else{
            if(curSection) sectionData.push_back(curLine);
            else sectionText.push_back(curLine);
        }
    }

    // grava no arquivo .pre (TEXT primeiro DATA depois)
    for(const string& s : sectionText) outFile << s << "\n";
    for(const string& s : sectionData) outFile << s << "\n";

    outFile.close();
    cout << "Arquivo " << outputFile << " gerado com sucesso!\n";
};

void runAssembler(string filename);
void runSimulator(string filename);

int main(int argc, char* argv[]) {
    // verifica se há o nome do arquivo
    if(argc != 2){
        cout << "Erro! Execute: ./montador <arquivo>\n";
        return 1;
    }

    string fileArg = argv[1];

    // pega posição para verificar extensão
    size_t dotPos = fileArg.find_last_of('.');
    if(dotPos == string::npos){
        cout << "Erro: Arquivo sem extensão.\n";
        return 1;
    }

    string extension = fileArg.substr(dotPos);
    string baseFilename = fileArg.substr(0, dotPos);

    // roteamento dos 3 modos de execução
    if(extension == ".asm"){
        cout << "Modo: Pre-processador\n";
        // deve gerar o arquivo .pre
        runPreprocessor(baseFilename);
    }
    else if (extension == ".pre"){
        cout << "Modo: Montador\n";
        // deve gerar os arquivos .obj e .pen
        //runAssembler(baseFilename);
    }
    else if(extension == ".obj"){
        cout << "Modo: Simulador\n";
        // deve simular a execução lendo INPUT do teclado e OUTPUT pro monitor 
        //runSimulator(baseFilename);
    }
    else{
        cout << "Erro: Extensão nao suportada ()" << extension << ").\n";
        return 1;
    }
    
    return 0;
}