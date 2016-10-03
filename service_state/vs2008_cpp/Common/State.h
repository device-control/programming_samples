#ifndef _STATE_H_
#define _STATE_H_

#include <windows.h>
#include <string>

#include "Event.h"
#include "switchport.h"

class Service;
class API State
{

protected:
	// ステート名
    std::string m_name;

public:
	// コンストラクタ
	State(std::string name);
	// デストラクタ
	virtual ~State(){}
	// ステート名を取得
    std::string getName() const;

	// イベント実行
	virtual void executeEvent(Service& sv, Event& ev) = 0;

};

#endif	// _STATE_H_
