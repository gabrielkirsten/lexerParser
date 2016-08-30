# Lexer Parser

This project is a software function to perform lexical and syntactic analysis

      Ps. Description in PT-BR

Atividade - Compiladores - Analisador Lexico e sintático

------------------------------------------------------------------------
# Descrição

Desenvolvido em C++ com o compilador g++ v. 5.0.4
Sistema Operacional: Ubuntu 16.04 LTS 

Software desenvolvido para disciplina de Compiladores II.

#Descrição da atividade
Implementar analisador SINTÁTICO utilizando a linguagem algorítmica LALGOL
Método: Análise Sintática Descendente Preditiva Recursiva
Descrição da linguagem: Arquivo linguagem lalgol.pdf no site
Livre uso de qualquer linguagem de programação (não conter métodos prontos)
Realizar Integração com Analisador Léxico desenvolvido em Compiladores I

Tratamento de erros:
Pânico – Ponto confiável para continuar a análise sintática – Análise da Linguagem LALGOL

------------------------------------------------------------------------
#Utilização
	codigo para compilação:
	   $ g++ main.cpp -o compilador

	codigo para execução:
	   $ ./compilador -p teste

------------------------------------------------------------------------		
#Estrutura de diretórios
    .
    |-- lexerParser
    |	|-- analisadorLexico.h (biblioteca criada para o lexer)
    |	|-- analisadorSintatico.h (biblioteca criada para o parser)
    |	|-- main.cpp  (main)
    | |-- teste.txt (arquivo de teste de linguagem
    | |-- teste1.txt (outro arquivo de teste da linguagem)

------------------------------------------------------------------------
#Parâmetros

  Parâmetro                       Descrição
    -p <caminho>                    Especificar caminho             
    -l                              Ativar a saida do analizador lexico
    --h                             Ajuda
  
------------------------------------------------------------------------
