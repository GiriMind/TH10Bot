#include "Th10Ai/Common.h"
#include "Th10Ai/Th10Context.h"

#include <Base/ScopeGuard.h>

#include "Th10Ai/DllInject.h"

namespace th
{
	using namespace boost::interprocess;
	using namespace boost::posix_time;

	Th10Context::Th10Context() :
		m_data(nullptr)
	{
		HWND window = FindWindowW(L"BASE", L"�������_�^�@�` Mountain of Faith. ver 1.00a");
		if (window == nullptr)
			BASE_THROW(Exception(u8"��������¼δ���С�"));
		DWORD processId = 0;
		DWORD threadId = GetWindowThreadProcessId(window, &processId);

		m_memory = managed_windows_shared_memory(create_only, "Th10SharedMemory", 8 * 1024 * 1024);
		m_data = m_memory.construct<Th10SharedData>("Th10SharedData")();
		if (m_data == nullptr)
			BASE_THROW(Exception(u8"Th10SharedData�����ѱ�ʹ�á�"));

		m_data->window = window;
		m_data->hooked = false;
		m_data->unhooked = false;
		m_data->updated = false;
		m_data->exited = false;
		m_data->updateTime = 0;

		string dllName = Apis::GetModuleDir() + "/Th10Hook.dll";
		DllInject::EnableDebugPrivilege();
		DllInject::Inject(processId, dllName);

		if (!timedWaitHook(3000))
			BASE_THROW(Exception(u8"Th10Hook��ʼ��ʧ�ܣ���ϸ��Ϣ��鿴Th10Hook.log��"));
	}

	Th10Context::~Th10Context()
	{
		if (m_data != nullptr)
			m_memory.destroy_ptr(m_data);
	}

	void Th10Context::activate()
	{
		if (!SetForegroundWindow(m_data->window))
			BASE_THROW(WindowsError());
	}

	bool Th10Context::timedWaitHook(time_t ms)
	{
		bool notTimeout = true;
		scoped_lock<interprocess_mutex> lock(m_data->hookMutex);
		if (!m_data->hooked)
		{
			ptime absTime = microsec_clock::universal_time() + milliseconds(ms);
			notTimeout = m_data->hookCond.timed_wait(lock, absTime);
		}
		return notTimeout;
	}

	void Th10Context::notifyUnhook()
	{
		scoped_lock<interprocess_mutex> lock(m_data->unhookMutex);
		m_data->unhooked = true;
		m_data->unhookCond.notify_one();
	}

	bool Th10Context::waitUpdate()
	{
		scoped_lock<interprocess_mutex> lock(m_data->updateMutex);
		if (!m_data->updated)
			m_data->updateCond.wait(lock);
		m_data->updated = false;
		//m_clock.update();
		//cout << m_clock.getTime() - m_data->updateTime << endl;
		return !m_data->exited;
	}

	const RawStatus& Th10Context::getStatus() const
	{
		return m_data->status;
	}
}