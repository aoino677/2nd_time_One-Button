#include "Collision2D.h"

float dot(const VECTOR2& a, const VECTOR2& b)
{
	return a.x * b.x + a.y * b.y;
}

VECTOR3 cross(const VECTOR3& a, const VECTOR3& b)
{
	return VECTOR3{ a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

bool Collision2D::circle_vs_circle(const VECTOR2& pos1, const float& r1, const VECTOR2& pos2, const float& r2)
{
	return (pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) <= (r1 + r2) * (r1 + r2);
}

bool Collision2D::circle_vs_segment(const VECTOR2& circle_pos, float circle_radius, const VECTOR2& seg_begin, const VECTOR2& seg_end)
{
	VECTOR2 BC = circle_pos - seg_begin;
	VECTOR2 BE = seg_end - seg_begin;
	VECTOR2 CE = seg_end - circle_pos;

	if (dot(BC, BE) < 0)
	{
		return vec2LengthSq(BC) <= circle_radius * circle_radius;
	}
	else if (dot(CE, BE) < 0)
	{
		return vec2LengthSq(CE) <= circle_radius * circle_radius;
	}
	else
	{
		float cross = fabsf(BC.x * BE.y - BC.y * BE.x) / vec2Length(BE);

		return cross <= circle_radius;
	}
}

bool Collision2D::circle_in_circle(const VECTOR2& pos1, const float& r1, const VECTOR2& pos2, const float& r2)
{
	return (pos2.x - pos1.x) * (pos2.x - pos1.x) + (pos2.y - pos1.y) * (pos2.y - pos1.y) <= (r1 - r2) * (r1 - r2);
}

bool Collision2D::point_in_circle(const VECTOR2& point_pos, const VECTOR2& circle_pos, float circle_radius)
{
	return (circle_pos.x - point_pos.x) * (circle_pos.x - point_pos.x) + (circle_pos.y - point_pos.y) * (circle_pos.y - point_pos.y) <= circle_radius * circle_radius;
}

bool Collision2D::point_in_rect(const VECTOR2& point_pos, const VECTOR2& rect_pos, const VECTOR2& rect_length)
{
	return rect_vs_rect(point_pos, { 0,0 }, rect_pos, rect_length);
}

bool Collision2D::rect_vs_rect(const VECTOR2& pos1, const VECTOR2& length1, const VECTOR2& pos2, const VECTOR2& length2)
{
	return (pos1.x + length1.x >= pos2.x && pos1.x <= pos2.x + length2.x
		&& pos1.y + length1.y >= pos2.y && pos1.y <= pos2.y + length2.y);
}

bool Collision2D::circle_vs_rect(const VECTOR2& circle_pos, const float& circle_radius, const VECTOR2& rect_pos, const VECTOR2& rect_length)
{
	if (point_in_rect(circle_pos, rect_pos - VECTOR2{ circle_radius, 0 }, rect_length + VECTOR2{ circle_radius * 2, 0 })) return true;
	else if (point_in_rect(circle_pos, rect_pos - VECTOR2{ 0, circle_radius }, rect_length + VECTOR2{ 0, circle_radius * 2 })) return true;
	else if (point_in_circle(rect_pos, circle_pos, circle_radius)
		|| point_in_circle(rect_pos + VECTOR2{ rect_length.x, 0 }, circle_pos, circle_radius)
		|| point_in_circle(rect_pos + VECTOR2{ 0, rect_length.y }, circle_pos, circle_radius)
		|| point_in_circle(rect_pos + rect_length, circle_pos, circle_radius)) return true;
	else return false;
}

bool Collision2D::point_in_triangle(const VECTOR2& P, const VECTOR2& A, const VECTOR2& B, const VECTOR2& C)
{
	VECTOR2 AB = A - B;
	VECTOR2 BC = B - C;
	VECTOR2 CA = C - A;

	VECTOR2 BP = P - B;
	VECTOR2 CP = P - C;
	VECTOR2 AP = P - A;

	bool isfront1 = cross(AB, BP).z > 0;
	bool isfront2 = cross(BC, CP).z > 0;
	bool isfront3 = cross(CA, AP).z > 0;

	return (isfront1 && isfront2 && isfront3)
		|| (!isfront1 && !isfront2 && !isfront3);
}

bool Collision2D::point_in_triangle(const VECTOR2& point_pos, const VECTOR2 vertex_pos[])
{
	return point_in_triangle(point_pos, vertex_pos[0], vertex_pos[1], vertex_pos[2]);
}


float Collision2D::crossZ(VECTOR2 a, VECTOR2 b)
{
	return a.y * b.x - a.x * b.y;
}

bool Collision2D::OBB_vs_OBB(const VECTOR2& pos1, const VECTOR2& length1, const VECTOR2& pivot1, float a1, const VECTOR2& pos2, const VECTOR2& length2, const VECTOR2& pivot2, float a2)
{
	VECTOR2 p1[4];
	VECTOR2 p2[4];
	VECTOR2 vec[4];

	{
		VECTOR2 vec1_x = { cosf(a1), sinf(a1) };
		VECTOR2 vec1_y = { -vec1_x.y, vec1_x.x };
		VECTOR2 vec2_x = { cosf(a2), sinf(a2) };
		VECTOR2 vec2_y = { -vec2_x.y, vec2_x.x };
		VECTOR2 position1 = pos1 - pivot1.x * vec1_x - pivot1.y * vec1_y;
		VECTOR2 position2 = pos2 - pivot2.x * vec2_x - pivot2.y * vec2_y;
		VECTOR2 width1 = length1.x * vec1_x;
		VECTOR2 height1 = length1.y * vec1_y;
		VECTOR2 width2 = length2.x * vec2_x;
		VECTOR2 height2 = length2.y * vec2_y;

		p1[0] = position1;
		p1[1] = position1 + width1;
		p1[2] = position1 + width1 + height1;
		p1[3] = position1 + height1;
		p2[0] = position2;
		p2[1] = position2 + width2;
		p2[2] = position2 + width2 + height2;
		p2[3] = position2 + height2;
		vec[0] = vec1_x;
		vec[1] = vec1_y;
		vec[2] = vec2_x;
		vec[3] = vec2_y;
	}

	bool check = true;
	for (auto& v : vec)
	{
		float p1_min = FLT_MAX, p1_max = -FLT_MAX, p2_min = FLT_MAX, p2_max = -FLT_MAX;
		for (auto& p : p1)
		{
			float dot = v.x * p.x + v.y * p.y;
			if (p1_min > dot)
			{
				p1_min = dot;
			}
			if (p1_max < dot)
			{
				p1_max = dot;
			}
		}

		for (auto& p : p2)
		{
			float dot = v.x * p.x + v.y * p.y;
			if (p2_min > dot)
			{
				p2_min = dot;
			}
			if (p2_max < dot)
			{
				p2_max = dot;
			}
		}

		if (p1_min > p2_max || p1_max < p2_min)
		{
			check = false;
			break;
		}
	}

	return check;
}

//bool Collision2D::segment_ve_segment(const VECTOR2& a, const VECTOR2& b, const VECTOR2& c, const VECTOR2& d, VECTOR2& hit_pos, float& t)
//{
//	VECTOR2 AB = b - a;
//	VECTOR2 CA = a - c;
//	VECTOR2 CD = d - c;
//	t = crossZ(-CD, CA) / crossZ(CD, AB);
//	float u = crossZ(-AB, -CA) / crossZ(AB, CD);
//	if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
//	{
//		hit_pos = a + AB * t;
//		return true;
//	}
//	return false;
//}
//
//bool Collision2D::segment_vs_rect(const VECTOR2& a, const VECTOR2& b, const VECTOR2& pos, const VECTOR2& length)
//{
//	VECTOR2 hit;
//	float t{ 0.0f };
//	return segment_vs_rect(a, b, pos, length, hit, t);
//}
//
//bool Collision2D::segment_vs_rect(const VECTOR2& a, const VECTOR2& b, const VECTOR2& pos, const VECTOR2& length, VECTOR2& hit_pos, float& min_t)
//{
//	VECTOR2 LU{ pos };
//	VECTOR2 RU{ pos.x + length.x, pos.y };
//	VECTOR2 LD{ pos.x , pos.y + length.y };
//	VECTOR2 RD{ pos + length };
//
//	if (a.x > pos.x && a.x < pos.x + length.x
//		&& a.y > pos.y && a.y < pos.y + length.y)
//	{
//		hit_pos = a;
//		return true;
//	}
//
//	VECTOR2 hit;
//	float t = 0;
//	min_t = FLT_MAX;
//	bool check = false;
//
//	if (segment_ve_segment(a, b, LU, RU, hit, t))
//	{
//		if (min_t > t)
//		{
//			min_t = t;
//			hit_pos = hit;
//		}
//		check = true;
//	}
//	if (segment_ve_segment(a, b, RD, RU, hit, t))
//	{
//		if (min_t > t)
//		{
//			min_t = t;
//			hit_pos = hit;
//		}
//		check = true;
//	}
//	if (segment_ve_segment(a, b, RD, LD, hit, t))
//	{
//		if (min_t > t)
//		{
//			min_t = t;
//			hit_pos = hit;
//		}
//		check = true;
//	}
//	if (segment_ve_segment(a, b, LU, LD, hit, t))
//	{
//		if (min_t > t)
//		{
//			min_t = t;
//			hit_pos = hit;
//		}
//		check = true;
//	}
//	return check;
//}
//
//
//bool Collision2D::triangle_vs_rect(const VECTOR2& A, const VECTOR2& B, const VECTOR2& C, const VECTOR2& pos, const VECTOR2& size)
//{
//	return point_in_rect(A, pos, size)
//		|| point_in_rect(B, pos, size)
//		|| point_in_rect(C, pos, size)
//		|| segment_vs_rect(A, B, pos, size)
//		|| segment_vs_rect(B, C, pos, size)
//		|| segment_vs_rect(C, A, pos, size);
//}
//