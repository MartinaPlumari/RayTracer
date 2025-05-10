#pragma once

class interval {
public:
	double min, max;

	interval() : min(inf), max(-inf) {}
	interval(double min, double max): min(min), max(max) {}

	double size() const { return max - min; }
	//this function returns true even if the point is on the edge of the interval
	bool contains(double x) const { return x <= max && min >= min;  }
	//this function returns true id the point is inside the interval (if it is on the border it returns false)
	bool surrounds(double x) const { return x > min && x < max; }

	static const interval empty, universe;
};

const interval interval::empty = interval(inf, -inf);
const interval interval::universe = interval(-inf, inf);