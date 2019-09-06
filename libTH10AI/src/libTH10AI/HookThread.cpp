#include "libTH10AI/Common.h"
#include "libTH10AI/HookThread.h"

#include "libTH10AI/HookMain.h"
#include "libTH10AI/Th10Ai.h"

namespace th
{
	HookThread::HookThread() :
		m_done(false)
	{
		m_ai = std::make_shared<Th10Ai>();
		m_thread = std::thread(std::bind(&HookThread::hookProc, this));
	}

	HookThread::~HookThread()
	{
		m_done = true;
		if (m_thread.joinable())
			m_thread.join();
		m_ai = nullptr;
	}

	void HookThread::hookProc()
	{
		try
		{
			m_ai->run(*this);
		}
		catch (...)
		{
			std::string what = boost::current_exception_diagnostic_information();
			BOOST_LOG_TRIVIAL(error) << what;
		}

		// 按D自行退出，m_done是false，需要destroy
		// 跟随WM_CLOSE退出，m_done是true，不需要destroy
		HookExit(!m_done);
	}

	bool HookThread::isDone() const
	{
		return m_done;
	}
}
