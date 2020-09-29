#ifndef _UTIL_H_
#define _UTIL_H_

template<typename T>
class Vector2 {
public:

	Vector2(T x=T(), T y=T()) {
		this->x = x;
		this->y = y;
	}

	T x;
	T y;

};

#endif