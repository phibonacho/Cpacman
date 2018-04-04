// FILE pacman.h


// TIPI DI DATO


// COSTANTI

// Qui si definisce la dimensione del labirinto, chiamata SIZE
// Se si vuole usare il file maze.cfg fornito, SIZE deve essere 15
const int SIZE = 15;
// Qui si definiscono eventualmente altre costanti
// che rappresentano le direzioni, NORTH/EAST/SOUTH/WEST
// e i tipi di elemento che formano il labirinto, EMPTY/WALL/DOT

enum DIR {N = 'w', S = 's' , E = 'd' , W = 'a' };
enum ELM {WALL , DOT , EMPTY, APPLE, PHANTOM, POWERUP, UP, DOWN, LEFT, RIGHT, RIGHTP, UPP, DOWNP, LEFTP};
struct ent{
	int x;
	int y;
	bool type;
	int replace;
	int am_i_still_here;
	int cust[4];
	int counter[2];
	char last_input;
	bool has_moved;
	bool powered;
	bool alive;
};
	
// FUNZIONI DEL GIOCO

// Qui si dichiarano le funzioni (prototipi)

// Inizializza leggendo schema da file (assumendolo corretto)
bool init(int a[][SIZE], const int SIZE, int& dt);

// Legge input da tastiera
char input();

// Disegna lo stato corrente del gioco
void display(int a[][SIZE], const int SIZE);

// Esegue una mossa 
// Il pac-man viene modificato, quindi deve essere passato per riferimento
// Anche il labirinto viene modificato, ma e' un array 
// e gli array sono sempre "passati per riferimento"
void move(int a[][SIZE], const int SIZE, char dir, ent& pos);

void p_init(int a[][SIZE], const int SIZE, ent& phantom);

void cpu_move(int a[][SIZE], const int SIZE, ent& pos);

void obj(int maze[][SIZE], const int SIZE, ent& g1, ent& g2, ent& g3, ent& g4, int fruit[10], int powerup[10], int& pt, int& dt);
// FINE FILE pacman.h
