#pragma once

#include "Th10Hook/Common.h"

#include <d3d9.h>
#include <Base/Singleton.h>

#include "Th10Hook/MyDetours.h"

namespace th
{
	class D3D9Hook :
		public Singleton<D3D9Hook>
	{
	public:
		D3D9Hook();

		void attach();
		void detach();

	private:
		using Direct3DCreate9_t = decltype(&Direct3DCreate9);

		static IDirect3D9* WINAPI Direct3DCreate9Hook(UINT SDKVersion);

		IDirect3D9* direct3DCreate9Hook(UINT SDKVersion);

		Direct3DCreate9_t m_direct3DCreate9Orig;
	};
}