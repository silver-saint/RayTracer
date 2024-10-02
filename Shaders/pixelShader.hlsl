cbuffer SceneConstantBuffer : register(b0)
{
    float radius;
    float2 position;
    float4 borderColor;
    float borderThickness;
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
    
    if ((distanceX * distanceX + distanceY * distanceY) > (radius * radius))
    {
        discard;
    }
    else if
        ((distanceX * distanceX + distanceY * distanceY) <= (radius * radius)  &&
         (distanceX * distanceX + distanceY * distanceY) >= ((radius * radius) - (borderThickness * borderThickness)))
    {
        input.color = borderColor;
    }
        return input.color;
}