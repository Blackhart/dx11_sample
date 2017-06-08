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
	float4	ClipPosition : SV_POSITION;
	float3	WorldNormal : NORMAL;
	float3	WorldLightDir : TEXCOORD0;
	float3	WorldViewDir : TEXCOORD1;
};

void	vert(in VertInput pIN, out VertOutput pOUT)
{
	// ~~~~~ Data ~~~~~
	
	pIN.position.w = 1.0f;
	
	pOUT.WorldNormal =  normalize(mul(pIN.normal, worldMatrix));
	float3 lWorldPos = mul(pIN.position, worldMatrix);
	float3 lWorldCameraPos = float3(0.0f, 1.0f, 5.0f);
	pOUT.WorldViewDir = lWorldCameraPos - lWorldPos;
	pOUT.WorldLightDir = normalize(float3(0.0f, 1.0f, 0.0f));
	
	// ~~~~~ OUTPUT ~~~~~
	
	pOUT.ClipPosition = mul(mul(mul(pIN.position, worldMatrix), viewMatrix), projectionMatrix);
}