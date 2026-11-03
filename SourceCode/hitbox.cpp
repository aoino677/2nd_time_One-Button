#include "all.h"
#include "HitBox.h"

CIRCLEHITBOX CreateCircleHitBox(float x, float y, float radius)
{
    CIRCLEHITBOX hitBox;

    hitBox.x = x;
    hitBox.y = y;
    hitBox.radius = radius;

    return hitBox;
}

RECTHITBOX CreateRectHitBox(
    float left,
    float right,
    float top,
    float bottom,
    float angle)
{
    RECTHITBOX hitBox;

    hitBox.left = left;
    hitBox.right = right;
    hitBox.top = top;
    hitBox.bottom = bottom;
    hitBox.angle = angle;

    return hitBox;
}

POINT2 RotatePoint(
    float x,
    float y,
    float centerX,
    float centerY,
    float angle)
{
    // íÜêSÇ©ÇÁÇÃãóó£Ç…Ç∑ÇÈ
    float dx = x - centerX;
    float dy = y - centerY;

    // âÒì]
    float rotatedX =
        dx * cosf(angle) - dy * sinf(angle);

    float rotatedY =
        dx * sinf(angle) + dy * cosf(angle);

    POINT2 point;

    // å≥ÇÃà íuÇ…ñﬂÇ∑
    point.x = rotatedX + centerX;
    point.y = rotatedY + centerY;

    return point;
}