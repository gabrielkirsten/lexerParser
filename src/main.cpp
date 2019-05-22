/* *** BIBLIOTECAS *** */
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>  

using namespace std;

#include "models/returnLex.hpp"
#include "models/arquivo.hpp"

/* *** VARIAVEIS GLOBAIS *** */
bool 	printLexs 		= false	;
bool 	nextTokenN 		= false	;
int		numeroDeErros			;
int 	testesinc 		= 1		;

returnLex tokenLex;
Arquivo arquivo("../examples/example1.txt"); 

//inclui as bibliotecas do analizador lexico e sintatico
#include "analizers/analisadorLexico.hpp"
#include "analizers/analisadorSintatico.hpp"

/* *** MAIN *** */
int main(int argc, char *argv[]){	
	bool patchPadrao = true;	// boleno que indica se o caminho padrão foi alterado
	
	//condição se for enviados argumentos
	if(argc > 1){ 
		for(int i = 1; i<argc; i++){
			if(strcmp(argv[i],"--h") == 0){
				cout<<"Usage la + [options]"<<endl
					<<"Options:"<<endl
					<<"\t-p <patch>\tEspecifica o caminho do arquivo a ser analizado"<<endl
					<<"\t-l \t\tAtiva a saida do Analisador Sintatico"<<endl
					<<"\t--h \t\tAjuda"<<endl;
				exit(0);
			} else if(strcmp(argv[i],"-l") == 0){
				printLexs = true;
				cout<<"Num\tCadeia\t\tToken\n";						// cabeçalho do analisador
			} else if(strcmp(argv[i],"-p") == 0){
				arquivo.setUrl(argv[i+1]);
				i++;
				patchPadrao = false;
			}else{
				cout<<"Opcao '"<<argv[i]<<"' Invalida"<<endl;
				cout<<"Usage la + [options]"<<endl
					<<"Options:"<<endl
					<<"\t-p <patch>\tEspecifica o caminho do arquivo a ser analizado"<<endl
					<<"\t-l \t\tAtiva a saida do Analisador Lexico"<<endl
					<<"\t--h \t\tAjuda"<<endl;
				exit(0);
			}
		}
	}
	
	//se não foi setado o caminho padrão, uma mensagem de aviso é exibida
	if(patchPadrao){
		cout<<"Arquivo padrao '"<<arquivo.getUrl()<<"' carregado, para selecionar outro arquivo utilize a opcao '-p' + 'caminho'."<<endl;
	}
	
	arquivo.abrirArquivo(); //abre o arquivo
	
	buildHash();//constroi o hash do analizador lexico
	
	analiseSintatica(); //chama o analizador sintatico
	
	arquivo.fecharArquivo(); //fecha o arquivo
	
	cout<<numeroDeErros<<" Erro(s) Encontrado(s)"<<endl<<endl;
		
}