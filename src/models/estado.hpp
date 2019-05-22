/*
*	Classe estado, armazena cada estado do automato 
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

class Estado
{
private:
    int indice;
    string mensagem;
    bool isFinal;
    bool retroceder;
    bool verificar;
    bool erro;

public:
    int getIndice()
    {
        return this->indice;
    }

    bool getIsFinal()
    {
        return this->isFinal;
    }

    void setIndice(int indice)
    {
        this->indice = indice;
    }

    void setIsFinal(bool isFinal)
    {
        this->isFinal = isFinal;
    }

    bool getRetroceder()
    {
        return this->retroceder;
    }

    void setRetroceder(bool retroceder)
    {
        this->retroceder = retroceder;
    }

    bool getVerificar()
    {
        return this->verificar;
    }

    void setErro(bool erro)
    {
        this->erro = erro;
    }

    bool getErro()
    {
        return this->erro;
    }

    string getMensagem()
    {
        return this->mensagem;
    }

    Estado(int indice, bool isFinal, bool retroceder, bool verificar, bool erro, string mensagem)
    {
        this->indice = indice;
        this->isFinal = isFinal;
        this->retroceder = retroceder;
        this->verificar = verificar;
        this->erro = erro;
        this->mensagem = mensagem;
    }

    Estado() {}
};