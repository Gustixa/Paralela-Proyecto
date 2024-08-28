#include "renderer.hpp"

void renderPoint(SDL_Renderer* renderer, const uint16_t resx, const uint16_t resy, const uvec2& point, const ivec3& color) {
	if (point.x < resx && point.x >= 0 && point.y < resy && point.y >= 0) {
		SDL_SetRenderDrawColor(renderer, color.x, color.y , color.z, 255);
		SDL_RenderDrawPoint(renderer, point.x, resy - point.y);
	}
}

void renderLine(SDL_Renderer* renderer, const uint16_t resx, const uint16_t resy, const uvec2& pointA, uvec2& pointB, const ivec3& color) {
	int dx = abs(pointB.x - pointA.x);
	int dy = abs(pointB.y - pointA.y);
	int err = dx - dy;
	int x = pointA.x;
	int y = pointA.y;

	renderPoint(renderer, resx, resy, ivec2(x, y), color);

	while (x != pointB.x || y != pointB.y) {
		renderPoint(renderer, resx, resy, ivec2(x, y), color);

		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x += (pointA.x < pointB.x) ? 1 : -1;
		}
		if (e2 < dx) {
			err += dx;
			y += (pointA.y < pointB.y) ? 1 : -1;
		}
	}
}

void renderCircle(SDL_Renderer* renderer, const uint16_t resx, const uint16_t resy, const uvec2 center, const uint16_t radius, const ivec3& color) {
	int offsetX, offsetY, d;
	offsetX = 0;
	offsetY = radius;
	d = radius - 1;

	SDL_SetRenderDrawColor(renderer, color.x, color.y , color.z, 255);
	while (offsetY >= offsetX) {
		SDL_RenderDrawPoint(renderer, center.x + offsetX, center.y + offsetY);
		SDL_RenderDrawPoint(renderer, center.x + offsetY, center.y + offsetX);
		SDL_RenderDrawPoint(renderer, center.x - offsetX, center.y + offsetY);
		SDL_RenderDrawPoint(renderer, center.x - offsetY, center.y + offsetX);
		SDL_RenderDrawPoint(renderer, center.x + offsetX, center.y - offsetY);
		SDL_RenderDrawPoint(renderer, center.x + offsetY, center.y - offsetX);
		SDL_RenderDrawPoint(renderer, center.x - offsetX, center.y - offsetY);
		SDL_RenderDrawPoint(renderer, center.x - offsetY, center.y - offsetX);

		if (d >= 2 * offsetX) {
			d -= 2 * offsetX + 1;
			offsetX += 1;
		}
		else if (d < 2 * (radius - offsetY)) {
			d += 2 * offsetY - 1;
			offsetY -= 1;
		}
		else {
			d += 2 * (offsetY - offsetX - 1);
			offsetY -= 1;
			offsetX += 1;
		}
	}
}

mat2 rot(float a) {
	float c = cos(a);
	float s = sin(a);
	return mat2(c, -s, s, c);
}

vector<Circle> initializeCircles(const uint16_t& numCircles, const uint16_t resx, const uint16_t resy) {
	vector<Circle> circles(numCircles);
	for (uint16_t i = 0; i < numCircles; ++i) {
		circles[i].position = vec2(rand() % resx, rand() % resy);
		circles[i].velocity = vec2((rand() % 500) - 250, (rand() % 500) - 250);
		circles[i].radius = (rand() % 10) + 1;
		circles[i].mass = pi<float>() * pow(circles[i].radius, 2);
		circles[i].color = ivec3(255,255,255);
	}
	return circles;
}

void checkBoundingBoxCollision(Circle& circle, const vec2& bounds) {
	for (int i = 0; i < 2; ++i) {
		if (circle.position[i] - circle.radius < 0) {
			circle.position[i] = 0 + circle.radius;
			circle.velocity[i] = -circle.velocity[i];
		}
		else if (circle.position[i] + circle.radius > bounds[i]) {
			circle.position[i] = bounds[i] - circle.radius;
			circle.velocity[i] = -circle.velocity[i];
		}
	}
}

vec3 getPattern(const vec2& position, const vec2& bounds, const float& steps, const float& time) {
	vec3 val = vec3(0.0);
	for (float i = 0.0f; i < steps; i++) {
	}
	return vec3(position.x / bounds.x, position.y / bounds.y, 0);
}

void simulateStep(vector<Circle>& circles, const vec2& bounds, const float& deltaTime) {
	#pragma omp parallel for
	for (int i = 0; i < circles.size(); ++i) {
		circles[i].position += circles[i].velocity * deltaTime;
		circles[i].color = ivec3(getPattern(circles[i].position, bounds, 128, deltaTime) * 255.0f);
		#pragma critical
		checkBoundingBoxCollision(circles[i], bounds);
	}
}