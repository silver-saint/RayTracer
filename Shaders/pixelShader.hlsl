cbuffer SceneConstantBuffer : register(b0)
{
    float radius;
    float borderThickness;
    float2 position;
    float4 borderColor;
    float padding[56];
}

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PSInput input) : SV_TARGET
{
    float distanceX = abs(input.position.x - position.x);
    float distanceY = abs(input.position.y - position.y);
    
    if ((distanceX * distanceX + distanceY * distanceY) < (radius * radius))
    {
        input.color = float4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else if
        ((distanceX * distanceX + distanceY * distanceY) < ((radius + borderThickness) * (radius + borderThickness)))
    {
        input.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
    }
        return input.color;
}