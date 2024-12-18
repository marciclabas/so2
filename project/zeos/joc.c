#include <libc.h>
#include <dibuixos.h>
#include <io.h>

sem_t* sem_final;
sem_t* sem_pos;
sem_t* sem_draw_done;

char KEY;
int POS_X, POS_Y;

void read_keyboard(void * arg){
	printf("read %d\n", gettid());
	int wait_time = 10;
	char last_pressed = -1;
	int i = 0;
	while(1){
		//i++;
		//if(i == 25){
		//	printf("sip");
		//}
		char b;
		b = 'A';
		int aux = getkey(&b, wait_time);
		//printf("%c\n", b);
		if(aux == -1) KEY = -1;
		else if (b == 'm'){
			semSignal(sem_final);
		}
		else if (b != last_pressed){
			last_pressed = b;
			
			//aqui caldra algun semafor??
			KEY = b;
		}
		
	}
}

void movement(void * arg){
	printf("movement %d\n", gettid());
	/*float velocitat_max = 2;
	float pos_x = 0;
	float pos_y = 24;
	float v_x = 0;
	float v_y = 0;
	int last_time = gettime();
	
	//....................................
	clrscr(map);
	int pos_x_anterior = -1;
	int pos_y_anterior = -1;
	//semSignal(sem_draw_done);
	//.....................................
	
	while(1){
		//semWait(sem_draw_done);
		char key = KEY;
		
		int aux = gettime();
		float time = (aux - last_time)/100;
		last_time = aux;
		
		switch(key){
			case 'a': 
				v_x = -velocitat_max;
				break;
			case 'd': 
				v_x = velocitat_max;
				break;
			case 'w': 
				v_y = velocitat_max;
				break;
			case -1:
				v_x = 0;
				break;
		}
		pos_x = pos_x + v_x * time;
		pos_y = pos_y + v_y * time;
		
		if(pos_x > 79) pos_x = 79;
		else if(pos_x < 0) pos_x = 0;
		
		if(pos_y > 24) pos_y = 24;
		else if(pos_y < 0) pos_y = 0;

		//int t0 = gettime();
  		//while (gettime() < t0 + 50);
  		//semafors??
  		POS_X = (int)pos_x;
  		POS_Y = (int)pos_y;
  		
  		//semSignal(sem_pos);
  		
  		
  		//......................
  		

		// semWait(sem_pos);
		
		//Borrem el char anterior


		

  		//int pos_x = (int)POS_X;
  		//int pos_y = (int)POS_Y;
  		if(!(POS_X == pos_x_anterior && POS_Y == pos_y_anterior)){
  			printf("Pintant...\n");
  			changeColor(7, 2);//map[(pos_y_anterior * 80 + pos_x_anterior)]);
	  		gotoXY(pos_x_anterior, pos_y_anterior);
	  		printf(" "); //printc_xy(pos_x_anterior, pos_y_anterior, ' ');
	  		char color_fons = 4;//map[(POS_Y * 80 + POS_X)];
			changeColor(7, color_fons);
			gotoXY(pos_x, pos_y);
			printf("R");
			//printc_xy(pos_x, pos_y, 'R');
			
			pos_x_anterior = POS_X;
			pos_y_anterior = POS_Y;
		}

		// semSignal(sem_draw_done);
		//...............
  		
	}*/
	while(1)
	exit();
}


void draw_screen(void * arg){
	/*clrscr(map);
	int pos_x_anterior = 0;
	int pos_y_anterior = 0;
	semSignal(sem_draw_done);
	while(1){

		//printf("1");
		semWait(sem_pos);
		//printf("comença draw...");
		//borrem el char anterior
		//printf("2");
		//changeColor(7, map[(pos_y_anterior * 80 + pos_x_anterior)]);
		printc_xy(pos_x_anterior, pos_y_anterior, ' ');
		//printf("3");
		
  		int pos_x = (int)POS_X;
  		int pos_y = (int)POS_Y;
  		char color_fons = 0; //smap[(pos_y2 * 80 + pos_x2)];
		changeColor(7, color_fons);
		printc_xy(pos_x, pos_y, 'R');
		//printf("R dibuixada: ");
		//printf("Pos x = %d", pos_x);
		//printf("   Pos y = %d\n", pos_y);

		
		pos_x_anterior = pos_x;
		pos_y_anterior = pos_y;
		//printf("acaba draw\n");
		semSignal(sem_draw_done);
		
	}*/
	while(1);
}

void game(){
	printf("Comença el joc!\n");
	//Iniciem semàfors!
	sem_final = semCreate(0);
	//sem_pos = semCreate(0);
	//sem_draw_done = semCreate(0);
	
	// Creem els diferents threads
	threadCreateWithStack(read_keyboard, 1, (void*) 0);
	threadCreateWithStack(movement, 1, (void*) 0);
	
	//threadCreateWithStack(draw_screen, 1, (void*) 0);
	
	while(1);
	//esperem al final del joc
	semWait(sem_final);
	exit();	
}


