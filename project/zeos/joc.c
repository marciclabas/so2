#include <libc.h>
#include <dibuixos.h>
#include <io.h>

sem_t* sem_final;
sem_t* sem_pos;
sem_t* sem_draw_done;

char KEY;
int POS_X_G, POS_Y_G;
int no_ha_acabat = 1;

void read_keyboard(void * arg){
	int wait_time = 1000;
	char last_pressed;
	char b;
	while(no_ha_acabat){
		int aux = getkey(&b, wait_time);
		if(aux == -1) KEY = -1;
		else if (b == 'm'){
			semSignal(sem_final);
		}
		else{
			//semafor?
			KEY = b;
		}
	}
	exit();
}

int xoca(int x, int y){
	return (map[(y * 80 + x)] != 0) || x>79 || x<0 || y<0 || y>24;
}



void movement(void * arg){
	int velocitat_max = 1;
	int pos_x = 0;
	int pos_y = 21;
	int last_time = gettime();
	int salts_restants = 0;
	int salt_maxim = 8;
	int temps_loop = 50;

	
	
	while(no_ha_acabat){
		semWait(sem_draw_done);
		//semafor? 
  		while (gettime() < last_time + temps_loop);
  		last_time = gettime();
		
		char key = KEY;
		KEY = ' ';
		switch(key){
			case 'a': 
				if(!xoca(pos_x - 1,pos_y)) pos_x -= 1;
				break;
			case 'd': 
				if(!xoca(pos_x + 1,pos_y)) pos_x += 1;
				break;
			case 'w': 
				if(xoca(pos_x,pos_y + 1)) salts_restants = salt_maxim;
				break;
		}
		
		if(salts_restants > 0){
			if(!xoca(pos_x,pos_y - 1)) pos_y -= 1;
			else salts_restants = 0;
			salts_restants--;
		}
		else if(!xoca(pos_x,pos_y + 1)) pos_y += 1;
		
		if(pos_x>=75 && pos_y >=17){
			short* fin; 
			clrscr(fin);
			printf("HAS QUANYAT!!!");
			int t0 = gettime();
  			while (gettime() < t0 + 500);
  			semSignal(sem_final);
  			exit();
		}
		
  		POS_X_G = pos_x;
  		POS_Y_G = pos_y;
		
  		semSignal(sem_pos);
	}
	exit();
}


void draw_screen(void * arg){
	clrscr(map);
	changeColor(0, 1);
	for(int x = 75; x < 80; x++){
		for(int y = 17; y<22; y++) printc_xy(x,y,' ');
	}
	gotoXY(5,5);
	changeColor(7, 0);
	printf("Arriba a la porta blava!");
	
	int pos_x_anterior = 0;
	int pos_y_anterior = 0;
	semSignal(sem_draw_done);
	changeColor(7, 0);
	
	while(no_ha_acabat){

		semWait(sem_pos);
		int x = POS_X_G;
	  	int y = POS_Y_G;
	  	if(x!=pos_x_anterior || y != pos_y_anterior){
			//borrem el char anterior
			printc_xy(pos_x_anterior, pos_y_anterior, ' ');
			
	  		
			printc_xy(x, y, 'R');
			
			pos_x_anterior = x;
			pos_y_anterior = y;
			}
		semSignal(sem_draw_done);
		
	}
	exit();
}

void game(){
	printf("Comença el joc!\n");
	//Iniciem semàfors!
	sem_final = semCreate(0);
	sem_pos = semCreate(0);
	sem_draw_done = semCreate(0);
	
	// Creem els diferents threads
	threadCreateWithStack(read_keyboard, 1, (void*) 0);
	threadCreateWithStack(movement, 1, (void*) 0);
	threadCreateWithStack(draw_screen, 1, (void*) 0);

	//esperem al final del joc
	semWait(sem_final);
	no_ha_acabat = 0;
	printf("s'ha acabat el joc");

}


