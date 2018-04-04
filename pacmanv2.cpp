// FILE pacman.cpp

// Qui vanno gli #include
#include "pacman.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
using namespace std;
//
const string YELLOW="\e[1m\e[33m";
const string RED ="\e[1m\e[91m";
const string GREY ="\e[1m\e[35m"; 
const string BLUE = "\e[1m\e[44m\e[34m";
const string CYAN = "\e[1m\e[36m";

// Legge lo schema del labirinto da un file con nome predefinito "maze.cfg"
// Restituisce false se il file non si trova
// Nella versione base potete assumere che il file contenga (almeno) SIZE righe
// ciascuna di SIZE caratteri
bool init(int a[][SIZE], const int SIZE, int& dt){
	fstream in;
	char tmp;
	in.open("maze.cfg");
	if(in.fail()){
		return false;
	}
	for(int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			tmp = in.get();
			switch(tmp){
				case '#':
					a[i][j] = WALL;
					break;
				case '.':
					dt++;
					a[i][j] = DOT;
					break;
				case ' ':
					a[i][j] = EMPTY;
					break;			
			}
		}
		in.ignore(256, '\n');
	}
	in.close();
	return true;
}

// Legge e restituisce un carattere, ignorando eventuali altri caratteri sulla stessa riga
char input(){
	char move; 
	cin>>move;
	cin.ignore(256, '\n');
	return move;
}

// Visualizza il labirinto con il personaggio
void display(int a[][SIZE], const int SIZE){
	cout<<BLUE<<"#################"<<endl;
	for(int i=0; i<SIZE; i++){
		cout<<BLUE<<"#"<<"\e[49m\e[39m\e[0m";
		for(int j=0; j<SIZE; j++){
			switch(a[i][j]){
				case WALL:
					cout<<BLUE<<"#"<<"\e[49m\e[39m\e[0m";
					break;
				case DOT:
					cout<<"\e[93m"<<"."<<"\e[49m\e[39m\e[0m";
					break;
				case UPP:
					cout<<CYAN<<"v"<<"\e[49m\e[39m\e[0m";
					break;					
				case UP:
					cout<<YELLOW<<"v"<<"\e[49m\e[39m\e[0m";
					break;
				case DOWNP:
					cout<<CYAN<<"^"<<"\e[49m\e[39m\e[0m";
					break;					
				case DOWN:
					cout<<YELLOW<<"^"<<"\e[49m\e[39m\e[0m";
					break;
				case LEFTP:
					cout<<CYAN<<">"<<"\e[49m\e[39m\e[0m";
					break;					
				case LEFT:
					cout<<YELLOW<<">"<<"\e[49m\e[39m\e[0m";
					break;
				case RIGHTP:
					cout<<CYAN<<"<"<<"\e[49m\e[39m\e[0m";	
					break;				
				case RIGHT:
					cout<<YELLOW<<"<"<<"\e[49m\e[39m\e[0m";	
					break;	
				case PHANTOM:
					cout<<GREY<<"M"<<"\e[49m\e[39m\e[0m";
					break;	
				case APPLE:
					cout<<RED<<"o"<<"\e[49m\e[39m\e[0m";
					break;
				case POWERUP:
					cout<<"*";
					break;	
				default:
					cout<<" "<<"\e[49m\e[39m\e[0m";
					break;		
			}
		}
		cout<<BLUE<<"#"<<endl;
	}
	cout<<"#################"<<"\e[49m\e[39m\e[0m"<<endl;
	
}

// FUNZIONE "PRIVATA", usata solo in pacman.cpp, senza prototipo in pacman.h
// Verifica validita' di una coppia di coordinate in un labirinto:
// - i valori devono essere entro i limiti di dimensioni [0,SIZE)
// - il labirinto non deve contenere un muro in (X,Y)
bool check(int a[][SIZE], const int SIZE, ent& pos, int incx, int incy){
	int x = pos.x+incx;
	int y = pos.y+incy;
	if(x>=0 && x<SIZE && y>=0 && y<SIZE){
		switch(a[x][y]){
			case DOT:
				if(pos.type){
				pos.counter[0]+=1;
				pos.counter[1]+=10;
				}
				pos.has_moved = true;
				return true;
				break;
			case EMPTY:
				pos.has_moved = true;
				return true;
				break;
			case PHANTOM:
				if(pos.type){ 
					if(!pos.powered){
						pos.alive=false;					
					}
					else{			
						return true;	
					}
				}	
				pos.has_moved=false;
				return false;
				break;
			case UP:
			case DOWN:
			case LEFT:
			case RIGHT:
				if(!pos.type){
					pos.has_moved = true;
					return true;
				}	
				break;
			case UPP:
			case DOWNP:
			case LEFTP:
			case RIGHTP:
				if(!pos.type){
					pos.alive = false;
					pos.has_moved = true;
					return true;
				}
				break;	
			case APPLE:
				if(pos.type){
				pos.counter[0]+=1;
				pos.counter[1]+=300;
				}
				pos.has_moved = true;
				return true;		
				break;
			case POWERUP:
				if(pos.type){
					pos.powered = true;
					pos.replace = 0;
					pos.cust[0]=UPP;
					pos.cust[1]=DOWNP;
					pos.cust[2]=RIGHTP;
					pos.cust[3]=LEFTP;
				}
				pos.has_moved = true;
				return true;
				break;		
			default:
				pos.has_moved = false;
				return false;
				break;		
		}
	}
	pos.has_moved = false;
	return false;
}

