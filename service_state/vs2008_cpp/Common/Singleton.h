#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template<typename T>
class Singleton
{
public:
	// インスタンス取得
	static T& getInstance() {
		static T instance;
		return instance;
	}
};

#endif	// _SINGLETON_H_
