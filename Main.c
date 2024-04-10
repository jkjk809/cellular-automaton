#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>
#include <time.h>
#include <conio.h>
#include <stdlib.h>

int row = 250;
int col = 400;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

struct cell {
	float x;
	float y;
	float height;
	float width;
} cell;

// methods
bool init_window();
void render(bool** cellArray);
void destroy_window();
void setup_cell_array(bool** cellArray);
void print_array(bool** cellArray);
void copy_array(bool** cellArray, bool** temp);
void next_gen(bool** cellArray, bool** temp);
void clrscr();

void setup() {
	cell.x = 0.0;
	cell.y = 0.0;
	cell.height = 5.0;
	cell.width = 5.0;
}

int main(int argc, char* args[]) {

	 
	bool isRunning = init_window();
	
	//int game_run = TRUE;
	unsigned int seed = (unsigned int)time(NULL); // Line 43
	srand(seed);

	setup();
	// create cell array and temp.
	bool** cellArray = (bool**)malloc(row * sizeof(bool*));
	for (int i = 0; i < row; i++) {
		cellArray[i] = (bool*)malloc(col * sizeof(bool));
	}

	bool** temp = (bool**)malloc(row * sizeof(bool*));
	for (int i = 0; i < row; i++) {
		temp[i] = (bool*)malloc(col * sizeof(bool));
	}

	

	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;
	double timeSpent = 0;

	SDL_Event ev;
	setup_cell_array(cellArray);

	while (isRunning)
	{
		while (SDL_PollEvent(&ev) != 0) {
			if (ev.type == SDL_QUIT)
				isRunning = false;
			if (ev.key.keysym.sym == SDLK_ESCAPE)
				isRunning = false;
		}
		
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		deltaTime = (double)((NOW - LAST) * 1000 / (double)SDL_GetPerformanceFrequency());
		timeSpent += deltaTime;
		
		
		if (timeSpent >= 50) {
			render(cellArray);
			next_gen(cellArray, temp);
			timeSpent = 0;
		}

	}
	// free memory from arrays
	for (int i = 0; i < row; i++) {
		free(cellArray[i]);
	}
	free(cellArray);

	for (int i = 0; i < row; i++) {
		free(temp[i]);
	}
	free(temp);

	destroy_window();
	return 0;
}

bool init_window() {
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_FOREIGN);

	if (!window) {
		fprintf(stderr, "Error creating window. \n");
		return false;
	}
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		fprintf(stderr, "Error creating renderer. \n");
		return false;
	}
	return true;
}

void render(bool** cellArray) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	for (int i = 0.0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (cellArray[i][j]) {
				SDL_Rect cellRect = { (float)j * cell.width, (float) i * cell.height, cell.width, cell.height };
				SDL_SetRenderDrawColor(renderer, 210, 215, 211, 255);
				SDL_RenderFillRect(renderer, &cellRect);
			}
		}
	}

	SDL_RenderPresent(renderer);
}

void destroy_window() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void setup_cell_array(bool** cellArray) {

	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			int n = rand() % 2;
			cellArray[i][j] = n;
		}
	}
}

void print_array(bool** cellArray) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (j == col - 1) {
				if (i == row - 1) {
					if (cellArray[i][j])
						printf("5");
					else
						printf("-");
				}
				else {
					if (cellArray[i][j])
						printf("5\n");
					else
						printf("-\n");
				}
			}

			else {
				if (cellArray[i][j])
					printf("5");
				else
					printf("-");
			}

		}
	}
}

void copy_array(bool** cellArray, bool** temp) {
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			temp[i][j] = cellArray[i][j];
		}
	}
}

void next_gen(bool** cellArray, bool** temp) {

	copy_array(cellArray, temp);
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			int alive = 0;
			// Go through each neighbor.
			// For example if c and d are negative 1 we get top left cell 
			for (int h = -1; h < 2; h++)
			{
				for (int v = -1; v < 2; v++)
				{
					if (!(h == 0 && v == 0))
					{
						//
						int x = i + h;
						int y = j + v;

						if (x >= 0 && x < row && y >= 0 && y < col && temp[x][y])
						{
							++alive;
						}
					}
				}
			}
			if (alive < 2 || alive > 3)
			{
				cellArray[i][j] = false;
			}
			else if (alive == 3)
			{
				cellArray[i][j] = true;
			}
		}
	}

}

void clrscr()
{
	system("@cls||clear");
}