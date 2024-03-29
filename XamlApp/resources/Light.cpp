﻿
// ===========================================================================
/// <summary>
/// Light.cpp
/// GameEngine
/// created by Mehrdad Soleimanimajd on 27.06.2020
/// </summary>
/// <created>ʆϒʅ, 27.06.2020</created>
/// <changed>ʆϒʅ, 06.07.2023</changed>
// ===========================================================================

#include "pch.h"
#include "Light.h"


DiffuseLight::DiffuseLight () :
    /*m_diffuseLightBuffer ( nullptr ),*/ m_initialized (false)
{

    m_ambientColour = DirectX::XMFLOAT4 (0.0f, 0.0f, 0.0f, 1.0f);
    m_diffuseColour = DirectX::XMFLOAT4 (0.0f, 0.0f, 0.0f, 1.0f);
    m_diffuseDirection = DirectX::XMFLOAT3 (0.0f, 0.0f, 0.0f);
    m_initialized = true;

    //if (m_diffuseLightBuffer)
    //{
    //  m_diffuseLightBuffer->Release ();
    //  m_diffuseLightBuffer = nullptr;
    //}

};


//DiffuseLight::~DiffuseLight ( void )
//{
//
//};


void DiffuseLight::m_setAmbientColour (const float* colour)
{
    m_ambientColour = DirectX::XMFLOAT4 {colour [0], colour [1], colour [2], colour [3]};
};


void DiffuseLight::m_setDiffuseColour (const float* colour)
{
    m_diffuseColour = DirectX::XMFLOAT4 {colour [0], colour [1], colour [2], colour [3]};
};


void DiffuseLight::m_setDirection (const float* direction)
{
    m_diffuseDirection = DirectX::XMFLOAT3 {direction [0], direction [1], direction [2]};
};
