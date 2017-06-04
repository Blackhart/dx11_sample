struct fragInput
{
	float4	position : SV_POSITION;
	float4	color : COLOR;
};

float4 frag(in fragInput IN) : SV_TARGET
{
	return IN.color;
}