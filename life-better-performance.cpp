#include <SDL.h>
#include <chrono>
#include <random>
#include <iostream>

const int tile_amount = 50000;

int data[tile_amount];
int datawrite[tile_amount];

const int path[8] = {
	-101
	-100,
	-99
	-1,
	1,
	101,
	100,
	99
};

const int shoot[5] = {
	-198,
	-98,
	-2,
	98,
	198
};

int random(int x) {
	std::uniform_int_distribution<int> dist(0, x);
	int time = std::chrono::high_resolution_clock().now().time_since_epoch().count();
	std::default_random_engine rand(time);
	int randf = dist(rand);
	return randf;
}
bool changepos(int t,int i,int c) {
	if (t > 0 && t < tile_amount && datawrite[t] == 0) {
		datawrite[i + 2] = 0;
		switch (c) {
		case 0:
			datawrite[t + 1] = data[i + 3] - 1;
			break;
		case 1:
			datawrite[t + 1] = data[i + 3];
			if (datawrite[t + 1] > 510) {
				datawrite[t + 5] = 1;
				datawrite[t + 1] /= 2;
				datawrite[t + 6] = datawrite[t + 1];
				if (true) {
					std::cout << "yeet";
					switch (data[i + 4]) {
					case 0:
						datawrite[t + 7] = 1;
						break;
					case 1:
						datawrite[t + 7] = 0;
						break;
					}
				}
			}
			break;
		}
		if (data[i + 3] == 0) {
			return 1;
		}
		datawrite[t] = 1;
		return 1;
	}
	return 0;
}
int main(int argc, char* argv[]) {
	SDL_INIT_EVERYTHING;
	SDL_Window* window;
	SDL_Renderer* render;
	window = SDL_CreateWindow("yeet", 0, 0, 1000, 1000, NULL);
	render = SDL_CreateRenderer(window, 0, NULL);
	SDL_Rect rect;
	rect.h = 10;
	rect.w = 10;
	for (int i = 0; i < tile_amount; i += 5) {
		data[i] = i / 5 % 100;
		data[i + 1] = i / 500;
		if (random(1000) == 0) {
			data[i + 2] = 1;
		}
		if (data[i + 2] == 1) {
			data[i + 3] = 510;
		}
		else {
			data[i + 3] = 0;
		}
		data[i + 4] = 0;
	}
	while (true) {
		//render
		for (int i = 0; i < tile_amount; i += 5) {
			switch (data[i + 2]) {
			case 1:
				SDL_SetRenderDrawColor(render, data[i + 3] / 4 + 127, 0, 0, 255);
				rect.x = data[i] * 10;
				rect.y = data[i + 1] * 10;
				SDL_RenderFillRect(render, &rect);
				break;
			case 2:
				SDL_SetRenderDrawColor(render, 140, 140, 140, 255);
				rect.x = data[i] * 10;
				rect.y = data[i + 1] * 10;
				SDL_RenderFillRect(render, &rect);
				break;
			}
			
		}
		//change data
		for (int i = 0; i < tile_amount; i += 5) {
			if (data[i + 2] == 1) {	
				for (int i2 = 0; i2 < 8; i2++) {
					if (data[i + path[i2] * 5 + 2] == 2) {
						datawrite[i + path[i2] * 5 + 2] = 0;
						changepos(i + path[i2] * 5 + 2, i,1);
						break;
					}
				}
				switch (data[i + 4]) {
				case 1:
					std::cout << "yeet";
					for (int i2 = 0; i2 < 5;i2++) {
						for (int i3 = 0; i3 < 25; i3 += 5) {
							if (data[i + shoot[i2] * 5 + i3 + 2] == 1 && i3 + i2 != i) {
								datawrite[i + shoot[i2] * 5 + i3 + 2] = 2;
								datawrite[i + shoot[i2] * 5 + i3 + 3] = 0;
								datawrite[i + shoot[i2] * 5 + i3 + 4] = 0;
							}
						}
					}
				}
				switch (random(3)) {
				case 0:
					if (changepos(i - 1 * 5 + 2,i,0) == 1) {
						break;
					}
				case 1:
					if (changepos(i + 1 * 5 + 2,i,0) == 1) {
						break;
					}
				case 2:
					if (changepos(i - 100 * 5 + 2,i,0) == 1) {
						break;
					}
				case 3:
					if (changepos(i + 100 * 5 + 2,i,0) == 1) {
						break;
					}
				}
			}
		}
		//write data to new frame
		if (random(5) == 0) {
			datawrite[random(tile_amount / 5) * 5 + 2] = 2;
		}
		for (int i = 0; i < tile_amount; i += 5) {
			data[i + 2] = datawrite[i + 2];
			data[i + 3] = datawrite[i + 3];
			data[i + 4] = datawrite[i + 4];
		}
		SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
		SDL_RenderPresent(render);
		SDL_RenderClear(render);
		SDL_Delay(100);
	}
	return 0;
}
