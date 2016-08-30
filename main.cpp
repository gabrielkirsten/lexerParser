/* *** BIBLIOTECAS *** */
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>  

using namespace std;

/* *** VARIAVEIS GLOBAIS *** */
bool 	printLexs 		= false	;
bool 	nextTokenN 		= false	;
int		numeroDeErros			;
int 	testesinc 		= 1		;

//classe utilizada no retorno do analizador lexico para ser utilizado no analizador sintatico
class returnLex{
	private:
		string cadeia;
		string token;
		int line;
		bool retrocedido;
		
	public:
		void setCadeia(string cadeia){
			this->cadeia = cadeia;
		}
		void setToken(string token){
			this->token = token;
		}
		void setAll(string cadeia, string token, int line, bool retrocedido){
			this->cadeia = cadeia;
			this->token = token;
			this->line = line;
			this->retrocedido = retrocedido;
		}
		void setLine(int line){
			this->line = line;
		}
		int getLine(){
			return this->line;
		}
		string getCadeia(){
			return this->cadeia;
		}
		string getToken(){
			return this->token;
		}
		bool getRetrocedido(){
			return this->retrocedido;
		}
		void getAll(){
			cout<<this->cadeia<<this->token<<this->line<<endl;
		}
};

returnLex tokenLex;

/* Classe Arquivo
*	Atributos
*		$url(string) - armazena o caminho do arquivo a ser aberto
*		$arquivo(FILE) - Armazena o arquivo
*		$linha(int) - armazena a linha de leitura atual
* 	Metodos:
* 		abrirArquivo - Abre tenta abrir o arquivo especificado
*		fecharArquivo - fecha o arquivo especificado
* 		proximaLetra - retorna a proximo caractere a ser lido
*		incrementaLinha - incrementa a variavel linha, indicando que percorreu mais uma linha
* 		getLinha - retorna a linha atual
*	A classe também conta com um construtor que ja define a url do arquivo e zera o contador de linhas */
class Arquivo{
	private:
		string url;
		FILE *arquivo;
		int linha;
	
	public:
		void abrirArquivo(){
			const char *caminho = url.c_str();
			this->arquivo = fopen(caminho, "rw+");
			if(this->arquivo == NULL){
				cout<<"Erro, nao foi possivel abrir o arquivo("<<url<<")!";
				exit(0);
			}
		}
		void fecharArquivo(){
			fclose(arquivo);
		}
		char proximaLetra(){
			fgetc(arquivo);			
		}	
		void incrementaLinha(){
			this->linha++;
		}
		int getLinha(){
			return this->linha;
		}
		void setUrl(string url){
			this->url = url;
		}
		string getUrl(){
			return this->url;
		}
		Arquivo(string url){
			this->url = url;
			this->linha = 1;
		}
};

//Cria um objeto Arquivo para analisar
Arquivo arquivo("teste1.txt"); //ALTERAR AQUI O ARQUIVO A SER ANALISADO 

//inclui as bibliotecas do analizador lexico e sintatico
#include "analisadorLexico.h"
#include "analisadorSintatico.h"

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