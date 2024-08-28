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

void checkBoundingBoxCollision(Circle& circle, const vec2& minBounds, const vec2& maxBounds) {
	for (int i = 0; i < 2; ++i) {
		if (circle.position[i] - circle.radius < minBounds[i]) {
			circle.position[i] = minBounds[i] + circle.radius;
			circle.velocity[i] = -circle.velocity[i];
		}
		else if (circle.position[i] + circle.radius > maxBounds[i]) {
			circle.position[i] = maxBounds[i] - circle.radius;
			circle.velocity[i] = -circle.velocity[i];
		}
	}
}

vec3 palette(const float& time) {
	vec3 d = vec3(0.263, 0.416, 0.557);
	return cos(2.5f * (time + d));
}

vec3 getPattern(vec2 uv, const float& steps, const float& time) {
	vec2 uv_0 = uv;
	vec3 val = vec3(0.0);
	for (float i = 0.0f; i < steps; i++) {
		uv = glm::fract(uv * 1.5f) - 0.5f;

		float d = length(uv) * exp(-length(uv_0));
		vec3 col = palette(length(uv_0) + i * 0.4f + time * 0.4f);

		d = sin(d * 8.0f + time) / 8.0f;
		d = abs(d);

		d = pow(0.01f / d, 1.2f);

		val += col * d;
	}
	return val;
}

void simulateStep(vector<Circle>& circles, const vec2& minBounds, const vec2& maxBounds, const float& deltaTime) {
	#pragma omp parallel for
	for (int i = 0; i < circles.size(); ++i) {
		circles[i].position += circles[i].velocity * deltaTime;
		circles[i].color = ivec3(getPattern(circles[i].position, 128, deltaTime) * 255.0f);
		#pragma critical
		checkBoundingBoxCollision(circles[i], minBounds, maxBounds);
	}
}