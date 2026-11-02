#pragma once
#include "all.h"
struct CIRCLEHITBOX
{
	float x;
	float y;
	float radius;
};

struct RECTHITBOX
{
	float left;
	float right;
	float top;
	float bottom;
	float angle;
};

// ‰~HitBox‚ğì‚é
CIRCLEHITBOX CreateCircleHitBox(
	float x,
	float y,
	float radius);

// lŠpHitBox‚ğì‚é
RECTHITBOX CreateRectHitBox(
	float left,
	float right,
	float top,
	float bottom,
	float angle);

struct POINT2
{
	float x;
	float y;
};

POINT2 RotatePoint(
	float x,
	float y,
	float centerX,
	float centerY,
	float angle);