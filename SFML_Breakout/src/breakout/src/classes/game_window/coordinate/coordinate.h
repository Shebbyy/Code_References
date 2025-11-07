#pragma once

class coordinate {
public:
  coordinate() = delete;

  coordinate(float x, float y);

  [[nodiscard]] float get_x() const;
  [[nodiscard]] float get_y() const;
  void set_y(float new_y);
  void set_x(float new_x);

private:
  float m_x;
  float m_y;
};
