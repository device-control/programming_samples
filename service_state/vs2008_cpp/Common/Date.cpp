#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>
#include <vector>
#include <sstream>
#include "Date.h"

// 1582年10月4日まではユリウス暦、次の日(10月15日)からはグレゴリオ暦 とするべきだけど、
// 面倒なんでグレゴリオ暦で計算。Dateクラス自体は 1900年以降を保証できれば十分。
// ただし、日付の減算処理は、mktime()を利用しているため 1970年以降でないと正しく処理できない
// 愚直に計算するバージョンなら1900年より過去でも問題ないはず。
#undef USE_MKTIME // 今は愚直版を利用しておく

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

// "Y4/M2/D2" = "2016/10/11"
Date::Date(const char* pStr)
{
	if(!setY4M4D2(pStr)){
		throw std::runtime_error("Date::Date(const char* pStr) illegal format");
	}
}

Date::~Date()
{
}

bool Date::setY4M4D2(const char* pStr)
{
	try{
		std::string str = pStr;
		std::vector<std::string> list = split(str, '/');
		for(int i=0;i<3;i++){
			char *endptr = NULL;
			long x = strtol(list[i].c_str(), &endptr, 0);
			if( 0 != *endptr ) throw std::runtime_error("Date::setY4M4D2 illegal format");
			if( 0==i) m_year = x;
			if( 1==i) m_month = x;
			if( 2==i) m_day = x;
		}
		//// 異常を考慮しない場合
		// m_year = atoi(list[0].c_str());
		// m_month = atoi(list[1].c_str());
		// m_day = atoi(list[2].c_str());
		//// C++11版
		// m_year = std::stoi(list[0].c_str());
		// m_month = std::stoi(list[1].c_str());
		// m_day = std::stoi(list[2].c_str());
	}
	catch(...){
		return false; // 失敗
	}
	return true; // 成功
}

void Date::addDays(int days)
{
	if( !isValid() ){
		throw std::runtime_error("void Date::addDays(int days) illegal format");
	}

#ifdef USE_MKTIME /* use mktime() */
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
#else /* 愚直に計算する */
	if( 0 == days ) return; // 加減算する必要がない
	m_day += days;

	if (m_day >= 0){
		for (;;){
			int daysInMonth = Date::getDaysInMonth(m_year, m_month);
			if (m_day <= daysInMonth) {
				break;
			}
			m_day -= daysInMonth;
			if (m_month == 12){
				m_month = 1;
				if (m_year == -1){
					m_year =1;
				}
				else{
					++m_year;
				}
			}
			else{
				++m_month;
			}
		}
	}
	else {
		for (;;){
			if (m_day >= 1) {
				break;
			}
			if (m_month == 1){
				if (m_year == 1){
					m_year = -1;
				}
				else {
					--m_year;
				}
				m_month = 12;
			}
			else {
				--m_month;
			}
			m_day += Date::getDaysInMonth(m_year, m_month);
		}
	}
	return;
#endif
}


// 前置
Date& Date::operator ++ ()
{
	this->addDays(1);
	return *this;
}

// 後置
Date Date::operator ++ (int)
{
	Date date(*this);
	this->addDays(1);
	return date;
}

// 前置
Date& Date::operator -- ()
{
	this->addDays(-1);
	return *this;
}

// 後置
Date Date::operator -- (int)
{
	Date date(*this);
	this->addDays(-1);
	return date;
}

Date Date::operator + (int days)
{
	Date date(*this);
	date.addDays(days);
	return date;
}

Date Date::operator - (int days)
{
	Date date(*this);
	date.addDays(-days);
	return date;
}

Date& Date::operator += (int days)
{
	this->addDays(days);
	return *this;
}

Date& Date::operator -= (int days)
{
	this->addDays(-days);
	return *this;
}

