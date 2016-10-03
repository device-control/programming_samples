#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template<typename T>
class Singleton
{
public:
	// �C���X�^���X�擾
	static T& getInstance() {
		static T instance;
		return instance;
	}
};

#endif	// _SINGLETON_H_
