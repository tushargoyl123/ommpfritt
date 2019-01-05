#pragma once

#include <armadillo>

namespace omm
{

class Point
{
public:
  explicit Point(const arma::vec2& position);
  Point();
  Point(double argument, double magnitude);
  arma::vec2 position;
  arma::vec2 left_tangent;
  arma::vec2 right_tangent;
  arma::vec2 left_position() const;
  arma::vec2 right_position() const;
  static constexpr auto TYPE = "Point";
  bool is_selected = false;
  void swap(Point& other);
};

}  // namespace omm
