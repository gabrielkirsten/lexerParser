//CONSTANTES
#define QUANTIDADEDEESTADOS 				23
#define QUANTIDADEDEESTADOSFINAIS 			14
#define QUANTIDADEDEESTADOSQUERETROCEDEM 	8
#define QUANTIDADEDEESTADOSPARAVERIFICAR 	2
#define QUANTIDADEDEESTADOSERRO 			3
#define QUANTIDADEDEPALAVRASRESERVADAS		14
#define QUANTIDADEDESIMBULOS 				9

//variavel que informa se o caractere foi consumido, ou se o estado foi retrocedido
//essa variavel tem a finalidade de imprimir a linha corretamente no analizador sintatico
bool 	caractereConsumido;

//Matriz reprensentando a tabela de transição de estados
int tabelaTransicaoDeEstados[QUANTIDADEDEESTADOS][9] = {	{1, 3, 7, 0, 14, 15, 18, 10, 7},
															{1, 1, 2, 2, 2, 2, 2, 2, 2},
															{0, 0, 0, 0, 0, 0, 0, 0, 0},
															{21, 3, 4, 8, 8, 8, 8, 8, 8}, 
															{9, 5, 9, 9, 9, 9, 9, 9, 9},
															{21, 5, 6, 6, 6, 6, 6, 6, 6},
															{0, 0, 0, 0, 0, 0, 0, 0, 0},
															{0, 0, 0, 0, 0, 0, 0, 0, 0},
															{0, 0, 0, 0, 0, 0, 0, 0, 0},
															{0, 0, 0, 0, 0, 0, 0, 0, 0},
															{13, 13, 13, 13, 11, 12, 13, 13, 13},
															{0, 0, 0, 0, 0, 0, 0, 0, 0},
															{0, 0, 0, 0, 0, 0, 0, 0, 0},
															{0, 0, 0, 0, 0, 0, 0, 0, 0},
															{0, 0, 0, 0, 0, 0, 0, 0, 0},
															{17, 17, 17, 17, 16, 17, 17, 17, 17},
															{0, 0, 0, 0, 0, 0, 0, 0, 0},
															{0, 0, 0, 0, 0, 0, 0, 0, 0},
															{20, 20, 20, 20, 19, 20, 20, 20, 20},
															{0, 0, 0, 0, 0, 0, 0, 0, 0},
															{0, 0, 0, 0, 0, 0, 0, 0, 0},
															{21, 21, 22, 22, 22, 22, 22, 22, 22},
															{0, 0, 0, 0, 0, 0, 0, 0, 0}};
															
//Palavras e Simbulos reservados e Mensagens de Tokens (Variaveis utilizadas no analizador sintatico)															
string 	palavrasReservadas[] 						= {"program", "var", "real", "integer", "procedure", "begin", "end", "else", "read", "write", "while", "if", "then", "do"};
string 	simbolosReservados[] 						= {"+", "-", "*", "/", "(", ")", ",", ";", "."};
string 	mensagensTokens[] 							= {"","","","","","","REAL","CARACTERE INVALIDO","INTEIRO","NUMERO REAL MAL FORMADO","", "<=","<>","<","=","",">=",">","",":=",":", "", "FORMACAO DE ID INCORRETA"};
int 	hashPalavra[QUANTIDADEDEPALAVRASRESERVADAS]; 		// tabela HASH para palavras reservadas
int 	hashSimbulo[QUANTIDADEDESIMBULOS];					// tabela HASH para simbulos reservados

/*	Classe estado, armazena cada estado do automato 
*	Atributos:
*		$indice(int) - Armazena o indice do estado 
*		$mensagem(string) - armazena a mensagem de erro ou o token de um estado final
*		$isFinal(bool) - indica se o estado é final
*		$retroceder(bool) - indica se o estado final exige que retroceda um caractere
*		$verificar(bool) - indica ambiguidade do estado final, ele pode simbolizar mais de uma coisa
*		$erro(bool) - indica se o estado final representa um erro
*	Metodos:
*		Contem os métodos set e get de cada atributo
* 	A classe também conta com um construtor com os parametros (int indice, bool isFinal, bool retroceder, bool verificar, bool erro)
*	e um construtor vazio.
*
*/
class Estado {
	private:
		int		indice;
		string 	mensagem;
		bool 	isFinal, retroceder, verificar, erro;
		
