%{

#include <iostream>
#include <sys/renderSys.hpp>
#include <sys/physicsSys.hpp>
#include <sys/AISys.hpp>
#include <util/calculation.hpp>
#include "pointer.hpp"

// DIMENSIONS AND NUMBER OF FRAMES PER SECOND 
// OF UPDATE OF THE RENDERING SYSTEM
constexpr uint32_t WIDTH    { 640 };
constexpr uint32_t HEIGHT   { 360 };
constexpr float FPS         {  60 };
constexpr float OUT_LIMITS  {  -1 };

// POINTER
Pointer_t pointer;

// SYSTEMS FOR RENDERING AND MOVING POINTER
RenderSys_t  renderSys { WIDTH, HEIGHT };
PhysicsSys_t phySys;
AISys_t      aiSys;

// FUNCTION TO QUESTION 1
void mover(float, float);

int  yylex();
void yyerror(const char *msg);


%}

// YACC fundamentally works by asking lex to get the next token, which it returns as
// an object of type "yystype".  But
// the next token could be of an arbitrary data type, so you can define a C union to
// hold each of the types of tokens that lex could return, and yacc will typedef
// "yystype" as the union instead of its default (int):
%union {
    float fval;
}

/* DECLARACIONES DE TOKENS */
%token <fval> NUMBER
%token <fval> DECIMAL
%token EOL
%token ADD SUB MUL DIV ABS AP CP COMA
%token MOVE ON OF

%type<fval> exp factor term func

%%

linea: 
 | linea EOL
 | linea exp EOL
 ;
exp: factor
 | exp ADD factor  { $$ = CALC::suma($1, $3);    }
 | exp SUB factor  { $$ = CALC::resta($1, $3);   }
 | func            {}
 ;
factor: term
 | factor MUL term { $$ = CALC::multi($1, $3); }
 | factor DIV term { $$ = CALC::divi($1, $3);  }
 ;
term: NUMBER
 | DECIMAL
 | SUB term  { $$ = -$2;             }
 | ABS term  { $$ = $2>=0? $2 : -$2; }
 | AP exp CP { $$ = $2;              }
 ;
func: MOVE AP term COMA term CP { mover($3, $5); }
 | ON  { renderSys.setTracer(true);  }
 | OF  { renderSys.setTracer(false); }
 ;
%%

int main(void)
{
    renderSys.update(pointer);
    yyparse();
}

void yyerror(const char *msg){
    fprintf(stderr, "%s\n", msg);
}

void mover(float x, float y)
{   
    float degreesT = [&]() // DEGREES OF ROTATION TO TARGET
    {
        float degrees {0.0};

        // VERIFY IF POINT IS IN RENDER BOUNDRING
        if (!renderSys.inBounds(x, y)) { 
            yyerror("Invalid movement: outside the limits of the frame"); 
            degrees = OUT_LIMITS; 
        }
        else {
            // DISTANCE TARGET
            auto const [disxT, disyT, distanceT] = CALC::distanceToPoint(pointer.phycmp.point, { static_cast<float>(x), static_cast<float>(y) });

            float orienTarget  { CALC::calculateAngle(disxT, disyT) };

            float angularSteer { CALC::alignAngle(pointer.phycmp.orientation, orienTarget) };
            pointer.phycmp.orientation += angularSteer;

            CALC::clampAngle(pointer.phycmp.orientation);
            degrees = CALC::radianToDegrees(pointer.phycmp.orientation);
        }

        return degrees;

    }(); // auto invoke lamda

    if (degreesT == OUT_LIMITS) return;   // CHECK

    // Get AI component
    auto& aicmp { pointer.aicmp };

    // SET POINT TARGET
    aicmp.pointTarget.x = x;
    aicmp.pointTarget.y = y;
    aicmp.targetActive  = true;

    // MOVE POINTER WHILE IT ISN'T IN TARGET
    while (aicmp.targetActive)
    { 
        renderSys.update(pointer);
        phySys.update(pointer.phycmp, 1/FPS);
        aiSys.update(pointer);
    }
}