#pragma once
#include <cmath>
#include <functional>
#include <random>

namespace  TerrainHeightFunctions {

    using HeightFunction = std::function<float(float,float)>;


// =============================================================================
// MATHEMATICAL SURFACES
// =============================================================================

HeightFunction paraboloid(float a = 0.1f, float b = 0.1f, float offsetX = 0.0f, float offsetY = 0.0f) {
    return [a, b, offsetX, offsetY](float x, float y) -> float {
        float dx = x - offsetX;
        float dy = y - offsetY;
        return a * dx * dx + b * dy * dy;
    };
}

HeightFunction hyperbolicParaboloid(float scale = 0.05f, float offsetX = 0.0f, float offsetY = 0.0f) {
    return [scale, offsetX, offsetY](float x, float y) -> float {
        float dx = x - offsetX;
        float dy = y - offsetY;
        return scale * (dx * dx - dy * dy);
    };
}

HeightFunction torus(float majorRadius = 5.0f, float minorRadius = 2.0f, float offsetX = 0.0f, float offsetY = 0.0f) {
    return [majorRadius, minorRadius, offsetX, offsetY](float x, float y) -> float {
        float dx = x - offsetX;
        float dy = y - offsetY;
        float r = std::sqrt(dx*dx + dy*dy);
        float distFromTorus = std::abs(r - majorRadius);
        return minorRadius - distFromTorus;
    };
}

HeightFunction gaussianBell(float amplitude = 3.0f, float sigmaX = 3.0f, float sigmaY = 3.0f, float offsetX = 0.0f, float offsetY = 0.0f) {
    return [amplitude, sigmaX, sigmaY, offsetX, offsetY](float x, float y) -> float {
        float dx = (x - offsetX) / sigmaX;
        float dy = (y - offsetY) / sigmaY;
        return amplitude * std::exp(-(dx*dx + dy*dy) / 2.0f);
    };
}

HeightFunction sinc(float frequency = 1.0f, float amplitude = 3.0f, float offsetX = 0.0f, float offsetY = 0.0f) {
    return [frequency, amplitude, offsetX, offsetY](float x, float y) -> float {
        float dx = x - offsetX;
        float dy = y - offsetY;
        float r = std::sqrt(dx*dx + dy*dy) * frequency;
        if (r == 0.0f) return amplitude;
        return amplitude * std::sin(r) / r;
    };
}

// =============================================================================
// GEOMETRIC PATTERNS
// =============================================================================

HeightFunction checkerboard(float tileSize = 2.0f, float height = 1.0f, float offsetX = 0.0f, float offsetY = 0.0f) {
    return [tileSize, height, offsetX, offsetY](float x, float y) -> float {
        float dx = x - offsetX;
        float dy = y - offsetY;
        int checkX = static_cast<int>(std::floor(dx / tileSize));
        int checkY = static_cast<int>(std::floor(dy / tileSize));
        return ((checkX + checkY) % 2 == 0) ? height : -height;
    };
}

HeightFunction hexagonalTiling(float radius = 2.0f, float height = 1.0f, float offsetX = 0.0f, float offsetY = 0.0f) {
    return [radius, height, offsetX, offsetY](float x, float y) -> float {
        float dx = x - offsetX;
        float dy = y - offsetY;
        // Hexagonal grid approximation
        float hexX = dx / (radius * 1.5f);
        float hexY = (dy - (static_cast<int>(hexX) % 2) * radius * 0.866f) / (radius * 1.732f);

        int gridX = static_cast<int>(std::round(hexX));
        int gridY = static_cast<int>(std::round(hexY));

        return ((gridX + gridY) % 3 == 0) ? height : 0.0f;
    };
}

HeightFunction voronoiCells(int numSeeds = 16, float cellHeight = 2.0f, float offsetX = 0.0f, float offsetY = 0.0f) {
    static std::vector<std::pair<float, float>> seeds;
    static bool initialized = false;

    if (!initialized) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(-10.0f, 10.0f);

        seeds.clear();
        for (int i = 0; i < numSeeds; i++) {
            seeds.emplace_back(dis(gen), dis(gen));
        }
        initialized = true;
    }

