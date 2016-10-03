#ifndef _MSGQUEUE_H_
#define _MSGQUEUE_H_

#include "Locker.h"
#include <queue>

template<class T>
class MSGQueue
{
  private:
	std::queue<T>	m_queue;
	HANDLE			m_mutex;
	HANDLE			m_notEmpty;
	bool			m_finished;

  public:
	// コンストラクタ
	MSGQueue::MSGQueue(){
		m_mutex = CreateMutex(NULL, FALSE, NULL);
		m_notEmpty = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_finished = false;
	}

	// デストラクタ
	MSGQueue::~MSGQueue(){
		CloseHandle(m_mutex);
		CloseHandle(m_notEmpty);
	}

	// 追加
	void push(T& value)
	{
		Locker guard(m_mutex);
		bool notify = m_queue.empty();
		m_queue.push(value);
		if (notify) {
			SetEvent(m_notEmpty);
		}
	}

	// 取得
	// キューが空なら、pushされるまで待機する
	bool pop(T& value)
	{
		Locker guard(m_mutex);
		// キューが空なら、pushされるまで待機する
		while (m_queue.empty()) {
			// 終了済みなら待機しない
			if(!m_finished){
				ResetEvent(m_notEmpty);
				SignalObjectAndWait(m_mutex, m_notEmpty, INFINITE, FALSE);
			}
			// 終了済みならfalseとする
			if(m_finished) {
				return false;
			}
		}
		
		// キューから要素を取り出す
		value = m_queue.front();
		m_queue.pop();
		return true;
	}

	// キューが空かどうか
	bool empty(){
		Locker guard(m_mutex);
		return m_queue.empty();
	}

	// 終了
	void finish(){
		Locker guard(m_mutex);
		m_finished = true;
		SetEvent(m_notEmpty);
	}

};

#endif	// _MSGQUEUE_H_
