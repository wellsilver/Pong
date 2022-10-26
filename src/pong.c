#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#define SDL_MAIN_HANDLED // doesnt work otherwise, even with argc argv and things
#include <SDL2/SDL.h>
int menu(SDL_Window *window,SDL_Surface *renderer) {
	SDL_Surface *start_btn_i = SDL_LoadBMP("./data/START.bmp");
	SDL_Surface *quit_btn_i = SDL_LoadBMP("./data/QUIT.bmp");
	SDL_Rect start_btn;
	start_btn.w=100;
	start_btn.h=50;
	start_btn.x=300;
	start_btn.y=150;
	SDL_Rect quit_btn;
	quit_btn.w=100;
	quit_btn.h=50;
	quit_btn.x=50;
	quit_btn.y=150;
	SDL_Rect pong1;
	pong1.y = 150;
	pong1.x = 5;
	pong1.w = 5;
	pong1.h = 50;
	SDL_Rect pong2;
	pong2.y = 150;
	pong2.x = 395;
	pong2.w = 5;
	pong2.h = 50;
	int active=1;
	int m_x,m_y; // mouse coords (updated on button event)
	SDL_Event event;
	while (active) {
		// events
		while (SDL_PollEvent(&event)!=0) {
			switch(event.type) {
				case SDL_QUIT:
					return 1; // exit entire game
				break;
				case SDL_MOUSEBUTTONDOWN:
					// it does not matter wich button was clicked
					SDL_GetMouseState(&m_x,&m_y);
					// check if its in a box
					if ((m_x-start_btn.x)>0&&(m_x-start_btn.x)<start_btn.w) { // check if x is in start_btn
						if ((m_y-start_btn.y)>0&&(m_y-start_btn.y)<start_btn.h) { // check if y is in start_btn
							return 0; // quit menu, and go back
						}
					}
					if ((m_x-quit_btn.x)>0&&(m_x-quit_btn.x)<quit_btn.w) { // check if x is in quit_btn
						if ((m_y-quit_btn.y)>0&&(m_y-quit_btn.y)<quit_btn.h) { // check if y is in quit_btn
							return 1; //exit entire game
						}
					}
			}
		}
		//render
		SDL_FillRect(renderer, NULL, SDL_MapRGB(renderer->format, 0,0,0));
		SDL_BlitSurface(start_btn_i, NULL, renderer, &start_btn);
		SDL_BlitSurface(quit_btn_i, NULL, renderer, &quit_btn);
		SDL_UpdateWindowSurface(window);
	}
	
	return 1;
}
int game(SDL_Window *window,SDL_Renderer *renderer) {
	int point1=0; // points for p1
	int point2=0; // points for p2
	SDL_Rect pong1;
	pong1.y = 175;
	pong1.x = 5;
	pong1.w = 5;
	pong1.h = 50;
	SDL_Rect pong2;
	pong2.y = 175;
	pong2.x = 395;
	pong2.w = 5;
	pong2.h = 50;
	SDL_Rect ball;
	ball.y=200;
	ball.x=200;
	ball.w=5;
	ball.h=5;
	// my sister was doing the art but was very mean about it and cut out after taking 2 seconds on the start and quit buttons
	// she didnt even send it in .bmp at first even though I asked her to
	SDL_Rect points_c1; // display points for player
	points_c1.y=5;
	points_c1.x=160;
	points_c1.h=20;
	points_c1.w=20;
	SDL_Rect points_c2; // display points for player
	points_c2.y=5;
	points_c2.x=220;
	points_c2.w=20;
	points_c2.h=20;
	int pong1_up=0;
	int pong1_down=0;
	int pong2_up=0;
	int pong2_down=0;
	int pause=0; // time for ball
	int ball_velocityX=1;
	int ball_velocityY=0;
	int disabled=0; // if its dramatic pause time
	// get direction "randomly"
	int ball_direction;
	int variancy=((clock()/10000)+(clock()/10000))*2; // "random" so the ball isnt predictable
	if (clock() % 2 == 0) { // if odd
		ball_direction=0; // left
	}
	else { // if even
		ball_direction=1; // right
	}
	int active=1;
	SDL_Event event;
	const Uint8 *keys;
	while (active) {
		/// game logic
		// paddles
		if (!disabled) {
		if (pong1_up) {
			pong1.y--;
		}
		if (pong1_down) {
			pong1.y++;
		}
		if (pong2_up) {
			pong2.y--;
		}
		if (pong2_down) {
			pong2.y++;
		}}
		// ball
		if (pause>120) { // has it been 2 seconds? then move ball
			if (ball.x>=390) { // if its on the right side
				if (ball.y-pong2.y<50 && ball.y-pong2.y>0) {
					if (!disabled) {
					point2++;
					ball_velocityX=0-ball_velocityX;
					variancy=0-variancy;
					ball_velocityY=-1;
					}
				}
				else { // ball is passed the point
					disabled=1; // pause for dramatic effect
					if (ball.x>=405) {
						return 0;
					}
					
				}
			}
			if (ball.x<=10) { // if its on the left side
				if (ball.y-pong1.y<50 && ball.y-pong1.y>0) {
					if (!disabled) {
					point1++;
					ball_velocityX=abs(ball_velocityX);
					variancy=abs(variancy);
					ball_velocityY=1;
					}
				}
				else {
					disabled=1; // pause for dramatic affect
					if (ball.x<=-5) {
						return 0;
					}
				}
			}
			if (ball.y>395) {// bounce off bottom
				ball_velocityY=1; 
			}
			if (ball.y<5) {// bounce off top
				ball_velocityY=-1; 
			}
			ball.x+=ball_velocityX+variancy;
			ball.y-=ball_velocityY;
		}
		else {
			pause++;
		}
		/// event logic
		while (SDL_PollEvent(&event)!=0) {
			switch (event.type) {
				case SDL_QUIT:
					return 1; // exit entire game
				break;
				case SDL_KEYDOWN:
					if (event.key.repeat != 0) {break;}
					keys = SDL_GetKeyboardState(NULL);
					if (keys[SDL_SCANCODE_W]) {
						pong1_up=1;
					}
					if (keys[SDL_SCANCODE_S]) {
						pong1_down=1;
					}
					if (keys[SDL_SCANCODE_R]) {
						pong2_up=1;
					}
					if (keys[SDL_SCANCODE_F]) {
						pong2_down=1;
					}
				break;
				case SDL_KEYUP:
					if (event.key.repeat != 0) {break;}
					keys = SDL_GetKeyboardState(NULL);
					if (keys[SDL_SCANCODE_W]==0) {
						pong1_up=0;
					}
					if (keys[SDL_SCANCODE_S]==0) {
						pong1_down=0;
					}
					if (keys[SDL_SCANCODE_R]==0) {
						pong2_up=0;
					}
					if (keys[SDL_SCANCODE_F]==0) {
						pong2_down=0;
					}
				break;
			}
		}
		/// render
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer); 
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		// draw things here
		SDL_RenderFillRect(renderer, &pong1);
		SDL_RenderFillRect(renderer, &pong2);
		SDL_RenderFillRect(renderer, &ball);
		SDL_RenderFillRect(renderer, &points_c1);
		SDL_RenderFillRect(renderer, &points_c2);
		//
		SDL_RenderPresent(renderer);
		SDL_Delay(1000/60); // 60 fps
	}
}
int main() {
	// loading
    SDL_Init(SDL_INIT_VIDEO);
	if (SDL_Init(SDL_INIT_VIDEO)>0) {printf("Fail init");}
    SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_CreateWindowAndRenderer(400, 400, SDL_WINDOW_RESIZABLE,&window, &renderer);
	if (window==NULL) {printf("fail window");}
    SDL_Surface *window_surface = SDL_GetWindowSurface(window);
	if (window_surface==NULL) {printf("fail surface");}
	SDL_RenderSetLogicalSize(renderer, 400, 400);
	int from;
	while (1) {
		// menu (separate gameloop)
		from=menu(window,window_surface);
		if (from!=0) { //if quit from menu
			return 0;
		}
		// game (separate gameloop)
		from=game(window,renderer);
		if (from!=0) {
			return 0;
		}
	}
}
