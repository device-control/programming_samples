#include "Service.h"
#include "State.h"

////////////////////////////////////////////////////////////////////////////////
Service::Service()
	: m_currentState(NULL)
	, m_hLoopEventThread(NULL)
	, m_eventList(NULL)
	, m_isOpen(false)
{
}

////////////////////////////////////////////////////////////////////////////////
Service::~Service()
{
	close();
	deleteState();
	CloseHandle(m_hLoopEventThread);
}

////////////////////////////////////////////////////////////////////////////////
void Service::open()
{
	OutputDebugString(L"サービス開始\n");

	if(m_isOpen) return; // 開始済み
	// サービス開始
	m_isOpen = true;

	// イベントキューを生成
	if (m_eventList == NULL) {
		m_eventList = new MSGQueue<Event>;
	}

	// イベントループのスレッドを生成
	if (m_hLoopEventThread == NULL) {
		m_hLoopEventThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &Service::eventFunc, this, 0, 0));
	}
}

////////////////////////////////////////////////////////////////////////////////
void Service::close()
{
	OutputDebugString(L"サービス終了\n");

	if(!m_isOpen) return; // 終了済み
	// サービス終了
	m_isOpen = false;

	// イベントキューを終了する
	if (m_eventList) {
		// イベントキュー終了
		m_eventList->finish();

		// イベントをクリアする必要があればここで実施

		// キューを削除
		delete m_eventList;
		m_eventList = NULL;
	}
	
	// スレッド終了待ち
	if (m_hLoopEventThread) {
		WaitForSingleObject(m_hLoopEventThread , INFINITE);
		m_hLoopEventThread = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
void Service::addEvent(Event& ev)
{
	OutputDebugString(L"イベント追加\n");
	m_eventList->push(ev);
}

////////////////////////////////////////////////////////////////////////////////
State& Service::getCurrentState() const
{
	return *m_currentState;
}

////////////////////////////////////////////////////////////////////////////////
void Service::changeState(std::string stateName)
{
    m_currentState = findState(stateName);
}

////////////////////////////////////////////////////////////////////////////////
void Service::addState(State& state)
{
	m_stateMap.insert(std::make_pair(state.getName(), &state));
}

////////////////////////////////////////////////////////////////////////////////
void Service::deleteState()
{
	std::map<std::string, State*>::const_iterator iter = m_stateMap.begin();
	while(iter != m_stateMap.end()){
		delete (*iter).second;
		++iter;
	}
	m_stateMap.clear();
}

////////////////////////////////////////////////////////////////////////////////
State* Service::findState(std::string stateName) const
{
	std::map<std::string, State*>::const_iterator iter = m_stateMap.find(stateName);
	if(iter != m_stateMap.end()){
		return (*iter).second;
	}
	else {
		return 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
void Service::LoopEvent()
{
	OutputDebugString(L"イベントのハンドリング開始\n");
	
	// サービス初期処理
	LoopEventInitialize();
	
	// イベントハンドリング処理
	while(true){
		// イベント実行
		Event ev;
		if (m_eventList->pop(ev)) {
			executeEvent(ev);
		}
		// サービス停止チェック
		if (!m_isOpen) {
			break;
		}
	}
	
	// サービス終了処理
	LoopEventFinalize();
	
	OutputDebugString(L"イベントのハンドリング終了\n");
}

////////////////////////////////////////////////////////////////////////////////
void Service::LoopEventInitialize()
{
	OutputDebugString(L"イベントループ初期処理\n");
}

////////////////////////////////////////////////////////////////////////////////
void Service::LoopEventFinalize()
{
	OutputDebugString(L"イベントループ終了処理\n");
}

////////////////////////////////////////////////////////////////////////////////
void Service::executeEvent(Event& ev)
{
	OutputDebugString(L"イベント実行\n");

	// 現在のステータスを取得
	State& state = getCurrentState();
	// イベントを実行
	state.executeEvent(*this, ev);
}

////////////////////////////////////////////////////////////////////////////////
unsigned __stdcall Service::eventFunc(void* param)
{
	OutputDebugString(L"イベントループのスレッド生成\n");

	Service* service = static_cast<Service*>(param);
	service->LoopEvent();
	return 0;
}
