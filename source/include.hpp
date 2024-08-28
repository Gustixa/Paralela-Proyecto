#pragma once

#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <windows.h>
#include <stdexcept>
#include <iostream>
#include <optional>
#include <direct.h>
#include <stdint.h>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <numeric>
#include <cerrno>
#include <vector>
#include <math.h>
#include <string>
#include <omp.h>
#include <ctime>
#include <array>
#include <map>

#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;
using namespace glm;

#undef min
#undef max
#undef clamp
#undef near
#undef far

#define PI          float(3.14159265f)
#define TWO_PI      float(6.28318531f)
#define INVERTED_PI float(0.31830989f)
#define DEG_RAD     float(0.01745329f)