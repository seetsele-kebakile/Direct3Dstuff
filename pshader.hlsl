
struct VOUT
{
	float4 pos : SV_POSITION;
	float4 col : COLOR;
};

float4 main(VOUT v) : SV_TARGET
{

return v.col;
}