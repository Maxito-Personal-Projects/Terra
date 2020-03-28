#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

class Transform
{
public:

	Transform();
	~Transform();

public:

	float3 position = { 0.0f,0.0f,0.0f };
	float3 scale = float3::one;
	Quat rotation = Quat::identity;

	float4x4 globalMatrix = float4x4::identity;
	float4x4 localMatrix = float4x4::identity;
};

#endif // !__TRANSFORM_H__
