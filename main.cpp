#include "source/renderer.hpp"
#include "source/include.hpp"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const uint16_t RESX = 1800;
const uint16_t RESY = 1000;
const uint16_t numCircles = 2048;

double update_time = 0.2;
double window_time = 0;
double delta_time = 0;
double run_time = 0;
clock_t last_time = 0;
clock_t current_time = 0;

void init(const uint16_t RESX, const uint16_t RESY);
void render(vector<Circle>& circles);

int main(int argc, char* argv[]) {

	init(RESX, RESY);
	bool running = true;

	vector<Circle> circles = initializeCircles(numCircles, RESX, RESY);

	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}
		render(circles);
	}
	return 0;
}

void init(const uint16_t RESX, const uint16_t RESY) {
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("Paralela | 0.0 FPS", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, RESX, RESY, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void render(vector<Circle>& circles) {
	current_time = clock();
	delta_time = float(current_time - last_time) / CLOCKS_PER_SEC;
	last_time = current_time;
	run_time += delta_time;
	window_time += delta_time;

	if (window_time > update_time) {
		window_time -= update_time;
		double fps = 1.0 / delta_time;
		stringstream stream;
		stream << fixed << setprecision(2) << fps;
		string strFps = stream.str();
		SDL_SetWindowTitle(window, ("Paralela | " + strFps + " FPS").c_str());
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);



	//const int maxIterations = 8;
	//#pragma omp parallel for collapse(2)
	//for (int x = 0; x < RESX; x++) {
	//	for (int y = 0; y < RESY; y++) {
	//		vec2 uv = (vec2(x,y) - (vec2(RESX, RESY) / 2.0f)) / float(RESX);
	//		uv *= sin(float(run_time)) * 0.5f + 1.5f;
	//
	//		for(int i = 0; i < maxIterations; i++){
	//			uv *= 2.1f;
	//			uv = rot(run_time) * uv;
	//			uv = abs(uv);
	//			uv -= 0.5f;
	//		}
	//
	//		#pragma omp critical
	//		renderPoint(renderer, RESX, RESY, uvec2(x,y), static_cast<ivec3>(vec3(length(uv) < 0.4f) * 255.0f));
	//	}
	//}

	simulateStep(circles, vec2(0,0), vec2(RESX, RESY), delta_time);
	for (const Circle& circle : circles) {
		renderCircle(renderer, RESX, RESY, static_cast<uvec2>(circle.position), circle.radius, circle.color);
	}


	SDL_RenderPresent(renderer);
}