// Copyright 2019 Loagan

#include "chrono.h"

namespace Chrono {

Date::Date(int yy, Month mm, int dd) : y{yy}, m{mm}, d{dd} {
  if (!is_date(yy, mm, dd)) throw Invalid();
}

const Date& default_date() {
  static Date dd{2001, Month::jan, 1};
  return dd;
}

Date::Date()
    : y{default_date().year()},
      m{default_date().month()},
      d{default_date().day()} {}

void Date::add_day(int n) {
  // TODO(logan): Must Implement
}
void Date::add_month(int n) {
  // TODO(logan): Must Implement
}
void Date::add_year(int n) {
  if (m == Month::feb && d == 29 && !leapyear(y + n)) {
    m = Month::mar;
    d = 1;
  }
  y += n;
}

bool is_date(int y, Month m, int d) {
  // TODO(logan): Must Implement
}
bool leapyear(int y) {
  // TODO(logan): Must Implement
}

bool operator==(const Date& l, const Date& r) {
  // TODO(logan): Must Implement
}
bool operator!=(const Date& l, const Date& r) {
  // TODO(logan): Must Implement
}

bool operator<<(std::ostream& os, const Date& d) {
  // TODO(logan): Must Implement
}
bool operator>>(std::istream& is, Date& dd) {
  // TODO(logan): Must Implement
}

enum class Day {
  sunday,
  monday,
  tuesday,
  wednesday,
  thursday,
  friday,
  saturday
};

Day day_of_week(const Date& d) {
  // TODO(logan): Must Implement
}
Day next_sunday(const Date& d) {
  // TODO(logan): Must Implement
}
Day next_weekdayy(const Date& d) {
  // TODO(logan): Must Implement
}

}  // namespace Chrono
