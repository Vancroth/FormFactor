#include "Point.h"

namespace FormFactor {


	Point Point::minPt(const FormFactor::Point &pt1, const FormFactor::Point &pt2) {
		return Point(std::min(pt1.x, pt2.x), std::min(pt1.y, pt2.y), std::min(pt1.z, pt2.z));
			
	}

	Point Point::maxPt(const FormFactor::Point &pt1, const FormFactor::Point &pt2) {
		return Point(std::max(pt1.x, pt2.x), std::max(pt1.y, pt2.y), std::max(pt1.z, pt2.z));
		
	}

} // end FormFactor