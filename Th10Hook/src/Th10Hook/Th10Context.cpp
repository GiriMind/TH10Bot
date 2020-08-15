#include "Th10Hook/Common.h"
#include "Th10Hook/Th10Context.h"

#include "Th10Hook/Reader.h"

namespace th
{
	using namespace boost::interprocess;

	Th10Context::Th10Context() :
		m_memory(open_only, "Th10SharedMemory"),
		m_data(nullptr)
	{
		m_data = m_memory.find<Th10SharedData>("Th10SharedData").first;
		if (m_data == nullptr)
			BASE_THROW(Exception(u8"Th10SharedDataδ�ҵ���"));
	}

	Th10Context::~Th10Context()
	{
	}

	HWND Th10Context::getWindow() const
	{
		return m_data->window;
	}

	void Th10Context::notifyHook()
	{
		scoped_lock<interprocess_mutex> lock(m_data->hookMutex);
		m_data->hooked = true;
		m_data->hookCond.notify_one();
	}

	void Th10Context::notifyUnhook()
	{
		scoped_lock<interprocess_mutex> lock(m_data->unhookMutex);
		m_data->unhooked = true;
		m_data->unhookCond.notify_one();
	}

	void Th10Context::waitUnhook()
	{
		scoped_lock<interprocess_mutex> lock(m_data->unhookMutex);
		if (!m_data->unhooked)
			m_data->unhookCond.wait(lock);
	}

	void Th10Context::notifyUpdate()
	{
		scoped_lock<interprocess_mutex> lock(m_data->updateMutex);
		m_data->updated = true;
		m_data->updateCond.notify_one();
	}

	void Th10Context::notifyExit()
	{
		scoped_lock<interprocess_mutex> lock(m_data->updateMutex);
		m_data->updated = true;
		m_data->exited = true;
		m_data->updateCond.notify_one();
	}

	void Th10Context::update()
	{
		//m_clock.update();
		//m_data->updateTime = m_clock.getTime();
		Reader::ReadPlayer(m_data->status.player);
		m_data->status.itemCount = Reader::ReadItems(m_data->status.items);
		m_data->status.enemyCount = Reader::ReadEnemies(m_data->status.enemies);
		m_data->status.bulletCount = Reader::ReadBullets(m_data->status.bullets);
		m_data->status.laserCount = Reader::ReadLasers(m_data->status.lasers);
	}
}