// assinatura de métodos
void variaveis();
void expressao();
bool cmd();
void argumentos();
void lista_par();
void follow(int estadoSintatico);

string erro[] = {"expected 'program'",
				"expected 'ID'",
				"expected 'begin'",
				"expected 'end'",
				"expected '('",
				"expected ')'",
				"expected ';'",
				"expected 'arg'",
				"expected 'primary-expression'", 
				"expected 'program'", 
				"expected 'var-type'", 
				"expected 'then'",
				"expected ':'", 
				"expected '.'",
				"expected 'do'",
				"expected 'command'"};
// Indice de erros :
//	0 - expected program 
//	1 - expected ID
//	2 - expected begin
//	3 - expected end
//	4 - expected (
//	5 - expected )
//	6 - expected ; 
//	7 - expected arg
//	8 - expected primary-expression
//	9 - expected program
//	10 - expected var-type
//	11 - expected then
//	12 - expected :
//	13 - expected .
//	14 - expected do
//	15 - expected command

string * sincronizacao; //variavel para o lista de follows
int qtdsinc; //quantidade da lista

//Função de erro, imprime o erro e sincroniza
//paramentros	-num_erro = informa o tipo do erro
//				-estadoSintatico = informa o estado que ele parou para podese sincronizar
void error_f(int num_erro, int estadoSintatico){
	numeroDeErros++;
	int lineAux = tokenLex.getLine();
	if(nextTokenN && tokenLex.getRetrocedido()){
		lineAux = lineAux - 1;
	}
	cout<<arquivo.getUrl()<<":"<<lineAux<<": error: "<<erro[num_erro]<<" before '"<<tokenLex.getCadeia()<<"' "<<endl;
	follow(estadoSintatico);
	bool fim = false;
	while(!fim){ // enquanto não encontrar o simbulo de sincronização 
		int auxToken;
		for(auxToken = 0; auxToken<qtdsinc-1 && strcmp(sincronizacao[auxToken].c_str(),tokenLex.getToken().c_str()); auxToken++); //até o ultimo ou até encontrar o simbulo correspondente
		auxToken = auxToken - 1;
		//se o simbulo em que se encontra após sair do FOR for o simbulo que ele esta buscando
		if(strcmp(sincronizacao[auxToken+1].c_str(), tokenLex.getToken().c_str()) == 0){
			fim = true; //encerra o laço
			break;
		}
		consomeToken();
	}
}

//função que mostra os proximos tokens confiaveis
//A função follow() foi criada baseando-se em conceitos da função FOLLOW utilizada no metodo do panico
//porém essa função não implementa completamente a função follow, pois esse não seria a melhor solução
void follow(int estadoSintatico){
	//caso em que sincroniza com o token 'end' e '.'
	//esse caso é utilizado ao final do progrma
	string auxsinc1[] = {"end", "."};
	//caso em que sincroniza com tokens a partir do ponto de ID do programa
	//esse caso é utilizado quando o erro ocorre antes da declaração do ID do programa
	string auxsinc2[] = {";", "var", "procedure", "begin", "if", "read", "write", "while", "end"};	
	//caso em que sincroniza com qualquer comando, declaração de parametro e de variavel 
	//esse caso é utilizado quando o erro é encontrado fora do corpo do programa
	string auxsinc4[] = {"var", "procedure", "begin", "if", "read", "write", "while", "end"};	
	//caso em que sincroniza com qualquer declamaração de comando
	//esse caso é utilizado principamente quando o erro ocorre dentro do corpo do programa
	string auxsinc5[] = {";", "ID", "begin", "if", "read", "write", "while", "end"};	

	switch (estadoSintatico){
		case 1: 
			delete [] sincronizacao;
			qtdsinc = 0;
			sincronizacao = new string[qtdsinc];
			for(int i=0;i<qtdsinc;i++){
				sincronizacao[i] = auxsinc1[i];
			}
			break;
			
		case 2:
			delete [] sincronizacao;
			qtdsinc = 9;
			sincronizacao = new string[qtdsinc];
			for(int i=0;i<qtdsinc;i++){
				sincronizacao[i] = auxsinc2[i];
			}
			break;
			
		case 4:
			delete [] sincronizacao;
			qtdsinc = 9;
			sincronizacao = new string[qtdsinc];
			for(int i=0;i<qtdsinc;i++){
				sincronizacao[i] = auxsinc4[i];
			}
			break;
		case 5:
			delete [] sincronizacao;
			qtdsinc = 8;
			sincronizacao = new string[qtdsinc];
			for(int i=0;i<qtdsinc;i++){
				sincronizacao[i] = auxsinc5[i];
			}
			break;
		default: // se o erro ocorrer no final do programa
			qtdsinc = 0;
			break;

	}
}
	
