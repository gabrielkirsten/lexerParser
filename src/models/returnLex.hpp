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