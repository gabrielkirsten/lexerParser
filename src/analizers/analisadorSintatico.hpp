void processVariables();
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

string *sincronizacao; //variavel para o lista de follows
int qtdsinc;           //quantidade da lista

//Função de erro, imprime o erro e sincroniza
//paramentros    -errorIndex = informa o tipo do erro
//               -estadoSintatico = informa o estado que ele parou para pudesse sincronizar
void throwErrorInSintaticState(int errorIndex, int estadoSintatico)
{
    numeroDeErros++;

    int lineAux = tokenLex.getLine();
    if (nextTokenN && tokenLex.getRetrocedido())
    {
        lineAux = lineAux - 1;
    }
    cout << arquivo.getUrl() << ":" << lineAux << ": error: " << erro[errorIndex] << " before '" << tokenLex.getCadeia() << "' " << endl;
    follow(estadoSintatico);

    bool end = false;
    while (true)
    { // enquanto não encontrar o simbulo de sincronização
        int auxToken;
        for (auxToken = 0; auxToken < qtdsinc - 1 && strcmp(sincronizacao[auxToken].c_str(), tokenLex.getToken().c_str()); auxToken++); //até o ultimo ou até encontrar o simbulo correspondente

        auxToken--;
        
        //se o simbulo em que se encontra após sair do FOR for o simbulo que ele esta buscando
        if (strcmp(sincronizacao[auxToken + 1].c_str(), tokenLex.getToken().c_str()) == 0)
        {
            break;
        }
        consomeToken();
    }
}

void getTokenOrThrowError(string token, int errorCode, int sintaticState)
{
    if (tokenLex.isEqual(token))
    {
        consomeToken();
    }
    else
    {
        throwErrorInSintaticState(errorCode, sintaticState);
    }
}

//função que mostra os proximos tokens confiaveis
//A função follow() foi criada baseando-se em conceitos da função FOLLOW utilizada no metodo do panico
//porém essa função não implementa completamente a função follow, pois esse não seria a melhor solução
void follow(int estadoSintatico)
{
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

    switch (estadoSintatico)
    {
    case 1:
        delete[] sincronizacao;
        qtdsinc = 0;
        sincronizacao = new string[qtdsinc];
        for (int i = 0; i < qtdsinc; i++)
        {
            sincronizacao[i] = auxsinc1[i];
        }
        break;

    case 2:
        delete[] sincronizacao;
        qtdsinc = 9;
        sincronizacao = new string[qtdsinc];
        for (int i = 0; i < qtdsinc; i++)
        {
            sincronizacao[i] = auxsinc2[i];
        }
        break;

    case 4:
        delete[] sincronizacao;
        qtdsinc = 9;
        sincronizacao = new string[qtdsinc];
        for (int i = 0; i < qtdsinc; i++)
        {
            sincronizacao[i] = auxsinc4[i];
        }
        break;
    case 5:
        delete[] sincronizacao;
        qtdsinc = 8;
        sincronizacao = new string[qtdsinc];
        for (int i = 0; i < qtdsinc; i++)
        {
            sincronizacao[i] = auxsinc5[i];
        }
        break;
    default: // se o erro ocorrer no final do programa
        qtdsinc = 0;
        break;
    }
}

// As funções abaixo foram definidas conforme a documentação da linguagem contida na página do professor
bool op_ad()
{
    if (tokenLex.isEqual("+"))
    {
        consomeToken();
        return true;
    }
    else if (tokenLex.isEqual("-"))
    {
        consomeToken();
        return true;
    }
    return false;
}

bool op_mult()
{
    if (tokenLex.isEqual("*"))
    {
        consomeToken();
        return true;
    }
    else if (tokenLex.isEqual("/"))
    {
        consomeToken();
        return true;
    }
    return false;
}

void fator()
{
    if (tokenLex.isEqual("ID"))
    {
        consomeToken();
    }
    else if (tokenLex.isEqual("INTEIRO"))
    {
        consomeToken();
    }
    else if (tokenLex.isEqual("REAL"))
    {
        consomeToken();
    }
    else if (tokenLex.isEqual("("))
    {
        consomeToken();
        expressao();
        if (tokenLex.isEqual(")"))
        {
            consomeToken();
        }
        else
        {
            throwErrorInSintaticState(5, 5);
        }
    }
    else
    {
        throwErrorInSintaticState(7, 5);
    }
}

void op_un()
{
    if (tokenLex.isEqual("+"))
    {
        consomeToken();
    }
    else if (tokenLex.isEqual("-"))
    {
        consomeToken();
    }
}

void mais_fatores()
{
    if (op_mult())
    {
        fator();
        mais_fatores();
    }
}

void termo()
{
    op_un();
    fator();
    mais_fatores();
}

void outros_termos()
{
    if (op_ad())
    {
        termo();
        outros_termos();
    }
}

void expressao()
{
    termo();
    outros_termos();
}

void relacao()
{
    if (tokenLex.isEqual("="))
    {
        consomeToken();
    }
    else if (tokenLex.isEqual("<>"))
    {
        consomeToken();
    }
    else if (tokenLex.isEqual(">="))
    {
        consomeToken();
    }
    else if (tokenLex.isEqual("<="))
    {
        consomeToken();
    }
    else if (tokenLex.isEqual(">"))
    {
        consomeToken();
    }
    else if (tokenLex.isEqual("<"))
    {
        consomeToken();
    }
}

void condicao()
{
    expressao();
    relacao();
    expressao();
}

void processAnotherVariables()
{
    if (tokenLex.isEqual(","))
    {
        consomeToken();
        processVariables();
    }
}

void processVariables()
{
    getTokenOrThrowError("ID", 8, 5);
    processAnotherVariables();
}

