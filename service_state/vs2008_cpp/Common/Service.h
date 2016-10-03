#ifndef _SERVICE_H_
#define _SERVICE_H_

#ifdef _MSC_VER
    // warning C4251への対応
    #pragma warning(push)
    #pragma warning(disable:4251)
#endif

#include <windows.h>
#include <process.h>
#include <string>
#include <map>

#include "MSGQueue.h"
#include "Event.h"
#include "switchport.h"

class State;
class API Service
{

private:
	// ステータスのマップ
	std::map<std::string, State*> m_stateMap;
	// 現在のステータス
	State* m_currentState;
	
	// イベントループ用スレッド
	HANDLE m_hLoopEventThread;
	// イベントキュー
	MSGQueue<Event>* m_eventList;

	// サービス開始済みフラグ
	bool m_isOpen;
protected:

public:
	// コンストラクタ
	Service();
	// デストラクタ
	virtual ~Service();

	// サービス開始
	void open();
	// サービス終了
	void close();
	// イベント追加
	void addEvent(Event& ev);
	// 現在のステートを取得
	State& getCurrentState() const;
	// ステート遷移
	void changeState(std::string stateName);
protected:
	// ステート登録
	void addState(State& state);
	// ステート検索
	State* findState(std::string stateName) const;

	// イベントループ
	void LoopEvent();
	// イベントループ初期処理
	virtual void LoopEventInitialize();
	// イベントループ終了処理
	virtual void LoopEventFinalize();
	// イベント実行
	void executeEvent(Event& ev);

	// イベントループスレッドの生成
	static unsigned __stdcall eventFunc(void* param);
};

#ifdef _MSC_VER
    #pragma warning(pop)
#endif

#endif	// _SERVICE_H_
