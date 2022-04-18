#include "IcarusRandom.h"

std::mt19937 IcarusRandom::s_RandomEngine;
std::uniform_int_distribution<std::mt19937::result_type> IcarusRandom::s_Distribution;
