#include "pch.h"

#include "SocketStationListeners.h"

namespace StationLib {


	SocketStationListeners::SocketStationListeners()
	{
		this->m_SocketListeners = ref new Platform::Collections::Vector<StationLib::StationSocketListener^>();

	//	WeatherStationLib::BME280SocketListener^ listener = ref new WeatherStationLib::BME280SocketListener();

	//	this->m_BME280SocketListener->Append(listener);

		/*
		OpenCVFFMpegLib::FFMpegListener^ pFFMpegListener = ref new OpenCVFFMpegLib::FFMpegListener();

		this->m_FFMpegListeners->Append(pFFMpegListener);
		pFFMpegListener = ref new OpenCVFFMpegLib::FFMpegListener();

		this->m_FFMpegListeners->Append(pFFMpegListener);
		*/
		m_SelectedIndex = -1;



	}

	bool SocketStationListeners::Initialization() {
		for (unsigned int i = 0; i < m_SocketListeners->Size; i++) {

			StationLib::StationSocketListener^ listener = m_SocketListeners->GetAt(i);
			listener->startProcessingPackagesAsync();

		}
		return true;
	}

	void StationLib::SocketStationListeners::OnstartStreaming(Platform::Object ^sender, Windows::Networking::Sockets::StreamSocket ^args)
	{
		this->startStreaming(sender, args);
		//throw ref new Platform::NotImplementedException();
	}


	void StationLib::SocketStationListeners::OnstopStreaming(Platform::Object ^sender, Platform::String ^ args)
	{
		this->stopStreaming(sender, args);
		//Connection  error by remote or stopped by user (CancelConnections)
		//throw ref new Platform::NotImplementedException();
	}
	
	void StationLib::SocketStationListeners::OnFailed(Platform::Object ^sender, Platform::String ^args)
	{// Trying to connect failed

		this->Failed(sender, args);
	}

	void StationLib::SocketStationListeners::OnNotificationLevel(Platform::Object ^sender, int args)
	{
		this->NotificationLevel(sender, args);
	}



	StationLib::StationSocketListener^ SocketStationListeners::AddListener(StationLib::StationSocketListener^ listener)
	{
		this->m_SocketListeners->Append(listener);

		m_FailedEventRegister, listener->Failed += ref new Windows::Foundation::TypedEventHandler<Platform::Object ^, Platform::String ^>(this, &StationLib::SocketStationListeners::OnFailed);
	
		m_startStreamingEventRegister, listener->startStreaming += ref new Windows::Foundation::TypedEventHandler<Platform::Object ^, Windows::Networking::Sockets::StreamSocket ^>(this, &StationLib::SocketStationListeners::OnstartStreaming);

		m_stopStreamingEventRegister, listener->stopStreaming += ref new Windows::Foundation::TypedEventHandler<Platform::Object ^, Platform::String ^>(this, &StationLib::SocketStationListeners::OnstopStreaming);

		m_NotificationLevel,listener->NotificationLevel += ref new Windows::Foundation::TypedEventHandler<Platform::Object ^, int>(this, &StationLib::SocketStationListeners::OnNotificationLevel);





		return listener;

	}

	bool SocketStationListeners::DeInitialization() {

		for (unsigned int i = 0; i < m_SocketListeners->Size; i++) {

			StationLib::StationSocketListener^ listener = m_SocketListeners->GetAt(i);

			listener->stopProcessingPackagesAsync();

		}

		return false;
	}

	int SocketStationListeners::SelectedIndex::get() {
		return this->m_SelectedIndex;
	}
	void SocketStationListeners::SelectedIndex::set(int value) {
		this->m_SelectedIndex = value;
	}


	StationLib::StationSocketListener^ SocketStationListeners::getSelectedItem() {

		if (this->SelectedIndex == -1)
		{
			return nullptr;
		}
		if (this->SelectedIndex < m_SocketListeners->Size)	return(StationLib::StationSocketListener^) m_SocketListeners->GetAt((unsigned int)SelectedIndex);
		else return nullptr;

	}

	void SocketStationListeners::clearallListeners() {

		while (this->m_SocketListeners->Size)
		{
			StationLib::StationSocketListener^ listener = this->m_SocketListeners->GetAt(0);

			listener->Failed -= m_FailedEventRegister;
			listener->startStreaming -= m_startStreamingEventRegister;
			listener->stopStreaming -= m_stopStreamingEventRegister;

			listener->NotificationLevel -= m_NotificationLevel;


		//	this->Failed -= m_FailedEventRegister;
		//	this->startStreaming -= m_startStreamingEventRegister;
		//	this->stopStreaming -= m_stopStreamingEventRegister;


			delete listener;
			this->m_SocketListeners->RemoveAt(0);

		}
		SelectedIndex = -1;

	}


	bool SocketStationListeners::deleteListener(StationLib::StationSocketListener^ todeletelistener) {

		for (unsigned int i = 0; i < m_SocketListeners->Size; i++) {

			StationLib::StationSocketListener^ listener = m_SocketListeners->GetAt(i);

			if (listener == todeletelistener)
			{
				m_SocketListeners->RemoveAt(i);

				listener->Failed -= m_FailedEventRegister;
				listener->startStreaming -= m_startStreamingEventRegister;
				listener->stopStreaming -= m_stopStreamingEventRegister;

				listener->NotificationLevel -= m_NotificationLevel;

				delete listener;
				return true;

			}
	

		}

		return false;

	}


	SocketStationListeners::~SocketStationListeners()
	{
		delete m_SocketListeners;
	}




}


