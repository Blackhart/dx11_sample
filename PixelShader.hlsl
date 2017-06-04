struct fragInput
{
	float4	position : SV_POSITION;
	float4	color : COLOR;
};

struct fragOutput
{
	float4	color : SV_TARGET;
};

void	frag(in fragInput IN, out fragOutput OUT)
{
	OUT.color = IN.color;
}