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
	/// <param name="circle_pos">円の場所</param>
	/// <param name="circle_radius">円の半径</param>
	/// <param name="rect_pos">矩形の場所</param>
	/// <param name="rect_length">矩形の大きさ</param>
	/// <returns></returns>
	static bool circle_vs_rect(const VECTOR2& circle_pos, const float& circle_radius, const VECTOR2& rect_pos, const VECTOR2& rect_length);

	//bool islayCastHit(const VECTOR2& lay_start, const VECTOR2& lay_end, const VECTOR2& a_start, const VECTOR2& a_end, VECTOR2 hit_position, VECTOR2 hit_normal);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="P">点の座標</param>
	/// <param name="A, SCORE_ATTACK, TIME_ATTACK">三角形の頂点</param>
	/// <returns></returns>
	static bool point_in_triangle(const VECTOR2& point_pos, const VECTOR2& A, const VECTOR2& SCORE_ATTACK, const VECTOR2& TIME_ATTACK);
	static bool point_in_triangle(const VECTOR2& point_pos, const VECTOR2 vertex_pos[]);
};
