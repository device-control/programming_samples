#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>
#include <vector>
#include <sstream>
#include "Date.h"

/*static */
const int Date::DAYS_PER_MONTH[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

Date::Date()
{
	m_month = 1;
	m_day = 1;
	m_year = 1;
}

Date::Date(int y, int m, int d)
{
	m_year = y;
	m_month = m;
	m_day = d;

}

// "2016/10/11"
Date::Date(std::string& str)
{
	std::vector<std::string> list = split(str, '/');
	m_year = atoi(list[0].c_str());
	m_month = atoi(list[1].c_str());
	m_day = atoi(list[2].c_str());
}

Date::~Date()
{
}

Date Date::addDays(int days)
{
	struct tm t = { 0 };

	t.tm_year = m_year - 1900;
	t.tm_mon  = m_month - 1;
	t.tm_mday = m_day + days;
	if (mktime(&t) == -1){
		// 異常
		throw std::out_of_range("date out of range");
		//return Date(m_year, m_month, m_day);
	}
	return Date(t.tm_year + 1900, t.tm_mon + 1, t.tm_mday);
}

Date Date::operator + (int days)
{
	return (*this).addDays(days);
}

Date Date::operator - (int days)
{
	return (*this).addDays(-days);
}

Date Date::operator += (int days)
{
	Date date = (*this).addDays(days);
	(*this).m_year = date.m_year;
	(*this).m_month = date.m_month;
	(*this).m_day = date.m_day;
	return (*this);
}

Date Date::operator -= (int days)
{
	Date date = (*this).addDays(-days);
	(*this).m_year = date.m_year;
	(*this).m_month = date.m_month;
	(*this).m_day = date.m_day;
	return (*this);
}

bool Date::operator == (const Date& op2)
{
	if( ((*this).m_year == op2.m_year) &&
		((*this).m_month == op2.m_month) &&
		((*this).m_day == op2.m_day)){
			return true;
	}
	return false;
}

bool Date::operator != (const Date& op2)
{
	return !((*this) == op2);
}

bool Date::operator > (const Date& op2)
{
	// is_valid() が有効でない場合は、正しく処理されない
	int day1 = Date::get_days( (*this).m_year, (*this).m_month, (*this).m_day);
	int day2 = Date::get_days( op2.m_year, op2.m_month, op2.m_day);
	return day1 > day2;
}

bool Date::operator < (const Date& op2)
{
	// is_valid() が有効でない場合は、正しく処理されない
	int day1 = Date::get_days( (*this).m_year, (*this).m_month, (*this).m_day);
	int day2 = Date::get_days( op2.m_year, op2.m_month, op2.m_day);
	return day1 < day2;
}

bool Date::operator >= (const Date& op2)
{
	// is_valid() が有効でない場合は、正しく処理されない
	int day1 = Date::get_days( (*this).m_year, (*this).m_month, (*this).m_day);
	int day2 = Date::get_days( op2.m_year, op2.m_month, op2.m_day);
	return day1 >= day2;
}

bool Date::operator <= (const Date& op2)
{
	// is_valid() が有効でない場合は、正しく処理されない
	int day1 = Date::get_days( (*this).m_year, (*this).m_month, (*this).m_day);
	int day2 = Date::get_days( op2.m_year, op2.m_month, op2.m_day);
	return day1 <= day2;
}

/*static*/
bool Date::is_leap (int year)
{
	return (year % 400 == 0) || (year % 100 != 0 && year % 4 == 0) ? true: false;
}

/*static*/
Date Date::getToday()
{
	time_t t = time(NULL);
	struct tm now;
	localtime_s(&now, &t);
	return Date(now.tm_year+1900, now.tm_mon + 1, now.tm_mday);
}


bool Date::is_valid()
{
	return Date::is_valid(m_year,m_month,m_day);
}

/* static */
bool Date::is_valid(int y, int m, int d)
{
	// year 1~
	if( y < 1 ){
		return false; // 有効でない
	}
	// month 1~12
	if( (m < 1) && (m > 12) ){
		return false; // 有効でない
	}
	int days = Date::DAYS_PER_MONTH[m-1];
	if( (m == 2) && is_leap(y) ){
		days++; //28->29
	}
	// day 1~daya
	if( (d < 1) && (d > days) ){
		return false; // 有効でない
	}
	return true; // 有効
}


/*static*/
int Date::get_days(int y, int m, int d)
{
  // 1・2月 → 前年の13・14月
  if (m <= 2)
  {
    --y;
    m += 12;
  }
  int dy = 365 * (y - 1); // 経過年数×365日
  int c = y / 100;
  int dl = (y >> 2) - c + (c >> 2); // うるう年分
  int dm = (m * 979 - 1033) >> 5; // 1月1日から m 月1日までの日数
  return dy + dl + dm + d - 1;
}


/*static*/
std::vector<std::string> Date::split(const std::string &str, char sep)
{
    std::vector<std::string> v;
    std::stringstream ss(str);
    std::string buffer;
    while( std::getline(ss, buffer, sep) ) {
        v.push_back(buffer);
    }
    return v;
}


std::ostream& operator << (std::ostream& out_file, const Date& d)
{
    out_file << std::setw(2) << std::setfill('0') << d.m_month << '/' <<
		std::setw(2) << std::setfill('0') << d.m_day << '/' <<
		std::setw(2) << std::setfill('0') << d.m_year;
    return (out_file);
}

std::istream& operator >> (std::istream& in_file, Date& d)
{
    int month;
    int day;
    int year;
    char ch;

    month = 1;
    day = 1;
    year = 0;

    std::istream::sentry the_sentry(in_file, true);

    if (the_sentry) {
		if (in_file.fail()) return (in_file);

		in_file >>  month;
		if (in_file.fail()) return (in_file);

		in_file >> ch;
		if (in_file.fail()) return (in_file);

		if (ch != '/') {
			in_file.setstate(std::ios::failbit);
			return (in_file);
		}

		in_file >> day;
		if (in_file.fail()) return (in_file);

		in_file >> ch;
		if (in_file.fail()) return (in_file);

		if (ch != '/') {
			in_file.setstate(std::ios::failbit);
			return (in_file);
		}

		in_file >> year;
		if (in_file.fail()) return (in_file);

		d.m_month = month;
		d.m_day = day;
		d.m_year = year;
    }
    else{
		in_file.setstate(std::ios::failbit);
	}

    return (in_file);
}
