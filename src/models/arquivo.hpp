/* 
* Classe Arquivo
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
*	A classe também conta com um construtor que ja define a url do arquivo e zera o contador de linhas 
*
*/

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