#include "TH10Bot/Common.h"
#include "TH10Bot/MyMath.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

namespace th
{
	// 勾股定理
	float_t MyMath::Distance(const Pointf& A, const Pointf& B)
	{
		float_t dx = A.x - B.x;
		float_t dy = A.y - B.y;
		return std::sqrt(dx * dx + dy * dy);
	}

	// 余弦定理
	float_t MyMath::Angle(const Pointf& A, const Pointf& B, const Pointf& C)
	{
		float_t AB = Distance(A, B);
		float_t AC = Distance(A, C);
		float_t BC = Distance(B, C);
		float_t cosA = (AB * AB + AC * AC - BC * BC) / (2.0f * AB * AC);
		float_t radianA = std::acos(cosA);
		// 角度 = 弧度 * 180 / PI
		return radianA * 180.0f / static_cast<float_t>(M_PI);
	}

	// 点到直线的垂足
	Pointf MyMath::FootPoint(const Pointf& A, const Pointf& B, const Pointf& P)
	{
		float_t dx = B.x - A.x;
		float_t dy = B.y - A.y;
		float_t u = ((P.x - A.x) * dx + (P.y - A.y) * dy) / (dx * dx + dy * dy);
		return Pointf(A.x + dx * u, A.y + dy * u);
	}

	Pointf MyMath::Rotate(const Pointf& P, const Pointf& C, float_t radianC)
	{
		// 平面中，一个点(x, y)绕任意点(cx, cy)顺时针旋转a度后的坐标
		// xx = (x - cx) * cos(-a) - (y - cy) * sin(-a) + cx;
		// yy = (x - cx) * sin(-a) + (y - cy) * cos(-a) + cy;
		// 平面中，一个点(x, y)绕任意点(cx, cy)逆时针旋转a度后的坐标
		// xx = (x - cx) * cos(a) - (y - cy) * sin(a) + cx;
		// yy = (x - cx) * sin(a) + (y - cy) * cos(a) + cy;
		float_t dx = P.x - C.x;
		float_t dy = P.y - C.y;
		float_t sinC = std::sin(radianC);
		float_t cosC = std::cos(radianC);
		Pointf PP;
		PP.x = dx * cosC - dy * sinC + C.x;
		PP.y = dx * sinC + dy * cosC + C.y;
		return PP;
	}
}
