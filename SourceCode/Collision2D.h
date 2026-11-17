#pragma once
#include "../GameLib/game_lib.h"

class Collision2D
{
public:
	static bool circle_vs_circle(const VECTOR2& pos1, const float& r1, const VECTOR2& pos2, const float& r2);
	static bool circle_in_circle(const VECTOR2& pos1, const float& r1, const VECTOR2& pos2, const float& r2);

	static bool point_in_circle(const VECTOR2& point_pos, const VECTOR2& circle_pos, float circle_radius);

	static bool point_in_rect(const VECTOR2& point_pos, const VECTOR2& rect_pos, const VECTOR2& rect_length);

	static bool circle_vs_segment(const VECTOR2& circle_pos, float circle_radius, const VECTOR2& seg_begin, const VECTOR2& seg_end);

	static bool rect_vs_rect(const VECTOR2& pos1, const VECTOR2& length1, const VECTOR2& pos2, const VECTOR2& length2);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="circle_pos">â~ÇÃèÍèä</param>
	/// <param name="circle_radius">â~ÇÃîºåa</param>
	/// <param name="rect_pos">ãÈå`ÇÃèÍèä</param>
	/// <param name="rect_length">ãÈå`ÇÃëÂÇ´Ç≥</param>
	/// <returns></returns>
	static bool circle_vs_rect(const VECTOR2& circle_pos, const float& circle_radius, const VECTOR2& rect_pos, const VECTOR2& rect_length);

	//bool islayCastHit(const VECTOR2& lay_start, const VECTOR2& lay_end, const VECTOR2& a_start, const VECTOR2& a_end, VECTOR2 hit_position, VECTOR2 hit_normal);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="point_pos">ì_ÇÃç¿ïW</param>
	/// <param name="A, B, C">éOäpå`ÇÃí∏ì_</param>
	/// <returns></returns>
	static bool point_in_triangle(const VECTOR2& P, const VECTOR2& A, const VECTOR2& B, const VECTOR2& C);
	static bool point_in_triangle(const VECTOR2& point_pos, const VECTOR2 vertex_pos[]);

	static float crossZ(VECTOR2 a, VECTOR2 b);

	static bool OBB_vs_OBB(const VECTOR2& pos1, const VECTOR2& length1, const VECTOR2& pivot1, float a1, const VECTOR2& pos2, const VECTOR2& length2, const VECTOR2& pivot2, float a2);

	//static bool segment_ve_segment(const VECTOR2& a, const VECTOR2& b, const VECTOR2& c, const VECTOR2& d, VECTOR2& hit_pos, float& t);
	//static bool segment_vs_rect(const VECTOR2& a, const VECTOR2& b, const VECTOR2& pos, const VECTOR2& length);
	//static bool segment_vs_rect(const VECTOR2& a, const VECTOR2& b, const VECTOR2& pos, const VECTOR2& length, VECTOR2& hit_pos, float& min_t);
	//static bool triangle_vs_rect(const VECTOR2& A, const VECTOR2& B, const VECTOR2& C, const VECTOR2& pos, const VECTOR2& size);
};