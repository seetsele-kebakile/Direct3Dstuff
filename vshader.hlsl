struct VOUT
{
 float4 pos : SV_POSITION;
 float4 col : COLOR;
};

struct VIN
{
  float3 pos : POSITION;
  float3 col : COLOR;
};

cbuffer cob
{
	float4x4 wvp;
};
VOUT main(VIN v)
{
VOUT vo;
vo.pos = mul(float4(v.pos,1.0f),wvp);
vo.col = float4(v.col,1.0f);

return vo;
}