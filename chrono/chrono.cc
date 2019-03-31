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
  if (y <= 0) {
    return false;
  }

  if (d <= 0) {
    return false;
  }

  if (m < Month::jan || Month::dec < m) {
    return false;
  }

  int days_in_month = 31;
  switch (m) {
    case Month::feb:
      days_in_month = (leapyear(y)) ? 29 : 28;
      break;
    case Month::apr:
    case Month::jun:
    case Month::sep:
    case Month::nov:
      days_in_month = 30;
      break;
    default:
      break;
  }

  if (days_in_month < d) {
    return false;
  }

  return true;
}

bool leapyear(int y) {
  // TODO(logan): Must Implement
}

bool operator==(const Date& l, const Date& r) {
  return l.year() == r.year() && l.month() == r.month() && l.day() == r.day();
}
bool operator!=(const Date& l, const Date& r) { return !(l == r); }

std::ostream& operator<<(std::ostream& os, const Date& d) {
  return os << '(' << d.year() << ',' << d.month() << ',' d.day() << ')';
}

std::istream& operator>>(std::istream& is, Date& dd) {
  int y, m, d;
  char ch1, ch2, ch3, ch4;
  is >> ch1 >> y >> ch2 >> m >> ch3 >> d >> ch4;
  if (!is) {
    return is;
  }

  if (ch1 != '(' || ch2 != ',' || ch3 != ',' || ch4 != ')') {
    is.clear(std::ios::ios_base::failbit);
    return is;
  }

  dd = Date(y, Month(m), d);

  return is;
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
