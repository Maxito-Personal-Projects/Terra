#ifndef __TRANSFORM_H__
#define __TRANSFORM_H__

class Transform
{
public:

	Transform();
	~Transform();

public:

	float3 position = float3::zero;
	float3 scale = float3::one;
	Quat rotation = Quat::identity;	

};

#endif // !__TRANSFORM_H__
