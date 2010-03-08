#include "GridAccel.h"


namespace FormFactor {

	GridAccel::GridAccel(const std::vector<Reference<Primitive> > &p, bool refineImmediately) {

		if(refineImmediately) {
			for(unsigned int i = 0; i < p.size(); i++)
				p[i]->fullyRefine(prims);
		} else {
			prims = p;
		}

		// Initialize mailboxes
		nMailboxes = p.size();
		mailboxes = new Mailbox[nMailboxes];
	}


} // end FormFactor