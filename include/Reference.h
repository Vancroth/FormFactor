/*
 * Provides framework for reference counting.
 *
 * A class to be handled by reference counting,
 * should inherit from ReferenceCounted.
 */

#pragma once

#include <vector>

#include "BoundingBox.h"


namespace FormFactor {

	// Inherit from this to use reference counting
	class ReferenceCounted {

	public:
		ReferenceCounted() {nReferences = 0;}

		int nReferences;

	private:
		ReferenceCounted(const ReferenceCounted &other);
		ReferenceCounted& operator=(const ReferenceCounted &rhs);
	};

	// Takes care of the memory management
	template<class T>
	class Reference {

	public:
		Reference(T *p = NULL) {
			ptr = p;
			if(ptr) ptr->nReferences++;
		}

		Reference(const Reference<T> &other) {
			ptr = other.ptr;
			if(ptr) ptr->nReferences++;
		}

		~Reference() {
			if(ptr && --(ptr->nReferences) == 0)
				delete ptr;
		}

		inline T* getPtr() const {return ptr;}

		Reference& operator=(const Reference<T> &other) {
			if(other.ptr) other.ptr->nReferences++;
			if(ptr && --(ptr->nReferences) == 0) delete ptr;
			ptr = other.ptr;
			return *this;
		}

		Reference& operator=(T *p) {
			if(p) p->nReferences++;
			if(ptr && --(ptr->nReferences) == 0) delete ptr;
			ptr = p;
			return *this;
		}

		T* operator->() {return ptr;}
		const T* operator->() const {return ptr;}
		operator bool() const {return ptr!=NULL;}
		bool operator<(const Reference<T> &other) const {return ptr < other.ptr;}
		bool operator>(const Reference<T> &other) const {return ptr > other.ptr;}

	private:
		T *ptr;
	};
} // end FormFactor
