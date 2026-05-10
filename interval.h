#ifndef INTERVAL_H
#define INTERVAL_H

#include "rtinclude.h"

class interval {
public:
  double min, max;
  interval() : min(infinity), max(-infinity) {}
  interval(double _min, double _max) : min(_min), max(_max) {}

  double size() const {
    return max - min;
  }

  bool contains(double x){
    return min <= x && x <= max;
  }

  bool surrounds(double x){
    return min < x && x < max;
  }

  static const interval empty, universe;
};

const interval interval::empty = interval(infinity, -infinity);
const interval interval::universe = interval(-infinity, infinity);

#endif