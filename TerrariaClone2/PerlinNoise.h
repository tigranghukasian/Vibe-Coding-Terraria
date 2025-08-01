#pragma once
#include <vector>
#include <random>

class PerlinNoise {
public:
    PerlinNoise(unsigned int seed = 0);
    double noise(double x, double y = 0, double z = 0) const;

private:
    std::vector<int> p; // Permutation vector

    double fade(double t) const;
    double lerp(double t, double a, double b) const;
    double grad(int hash, double x, double y, double z) const;
};

// Simple 1D noise function for terrain generation
double generateTerrain(double x, int octaves = 4, double persistence = 0.5, double scale = 0.01);