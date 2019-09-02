#pragma once

#include <vector>
#include <Windows/Process.h>

#include "TH10Bot/Entity/Player.h"
#include "TH10Bot/Entity/Item.h"
#include "TH10Bot/Entity/Enemy.h"
#include "TH10Bot/Entity/Bullet.h"
#include "TH10Bot/Entity/Laser.h"

namespace th
{
	// https://github.com/binvec/TH10_DataReversing
	class Api
	{
	public:
		Api(Process& process);

		bool readPlayer(Player& player) noexcept;
		bool readItems(std::vector<Item>& items) noexcept;
		bool readEnemies(std::vector<Enemy>& enemies) noexcept;
		bool readBullets(std::vector<Bullet>& bullets) noexcept;
		bool readLasers(std::vector<Laser>& lasers) noexcept;

	private:
		template <typename T>
		inline T readMemory(uint_t baseAddress) noexcept
		{
			T data = T();
			ReadProcessMemory(m_process, reinterpret_cast<LPCVOID>(baseAddress), &data, sizeof(data), nullptr);
			return data;
		}

		inline void readMemoryBuffer(uint_t baseAddress, byte_t* buffer, uint_t size) noexcept
		{
			ReadProcessMemory(m_process, reinterpret_cast<LPCVOID>(baseAddress), buffer, size, nullptr);
		}

		template <typename T>
		inline T readBuffer(byte_t* address) noexcept
		{
			return *reinterpret_cast<T*>(address);
		}

		Process& m_process;
		byte_t m_buffer[0x7F0 * 2000];
	};
}