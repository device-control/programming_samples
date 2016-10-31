#ifndef _DATE_H_
#define _DATE_H_

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

class Date {
private:
	static const int DAYS_PER_MONTH[];// = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
public:
    int m_month;
    int m_day;
    int m_year;
public:
    Date ();
    Date (int y, int m, int d);
	Date(std::string& str);
    ~Date ();
	Date addDays(int days);
	bool is_valid();

	Date operator + (int days);
	Date operator - (int days);
	Date operator += (int days);
	Date operator -= (int days);
	bool operator == (const Date& op2);
	bool operator != (const Date& op2);
	bool operator > (const Date& op2);
	bool operator < (const Date& op2);
	bool operator >= (const Date& op2);
	bool operator <= (const Date& op2);

	static bool is_leap (int year);
	static Date getToday();
	static bool is_valid(int y, int m, int d);
	static int get_days(int y, int m, int d);
	static std::vector<std::string> split(const std::string &str, char sep);
};

std::ostream& operator << (std::ostream& out_file, const Date& d);
std::istream& operator >> (std::istream& in_file, Date& d);

#endif /*_DATE_H_*/
