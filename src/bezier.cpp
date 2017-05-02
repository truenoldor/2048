#include "bezier.h"

namespace oxygine
{
	namespace Helper
	{
		oxygine::Vector2 CalcBezierCurve(std::vector<oxygine::Vector2> pts, const double& t)
		{
			int i, c;	
			double p;
			oxygine::Vector2 np;
			int n = static_cast<int>(pts.size()) - 1;

			c = 1;
			for (i = 0; i <= n; i++) {
				pts[i].x = pts[i].x * c;
				pts[i].y = pts[i].y * c;
				c = (n-i)*c/(i+1);
			}
			p = 1;
			for (i = 0; i <= n; i++) {
				pts[i].x = pts[i].x * p;
				pts[i].y = pts[i].y * p;
				p = p * t;
			}
			p = 1;
			for (i = n; i >= 0; i--) {
				pts[i].x = pts[i].x * p;
				pts[i].y = pts[i].y * p;
				p = p * (1-t);
			}
			np.x = 0;
			np.y = 0;
			for (i = 0; i <= n; i++) {
				np.x = np.x + pts[i].x;
				np.y = np.y + pts[i].y;
			}
			return np;
		}
	}
}
