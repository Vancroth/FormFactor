#include "Primitive.h"

namespace FormFactor {

	void Primitive::fullyRefine(std::vector<Reference<Primitive> > &refined) const {
		std::vector<Reference<Primitive> > toRefine;
		toRefine.push_back(const_cast<Primitive *>(this));
		while(toRefine.size()) {
			Reference<Primitive> p = toRefine.back();
			toRefine.pop_back();
			if(p->canIntersect()) {
				refined.push_back(p);
			} else {
				p->refine(toRefine);
			}
		}
	}

}