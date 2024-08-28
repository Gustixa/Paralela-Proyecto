#include "source/renderer.hpp"
#include "source/include.hpp"

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

const uint16_t RESX = 1000;
const uint16_t RESY = 600;
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

vec2 rot(vec2 uv,float a){
	return vec2(uv.x*cos(a)-uv.y*sin(a),uv.y*cos(a)+uv.x*sin(a));
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

	const int maxIterations = 6;
	const ivec2 start = ivec2((RESX / 2) - (RESX / 10), (RESY / 2) - (RESY / 10));
	const ivec2 end   = ivec2((RESX / 2) + (RESX / 10), (RESY / 2) + (RESY / 10));

	#pragma omp parallel for collapse(2)
	for (int x = start.x; x < end.x; x++) {
		for (int y = start.y; y < end.y; y++) {
			vec2 uv = (vec2(x,y) - (vec2(RESX, RESY) / 2.0f)) / float(RESX) * 10.0f;
			uv *= sin(float(run_time)) * 0.5f + 1.5f;
	
			for(int i = 0; i < maxIterations; i++){
				uv *= 2.1f;
				uv = rot(uv, run_time);
				uv = abs(uv);
				uv -= 0.5f;
			}
	
			#pragma omp critical
			if (length(uv) <= 0.4f)
				renderPoint(renderer, RESX, RESY, uvec2(x,y), ivec3(255));
		}
	}

	simulateStep(circles, vec2(RESX, RESY), delta_time, current_time);
	for (const Circle& circle : circles) {
		renderCircle(renderer, RESX, RESY, static_cast<uvec2>(circle.position), circle.display_radius, circle.color);
	}


	SDL_RenderPresent(renderer);
}