	public:
		int getIndice(){
			return this->indice;
		}
		bool getIsFinal(){
			return this->isFinal;
		}
		void setIndice(int indice){
			this->indice = indice;
		}
		void setIsFinal(bool isFinal){
			this->isFinal = isFinal;			
		}
		bool getRetroceder(){
			return this->retroceder;
		}
		void setRetroceder(bool retroceder){
			this->retroceder = retroceder;			
		}
		bool getVerificar(){
			return this->verificar;
		}
		void setErro(bool erro){
			this->erro = erro;			
		}
		bool getErro(){
			return this->erro;
		}
		string getMensagem(){
			return this->mensagem;
		}
		Estado(int indice, bool isFinal, bool retroceder, bool verificar, bool erro, string mensagem){
			this->indice		= indice;
			this->isFinal 		= isFinal;
			this->retroceder	= retroceder;
			this->verificar 	= verificar;
			this->erro 			= erro;
			this->mensagem 		= mensagem;
		}
		Estado(){}
};

//variaveis dependentes da classe estado
Estado	estadoAtual;										// inicia no estado 0 (inicial)
string	cadeia;												// cadeia em processamento
int		contadorDeTokens;									// contador de tokens utilizado na impressão 
char	letraAnterior, auxAtual, charactereAtual;			//  
bool 	retrocedido, iniciou;								//
Estado	*estados[QUANTIDADEDEESTADOS]; 						//cria um vetor com todos os estados exitentes

//Função para dizer se o caractere é uma letra
bool eUmaLetra(char letra){
	if(isalpha(letra))
		return true;
	return false;
}

//Função para dizer se o caractere é um numero
bool eUmNumero(char letra){
	if(isdigit(letra))
		return true;
	return false;
}

//Função para dizer se o caractere é um caractere não imprimivel
bool eUmCaractereNaoImprimivel(char letra){
	//cout<<"a letra e: "<<letra<<endl;
	if(letra == '\t' || letra == ' ')
		return true;
	else if (letra == '\n'){ 			//se encontrar uma quebra de linha, conta como mais uma linha
		if(!caractereConsumido){
			arquivo.incrementaLinha();
			caractereConsumido = true;
			nextTokenN = true;
		}		
		return true;
	}return false;
}

//Função que retorna o estado no automato que sera o resultante na transição de acordo com a tabela de transição de estados
int transicao(int estadoAtual, char charactereAtual){
	if(eUmaLetra(charactereAtual)){
		return tabelaTransicaoDeEstados[estadoAtual][0];
	}else if(eUmNumero(charactereAtual)){
		return tabelaTransicaoDeEstados[estadoAtual][1];
	}else if(charactereAtual == '.' ){
		return tabelaTransicaoDeEstados[estadoAtual][2];
	}else if(eUmCaractereNaoImprimivel(charactereAtual)){
		return tabelaTransicaoDeEstados[estadoAtual][3];
	}else if(charactereAtual == '=' ){
		return tabelaTransicaoDeEstados[estadoAtual][4];
	}else if(charactereAtual == '>' ){
		return tabelaTransicaoDeEstados[estadoAtual][5];
	}else if(charactereAtual == ':' ){
		return tabelaTransicaoDeEstados[estadoAtual][6];
	}else if(charactereAtual == '<' ){
		return tabelaTransicaoDeEstados[estadoAtual][7];
	}else if(estadoAtual == 0 && charactereAtual == EOF){// Caso especial para fim do codigo
		return 0;
	}else{
		return tabelaTransicaoDeEstados[estadoAtual][8];
	}
}

//método para encontrar a posição
int calculaPos(string cadeia, int tamanhoVet){
	int hash1, hash2, tamanho;		
	const char *stringConv = cadeia.c_str();
	tamanho = strlen(stringConv); 
	hash1 = cadeia.at(0);
	hash2 = cadeia.at(tamanho-1);
	int resultado = (hash2+hash1)%tamanhoVet;
	return resultado;
}

//método que constroi o hash
void calculaHash(int hash[], string stringVet[], int tamanhoVet){
	for(int i=0; i<tamanhoVet; i++){
		int resultado = calculaPos(stringVet[i], tamanhoVet);
		if(hash[resultado] == -1){
			hash [resultado] = i;
		}else{
			int auxResultado = resultado;
			while(hash[auxResultado] != -1){
				auxResultado++;
				if(auxResultado == tamanhoVet){
					auxResultado = 0;
				}
			}
			hash [auxResultado] = i;
		}		
	}
}

//função que imprime (ou não) a saida do analizador lexico, conforme definido na execução pela opção '-l'
void printLex(int n, string msg){
	if(printLexs == true){
		cout<<n<<msg<<endl;
	}
}

