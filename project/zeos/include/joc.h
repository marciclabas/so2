#ifndef __JOC_H__
#define __JOC_H__

void game();

void sleep(int ticks);

void read_keyboard(void * arg);
void movement(void * arg);
void draw_screen(void * arg);

#endif