// As funções abaixo foram definidas conforme a documentação da linguagem contida na página do professor
bool op_ad(){
	if(tokenLex.getToken() == "+"){
		consomeToken();
		return true;
	}else if(tokenLex.getToken() == "-"){
		consomeToken();
			return true;
	}
	return false;
}

bool op_mult(){
	if(tokenLex.getToken() == "*"){
		consomeToken();
		return true;
	}else if(tokenLex.getToken() == "/"){
		consomeToken();
		return true;
	}
	return false;
}

void fator(){
	if(tokenLex.getToken() == "ID"){
		consomeToken();
	}else if(tokenLex.getToken() == "INTEIRO"){
		consomeToken();
	}else if(tokenLex.getToken() == "REAL"){
		consomeToken();
	}else if(tokenLex.getToken() == "("){
		consomeToken();
		expressao();
		if(tokenLex.getToken() == ")"){
			consomeToken();
		}else{
			error_f(5, 5);
		}
	}else{
		error_f(7, 5);
	}
}

void op_un(){
	if(tokenLex.getToken() == "+"){
		consomeToken();
	}
	else if(tokenLex.getToken() == "-"){
		consomeToken();
	}
}

void mais_fatores(){
	if(op_mult()){
		fator();
		mais_fatores();
	}
}

void termo(){
	op_un();
	fator();
	mais_fatores();
}

void outros_termos(){
	if(op_ad()){
		termo();
		outros_termos();
	}
}

void expressao(){
	termo();
	outros_termos();
}

void relacao(){
	if(tokenLex.getToken() == "="){
		consomeToken();
	}else if(tokenLex.getToken() == "<>"){
		consomeToken();
	}else if(tokenLex.getToken() == ">="){
		consomeToken();
	}else if(tokenLex.getToken() == "<="){
		consomeToken();
	}else if(tokenLex.getToken() == ">"){
		consomeToken();
	}else if(tokenLex.getToken() == "<"){
		consomeToken();
	}
}

void condicao(){
	expressao();
	relacao();
	expressao();
}

void mais_var(){
	if(tokenLex.getToken() == ","){
		consomeToken();
		variaveis();
	}	
}

void variaveis(){
	if(tokenLex.getToken() == "ID"){
		consomeToken();
	}else{
		error_f(8, 5);
	}
	mais_var();
}

void mais_ident(){
	if(tokenLex.getToken() == ";"){
		consomeToken();
		argumentos();
	}
}

void argumentos(){
	if(tokenLex.getToken() == "ID"){
		consomeToken();
		mais_ident();
	}else{
		error_f(7, 5);
	}
}

void lista_arg(){
	if(tokenLex.getToken() == "("){
		consomeToken();
		argumentos();
		if(tokenLex.getToken() == ")"){
			consomeToken();
		}else{
			error_f(5, 5);
		}
	}
}

void comandos(){
	if(cmd()){
		if(tokenLex.getToken() == ";"){
			consomeToken();
		}else{
			error_f(6, 5);
		}
		comandos();
	}
}

void p_falsa(){
	if(tokenLex.getToken() == "else"){
		consomeToken();
		if(!cmd()){
			error_f(15,5);
		}
	}
}

