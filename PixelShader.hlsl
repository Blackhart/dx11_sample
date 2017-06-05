struct fragInput
{
	float4	position : SV_POSITION;
	float4	color : COLOR;
	float3	normal : NORMAL;
};

struct fragOutput
{
	float4	color : SV_TARGET;
};

void	frag(in fragInput IN, out fragOutput OUT)
{
	OUT.color = float4(IN.normal.x, IN.normal.y, IN.normal.z, 1.0f);
	//OUT.color = IN.color;
}