    return [cellHeight, offsetX, offsetY](float x, float y) -> float {
        float dx = x - offsetX;
        float dy = y - offsetY;
        float minDist = std::numeric_limits<float>::max();
        int closestSeed = 0;

        for (size_t i = 0; i < seeds.size(); i++) {
            float dist = std::sqrt(std::pow(dx - seeds[i].first, 2) + std::pow(dy - seeds[i].second, 2));
            if (dist < minDist) {
                minDist = dist;
                closestSeed = i;
            }
        }

        return cellHeight * std::sin(closestSeed * 0.7f);
    };
}



// =============================================================================
// ANIMATED FUNCTIONS
// =============================================================================

HeightFunction flowingRiver(float time, float meander = 2.0f, float depth = 2.0f, float width = 3.0f, float offsetX = 0.0f, float offsetY = 0.0f) {
    return [time, meander, depth, width, offsetX, offsetY](float x, float y) -> float {
        float dx = x - offsetX;
        float dy = y - offsetY;
        float riverCenter = meander * std::sin(dx * 0.1f + time * 0.5f);
        float distFromRiver = std::abs(dy - riverCenter);
        return -depth * std::exp(-std::pow(distFromRiver / width, 2.0f));
    };
}

HeightFunction breathingBlobs(float time, float frequency = 0.3f, int numBlobs = 5,
                              float offsetX = 0.0f, float offsetY = 0.0f)
{
    return [time, frequency, numBlobs, offsetX, offsetY](float x, float y) -> float {
        float height = 0.0f;
        for (int i = 0; i < numBlobs; i++) {
            float phase = i * 2.0f * 3.14f / numBlobs;
            float centerX = 6.0f * std::cos(phase + time * frequency) + offsetX;
            float centerY = 6.0f * std::sin(phase + time * frequency) + offsetY;
            float radius = 2.0f + std::sin(time * 2.0f + phase);

            float dist = std::sqrt((x - centerX)*(x - centerX) +
                                   (y - centerY)*(y - centerY));
            height += 2.0f * std::exp(-std::pow(dist / radius, 2.0f));
        }
        return height;
    };
}


HeightFunction morphingTerrain(float time, float morphSpeed = 0.1f, float offsetX = 0.0f, float offsetY = 0.0f) {
    return [time, morphSpeed, offsetX, offsetY](float x, float y) -> float {
        float dx = x - offsetX;
        float dy = y - offsetY;
        float t = time * morphSpeed;
        float base = std::sin(dx * 0.1f + t) * std::cos(dy * 0.1f + t);
        float ripples = 0.5f * std::sin(std::sqrt(dx*dx + dy*dy) * 0.5f - t * 5.0f);
        float noise = 0.3f * std::sin(dx * 0.3f + t * 2.0f) * std::cos(dy * 0.2f + t * 1.5f);

        return base + ripples + noise;
    };
}

HeightFunction orbitalRings(float time, int numRings = 4, float baseRadius = 3.0f, float offsetX = 0.0f, float offsetY = 0.0f) {
    return [time, numRings, baseRadius, offsetX, offsetY](float x, float y) -> float {
        float dx = x - offsetX;
        float dy = y - offsetY;
        float height = 0.0f;
        for (int i = 0; i < numRings; i++) {
            float radius = baseRadius * (i + 1);
            float speed = 1.0f / (i + 1);
            float angle = time * speed + i * 3.14f / 2.0f;

            float centerX = radius * std::cos(angle);
            float centerY = radius * std::sin(angle);

            float dist = std::sqrt((dx - centerX)*(dx - centerX) + (dy - centerY)*(dy - centerY));
            height += std::exp(-dist * 2.0f);
        }
        return height;
    };
}



}
