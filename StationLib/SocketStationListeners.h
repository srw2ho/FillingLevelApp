#pragma once


#include "StationSocketListener.h"

namespace StationLib {


[Windows::Foundation::Metadata::WebHostHidden]
 public ref class SocketStationListeners sealed
{

	Windows::Foundation::Collections::IObservableVector<StationLib::StationSocketListener^>^ m_SocketListeners;
	int m_SelectedIndex;

	Windows::Foundation::EventRegistrationToken m_startStreamingEventRegister;
	Windows::Foundation::EventRegistrationToken m_stopStreamingEventRegister;
	Windows::Foundation::EventRegistrationToken m_FailedEventRegister;

	Windows::Foundation::EventRegistrationToken m_NotificationLevel;


public:
	SocketStationListeners();
	virtual ~SocketStationListeners();

	event Windows::Foundation::TypedEventHandler<Platform::Object^, Windows::Networking::Sockets::StreamSocket^  >^ startStreaming;
	event Windows::Foundation::TypedEventHandler<Platform::Object^, Platform::String ^>^ stopStreaming;
	event Windows::Foundation::TypedEventHandler<Platform::Object^, Platform::String ^> ^ Failed;

	event Windows::Foundation::TypedEventHandler<Platform::Object^, int> ^ NotificationLevel;


	StationLib::StationSocketListener^ AddListener(StationLib::StationSocketListener^ listener);



	bool Initialization();
	bool DeInitialization();


	StationLib::StationSocketListener^ getSelectedItem();

	bool deleteListener(StationLib::StationSocketListener^ todeletelistener);

	property int SelectedIndex
	{
		int get();
		void set(int value);
	}
	property Windows::Foundation::Collections::IObservableVector<StationLib::StationSocketListener^>^ SocketListeners {
		Windows::Foundation::Collections::IObservableVector<StationLib::StationSocketListener^>^ SocketListeners::get() { return m_SocketListeners; };
	}

	void clearallListeners();

private:
	void OnFailed(Platform::Object ^sender, Platform::String ^args);
	void OnstartStreaming(Platform::Object ^sender, Windows::Networking::Sockets::StreamSocket ^args);
	void OnstopStreaming(Platform::Object ^sender, Platform::String ^ args);
//	WeatherStationLib::BME280SocketListener^ getBMEListenerBySocketListener(StreamSocketComm::SocketListener^ socket);
	void OnNotificationLevel(Platform::Object ^sender, int);

 };




}