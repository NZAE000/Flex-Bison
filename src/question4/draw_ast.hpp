#pragma once

inline int numTab {-1};

/* Nodo del AST */
struct AST {
	int nodetype{0};				// ID (EJ: '+' id for sum or 'B' id for loops)
	const char* action {nullptr};	// Action name (EJ: 2 '+' 2 => "sumar")
	int reps {0};					// for loops
	AST *l {nullptr};				// left operand
	AST *r {nullptr};				// right operand
};


/* Para las hojas del AST */
struct numVal {
	int nodetype{0}; 	// It use 'K' type for constants
	float number{0}; 	// Store value
};

/* Construye el AST */
AST *newAST(int nodetype, const char* action, AST *l, AST *r);
AST *newNum(float d);
AST *newLoop(int reps, const char* action, AST *l, AST *r);

/* Evalua un AST */
float eval(AST*);

/* Para borrar (y liberar memoria) un AST */
void treeFree(AST*);

void printTree(AST*);

void yyerrorV2(const char *msg);

