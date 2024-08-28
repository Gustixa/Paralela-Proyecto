/**
 * renderer.hpp
 * Funciones de renderizado y simulación de partículas
 * 
 * Contiene declaraciones para renderizar formas básicas,
 * manejar partículas y simular su movimiento.
 */

#pragma once
#include "include.hpp"

// Dibuja un punto en la pantalla
void renderPoint(SDL_Renderer* renderer, const uint16_t resx, const uint16_t resy, const uvec2& point, const ivec3& color);

// Dibuja una línea en la pantalla
void renderLine(SDL_Renderer* renderer, const uint16_t resx, const uint16_t resy, const uvec2& pointA, const uvec2& pointB, const ivec3& color);

// Dibuja un círculo en la pantalla
void renderCircle(SDL_Renderer* renderer, const uint16_t resx, const uint16_t resy, const uvec2 center, const uint16_t radius, const ivec3& color);

// Estructura que representa una partícula
struct Circle {
    vec2 position;        // Posición actual
    vec2 velocity;        // Velocidad actual
    ivec3 color;          // Color de la partícula
    float radius;         // Radio real (para colisiones)
    float display_radius; // Radio de visualización
    float mass;           // Masa de la partícula
};

// Crea un conjunto inicial de partículas
vector<Circle> initializeCircles(const uint16_t& numCircles, const uint16_t resx, const uint16_t resy);

// Maneja las colisiones de una partícula con los bordes
void checkBoundingBoxCollision(Circle& circle, const vec2& bounds);

// Obtiene un patrón de color basado en la posición y tiempo
vec3 getPattern(const vec2& position, const vec2& bounds, const float& radius, const float& deltaTime, const float& time);

// Actualiza la posición y velocidad de todas las partículas
void simulateStep(vector<Circle>& circles, const vec2& bounds, const float& deltaTime, const float& time);
