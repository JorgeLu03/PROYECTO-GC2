Texture2D colorTex : register(t0);
SamplerState colorSampler : register(s0);

cbuffer ConstantBuffer : register(b0) {
    matrix wordlMatrix;
}

struct VS_Input {
    float4 pos : POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL;
};

PS_Input VS_Main(VS_Input vertex)
{
    PS_Input psOut;
    
    psOut.pos = mul(vertex.pos, wordlMatrix);
    psOut.normal = mul(vertex.normal, wordlMatrix);
    psOut.uv = vertex.uv;
    
    return psOut;
}

float4 PS_Main(PS_Input pix) : SV_TARGET 
{
    float4 textura = colorTex.Sample(colorSampler, pix.uv);
    clip(textura.a - 0.5f);
    
    return textura;
}