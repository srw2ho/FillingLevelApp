#include "pch.h"

#include "StationEnvironment.h"

using namespace Windows::Storage;

namespace StationLib
{

	StationEnvironment::StationEnvironment()
	{
		m_StationListeners = ref new StationLib::SocketStationListeners();
		StorageFolder^ storageFolder = ApplicationData::Current->LocalFolder;
		Platform::String^ pathtoStorage = storageFolder->Path;
		
	}


	StationEnvironment::~StationEnvironment()
	{
		m_StationListeners->clearallListeners();
		delete m_StationListeners;
	}


	bool StationEnvironment::Initialization() {



		m_StationListeners->Initialization();

		return true;
	}

	bool StationEnvironment::DeInitialization() {

		m_StationListeners->DeInitialization();

		return true;
	}



}