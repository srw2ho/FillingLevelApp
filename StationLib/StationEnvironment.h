#pragma once

#include "SocketStationListeners.h"

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;

namespace StationLib
{

[Windows::Foundation::Metadata::WebHostHidden]
	public ref class StationEnvironment sealed
	{
		SocketStationListeners^ m_StationListeners;
	public:
		StationEnvironment();
		virtual ~StationEnvironment();

		property StationLib::SocketStationListeners^ StationSocketListeners {
			StationLib::SocketStationListeners^ StationSocketListeners::get() { return m_StationListeners; };
		}

		StationLib::SocketStationListeners ^ getWeatherStationListeners() { return m_StationListeners; };
		bool Initialization();
		bool DeInitialization();
	};

}