
// FILE main.cpp


// qui gli #include
#include "pacman.h"
#include <iostream>
#include <stdlib.h>
#include <ctime>
using namespace std;
// Il programma principale inizializza il gioco e mostra lo stato iniziale,
// poi esegue il ciclo di interazione LEGGI - ESEGUI - MOSTRA RISULTATO
// finche' l'utente non inserisce q o Q
int main()
{
  
  // Qui si definiscono le variabili: stato del pac-man e del labirinto
	int dt=0;
	int play_time = 1;
	
	int maze[SIZE][SIZE];
	int fruit[10];
	int powerup[10];
	
	srand(time(NULL));
	
	ent player;
	
	player.x=0;
	player.y=0;
	player.counter[0]=1;
	player.counter[1]=10;
	player.type=true;
	player.cust[0]=UP;
	player.cust[1]=DOWN;
	player.cust[2]=RIGHT;
	player.cust[3]=LEFT;
	player.replace=EMPTY;
	player.alive = true;
	player.powered = true;
	player.am_i_still_here = DOWN;
	
	ent ghost1;
	ent ghost2;
	ent ghost3;
	ent ghost4;
				
  // Qui si inizializza labirinto da file
	if(!init(maze, SIZE, dt)){
		return 0;
	}	
  // Qui si inizializza il personaggio: pos in alto a sinistra, direz. sud
  	maze[0][0]=DOWN;
  	p_init(maze, SIZE, ghost1);
  	p_init(maze, SIZE, ghost2);
  	p_init(maze, SIZE, ghost3);
  	p_init(maze, SIZE, ghost4);
  // Qui si mostra lo stato iniziale
	system("clear");
	display(maze, SIZE);
  // Ciclo di interazione
	char dir;
	do {
    		if(player.alive){			
			 cpu_move(maze, SIZE, ghost1);
			 cpu_move(maze, SIZE, ghost2);
			 cpu_move(maze, SIZE, ghost3);
			 cpu_move(maze, SIZE, ghost4);
			 obj(maze, SIZE, ghost1, ghost2, ghost3, ghost4, fruit, powerup, play_time, dt);
		}	
		
		if(player.alive){			
    // leggi input
			dir=input();
    // esegui movimento
			move(maze, SIZE, dir, player);
	    		if(player.powered && player.replace<5){
				player.replace++;    			
	    		}
	    		else{
	    			player.powered = false;
	    			player.cust[0]=UP;
				player.cust[1]=DOWN;
				player.cust[2]=RIGHT;
				player.cust[3]=LEFT;
				player.replace = 0;
 			} 
    		}
				
    // mostra risultato
		system("clear");
		display(maze, SIZE);
		cout<<" Punteggio:"<<player.counter[1]<<endl;			
		play_time++;
	} while (dir!='q' && player.counter[0]<dt && player.alive);  
  // Si puo' usare il costrutto di ciclo che si vuole;
  // do...while e' solo un suggerimento
	return 0;
}


// FINE FILE main.cpp