bool cmd(){
	if(tokenLex.getToken() == "read"){
		consomeToken();
		if(tokenLex.getToken() == "("){
			consomeToken();
			variaveis();
			if(tokenLex.getToken() == ")"){
				//OK
				consomeToken();
			}else{
				error_f(5, 5);
			}
		}else{
			error_f(4, 5);
		}
		return true;
	}else if(tokenLex.getToken() == "write"){
		consomeToken();
		if(tokenLex.getToken() == "("){
			consomeToken();
			variaveis();
			if(tokenLex.getToken() == ")"){
				consomeToken();
			}else{
				error_f(5, 5);
			}
		}else{
			error_f(4, 5);
		}		
		return true;
	}else if(tokenLex.getToken() == "while"){
		consomeToken();
		condicao();
		if(tokenLex.getToken() == "do"){
			consomeToken();
		}else{
			error_f(14, 5);
		}
		if(!cmd()){ //se não houver comanda, retorna erro
			error_f(15,5);
		}
		return true;
	}else if(tokenLex.getToken() == "if"){
		consomeToken();
		condicao();
		if(tokenLex.getToken() == "then"){
			consomeToken();
		}else{
			error_f(12, 5);
		}
		if(!cmd()){
			error_f(15,5);
		}
		p_falsa();
		return true;
	}else if(tokenLex.getToken() == "ID"){
		consomeToken();
		if(tokenLex.getToken() == ":="){
			consomeToken();
			expressao();
		}else{
			lista_arg();
		}		
		return true;
	}else if(tokenLex.getToken() == "begin"){
		consomeToken();
		comandos();
		if(tokenLex.getToken() == "end"){
			consomeToken();
		}
		return true;
	}else{//não encontrou comandos
		return false;
	}
	
}

void tipo_var(){
	if(tokenLex.getToken() == "real"){
		consomeToken();
	}else if(tokenLex.getToken() == "integer"){
		consomeToken();
	}else{
		error_f(10, 4);
	}
}

void dc_v(){
	if(tokenLex.getToken() == "var"){
		consomeToken();
		variaveis();
		if(tokenLex.getToken() == ":"){
			consomeToken();
		}else{
			error_f(12, 4);
		}		
		tipo_var();
		if(tokenLex.getToken() == ";"){
			consomeToken();
			
		}else{
			error_f(6, 4);
		}	
		dc_v();
	}
}

void mais_par(){
	if(tokenLex.getToken() == ";"){
		consomeToken();
	}
}

void lista_par(){
	variaveis();
	if(tokenLex.getToken() == ":"){
		consomeToken();
	}else{
		error_f(12, 4);
	}
	tipo_var();
	mais_par();
}

void parametros(){
	if(tokenLex.getToken() == "("){
		consomeToken();
		lista_par();
		if(tokenLex.getToken() == ")"){
			consomeToken();
		}else{
			error_f(5, 2);
		}
	}else{
		error_f(4, 2);
	}
}

void dc_loc(){
	dc_v();
}

void corpo_p(){
	dc_loc();
	if(tokenLex.getToken() == "begin"){
		consomeToken();
	}else{
		error_f(2, 4);
	}
	comandos();
	if(tokenLex.getToken() == "end"){
		consomeToken();
	}else{
		error_f(3, 4);
	}
	if(tokenLex.getToken() == ";"){
		consomeToken();
	}else{
		error_f(3, 4);
	}
}

void dc_p(){
	if(tokenLex.getToken() == "procedure"){
		consomeToken();
		if(tokenLex.getToken() == "ID"){
			consomeToken();
		}
		else{
			error_f(1, 2);
		}
		parametros();	
		if(tokenLex.getToken() == ";"){
			consomeToken();
		}
		else{
			error_f(6, 2);
		}
		corpo_p();
		dc_p();
	}
}

void dc(){
	dc_v();
	dc_p();
}

void corpo(){
	dc();
	if(tokenLex.getToken() == "begin"){
		consomeToken();
	}else{
		error_f(2, 4);
	}
	comandos();
	if(tokenLex.getToken() == "end"){
		consomeToken();
	}else{
		error_f(3, 1);
	}
	
}

void program(){
	if(tokenLex.getToken() == "program"){
		consomeToken();	
	}else{
		error_f(0, 2);
	}
	if(tokenLex.getToken() == "ID"){
		consomeToken();	
	}else{
		error_f(1, 4);
	}
	if(tokenLex.getToken() == ";"){
		consomeToken();	
	}else{
		error_f(6, 4);
	}

	corpo();

	if(tokenLex.getToken() == "."){
		consomeToken();
	}else{
		error_f(13, 1);
	}
}

//metodo de analise, pede o primeiro token e começa o analizador
void analiseSintatica(){

	consomeToken();	
	program();//program é o primeiro buscado
	while(tokenLex.getToken() != "EOF"){
		numeroDeErros++;
		cout<<"Token encontrado fora do escopo do programa: "<<tokenLex.getCadeia()<<endl;
		consomeToken();
	}


}