bool Date::operator == (const Date& op2)
{
	if( (this->m_year == op2.m_year) &&
		(this->m_month == op2.m_month) &&
		(this->m_day == op2.m_day)){
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
	if( !this->isValid() ){
		return false;
	}
	// getDays() するよりか compare() を利用した方が軽いはず
	int day1 = this->getDays();
	int day2 = Date::getDays( op2.m_year, op2.m_month, op2.m_day);
	return day1 > day2;
}

bool Date::operator < (const Date& op2)
{
	if( !this->isValid() ){
		return false;
	}
	// getDays() するよりか compare() を利用した方が軽いはず
	int day1 = this->getDays();
	int day2 = Date::getDays( op2.m_year, op2.m_month, op2.m_day);
	return day1 < day2;
}

bool Date::operator >= (const Date& op2)
{
	if( !this->isValid() ){
		return false;
	}
	// getDays() するよりか compare() を利用した方が軽いはず
	int day1 = this->getDays();
	int day2 = Date::getDays( op2.m_year, op2.m_month, op2.m_day);
	return day1 >= day2;
}

bool Date::operator <= (const Date& op2)
{
	if( !this->isValid() ){
		return false;
	}
	// getDays() するよりか compare() を利用した方が軽いはず
	int day1 = this->getDays();
	int day2 = Date::getDays( op2.m_year, op2.m_month, op2.m_day);
	return day1 <= day2;
}

//  1=op1が大きい
//  0=同じ
// -1=op2が大きい
// -2=異常
/*static*/
int Date::compare(const Date& op1, const Date& op2)
{
	if( !op1.isValid() || !op2.isValid() ){
		// 有効でない日付のため異常とする
		return -2;
	}

	if (op1.m_year != op2.m_year)
		return (op1.m_year < op2.m_year) ? -1 : 1;

	if (op1.m_month != op2.m_month)
		return (op1.m_month < op2.m_month) ? -1 : 1;

	if (op1.m_day != op2.m_day)
		return (op1.m_day < op2.m_day) ? -1 : 1;

	return 0;
}

/*static*/
bool Date::isLeap (int year)
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


bool Date::isValid() const
{
	return Date::isValid(m_year,m_month,m_day);
}

/* static */
bool Date::isValid(int y, int m, int d)
{
	// year 1~
	if( y < 1 ){
		return false; // 有効でない
	}
	// month 1~12
	if( (m < 1) && (m > 12) ){
		return false; // 有効でない
	}
	int daysInMonth = Date::getDaysInMonth(y,m);
	// day 1~daya
	if( (d < 1) && (d > daysInMonth) ){
		return false; // 有効でない
	}
	return true; // 有効
}

int Date::getDays() const
{
	return Date::getDays(m_year, m_month, m_day);
}
	
/*static*/
// 参考サイト
// http://ufcpp.net/study/algorithm/o_days.html
int Date::getDays(int y, int m, int d)
{
	if( ! Date::isValid(y, m, d) ){
		return -1;
	}
	
	// 1・2月 → 前年の13・14月
	if (m <= 2) {
		--y;
		m += 12;
	}
	int dy = 365 * (y - 1); // 経過年数×365日
	int c = y / 100;
	int dl = (y >> 2) - c + (c >> 2); // うるう年分
	int dm = (m * 979 - 1033) >> 5; // 1月1日から m 月1日までの日数
	return dy + dl + dm + d - 1;
}

/* static */
// geteDays() と同じになるはず。
int Date::toInt(int m_year, int m_month, int m_day)
{
	if( ! Date::isValid(m_year, m_month, m_day) ){
		return -1;
	}

	static const int anCumm[] ={
		0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 
	};

	int n;
	n = 365 * (m_year - 1) + (m_year - 1)/4 - (m_year - 1)/100 + (m_year - 1)/400;
	n += (m_month > 2 && Date::isLeap(m_year)) ?
		anCumm[m_month - 1] + 1 : anCumm[m_month - 1];
	/*
	if (m_year > 0){

		n =365*(m_year - 1) + (m_year - 1)/4 - (m_year - 1)/100 + (m_year - 1)/400;

		n +=(m_month > FEB && IsYearBissextile(m_year))
			? anCumm[m_month - 1] + 1
			: anCumm[m_month - 1];
	}
	else {

		n =365*m_year + (m_year - 3)/4 - (m_year - 3)/100 + (m_year - 3)/400;

		n +=(m_month > FEB && IsYearBissextile(m_year + 1))
			? anCumm[m_month - 1] + 1
			: anCumm[m_month - 1];
	}
	*/
	return n + (m_day - 1);
}


/*static*/
// 0:日曜,1:月曜…,6:土曜
// 参考サイト
// http://katsura-kotonoha.sakura.ne.jp/prog/c.shtml
int Date::getDayOfWeek(int y, int m, int d)
{
	if( ! Date::isValid(y, m, d) ){
		return -1; // 異常
	}

	// １月 と ２月 を前年の 13月 と 14月 として扱う
	if ( m <= 2 ){
		--y;
		m += 12;
	}
	int dow = d + ((8 + (13 * m)) / 5) + (y + (y / 4) - (y / 100) + (y / 400));
	return dow % 7;
#if 0
	// int n = getDays(y, m, d) + 1;
	int n = toInt(y, m, d) + 1;
	return (n >= 0) ? n % 7 : 6 + n % 7; // FIXME: マイナスはないのでプラス側だけ考慮してもいいはず
#endif
// #if 0
// 	int c = y / 100;
// 	y %= 100;
// 	int dow = d + 26 * (m + 1) / 10 + y + y / 4  + c / 4 - 2 * c;
// 	dow %= 7;
// 	return dow;
// #endif
}

/*static*/
int Date::getDaysInMonth(int y, int m)
{
	int days = Date::DAYS_PER_MONTH[m-1];
	if( (m == 2) && isLeap(y) ){
		days++; //28->29
	}
	return days;
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
