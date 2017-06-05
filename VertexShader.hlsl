cbuffer MatrixBuffer
{
	matrix	worldMatrix;
	matrix	viewMatrix;
	matrix	projectionMatrix;
};

struct VertInput
{
	float4	position : POSITION;
	float4	color : COLOR;
	float3	normal : NORMAL;
};

struct VertOutput
{
	float4	position : SV_POSITION;
	float4	color : COLOR;
	float3	normal : NORMAL;
};

void	vert(in VertInput IN, out VertOutput OUT)
{
	IN.position.w = 1.0f;

	OUT.position = mul(mul(mul(IN.position, worldMatrix), viewMatrix), projectionMatrix);
	OUT.color = IN.color;
	OUT.normal = normalize(mul(IN.normal, worldMatrix));
}