//Função utilizada para montar a tabela hash
void buildHash(){	
	//TABELA HASH DE PALAVRAS RESERVADAS
	for(int j=0; j<QUANTIDADEDEPALAVRASRESERVADAS; j++){	// inicia toda a tabela em -1
		hashPalavra[j] = -1;
	}
	for(int j=0; j<QUANTIDADEDESIMBULOS; j++){				// inicia toda a tabela em -1
		hashSimbulo[j] = -1;
	}
	
	calculaHash(hashPalavra, palavrasReservadas, QUANTIDADEDEPALAVRASRESERVADAS);
	calculaHash(hashSimbulo, simbolosReservados, QUANTIDADEDESIMBULOS);	
	
	//criador de estados
	int		indiceEstado 													= 0; //variavel auxiliar para percorrer o automato												
	int		estadosFinais[QUANTIDADEDEESTADOSFINAIS] 						= {2, 6, 7, 8, 9, 11, 12, 13, 14, 16, 17, 19, 20, 22};
	int		estadosFinaisQueRetrocedem[QUANTIDADEDEESTADOSQUERETROCEDEM] 	= {2, 6, 8, 9, 13, 17, 20, 22};
	int		estadosFinaisParaVerificar[QUANTIDADEDEESTADOSPARAVERIFICAR] 	= {2, 7};
	int		estadosFinaisErro[QUANTIDADEDEESTADOSERRO] 						= {7, 9, 22};
	bool	estadoAtualEFinal;
	bool	estadoAtualErro;
	bool	estadoAtualRetrocede;
	bool	estadoAtualVerificar;
	
	//laço para criar os objetos do automato
	while(indiceEstado < QUANTIDADEDEESTADOS){
		estadoAtualEFinal 		= false;
		estadoAtualRetrocede 	= false;
		estadoAtualVerificar 	= false;
		estadoAtualErro 		= false;
		for(int i = 0; i < QUANTIDADEDEESTADOSFINAIS; 			i++){
			if(indiceEstado == estadosFinais[i])				//se o estado atual estiver na lista de estados finais
				estadoAtualEFinal 		= true;
		}	
		for(int i = 0; i < QUANTIDADEDEESTADOSQUERETROCEDEM; 	i++){
			if(indiceEstado == estadosFinaisQueRetrocedem[i])	//se o estado atual estiver na lista de estados finais que retrocedem
				estadoAtualRetrocede 	= true;
		}
		for(int i = 0; i < QUANTIDADEDEESTADOSPARAVERIFICAR; 	i++){
			if(indiceEstado == estadosFinaisParaVerificar[i])	//se o estado atual estiver na lista de estados finais que exigem verificação
				estadoAtualVerificar 	= true;
		}
		for(int i = 0; i < QUANTIDADEDEESTADOSERRO; 			i++){
			if(indiceEstado == estadosFinaisErro[i])			//se o estado atual estiver na lista de estados que representam erro
				estadoAtualErro 		= true;
		}
		estados[indiceEstado] = new Estado(indiceEstado, estadoAtualEFinal, estadoAtualRetrocede, estadoAtualVerificar, estadoAtualErro, mensagensTokens[indiceEstado]);
		indiceEstado++;
	}
	
	estadoAtual 					= *estados[0];	//inicia no estado 0 (inicial)
	cadeia 							= "";
	contadorDeTokens 				= 1;
	numeroDeErros 					= 0;
	letraAnterior, auxAtual;
	retrocedido 					= false;
	iniciou = false;
}

