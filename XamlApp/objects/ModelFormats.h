
// ===========================================================================
/// <summary>
/// ModelFormats.h
/// GameEngine
/// created by Mehrdad Soleimanimajd on 27.06.2020
/// </summary>
/// <created>ʆϒʅ, 27.06.2020</created>
/// <changed>ʆϒʅ, 06.07.2023</changed>
// ===========================================================================

#ifndef MODELFORMATS_H
#define MODELFORMATS_H


struct Vertex
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 color;
};


struct VertexT
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 texture;
};


struct VertexL
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT3 normal;
};


struct VertexTL
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 texture;
    DirectX::XMFLOAT3 normal; // normal light
};


struct VertexCL
{
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 color;
    DirectX::XMFLOAT3 normal;
};


// Format: vertex containing the texture and diffuse light properties
// each triangle is read and written in one line
class VertexTexDiffuseL
{
public:
    static unsigned int read (const char* path, VertexTL** vertices); // read polygon vertices
    //static void write ( VertexTL*, unsigned int& ); // write a polygon
};


#endif // MODELFORMATS_H
