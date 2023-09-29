%{

#include <iostream>
#include "draw_ast.hpp"

int  yylex();
void yyerror(const char *msg);


%}

/*  
YACC fundamentally works by asking lex to get the next token, which it returns as
an object of type "yystype".  But
the next token could be of an arbitrary data type, so you can define a C union to
hold each of the types of tokens that lex could return, and yacc will typedef
"yystype" as the union instead of its default (int): 
*/
%union {
    float fval;
    struct AST* a;  // for AST
}

// DECLARACIONES DE TOKENS
%token <fval> NUMBER
%token <fval> DECIMAL
%token EOL
%token ADD SUB MUL DIV ABS AP CP COMA
%token ROTATION MOVE ON OF REP DOUBLEPOINT

%type<a> exp factor term func bucle

%%

linea: 
 | linea EOL {          /*Show curren pointer orientation when enter is pressed*/
    AST* orien = newAST('o', nullptr, nullptr, nullptr);
    std::cout<<"orien -> "<<eval(orien)<<"\n\n";
    treeFree(orien); 
    std::cout<<"> ";
 }
 | linea exp EOL {      /*Show expression result*/
    std::cout<<"= "<<eval($2)<<"\n\n";
    std::cout<<"\tATS\n\n";
    printTree($2); treeFree($2); std::cout<<"\n\n> ";
    numTab = 0;
 }
 ;
exp: factor
 | exp ADD factor  { $$ = newAST('+', "sumar", $1, $3); }
 | exp SUB factor  { $$ = newAST('-', "restar", $1, $3); }
 | func
 | bucle
 ;
factor: term
 | factor MUL term { $$ = newAST('*', "multiplicar", $1, $3); }
 | factor DIV term { $$ = newAST('/', "dividir", $1, $3); }
 ;
term: NUMBER { $$ = newNum($1);            }
 | DECIMAL   { $$ = newNum($1);            }
 | SUB term  { $$ = newAST('n', "valor negativo", $2, nullptr); }
 | ABS term  { $$ = newAST('|', "valor absoluto", $2, nullptr); }
 | AP exp CP { $$ = $2;                    }
 ;
func: ROTATION AP exp CP        { $$ = newAST('r', "rotar", $3, nullptr); }
 | ROTATION AP exp COMA exp CP  { $$ = newAST('R', "rotar", $3, $5);   }
 | MOVE AP exp CP               { $$ = newAST('m', "mover", $3, nullptr); }
 | MOVE AP exp COMA exp CP      { $$ = newAST('M', "mover", $3, $5);   }
 | ON { $$ = newAST('N', "tracerOn", nullptr, nullptr); }
 | OF { $$ = newAST('F', "tracerOf", nullptr, nullptr); }
 ;
bucle: REP NUMBER DOUBLEPOINT ROTATION AP exp CP      { $$ = newLoop($2, "rotar1", $6, nullptr); }
 | REP NUMBER DOUBLEPOINT ROTATION AP exp COMA exp CP { $$ = newLoop($2, "rotar2", $6, $8);      }
 | REP NUMBER DOUBLEPOINT MOVE AP exp CP              { $$ = newLoop($2, "mover1", $6, nullptr); }
 | REP NUMBER DOUBLEPOINT MOVE AP exp COMA exp CP     { $$ = newLoop($2, "mover2", $6, $8);      }
 ;
%%

int main(void)
{
    yyparse();
}

void yyerror(const char *msg){
    fprintf(stderr, "%s\n", msg);
}