//Função chamada pelo analizador sintatico para obter-se o proximo token
//não retorna nada, porém seta o objeto returnLex
void consomeToken(){

	if(!iniciou){
		charactereAtual = arquivo.proximaLetra(); 			//o primeiro caractere é a primeira letra do aquivo
	}
	iniciou = true;
	bool achou = false;
	caractereConsumido = false;
	while(achou == false){		
		if(estadoAtual.getIndice() == 0)												//somente consumiu caracteres não imprimiveis
			cadeia = "";																//retira da cadeia os caracteres não imprimiveis
			
		estadoAtual = *estados[transicao(estadoAtual.getIndice(), charactereAtual)]; 	//faz a transição dos estados
		
		if(estadoAtual.getIsFinal() == true){			// se o estado atual é final
			if(estadoAtual.getRetroceder() == true){		// se o estado atual exige um retrocesso
				auxAtual 		= charactereAtual;			// armazena o caractere atual em um auxiliar
				charactereAtual = letraAnterior;			// volta um caractere
				retrocedido 	= true;						// marca o status de retrocedido
			}else{											//se eu não retrocedi, o caractere pertence a cadeia
				cadeia = cadeia + charactereAtual;
			}
			if(estadoAtual.getVerificar() == true){ 	//se o estado atual exige uma verificação (estado ambiguo)
				achou = false;						// flag que indica que ainda não achou
				if(estadoAtual.getIndice() == 2){		// verificar se é palavra reservada
					int pos 	= calculaPos(cadeia, QUANTIDADEDEPALAVRASRESERVADAS);
					int auxPos	= pos;					//variavel para percorrer a palavra
					if(pos == 0) 						//condição especial para a pos ser != -1
						pos = QUANTIDADEDEPALAVRASRESERVADAS-1;					
					while(!achou && auxPos != pos-1){
						if(palavrasReservadas[hashPalavra[auxPos]] == cadeia){
							printLex(contadorDeTokens, "\t"+cadeia+"\t\t"+palavrasReservadas[hashPalavra[auxPos]]);
							tokenLex.setAll(cadeia, palavrasReservadas[hashPalavra[auxPos]], arquivo.getLinha(), retrocedido);
							contadorDeTokens++;
							achou = true;
							return;
						}else{
							auxPos++;
							if(auxPos == QUANTIDADEDEPALAVRASRESERVADAS)
								auxPos = 0;
						}
					}
					if(achou == false){													// se não achou, então se trata de um identificador
						printLex(contadorDeTokens, "\t"+cadeia+"\t\t"+"ID");
						tokenLex.setAll(cadeia, "ID", arquivo.getLinha(), retrocedido);
						contadorDeTokens++;
						return;
					}
				}
				if(estadoAtual.getIndice() == 7){										//verificar se é um simbulo reservado
					if(achou == false && charactereAtual == '{'){						// verifica se trata-se de um comentario
						while(charactereAtual != '}' && charactereAtual != EOF){		// percorre até achar o final do comentario
							charactereAtual = arquivo.proximaLetra();
							if(charactereAtual == EOF){
								cout<<arquivo.getUrl()<<": lex error : FORMACAO DE COMENTARIO INCORRETA, nao foi encontrado '}'"<<endl;
							}
						}
					}
					
					int pos = calculaPos(cadeia, QUANTIDADEDESIMBULOS);
					int auxPos = pos;
					if(pos == 0)
						pos = QUANTIDADEDESIMBULOS-1;
					while(!achou && auxPos != pos-1){
						if(simbolosReservados[hashSimbulo[auxPos]] == cadeia){
							printLex(contadorDeTokens, "\t"+cadeia+"\t\t"+simbolosReservados[hashSimbulo[auxPos]]);
							tokenLex.setAll(cadeia, simbolosReservados[hashSimbulo[auxPos]], arquivo.getLinha(), retrocedido);
							contadorDeTokens++;
							achou = true;
						}else{
							auxPos++;
							if(auxPos == QUANTIDADEDESIMBULOS)
								auxPos = 0;
						}
					}
					if(achou == false && cadeia != "{"){ // se não encontrou se trata de um caractere não aceito e não for comentario
						cout<<arquivo.getUrl()<<":"<<arquivo.getLinha()<<": lex error : in "<<cadeia<<"("<<contadorDeTokens<<") - "<<estadoAtual.getMensagem()<<endl;
						numeroDeErros++;
						contadorDeTokens++;
					}
				}
			}else{  // se o estado não exige verificação 
				if(estadoAtual.getErro() == true){
					cout<<arquivo.getUrl()<<":"<<arquivo.getLinha()<<": lex error : in "<<cadeia<<"("<<contadorDeTokens<<") - "<<estadoAtual.getMensagem()<<endl;
					numeroDeErros++;
					contadorDeTokens++;
				}else{
					printLex(contadorDeTokens, "\t"+cadeia+"\t\t"+estadoAtual.getMensagem());
					tokenLex.setToken(estadoAtual.getMensagem());
					tokenLex.setLine(arquivo.getLinha());
					tokenLex.setAll(cadeia, estadoAtual.getMensagem(),arquivo.getLinha(), retrocedido);
					contadorDeTokens++;
					achou = true;
				}
			}				
			estadoAtual = *estados[0];		//volta para o estado inicial
			cadeia = "";					//inicia uma nova cadeia
		}
		
		letraAnterior = charactereAtual;	//armazena a letra anterior
		
		if(retrocedido){ 					//se o caractere atual econtra-se retrocedido
			charactereAtual = auxAtual; 	//o caractere atual 
			retrocedido = false;			//tira o status de retrocedido
			nextTokenN = false;
		}else{											//se não foi retrocedido
			cadeia = cadeia + charactereAtual;			//coloca o caractere na cadeia
			charactereAtual = arquivo.proximaLetra();	//e le a proxima letra no arquivo
			if(letraAnterior == EOF){					//encontrou o fim do arquivo
				tokenLex.setAll(cadeia, "EOF", arquivo.getLinha(), retrocedido);
				return;
			}
		}
		
	}
	return;
}
