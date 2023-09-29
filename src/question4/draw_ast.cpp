#include <cstdio>
#include <stdexcept>
#include <string>
#include <sys/renderSys.hpp>
#include <sys/physicsSys.hpp>
#include <sys/AISys.hpp>
#include <util/calculation.hpp>
#include "pointer.hpp"
#include "draw_ast.hpp"

// ##############################################################

// DIMENSIONS AND NUMBER OF FRAMES PER SECOND 
// OF UPDATE OF THE RENDERING SYSTEM
constexpr uint32_t WIDTH    { 640 };
constexpr uint32_t HEIGHT   { 360 };
constexpr float FPS         {  60 };
constexpr float OUT_LIMITS  {  -1 };

// POINTER
Pointer_t pointer;

// SYSTEMS  FOR RENDERING AND MOVING POINTER
RenderSys_t  renderSys { WIDTH, HEIGHT };
PhysicsSys_t phySys;
AISys_t      aiSys;

// FUNCTIONS  TO QUESTION 4
float rotar(float);
float rotar(float, float);
float mover(float, float);
float mover(float);

// ##############################################################

AST* newAST(int nodetype, const char* action, AST *l, AST *r)
{
	AST *a = new AST();

	if(!a) {
    	throw std::runtime_error("out of space");
    	exit(0);
  	}

  	a->nodetype = nodetype;
  	a->action   = action;
  	a->l = l;
  	a->r = r;

	return a;
}

AST* newNum(float d)
{
  	numVal *a = new numVal();

  	if(!a) {
    	throw std::runtime_error("out of space");
    	exit(0);
  	}

  	a->nodetype = 'K';
  	a->number   = d;

  	return (AST*) a;
}

AST *newLoop(int reps, const char* action, AST *l, AST *r)
{
    AST *a = new AST();

    if(!a) {
        throw std::runtime_error("out of space");
        exit(0);
    }

    a->nodetype = 'B';
    a->action   = action;
    a->reps     = reps;
    a->l = l;
    a->r = r;

    return a;
}

float eval(AST* a)
{
	float v {0.0}; // Valor calculado para el sub arbol

  	switch(a->nodetype)
  	{
    case 'K': v = ((numVal*)a)->number;    break;
    case '+': v = eval(a->l) + eval(a->r); break;
    case '-': v = eval(a->l) - eval(a->r); break;
    case '*': v = eval(a->l) * eval(a->r); break;
    case '/': v = eval(a->l) / eval(a->r); break;
    case '|': v = eval(a->l);  if(v < 0) v = -v; break;
    case 'n': v = -eval(a->l); break;
    case 'M': v = mover(eval(a->l), eval(a->r)); break;
    case 'm': v = mover(eval(a->l)); break;
    case 'R': v = rotar(eval(a->l), eval(a->r)); break;
    case 'r': v = rotar(eval(a->l)); break;
    case 'N': { renderSys.setTracer(true);  v = CALC::radianToDegrees(pointer.phycmp.orientation); break; }
    case 'F': { renderSys.setTracer(false); v = CALC::radianToDegrees(pointer.phycmp.orientation); break; }
    case 'B':{
        if      (std::string(a->action) == "mover1") for (short i=0; i<a->reps; ++i) v=mover(eval(a->l));
        else if (std::string(a->action) == "mover2") for (short i=0; i<a->reps; ++i) v=mover(eval(a->l), eval(a->r));
        else if (std::string(a->action) == "rotar1") for (short i=0; i<a->reps; ++i) v=rotar(eval(a->l));
        else if (std::string(a->action) == "rotar2") for (short i=0; i<a->reps; ++i) v=rotar(eval(a->l), eval(a->r));
        break;
    }
    // Show current pointer orientation
    case 'o': v = CALC::radianToDegrees(pointer.phycmp.orientation); break;
    default: std::printf("internal error: bad node %c\n", a->nodetype);
  	}

 	return v;
}

void printTree(AST* a)
{
	switch(a->nodetype)
	{
    // Nodo hoja
    case 'K': { // numbers
    	for (int t=0; t<numTab; ++t) std::printf("\t");
    	std::printf("%.2f ", ((numVal*) a)->number);
    	break;
    }
	case 'N':
	case 'F': std::printf("%s ", a->action); break;
   
    // Nodo con dos hijos
    case '+':
    case '-':
    case '*':
    case '/': std::printf("\t%c\n\n", a->nodetype); ++numTab; printTree(a->l); printTree(a->r); break;
	
    case 'M':
	case 'R': std::printf("\t%s\n\n", a->action); ++numTab; printTree(a->l); printTree(a->r); break;
    
    // Nodo con solo un hijo
    case '|':
    case 'n': std::printf("\t%c\n\n", a->nodetype); printTree(a->l);
    break;
    case 'm':
    case 'r': std::printf("\t%s\n\n", a->action); printTree(a->l); break;

    // loop node
    case 'B': 
    {
        std::printf("\t%s %d:\n\n", "repetir", a->reps);
        std::printf("\t%s\n\n", a->action);
        ++numTab;

        if      (std::string(a->action) == "mover1") printTree(a->l);
        else if (std::string(a->action) == "mover2") { printTree(a->l); printTree(a->r); } 
        else if (std::string(a->action) == "rotar1") printTree(a->l);
        else if (std::string(a->action) == "rotar2") { printTree(a->l); printTree(a->r); }
        break;
    }
    default:  std::printf("printTree – error\n");
  	}
}

void treeFree(AST* a) 
{
  	switch(a->nodetype) 
  	{
    // Nodos con dos hijos
    case '+':
    case '-':
    case '*':
    case '/': 
    case 'M':
    case 'R': treeFree(a->r);

    // Nodos con solo un hijo
    case '|': 
    case 'n': 
    case 'm':
    case 'r': treeFree(a->l);

    // Hojas
    case 'K': 
    case 'N':
    case 'F': delete a; 
    break;

    // loop node
    case 'B':
    {
        if      (std::string(a->action) == "mover1")   treeFree(a->l);
        else if (std::string(a->action) == "mover2") { treeFree(a->l); treeFree(a->r); } 
        else if (std::string(a->action) == "rotar1")   treeFree(a->l);
        else if (std::string(a->action) == "rotar2") { treeFree(a->l); treeFree(a->r); }
        break;
    }
    // orientation node
    case 'o': delete a;
    break; 
    default:  std::printf("error: Nodo incorrecto%c\n", a->nodetype);
  	} 
}

void yyerrorV2(const char *msg) {
    fprintf(stderr, "%s\n", msg);
}

float rotar(float degrees)
{
    pointer.phycmp.orientation += CALC::degreesToRadian(degrees);
    CALC::clampAngle(pointer.phycmp.orientation);
    return CALC::radianToDegrees(pointer.phycmp.orientation);
}

float rotar(float x, float y)
{
    float degrees {0.0};

    // VERIFY IF POINT IS IN RENDER BOUNDRING
    if (!renderSys.inBounds(x, y))
    { 
        yyerrorV2("Invalid movement: outside the limits of the frame"); 
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
}

float mover(float distance)
{
    auto const [xDist, yDist] { CALC::distanceXY(distance, pointer.phycmp.orientation)         };
    Point2D_t pointTarget     { pointer.phycmp.point.x + xDist, pointer.phycmp.point.y + yDist };

    return mover(pointTarget.x, pointTarget.y);
}

float mover(float x, float y)
{
    float const degreesT { rotar(x, y) };   // FIRST ORIEN TO TARGET

    if (degreesT != OUT_LIMITS)     // CHECK
    {
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

    return degreesT;
}