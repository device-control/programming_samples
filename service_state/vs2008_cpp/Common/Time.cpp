#include <time.h>
#include <string>
#include <vector>
#include <sstream>
#include "Time.h"

Time::Time()
	: m_hour(0)
	, m_minute(0)
	, m_second(0)
{
}

Time::Time(int i)
{
	if( !set(i) ){
		throw std::runtime_error("Time::Time(int i) illegal format");
	}
}

Time::Time(const char* pStr)
{
	if( !set(pStr) ){
		throw std::runtime_error("Time::Time(const char* pStr) illegal format");
	}
}

Time::Time(int hour, int minute, int second)
	: m_hour(hour)
	, m_minute(minute)
	, m_second(second)
{
	/* 丸め込み処理 必要かな。。。２倍以上の場合上手くいかないな。。。
	m_minute += (m_second / 60);
	m_hour += (m_minute / 60);
	m_second = (m_second >= 60)? (m_second % 60) : m_second;
	m_minute = (m_minute >= 60)? (m_minute % 60) : m_minute;
	*/
}

bool Time::set(int i)
{
	// (hour * 60 * 60) + (minute * 60) + second
	m_hour = i / 3600;
	m_minute = (i / 60) % 60;
	m_second = i % 60;
	return true;
}

bool Time::set(int h, int m, int s)
{
	m_hour = h;
	m_minute = m;
	m_second = s;
	return true;
}

bool Time::set(const char* pStr)
{
	try{
		std::string str = pStr;
		std::vector<std::string> list = Time::split(pStr, ':');
		for(int i=0;i<3;i++){
			char *endptr = NULL;
			long x = strtol(list[i].c_str(), &endptr, 0);
			if( 0 != *endptr ) throw std::runtime_error("Time::setH2M2S2 illegal format");
			if( 0==i) m_hour = x;
			if( 1==i) m_minute = x;
			if( 2==i) m_second = x;
		}
		//// C++11版
		// m_hour = std::stoi(list[0].c_str());
		// m_minute = std::stoi(list[1].c_str());
		// m_second = std::stoi(list[2].c_str());
	}
	catch(...){
		return false; // 失敗
	}
	return true; // 成功
}


/*static*/
std::vector<std::string> Time::split(const char* pStr, char sep)
{
	std::string str = pStr;
    std::vector<std::string> v;
    std::stringstream ss(str);
    std::string buffer;
    while( std::getline(ss, buffer, sep) ) {
        v.push_back(buffer);
    }
    return v;
}

bool Time::isValid() const
{
	return Time::isValid(m_hour, m_minute, m_second);
}

/* static */
bool Time::isValid(int hour, int minute, int second)
{
	if( (0 <= hour) &&
		((0 <= minute) && (minute <= 59)) &&
		((0 <= second) && (second <= 59)) ){
		return true;
	}
	return false;
}

bool Time::isValid24() const
{
	return Time::isValid24(m_hour, m_minute, m_second);
}

/* static */
bool Time::isValid24(int hour, int minute, int second)
{
	if( ((0 <= hour) && (hour <= 23)) &&
		((0 <= minute) && (minute <= 59)) &&
		((0 <= second) && (second <= 59)) ){
		return true;
	}
	return false;
}

int Time::toInt() const
{
	return Time::toInt(m_hour, m_minute, m_second);
}

/* static */
int Time::toInt(int hour, int minute, int second)
{
	if( ! isValid(hour, minute, second) ){
		return -1;
	}
	return (hour * 60 * 60) + (minute * 60) + second;
}

//  1=op1が大きい
//  0=同じ
// -1=op2が大きい
// -2=異常
/* static */
int Time::compare(const Time& op1, const Time& op2)
{
	if( !op1.isValid() || !op2.isValid() ){
		// 有効でない時間のため異常とする
		return -2;
	}

	if (op1.m_hour != op2.m_hour)
		return (op1.m_hour < op2.m_hour) ? -1 : 1;

	if (op1.m_minute != op2.m_minute)
		return (op1.m_minute < op2.m_minute) ? -1 : 1;

	if (op1.m_second != op2.m_second)
		return (op1.m_second < op2.m_second) ? -1 : 1;

	return 0;
}

/* static */
Time Time::getNow()
{
	time_t t = time(NULL);
	struct tm now;
	localtime_s(&now, &t);
	return Time(now.tm_hour, now.tm_min, now.tm_sec);
}

Time Time::operator + (const Time& op2)
{
	int s = this->toInt() + op2.toInt();
	return Time(s);
}

Time Time::operator - (const Time& op2)
{
	int s = this->toInt() - op2.toInt();
	return Time(s);
}

Time& Time::operator += (const Time& op2)
{
	// FIXME: オーバーフローの考慮が必要
	int s = this->toInt() + op2.toInt();
	this->set(s);
	return *this;
}

Time& Time::operator -= (const Time& op2)
{
	// FIXME: マイナスの考慮が必要。。。
	int s = this->toInt() - op2.toInt();
	this->set(s);
	return *this;
}

bool Time::operator == (const Time& op2)
{
	return this->toInt() == op2.toInt();
	
}

bool Time::operator != (const Time& op2)
{
	return this->toInt() != op2.toInt();
}

bool Time::operator < (const Time& op2)
{
	return this->toInt() < op2.toInt();
}

bool Time::operator > (const Time& op2)
{
	return this->toInt() > op2.toInt();
}


bool Time::operator <= (const Time& op2)
{
	return this->toInt() <= op2.toInt();
}


bool Time::operator >= (const Time& op2)
{
	return this->toInt() >= op2.toInt();
}