void mais_ident()
{
    if (tokenLex.isEqual(";"))
    {
        consomeToken();
        argumentos();
    }
}

void argumentos()
{
    if (tokenLex.isEqual("ID"))
    {
        consomeToken();
        mais_ident();
    }
    else
    {
        throwErrorInSintaticState(7, 5);
    }
}

void lista_arg()
{
    if (tokenLex.isEqual("("))
    {
        consomeToken();
        argumentos();
        if (tokenLex.isEqual(")"))
        {
            consomeToken();
        }
        else
        {
            throwErrorInSintaticState(5, 5);
        }
    }
}

void comandos()
{
    if (cmd())
    {
        if (tokenLex.isEqual(";"))
        {
            consomeToken();
        }
        else
        {
            throwErrorInSintaticState(6, 5);
        }
        comandos();
    }
}

void p_falsa()
{
    if (tokenLex.isEqual("else"))
    {
        consomeToken();
        if (!cmd())
        {
            throwErrorInSintaticState(15, 5);
        }
    }
}

bool cmd()
{
    if (tokenLex.isEqual("read"))
    {
        consomeToken();
        if (tokenLex.isEqual("("))
        {
            consomeToken();
            processVariables();
            if (tokenLex.isEqual(")"))
            {
                //OK
                consomeToken();
            }
            else
            {
                throwErrorInSintaticState(5, 5);
            }
        }
        else
        {
            throwErrorInSintaticState(4, 5);
        }
        return true;
    }
    else if (tokenLex.isEqual("write"))
    {
        consomeToken();
        if (tokenLex.isEqual("("))
        {
            consomeToken();
            processVariables();
            if (tokenLex.isEqual(")"))
            {
                consomeToken();
            }
            else
            {
                throwErrorInSintaticState(5, 5);
            }
        }
        else
        {
            throwErrorInSintaticState(4, 5);
        }
        return true;
    }
    else if (tokenLex.isEqual("while"))
    {
        consomeToken();
        condicao();
        if (tokenLex.isEqual("do"))
        {
            consomeToken();
        }
        else
        {
            throwErrorInSintaticState(14, 5);
        }
        if (!cmd())
        { //se não houver comanda, retorna erro
            throwErrorInSintaticState(15, 5);
        }
        return true;
    }
    else if (tokenLex.isEqual("if"))
    {
        consomeToken();
        condicao();
        if (tokenLex.isEqual("then"))
        {
            consomeToken();
        }
        else
        {
            throwErrorInSintaticState(12, 5);
        }
        if (!cmd())
        {
            throwErrorInSintaticState(15, 5);
        }
        p_falsa();
        return true;
    }
    else if (tokenLex.isEqual("ID"))
    {
        consomeToken();
        if (tokenLex.isEqual(":="))
        {
            consomeToken();
            expressao();
        }
        else
        {
            lista_arg();
        }
        return true;
    }
    else if (tokenLex.isEqual("begin"))
    {
        consomeToken();
        comandos();
        if (tokenLex.isEqual("end"))
        {
            consomeToken();
        }
        return true;
    }
    else
    { //não encontrou comandos
        return false;
    }
}

void processVariableType()
{
    if (tokenLex.isEqual("real"))
    {
        consomeToken();
    }
    else if (tokenLex.isEqual("integer"))
    {
        consomeToken();
    }
    else
    {
        throwErrorInSintaticState(10, 4);
    }
}

void processVariableDeclaration()
{
    if (tokenLex.isEqual("var"))
    {
        consomeToken();
        processVariables();
        getTokenOrThrowError(":", 12, 4);
        processVariableType();
        getTokenOrThrowError(";", 6, 4);
        processVariableDeclaration();
    }
}

void mais_par()
{
    if (tokenLex.isEqual(";"))
    {
        consomeToken();
    }
}

void lista_par()
{
    processVariables();
    if (tokenLex.isEqual(":"))
    {
        consomeToken();
    }
    else
    {
        throwErrorInSintaticState(12, 4);
    }
    processVariableType();
    mais_par();
}

void processParameters()
{
    if (tokenLex.isEqual("("))
    {
        consomeToken();
        lista_par();
        getTokenOrThrowError(")", 5, 2);
    }
    else
    {
        throwErrorInSintaticState(4, 2);
    }
}

void processProcedureBody()
{
    processVariableDeclaration();
    getTokenOrThrowError("begin", 2, 4);
    comandos();
    getTokenOrThrowError("end", 3, 4);
    getTokenOrThrowError(";", 6, 4);
}

void processProcedureDeclaration()
{
    if (tokenLex.isEqual("procedure"))
    {
        consomeToken();
        getTokenOrThrowError("ID", 1, 2);
        processParameters();
        getTokenOrThrowError(";", 6, 2);
        processProcedureBody();
        processProcedureDeclaration();
    }
}

void processDeclaration()
{
    processVariableDeclaration();
    processProcedureDeclaration();
}

void processBody()
{
    processDeclaration();
    getTokenOrThrowError("begin", 2, 4);
    comandos();
    getTokenOrThrowError("end", 3, 1);
}

void processProgramHeader()
{
    getTokenOrThrowError("program", 0, 2);
    getTokenOrThrowError("ID", 0, 2);
    getTokenOrThrowError(";", 6, 4);
    processBody();
    getTokenOrThrowError(".", 13, 1);
}

void analiseSintatica()
{
    consomeToken();
    processProgramHeader();

    while (tokenLex.getToken() != "EOF")
    {
        numeroDeErros++;
        cout << "Token encontrado fora do escopo do programa: " << tokenLex.getCadeia() << endl;
        consomeToken();
    }
}