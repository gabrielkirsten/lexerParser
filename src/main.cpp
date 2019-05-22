/* *** BIBLIOTECAS *** */
using namespace std;

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "models/returnLex.hpp"
#include "models/arquivo.hpp"
#include "utils/parameterUtils.hpp"

string SINTATIC_HEADER = "Num \tCadeia \t\t Token\n";

bool printLexs = false;
bool nextTokenN = false;
int numeroDeErros;

returnLex tokenLex;
Arquivo arquivo("../examples/example1.txt");

#include "analizers/analisadorLexico.hpp"
#include "analizers/analisadorSintatico.hpp"

int main(int argc, char *argv[])
{
    bool defaultPath = true;

    if (ParameterUtils::hasParameters(argc))
    {
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "--h") == 0)
            {
                cout << "Usage la + [options]" << endl
                     << "Options:" << endl
                     << "\t-p <patch>\tEspecifica o caminho do arquivo a ser analizado" << endl
                     << "\t-l \t\tAtiva a saida do Analisador Sintatico" << endl
                     << "\t--h \t\tAjuda" << endl;
                exit(0);
            }

            else if (strcmp(argv[i], "-l") == 0)
            {
                printLexs = true;
                cout << SINTATIC_HEADER; 
            }

            else if (strcmp(argv[i], "-p") == 0)
            {
                arquivo.setUrl(argv[++i]);
                defaultPath = false;
            }

            else
            {
                cout << "Opcao '" << argv[i] << "' Invalida" << endl;
                cout << "Usage la + [options]" << endl
                     << "Options:" << endl
                     << "\t-p <patch>\tEspecifica o caminho do arquivo a ser analizado" << endl
                     << "\t-l \t\tAtiva a saida do Analisador Lexico" << endl
                     << "\t--h \t\tAjuda" << endl;
                exit(0);
            }
        }
    }

    if (defaultPath)
    {
        cout << "Arquivo padrao '" << arquivo.getUrl() << "' carregado, para selecionar outro arquivo utilize a opcao '-p' + 'caminho'." << endl;
    }

    arquivo.abrirArquivo();

    buildHash();

    analiseSintatica();

    arquivo.fecharArquivo();

    cout << numeroDeErros << " Erro(s) Encontrado(s)" << endl
         << endl;
}