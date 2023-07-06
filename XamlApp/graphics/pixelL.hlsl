
// ===========================================================================
/// <summary>
/// pixelL.hlsl
/// GameEngine
/// created by Mehrdad Soleimanimajd on 27.06.2020
/// </summary>
/// <created>}Y{, 27.06.2020</created>
/// <changed>}Y{, 06.07.2023</changed>
// ===========================================================================

// global declarations (modified externally)
Texture2D shaderTexture; // texture resource
SamplerState samplerType; // the way the texture is rendered

cbuffer LightBuffer // buffer object type (ambient / diffuse light properties)
{
    float4 ambientColour; // ambient light colour
    float4 diffuseColour; // diffuse light colour
    float3 lightDirection; // diffuse light direction
    float padding; // to size the structure at multiple of 4
}

struct Pixel // pixel shader input type
{
    float4 position : SV_POSITION; // corrected vertex position
    float2 tex : TEXCOORD0; // texture coordinate
    float3 normal : NORMAL; // the calculated and normalized light
};


float4 main(Pixel input) : SV_TARGET // processes on pixel
{
  
  // SV_TARGET: the semantic indicates that the return value of texture pixel shader should match the render target format
  
  
  // process:
  //-- sampling from the texture, render the polygon face 
  //-- calculation of the amount of light on polygon face
  
  // output:
  //-- resulted pixel containing location and properties combined from vertex, texture and light
  
  
    float4 textureColour;
    float3 lightDir;
    float lightIntensity;
    float4 colour;

  // using sampler, sample the pixel colour from the texture at this texture coordinate location.
    textureColour = shaderTexture.Sample(samplerType, input.tex);
  
  // default output colour is set to ambient light value for all pixels
    colour = ambientColour;
  
  // light direction inversion
    lightDir = -lightDirection;
  
  // calculation of the amount of light:
  // dot product between the normal vector of polygon and the light direction vector
  // clamp the result within the range of 0 and 1
    lightIntensity = saturate(dot(input.normal, lightDir));
  
    if (lightIntensity > 0.0f)
    {
    // calculate the final colour (ambient) based on diffuse colour and light intensity
        colour += (diffuseColour * lightIntensity);
    }
  
  // clamp the final light colour
    colour = saturate(colour);
  
  // combination (multiplication) of textured pixel and the final diffuse colour
    colour = colour * textureColour;
  
    return colour; // final pixel colour
  
}
