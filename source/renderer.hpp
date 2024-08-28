#pragma once

#include "include.hpp"

void renderPoint(SDL_Renderer* renderer, const uint16_t resx, const uint16_t resy, const uvec2& point, const ivec3& color);
void renderLine(SDL_Renderer* renderer, const uint16_t resx, const uint16_t resy, const uvec2& pointA, const uvec2& pointB, const ivec3& color);
void renderCircle(SDL_Renderer* renderer, const uint16_t resx, const uint16_t resy, const uvec2 center, const uint16_t radius, const ivec3& color);

struct Circle {
	vec2 position;
	vec2 velocity;
	ivec3 color;
	float radius;
	float mass;
};

const int maxIterations=6;

vector<Circle> initializeCircles(const uint16_t& numCircles, const uint16_t resx, const uint16_t resy);
void checkBoundingBoxCollision(Circle& circle, const vec2& bounds);

vec3 getPattern(const vec2& position, const vec2& bounds, const float& steps, const float& time);
void simulateStep(vector<Circle>& circles, const vec2& bounds, const float& deltaTime);