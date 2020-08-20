#pragma once

#include <boost/interprocess/managed_windows_shared_memory.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

#include "Th10Hook/Th10Data.h"

namespace th
{
	namespace interprocess = boost::interprocess;

	struct Th10SharedData
	{
		HWND window;

		interprocess::interprocess_mutex hookMutex;
		interprocess::interprocess_condition hookCond;
		bool hooked;

		interprocess::interprocess_mutex unhookMutex;
		interprocess::interprocess_condition unhookCond;
		bool unhooked;

		interprocess::interprocess_mutex updateMutex;
		interprocess::interprocess_condition updateCond;
		bool updated;
		bool exited;

		time_t updateTime;
		RawStatus status;
		ActionData action;
	};

	class Th10Context
	{
	public:
		Th10Context();
		~Th10Context();

		HWND getWindow() const;

		void notifyHook();
		void notifyUnhook();
		void waitUnhook();
		void notifyUpdate();
		void notifyExit();

		void update();

	private:
		interprocess::managed_windows_shared_memory m_memory;
		Th10SharedData* m_data;
	};
}
