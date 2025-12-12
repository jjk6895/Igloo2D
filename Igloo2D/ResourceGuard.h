#pragma once

#include <Windows.h>
#include <wincodec.h>
#include <vector>
#include <algorithm>

class ResourceGuard
{
private:
	HRESULT m_hr;
	std::vector<winrt::com_ptr<IUnknown>> resources;
public:
	inline ResourceGuard() : m_hr(CoInitialize(NULL)) {};
	inline ~ResourceGuard()
	{ 
		for (auto& r : resources)
		{
			r = nullptr;
		}
		if (SUCCEEDED(m_hr)) CoUninitialize();
	};
	ResourceGuard(ResourceGuard const&) = delete;
	ResourceGuard& operator=(ResourceGuard const&) = delete;
	ResourceGuard(ResourceGuard&&) = delete;
	ResourceGuard& operator=(ResourceGuard&&) = delete;
	inline HRESULT hr() const { return m_hr; }
	inline void AddResource(winrt::com_ptr<IUnknown> r)
	{
		resources.push_back(r);
	}
	inline void FlushEmpty()
	{
		std::erase_if(resources, [](winrt::com_ptr<IUnknown> const& r) { return r.get() == nullptr; });
	}
};