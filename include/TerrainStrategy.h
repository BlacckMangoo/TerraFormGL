#pragma once
#include "Noise.h"
#include "functional"

using HeightFunc = std::function<float(float,float)>;



class ITerrainHeightStrategy
{

public:
    virtual float GetHeight( float x , float y ) = 0  ;
    virtual ~ITerrainHeightStrategy()  = default;

};

struct  NoiseStrategy :  public ITerrainHeightStrategy
{
    public:
    float frequency ;
    float amplitude ;

    NoiseStrategy(float frequency, float amplitude) : frequency(frequency), amplitude(amplitude) {};

     float GetHeight( float x , float y ) override
     {
         FastNoiseLite noise ;
         noise.SetFrequency(this->frequency);

         return noise.GetNoise(x*frequency,y*frequency)*amplitude;
     }
};

struct GraphFunctionStrategy : public ITerrainHeightStrategy
{

    HeightFunc* relation ;
    GraphFunctionStrategy(HeightFunc &relation)
    {
        this->relation = &relation;
    }

    float GetHeight(float x, float y) override
    {
        return relation(x,y);
    }

};