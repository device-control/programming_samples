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
	Date(const char* pStr); // "Y4/M2/D2"
    ~Date ();
	bool setY4M4D2(const char* pStr); // "Y4/M2/D2"
	void addDays(int days);
	bool isValid() const;
	int getDays() const;

	Date& operator ++ (); // ‘O’u
	Date operator ++ (int); // Œã’u
	Date& operator -- ();
	Date operator -- (int);

	Date operator + (int days);
	Date operator - (int days);
	Date& operator += (int days);
	Date& operator -= (int days);
	bool operator == (const Date& op2);
	bool operator != (const Date& op2);
	bool operator > (const Date& op2);
	bool operator < (const Date& op2);
	bool operator >= (const Date& op2);
	bool operator <= (const Date& op2);

	static int compare(const Date& op1, const Date& op2);
	static bool isLeap (int year);
	static Date getToday();
	static bool isValid(int y, int m, int d);
	static int getDays(int y, int m, int d);
	static int toInt(int m_year, int m_month, int m_day);
	static int getDayOfWeek(int y, int m, int d);
	static int getDaysInMonth(int y, int m);
	static std::vector<std::string> split(const std::string &str, char sep);
};

std::ostream& operator << (std::ostream& out_file, const Date& d);
std::istream& operator >> (std::istream& in_file, Date& d);

#endif /*_DATE_H_*/
