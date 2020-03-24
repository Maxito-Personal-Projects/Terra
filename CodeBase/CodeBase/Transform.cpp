#include "Application.h"
#include "Transform.h"


Transform::Transform()
{
	localMatrix = localMatrix.Translate(position);
}


Transform::~Transform()
{
}
