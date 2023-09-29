
%{
#include "draw-stax.tab.h"
%}

numbers [0-9]+

%%

"+"         { return ADD;    }
"-"         { return SUB;    }
"*"         { return MUL;    }
"/"         { return DIV;    }
"|"         { return ABS;    }
"("         { return AP;     }
")"         { return CP;     }
","			{ return COMA;   }
\n          { return EOL;    }
[ \t]       { /*ignoring*/   }
{numbers}   				{ yylval.fval = atof(yytext); return NUMBER; }
{numbers}+"."{numbers}+   	{ yylval.fval = atof(yytext); return DECIMAL; }
.           { /*ignoring*/   }
"mover"		{ return MOVE;   }
"encender"  { return ON; }
"apagar"    { return OF; }

%%