// Copyright 2019 Loagan

#ifndef CHRONO_CHRONO_H_
#define CHRONO_CHRONO_H_

#include <iostream>

namespace Chrono {

enum class Month {
  jan = 1,
  feb,
  mar,
  apr,
  may,
  jun,
  jul,
  aug,
  sep,
  oct,
  nov,
  dec
};

class Date {
 public:
  class Invalid {};
  Date(int yy, Month mm, int dd);
  Date();

  int day() const { return d; }
  Month month() const { return m; }
  int year() const { return y; }

  void add_day(int n);
  void add_month(int n);
  void add_year(int n);

 private:
  int y;
  Month m;
  int d;
};

bool is_date(int y, Month m, int d);
bool leapyear(int y);

bool operator==(const Date& l, const Date& r);
bool operator!=(const Date& l, const Date& r);

bool operator<<(std::ostream& os, const Date& d);
bool operator>>(std::istream& is, Date& dd);

}  // namespace Chrono

#endif  // CHRONO_CHRONO_H_
