#define PI	3.14159265359
#define ONE_OVER_PI	0.3183098861

inline float	NDF_Trowbridge_Reitz_GGX(float NdotH, float roughness);
inline float	GF_Schlick_GGX(float NdotV, float NdotL, float roughness);
inline half3	Fresnel_Schlick(half3 Rspec, float3 Dir1, float3 Dir2, int Power);
inline half3	Diffuse_Lambertian(half3 Rdiff);
inline half3	Specular_Cook_Torrance(float NDF, float GF, half3 F, float NdotV, float NdotL);
inline half3	Irradiance(half3 El, float NdotL);

struct fragInput
{
	float4	ClipPosition : SV_POSITION;
	float3	WorldNormal : NORMAL;
	float3	WorldLightDir : TEXCOORD0;
	float3	WorldViewDir : TEXCOORD1;
};

struct fragOutput
{
	half4	Color : SV_TARGET;
};

void	frag(in fragInput pIN, out fragOutput pOUT)
{
	// ~~~~~ Data ~~~~~
	
	float3	WorldNormal = normalize(pIN.WorldNormal);
	float3	WorldLightDir = normalize(pIN.WorldLightDir);
	
	float	NdotL = dot(WorldNormal, WorldLightDir);
	if (NdotL <= 0.0f)
	{
		pOUT.Color = half4(0.0, 0.0, 0.0, 1.0);
		return;
	}
	float3	WorldViewDir = normalize(pIN.WorldViewDir);
	float3	H = normalize(WorldLightDir + WorldViewDir);
	
	float	NdotV = dot(WorldNormal, WorldViewDir);
	float	NdotH = dot(WorldNormal, H);
	
	// ~~~~~ Microfacet ~~~~~
	
	float	NDF = NDF_Trowbridge_Reitz_GGX(NdotH, 1.0f);
	float	GF = GF_Schlick_GGX(NdotV, NdotL, 1.0f);
	
	// ~~~~~ Reflectance Values ~~~~~
	
	half3	Rspec = Fresnel_Schlick(half3(0.0, 0.0, 0.0), H, WorldLightDir, 5.0);
	half3	Rdiff = (1.0 - Rspec) * half3(1.0, 1.0, 1.0);
	
	// ~~~~~ BRDF terms ~~~~~
	
	half3	Ldiff = Diffuse_Lambertian(Rdiff);
	half3	Lspec = Specular_Cook_Torrance(NDF, GF, Rspec, NdotV, NdotL);
	half3	L0 = (Ldiff + Lspec) * Irradiance(half3(1.0, 1.0, 1.0), NdotL);
	
	// ~~~~~ OUTPUT ~~~~~
	
	pOUT.Color = half4(L0, 1.0);
}

inline float	NDF_Trowbridge_Reitz_GGX(float NdotH, float roughness)
{
	float r = roughness * roughness;
	float r2 = r * r;
	float denom = NdotH * NdotH * (r2 - 1.0) + 1.0;
	denom = 1.0 / (denom * denom);
	return r2 * ONE_OVER_PI * denom;
}

inline float	GF_Schlick_GGX(float NdotV, float NdotL, float roughness)
{
	roughness = (roughness + 1.0) / 2.0;
	float k = roughness / 2.0;
	float Gv = NdotV * (1.0 / (NdotV * (1.0 - k) + k));
	float Gl = NdotL * (1.0 / (NdotL * (1.0 - k) + k));
	return Gv * Gl;
}

inline half3	Fresnel_Schlick(half3 Rspec, float3 Dir1, float3 Dir2, int Power)
{
	return Rspec + (1.0 - Rspec) * pow((1.0 - dot(Dir1, Dir2)), Power);
}

inline half3	Diffuse_Lambertian(half3 Rdiff)
{
	return Rdiff * ONE_OVER_PI;
}

inline half3	Specular_Cook_Torrance(float NDF, float GF, half3 F, float NdotV, float NdotL)
{
	return NDF * GF * F * ONE_OVER_PI * (1.0 / (NdotV * NdotL));
}

inline half3	Irradiance(half3 El, float NdotL)
{
	return El * NdotL;
}