#pragma once
#include <cmath>
#include <functional>
#include <random>

namespace  TerrainHeightFunctions {

    using HeightFunction = std::function<float(float,float)>;


// =============================================================================
// MATHEMATICAL SURFACES
// =============================================================================

HeightFunction paraboloid(float a = 0.1f, float b = 0.1f) {
    return [a, b](float x, float y) -> float {
        return a * x * x + b * y * y;
    };
}

HeightFunction hyperbolicParaboloid(float scale = 0.05f) {
    return [scale](float x, float y) -> float {
        return scale * (x * x - y * y);
    };
}

HeightFunction torus(float majorRadius = 5.0f, float minorRadius = 2.0f) {
    return [majorRadius, minorRadius](float x, float y) -> float {
        float r = std::sqrt(x*x + y*y);
        float distFromTorus = std::abs(r - majorRadius);
        return minorRadius - distFromTorus;
    };
}

HeightFunction gaussianBell(float centerX = 0.0f, float centerY = 0.0f, float sigmaX = 3.0f, float sigmaY = 3.0f) {
    return [centerX, centerY, sigmaX, sigmaY](float x, float y) -> float {
        float dx = (x - centerX) / sigmaX;
        float dy = (y - centerY) / sigmaY;
        return 3.0f * std::exp(-(dx*dx + dy*dy) / 2.0f);
    };
}

HeightFunction sinc(float frequency = 1.0f, float amplitude = 3.0f) {
    return [frequency, amplitude](float x, float y) -> float {
        float r = std::sqrt(x*x + y*y) * frequency;
        if (r == 0.0f) return amplitude;
        return amplitude * std::sin(r) / r;
    };
}

// =============================================================================
// GEOMETRIC PATTERNS
// =============================================================================

HeightFunction checkerboard(float tileSize = 2.0f, float height = 1.0f) {
    return [tileSize, height](float x, float y) -> float {
        int checkX = static_cast<int>(std::floor(x / tileSize));
        int checkY = static_cast<int>(std::floor(y / tileSize));
        return ((checkX + checkY) % 2 == 0) ? height : -height;
    };
}

HeightFunction hexagonalTiling(float radius = 2.0f, float height = 1.0f) {
    return [radius, height](float x, float y) -> float {
        // Hexagonal grid approximation
        float hexX = x / (radius * 1.5f);
        float hexY = (y - (static_cast<int>(hexX) % 2) * radius * 0.866f) / (radius * 1.732f);

        int gridX = static_cast<int>(std::round(hexX));
        int gridY = static_cast<int>(std::round(hexY));

        return ((gridX + gridY) % 3 == 0) ? height : 0.0f;
    };
}

HeightFunction voronoiCells(int numSeeds = 16, float cellHeight = 2.0f) {
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

    return [cellHeight](float x, float y) -> float {
        float minDist = std::numeric_limits<float>::max();
        int closestSeed = 0;

        for (size_t i = 0; i < seeds.size(); i++) {
            float dist = std::sqrt(std::pow(x - seeds[i].first, 2) + std::pow(y - seeds[i].second, 2));
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

HeightFunction flowingRiver(float time, float meander = 2.0f, float depth = 2.0f, float width = 3.0f) {
    return [time, meander, depth, width](float x, float y) -> float {
        float riverCenter = meander * std::sin(x * 0.1f + time * 0.5f);
        float distFromRiver = std::abs(y - riverCenter);
        return -depth * std::exp(-std::pow(distFromRiver / width, 2.0f));
    };
}

HeightFunction breathingBlobs(float time, float frequency = 0.3f, int numBlobs = 5) {
    return [time, frequency, numBlobs](float x, float y) -> float {
        float height = 0.0f;
        for (int i = 0; i < numBlobs; i++) {
            float phase = i * 2.0f * M_PI / numBlobs;
            float centerX = 6.0f * std::cos(phase + time * frequency);
            float centerY = 6.0f * std::sin(phase + time * frequency);
            float radius = 2.0f + std::sin(time * 2.0f + phase);

            float dist = std::sqrt((x - centerX)*(x - centerX) + (y - centerY)*(y - centerY));
            height += 2.0f * std::exp(-std::pow(dist / radius, 2.0f));
        }
        return height;
    };
}

HeightFunction morphingTerrain(float time, float morphSpeed = 0.1f) {
    return [time, morphSpeed](float x, float y) -> float {
        float t = time * morphSpeed;
        float base = std::sin(x * 0.1f + t) * std::cos(y * 0.1f + t);
        float ripples = 0.5f * std::sin(std::sqrt(x*x + y*y) * 0.5f - t * 5.0f);
        float noise = 0.3f * std::sin(x * 0.3f + t * 2.0f) * std::cos(y * 0.2f + t * 1.5f);

        return base + ripples + noise;
    };
}

HeightFunction orbitalRings(float time, int numRings = 4, float baseRadius = 3.0f) {
    return [time, numRings, baseRadius](float x, float y) -> float {
        float height = 0.0f;
        for (int i = 0; i < numRings; i++) {
            float radius = baseRadius * (i + 1);
            float speed = 1.0f / (i + 1);
            float angle = time * speed + i * M_PI / 2.0f;

            float centerX = radius * std::cos(angle);
            float centerY = radius * std::sin(angle);

            float dist = std::sqrt((x - centerX)*(x - centerX) + (y - centerY)*(y - centerY));
            height += std::exp(-dist * 2.0f);
        }
        return height;
    };
}

// =============================================================================
// COMBINATION UTILITIES
// =============================================================================

HeightFunction combine(const HeightFunction& f1, const HeightFunction& f2,
                      std::function<float(float,float)> combiner) {
    return [f1, f2, combiner](float x, float y) -> float {
        return combiner(f1(x, y), f2(x, y));
    };
}

HeightFunction additive(const HeightFunction& f1, const HeightFunction& f2) {
    return combine(f1, f2, [](float a, float b) { return a + b; });
}

HeightFunction multiplicative(const HeightFunction& f1, const HeightFunction& f2) {
    return combine(f1, f2, [](float a, float b) { return a * b; });
}

HeightFunction maximum(const HeightFunction& f1, const HeightFunction& f2) {
    return combine(f1, f2, [](float a, float b) { return std::max(a, b); });
}

HeightFunction minimum(const HeightFunction& f1, const HeightFunction& f2) {
    return combine(f1, f2, [](float a, float b) { return std::min(a, b); });
}

}
