#pragma once
#include "Noise.h"
#include <functional>

using HeightFunc = std::function<float(float, float)>;

class ITerrainHeightStrategy
{
public:
    virtual float GetHeight(float x, float y) = 0;
    virtual ~ITerrainHeightStrategy() = default;
};

struct NoiseStrategy : public ITerrainHeightStrategy
{
    float frequency;
    float amplitude;
    FastNoiseLite noise;

    NoiseStrategy(float frequency, float amplitude)
        : frequency(frequency), amplitude(amplitude)
    {
        noise.SetFrequency(frequency);
    }

    float GetHeight(float x, float y) override
    {
        return noise.GetNoise(x * frequency, y * frequency) * amplitude;
    }
};

struct GraphFunctionStrategy : public ITerrainHeightStrategy
{
    HeightFunc relation;

    GraphFunctionStrategy(HeightFunc relation)
        : relation(std::move(relation)) {}

    float GetHeight(float x, float y) override
    {
        return relation(x, y);
    }
};
