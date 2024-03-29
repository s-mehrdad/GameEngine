﻿
// ===========================================================================
/// <summary>
/// Game.h
/// GameEngine
/// created by Mehrdad Soleimanimajd on 01.11.2019
/// </summary>
/// <created>ʆϒʅ, 01.11.2019</created>
/// <changed>ʆϒʅ, 05.07.2023</changed>
// ===========================================================================

#ifndef GAME_H
#define GAME_H


#include "Core.h"
#include "directx/Universe.h"
#include "resources/Shader.h"
#include "resources/Texture.h"
#include "objects/Polygons.h"


class Game
{
    friend class TheCore;
private:
    TheCore* m_core; // pointer to the framework core

    Universe* m_universe; // world, camera, lights...

    ShaderColour* m_shaderColour;
    ShaderTexture* m_shaderTexture;
    Texture<TargaHeader>* m_texture; // texture resource
    ShaderDiffuseLight* m_shaderDiffuseLight;

    // 2D models:
    Triangles* _2d_triangles; // three triangles
    Line* _2d_line; // a line (clockwise turn, dynamic rewrite)
    TexturedTriangles* _2d_texturedTriangles; // two textured triangles
    LightedTriangle* _2d_lightedTriangle; // one triangle illuminated by diffuse light

    // 3D models
    Cube* _3d_cube;

    bool m_allocated; // true if resources allocation was successful
    bool m_paused; // true if the game is paused
    bool m_initialized; // true if initialization was successful

    void m_allocateResources (void); // resources creation
public:
    Game (::IUnknown*, const int&, const int&); // game initialization
    //~Game ( void );
    const bool& m_isReady (void); // get the initialized state

    const bool m_run (void); // game engine loop
    void m_render (void); // render the scene
    void m_update (void); // updating the game world
    bool& m_isPaused (void); // get the paused state
    TheCore* m_getCore (void); // get the pointer to D3D core
    Universe* m_getUniverse (void); // get the pointer to game universe
    void m_onSuspending (void); // suspension preparation
    void m_validate (void); // game's resources/objects validation/reallocation

};


#endif // !GAME_H