// Esegue mossa (se possibile), aggiornando personaggio e labirinto
// Dove si trova il personaggio, il puntino viene mangiato
void move(int a[][SIZE], const int SIZE, char dir, ent& pos){
	int tmp;
	if(a[pos.x][pos.y] == pos.am_i_still_here){
		switch(dir){
			case N:
				
				if(check(a, SIZE, pos, -1, 0)){
					pos.x--;
					if(!pos.type){			
						tmp = a[pos.x][pos.y];
						a[pos.x+1][pos.y]=pos.replace;
						pos.replace = tmp;
					}
					else{
						a[pos.x+1][pos.y]=EMPTY;
					}				
				}
				if(pos.alive){
				a[pos.x][pos.y] = pos.cust[0];
				pos.am_i_still_here = pos.cust[0];
				}	
				break;
			case S:
				if(check(a, SIZE, pos, +1, 0)){
					pos.x++;
					if(!pos.type){				
						tmp = a[pos.x][pos.y];
						a[pos.x-1][pos.y]=pos.replace;
						pos.replace = tmp;
					}
					else{
						a[pos.x-1][pos.y]=EMPTY;
					}				
				}
				if(pos.alive){
				a[pos.x][pos.y]=pos.cust[1];
				pos.am_i_still_here = pos.cust[1];
				}
				break;
			case E:
				if(check(a, SIZE, pos, 0, +1)){			
					pos.y++;
					if(!pos.type){			
						tmp = a[pos.x][pos.y];
						a[pos.x][pos.y-1]=pos.replace;
						pos.replace = tmp;
					}
					else{
						a[pos.x][pos.y-1]=EMPTY;
					}
				}
				if(pos.alive){
				a[pos.x][pos.y]=pos.cust[2];
				pos.am_i_still_here = pos.cust[2];
				}						
				break;
			case W:
				if(check(a, SIZE, pos, 0, -1)){
					pos.y--;					
					if(!pos.type){				
						tmp = a[pos.x][pos.y];
						a[pos.x][pos.y+1]=pos.replace;
						pos.replace = tmp;
					}
					else{
						a[pos.x][pos.y+1]=EMPTY;
					}
				}					
				if(pos.alive){
				a[pos.x][pos.y]=pos.cust[3];	
				pos.am_i_still_here = pos.cust[3];
				}				
				break;
			default:
				break;		
		}
	}
	else{
		pos.alive=false;
	}
}


void p_init(int a[][SIZE], const int SIZE, ent& pos){
	pos.type=false;
	pos.alive=true;
	pos.cust[0]=PHANTOM;
	pos.cust[1]=PHANTOM;
	pos.cust[2]=PHANTOM;
	pos.cust[3]=PHANTOM;
	pos.last_input = E;
	pos.am_i_still_here=pos.cust[0];
	do{
		pos.x=rand()%14+1;						
		pos.y=rand()%14+1;						
	}while(!check(a, SIZE, pos, 0, 0));	
	pos.replace=a[pos.x][pos.y];	
	a[pos.x][pos.y]=pos.cust[0];		
}

void cpu_move(int maze[][SIZE], const int SIZE, ent& pos){
	if(pos.alive){	
		float P = 0.25;
		float p = rand()%2;
		if(p>P){
		int i = 0;
			do{
				int index = rand()%4;
				switch(index){
					case 0:
						pos.last_input = N;
						move(maze, SIZE, pos.last_input, pos);
						break;
					case 1:
						pos.last_input = S;
						move(maze, SIZE, pos.last_input, pos);
						break;
					case 2:
						pos.last_input = E;
						move(maze, SIZE, pos.last_input, pos);
						break;
					case 3:
						pos.last_input = W;
						move(maze, SIZE, pos.last_input, pos);
						break;			
				}
				i++;		
			}while(!pos.has_moved && i<100);		
		}
		else{
			move(maze, SIZE, pos.last_input, pos);
		}
	}
	else{
		p_init(maze, SIZE, pos);	
		pos.alive=true;
	}	

}

void disp(int a[][SIZE], const int SIZE, int obj[10], int& ndot, int type){
		int x;
		int y;
		for(int i=0; i<10; i+=2){
			do{
				x=rand()%14+1;
				y=rand()%14+1;
			}while(a[x][y]!= DOT && a[x][y]!= EMPTY);
			obj[i] = x;
			obj[i+1] = y;
			if(a[x][y]==EMPTY){
				ndot++;
			}	
			a[x][y]= type;
		}
}

void undo(int a[][SIZE], const int SIZE, int obj[10], int& ndot, int type, ent& ghost1, ent& ghost2, ent& ghost3, ent& ghost4){
	for(int i=0; i<10; i+=2){
		if(a[obj[i]][obj[i+1]]==type){
			a[obj[i]][obj[i+1]] = EMPTY;
			ndot--;
		}
	}	
		if(ghost1.replace==type){
			ghost1.replace=EMPTY;
		}
		if(ghost2.replace==type){
			ghost1.replace=EMPTY;
		}
		if(ghost3.replace==type){
			ghost1.replace=EMPTY;
		}
		if(ghost4.replace==type){
			ghost1.replace=EMPTY;
		}
		
			
	}				

void obj(int a[][SIZE], const int SIZE, ent& g1, ent& g2, ent& g3, ent& g4, int fruit[10], int powerup[10], int& pt, int& dt){
	if(pt%60 == 30){
		disp(a, SIZE, fruit, dt, APPLE);
	}
	if(pt%100 == 50){
		disp(a, SIZE, powerup, dt, POWERUP);
	}
	if(pt%60 == 0){
		undo(a, SIZE, fruit, dt, APPLE, g1, g2, g3, g4);
	}
	if(pt%100 == 70){
		undo(a, SIZE, powerup, dt, POWERUP, g1, g2, g3, g4);
	}
}
// FINE FILE pacman.cpp
