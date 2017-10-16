#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

SDL_Surface* bg_surface;
SDL_Texture* bg_texture;
SDL_Surface* ball_surface;
SDL_Texture* ball_texture;
SDL_Surface* ball_surface_selected;
SDL_Texture* ball_texture_selected;
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
bool ball_selected;
int ball_selected_x;
int ball_selected_y;

bool game_over;

void init_board();
void board_cross();
bool board_clicked(int m_x, int m_y);

//------------------------------
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
	    board[y][x].is_empty = tmp_board[tmp_board_i].is_empty;
	    board[y][x].start_x = (x+1)*50 - 41;
	    board[y][x].start_y = (y+1)*50 - 41;
	    tmp_board_i++;
	    
	    board_cross();
	    game_over = false;
	}
    }
}

void board_cross() {
    ball_rect.x = board[1][3].start_x;
    ball_rect.y = board[1][3].start_y;
    board[1][3].is_empty = false;
    SDL_RenderCopy(renderer, ball_texture, NULL, &ball_rect);
    
    ball_rect.x = board[2][3].start_x;
    ball_rect.y = board[2][3].start_y;
    board[2][3].is_empty = false;
    SDL_RenderCopy(renderer, ball_texture, NULL, &ball_rect);
    
    ball_rect.x = board[2][2].start_x;
    ball_rect.y = board[2][2].start_y;
    board[2][2].is_empty = false;
    SDL_RenderCopy(renderer, ball_texture, NULL, &ball_rect);
    
    ball_rect.x = board[2][4].start_x;
    ball_rect.y = board[2][4].start_y;
    board[2][4].is_empty = false;
    SDL_RenderCopy(renderer, ball_texture, NULL, &ball_rect);
    
    ball_rect.x = board[3][3].start_x;
    ball_rect.y = board[3][3].start_y;
    board[3][3].is_empty = false;
    SDL_RenderCopy(renderer, ball_texture, NULL, &ball_rect);
    
    ball_rect.x = board[4][3].start_x;
    ball_rect.y = board[4][3].start_y;
    board[4][3].is_empty = false;
    SDL_RenderCopy(renderer, ball_texture, NULL, &ball_rect); 
}

void move_ball(int square_x, int square_y) {
    board[ball_selected_y][ball_selected_x].is_empty = true;
    board[square_y][square_x].is_empty = false;
    ball_selected = false;
}

bool board_clicked(int m_x, int m_y) {
    
    int square_x;
    int square_y;
    square_x = m_x/51;
    square_y = m_y/51;
    //printf("x: %i \n", square_x);
    //printf("y: %i \n", square_y);

    if (board[square_y][square_x].is_empty == false && ball_selected == false) {
	ball_selected_x = square_x;
	ball_selected_y = square_y;
	ball_selected = true;
	return(true);
    }
    if (ball_selected == true && board[square_y][square_x].is_empty == true) {
	if (ball_selected_x == square_x && ball_selected_y != square_y) {
	    if (square_y == ball_selected_y - 2) {
		if (board[ball_selected_y - 1][ball_selected_x].is_empty == false) {
		    move_ball(square_x,square_y);
		    board[ball_selected_y - 1][ball_selected_x].is_empty = true;
		    return(true);
		}
	    }
	    if (square_y == ball_selected_y + 2) {
		if (board[ball_selected_y + 1][ball_selected_x].is_empty == false) {
		    move_ball(square_x,square_y);
		    board[ball_selected_y + 1][ball_selected_x].is_empty = true;
		    return(true);
		}
	    }	    
	}
	if (ball_selected_x != square_x && ball_selected_y == square_y) {
	    if (square_x == ball_selected_x - 2) {
		if (board[ball_selected_y][ball_selected_x - 1].is_empty == false) {
		    move_ball(square_x,square_y);
		    board[ball_selected_y][ball_selected_x - 1].is_empty = true;
		    return(true);
		}
	    }
	    if (square_x == ball_selected_x + 2) {
		if (board[ball_selected_y][ball_selected_x + 1].is_empty == false) {
		    move_ball(square_x,square_y);
		    board[ball_selected_y][ball_selected_x + 1].is_empty = true;
		    return(true);
		}
	    }	    
	}
    }

    ball_selected = false;
    /* ball_rect.x = board[square_y][square_x].start_x; */
    /* ball_rect.y = board[square_y][square_x].start_y; */
    /* SDL_RenderCopy(renderer, ball_texture, NULL, &ball_rect); */
}

void render_update() {
    int not_empty_count = 0;
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, bg_texture, NULL, &bg_rect );
    for (int y = 0; y < 7; y++) {
	for (int x = 0; x < 7; x++) {
	    if (board[y][x].is_empty == false) {
		if (ball_selected == true) {
		    if (ball_selected_x == x && ball_selected_y == y) {
			ball_rect.x = board[ball_selected_y][ball_selected_x].start_x;
			ball_rect.y = board[ball_selected_y][ball_selected_x].start_y;
			SDL_RenderCopy(renderer, ball_texture_selected, NULL, &ball_rect);
			not_empty_count++;
		    } else {
			ball_rect.x = board[y][x].start_x;
			ball_rect.y = board[y][x].start_y;
			SDL_RenderCopy(renderer, ball_texture, NULL, &ball_rect);
			not_empty_count++;
		    }
		} else {
		    ball_rect.x = board[y][x].start_x;
		    ball_rect.y = board[y][x].start_y;
		    SDL_RenderCopy(renderer, ball_texture, NULL, &ball_rect);
		    not_empty_count++;
		}

	    }
	}
    }
    if (not_empty_count == 1) {
	printf("You won!\n");
	game_over = true;
    }
    SDL_RenderPresent(renderer);
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
    //Uint32 colorkey = SDL_MapRGB(ball_surface->format, 192,199, 200); // transparency 
    //SDL_SetColorKey(ball_surface, SDL_TRUE, colorkey); 
    ball_texture = SDL_CreateTextureFromSurface(renderer, ball_surface);
    
    ball_surface_selected = SDL_LoadBMP("img/ball_2x.bmp");    
    Uint32 colorkey = SDL_MapRGB(ball_surface_selected->format, 192,199,200); // transparency 
    SDL_SetColorKey(ball_surface_selected, SDL_TRUE, colorkey);
    ball_texture_selected = SDL_CreateTextureFromSurface(renderer, ball_surface_selected);
//    SDL_SetTextureAlphaMod(ball_texture_selected, 100);
    SDL_SetTextureColorMod(ball_texture_selected, 100, 100, 180);
	
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
		    int mouse_x;
		    int mouse_y;
		    SDL_GetMouseState(&mouse_x, &mouse_y);
		    board_clicked(mouse_x,mouse_y);
		    /* if (selected == false) { selected = true; */
		    /* } else { selected = false; } */
		    
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
	    if (!game_over) {
		render_update();
	    } else {
		SDL_RenderPresent(renderer);
	    }
	}
}
