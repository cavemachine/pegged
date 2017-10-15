#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

SDL_Surface* bg_surface;
SDL_Texture* bg_texture;
SDL_Surface* ball_surface;
SDL_Texture* ball_texture;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Rect bg_rect;
SDL_Rect ball_rect;

struct square {
    bool is_hole;
    bool is_empty;
    int start_x;
    int start_y;
};

struct square board[7][7];

void init_board() {
    struct square tmp_board[49];
    for (int i = 0; i < 49; i++) {
	if (i != 0 && i != 1 && i != 5 && i != 6 &&
	    i != 7 && i != 8 && i != 12 && i != 13 &&
	    i != 35 && i != 36 && i != 40 && i != 41 &&
	    i != 42 && i != 43 && i != 47 && i != 48) {
	    tmp_board[i].is_hole = true;
	} else {
	    tmp_board[i].is_hole = false;
	}
	tmp_board[i].is_empty = true;	
    }
    int tmp_board_i = 0;
    for (int y = 0; y < 7; y++) {
	for (int x = 0; x < 7; x++) {
	    board[y][x].is_hole = tmp_board[tmp_board_i].is_hole;
	    tmp_board_i++;

	    board[y][x].start_x = (x+1)*50 - 41;
	    board[y][x].start_y = (y+1)*50 - 41;
	    if (board[y][x].is_hole) {
		ball_rect.x = board[y][x].start_x;
		ball_rect.y = board[y][x].start_y;
		SDL_RenderCopy(renderer, ball_texture, NULL, &ball_rect);
	    }
	}
    }
}
int main () {
    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow("pegged",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              400,
                              400,
                              SDL_WINDOW_SHOWN );
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    bg_surface = SDL_LoadBMP("img/pegged_2x.bmp");   
    bg_texture = SDL_CreateTextureFromSurface(renderer, bg_surface);
    
    ball_surface = SDL_LoadBMP("img/ball_2x.bmp");    
    Uint32 colorkey = SDL_MapRGB(ball_surface->format, 192,199, 200);
    SDL_SetColorKey(ball_surface, SDL_TRUE, colorkey); 
    ball_texture = SDL_CreateTextureFromSurface(renderer, ball_surface);
    //SDL_SetTextureBlendMode(ball_texture, SDL_BLENDMODE_BLEND);
    //SDL_SetTextureAlphaMod(ball_texture, 255);
    bg_rect.x = 0;
    bg_rect.y = 0;
    bg_rect.w = 360;
    bg_rect.h = 360;
    ball_rect.x = 0;
    ball_rect.y = 0;
    ball_rect.h = 40;
    ball_rect.w = 40;
    
    SDL_RenderCopy(renderer, bg_texture, NULL, &bg_rect);

    SDL_bool done = SDL_FALSE;
    bool selected = false;
    int t = 0;
    init_board();
    while(!done)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                if(event.type == SDL_QUIT) {
                    done = SDL_TRUE;
                }
                if(event.type == SDL_KEYUP) {
                    //
                }
                if(event.type == SDL_MOUSEBUTTONDOWN) {
		    if (selected == false) { selected = true;
		    } else { selected = false; }
		    
		}
		if(event.type == SDL_MOUSEMOTION) {
		    if (selected) {
			int mouse_x;
			int mouse_y;
			printf("%i \n", t++);
			SDL_GetMouseState(&mouse_x, &mouse_y);
			ball_rect.x = mouse_x-20;
			ball_rect.y = mouse_y-20;
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, bg_texture, NULL, &bg_rect);
			SDL_RenderCopy(renderer, ball_texture, NULL, &ball_rect);
		    }
		}
	    }
	    SDL_Delay(10);
	    SDL_RenderPresent(renderer);
	}
}
