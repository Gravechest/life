  #include "include/SDL.h"
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <thread>
std::vector<std::vector<int>> vierkantjes;

SDL_Rect rect;

const int pixelcnt = 1;

int check = 0;
int del = -1;
int tt = 1;
long long end1, end2,result;
int camx, camy;

const int searchalg[8] = {
	0,1,
	1,0,
	-1,0,
	0,-1
};

bool hitbox(int x,int y) {
	tt = x;
	for (int i = x; i < vierkantjes.size(); i++) {
		if (vierkantjes[i][0] == x && vierkantjes[i][1] == y) {
			
			return 0;
		}
		if (x != tt) {
			return 1;
		}
		tt = x;
	}
	return 1;
}
bool search(int x, int y, int t) {
	tt = x;
	for (int i = x; i < vierkantjes.size(); i++) {
		if (vierkantjes[i][2] == t) {
			if (vierkantjes[i][0] == x && vierkantjes[i][1] == y) {
				vierkantjes[i][2] = -1;
				del = i;
				return 1;
			}
		}
		if (x != tt) {
			return 0;
		}
		tt = x;
	}
	return 0;
}
int rand(int x) {
	std::uniform_int_distribution<int> dist(0, x);
	int rand = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::default_random_engine yeet(rand);
	int yeet2 = dist(yeet);
	return yeet2;
}
int main(int argc,char *argv[]) {
	rect.w = 10;
	rect.h = 10;
	for (int i = 0; i < pixelcnt; i++) {
		vierkantjes.push_back({ i / 2 ,i / 2,0,255,0,0,0});
	}
	SDL_INIT_EVERYTHING;
	SDL_Window* window = SDL_CreateWindow("yeet", 200, 200, 500, 500, NULL);
	SDL_Event event;
	SDL_Renderer *renderer = SDL_CreateRenderer(window,-1, SDL_RENDERER_ACCELERATED);
	while (true) {
		SDL_SetRenderDrawColor(renderer, 140, 0, 0, 255);
		SDL_RenderClear(renderer);
		for (int i = 0; i < vierkantjes.size(); i++) {
			switch (vierkantjes[i][2]) {
			//de levensvormen
			case 0:
				SDL_SetRenderDrawColor(renderer, 0, vierkantjes[i][3], 0, 255);
				rect.x = vierkantjes[i][0] * 10;
				rect.y = vierkantjes[i][1] * 10;
				SDL_RenderFillRect(renderer, &rect);
				check = 0;
				//zoek naar voedsel
				for (int i2 = 0; i2 < sizeof(searchalg) / 4; i2 += 2) {
					if (search(vierkantjes[i][0] + searchalg[i2], vierkantjes[i][1] + searchalg[i2 + 1], 1) == 1) {
						vierkantjes[i][0] += searchalg[i2];
						vierkantjes[i][1] += searchalg[i2 + 1];
						check = 1;
						vierkantjes[i][3] += 100;
						if (vierkantjes[i][3] > 255) {
							vierkantjes.push_back({ vierkantjes[i][0] ,vierkantjes[i][1] ,0,vierkantjes[i][3] / 2});
							vierkantjes[i][3] /= 2;
						}
						break;
					}
				}
				if (check == 1) {
					break;
				}
				//willekeurige richting
				switch (rand(4)) {
				case 0:
					if (hitbox(vierkantjes[i][0] + 1, vierkantjes[i][1]) == 1 && vierkantjes[i][0] < 50) {
						vierkantjes[i][0]++;
						vierkantjes[i][3]--;
					};
					break;
				case 1:
					if (hitbox(vierkantjes[i][0] - 1, vierkantjes[i][1]) == 1 && vierkantjes[i][0] > 0) {
						vierkantjes[i][0]--;
						vierkantjes[i][3]--;
					};
					break;
				case 2:
					if (hitbox(vierkantjes[i][0], vierkantjes[i][1] + 1) == 1 && vierkantjes[i][1] < 50) {
						vierkantjes[i][1]++;
						vierkantjes[i][3]--;
					};
					break;
				case 3:
					if (hitbox(vierkantjes[i][0], vierkantjes[i][1] - 1) == 1 && vierkantjes[i][1] > 0) {
						vierkantjes[i][1]--;
						vierkantjes[i][3]--;
					};
					break;
				}
				if (vierkantjes[i][3] == 0) {
					vierkantjes.erase(vierkantjes.begin() + i);
				}
				break;
			//het voedsel
			case 1:
				SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
				rect.x = vierkantjes[i][0] * 10 + camx;
				rect.y = vierkantjes[i][1] * 10 + camy;
				SDL_RenderDrawRect(renderer, &rect);
				break;
			}
		}
		//voedselgen
		if (rand(0) == 0) {
			int x = rand(50);
			int y = rand(50);
			if (hitbox(x, y) == 1) {
				vierkantjes.push_back({ x ,y,1,0 });
			}
		}
		if (del != -1) {
			vierkantjes.erase(vierkantjes.begin() + del);
			del = -1;
		}
		SDL_RenderPresent(renderer);
		SDL_PollEvent(&event);
		std::sort(vierkantjes.begin(), vierkantjes.end());
		end2 = std::chrono::high_resolution_clock().now().time_since_epoch().count();
		result = end2 - end1;
	}
	return 0;
}
