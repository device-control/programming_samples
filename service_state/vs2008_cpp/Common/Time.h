#ifndef _TIME_H_
#define _TIME_H_

#include <string>
#include <vector>

class Time
{ 
public:
	int m_hour;
	int m_minute;
	int m_second;

	Time();
	Time(int i);
	Time(int h, int m, int s);
	Time(const char* pStr); // "HH:MM:SS"

	bool set(int i);
	bool set(int h, int m, int s);
	bool set(const char* pStr); // "HH:MM:SS"
	bool isValid() const;
	bool isValid24() const;
	int toInt() const;
	
	static bool isValid(int h, int m, int s);
	static bool isValid24(int h, int m, int s);
	static int compare(const Time&, const Time&);
	static Time getNow();
	static int toInt(int y, int m, int d);
	static std::vector<std::string> Time::split(const char* pStr, char sep);

	Time operator + (const Time& op2);
	Time operator - (const Time& op2);
	Time& operator += (const Time& op2);
	Time& operator -= (const Time& op2);

	bool operator == (const Time& op2);
	bool operator != (const Time& op2);
	bool operator <  (const Time& op2);
	bool operator >  (const Time& op2);
	bool operator <= (const Time& op2);
	bool operator >= (const Time& op2);

protected:
private:
};

#endif /* _TIME_H_ */
