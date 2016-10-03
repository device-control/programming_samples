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
	CloseHandle(m_hLoopEventThread);
}

////////////////////////////////////////////////////////////////////////////////
void Service::open()
{
	OutputDebugString(L"�T�[�r�X�J�n\n");

	// �T�[�r�X�J�n
	m_isOpen = true;

	// �C�x���g�L���[�𐶐�
	if (m_eventList == NULL) {
		m_eventList = new MSGQueue<Event>;
	}

	// �C�x���g���[�v�̃X���b�h�𐶐�
	if (m_hLoopEventThread == NULL) {
		m_hLoopEventThread = reinterpret_cast<HANDLE>(_beginthreadex(NULL, 0, &Service::eventFunc, this, 0, 0));
	}
}

////////////////////////////////////////////////////////////////////////////////
void Service::close()
{
	OutputDebugString(L"�T�[�r�X�I��\n");

	// �T�[�r�X�I��
	m_isOpen = false;

	// �C�x���g�L���[���I������
	if (m_eventList) {
		// �C�x���g�L���[�I��
		m_eventList->finish();

		// �C�x���g���N���A����K�v������΂����Ŏ��{

		// �L���[���폜
		delete m_eventList;
		m_eventList = NULL;
	}
	
	// �X���b�h�I���҂�
	if (m_hLoopEventThread) {
		WaitForSingleObject(m_hLoopEventThread , INFINITE);
		m_hLoopEventThread = NULL;
	}
}

////////////////////////////////////////////////////////////////////////////////
void Service::addEvent(Event& ev)
{
	OutputDebugString(L"�C�x���g�ǉ�\n");
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
	OutputDebugString(L"�C�x���g�̃n���h�����O�J�n\n");
	
	// �T�[�r�X��������
	LoopEventInitialize();
	
	// �C�x���g�n���h�����O����
	while(true){
		// �C�x���g���s
		Event ev;
		if (m_eventList->pop(ev)) {
			executeEvent(ev);
		}
		// �T�[�r�X��~�`�F�b�N
		if (!m_isOpen) {
			break;
		}
	}
	
	// �T�[�r�X�I������
	LoopEventFinalize();
	
	OutputDebugString(L"�C�x���g�̃n���h�����O�I��\n");
}

////////////////////////////////////////////////////////////////////////////////
void Service::LoopEventInitialize()
{
	OutputDebugString(L"�C�x���g���[�v��������\n");
}

////////////////////////////////////////////////////////////////////////////////
void Service::LoopEventFinalize()
{
	OutputDebugString(L"�C�x���g���[�v�I������\n");
}

////////////////////////////////////////////////////////////////////////////////
void Service::executeEvent(Event& ev)
{
	OutputDebugString(L"�C�x���g���s\n");

	// ���݂̃X�e�[�^�X���擾
	State& state = getCurrentState();
	// �C�x���g�����s
	state.executeEvent(*this, ev);
}

////////////////////////////////////////////////////////////////////////////////
unsigned __stdcall Service::eventFunc(void* param)
{
	OutputDebugString(L"�C�x���g���[�v�̃X���b�h����\n");

	Service* service = static_cast<Service*>(param);
	service->LoopEvent();
	return 0;
}
