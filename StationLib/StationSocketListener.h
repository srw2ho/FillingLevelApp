#pragma once
#include "SocketListener.h"
#include "StationSensors.h"
#include "GPIOInOut.h"
#include "SensorStatistic.h"

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Data;

namespace StationLib {


[Windows::Foundation::Metadata::WebHostHidden]
[Windows::UI::Xaml::Data::Bindable]

public ref class StationSocketListener sealed : public INotifyPropertyChanged
{
	StreamSocketComm::SocketListener^  m_pSocketListener;
	concurrency::cancellation_token_source* m_pPackageCancelTaskToken;

	GPIODriver::GPIOInOut^ m_GPIOClientInOut;
	GPIODriver::GPIOEventPackageQueue* m_pGPIOEventPackageQueue;

	SensorStatistic* m_pSensorStatistic;

	GPIODriver::GPIOHCSR04 * m_pGPIOHCSR04;
	GPIODriver::BME280Sensor* m_pBME280Sensor;
	GPIODriver::GPIOOutputPin* m_pOutPutLed;

	GPIODriver::GPIOOutputPin* m_pOutPutLevel1Pin;
	GPIODriver::GPIOOutputPin* m_pOutPutLevel2Pin;
	GPIODriver::GPIOOutputPin* m_pOutPutPinsEnable;

	GPIODriver::GPIOInputPinShutDown* m_pShutDownPin;

	StationLib::SensorPackage* m_RecSensorPackage;


	bool m_bProcessingPackagesStarted;
	concurrency::task<void> m_ProcessingPackagesTsk;
	Windows::Foundation::EventRegistrationToken m_startStreamingEventRegister;
	Windows::Foundation::EventRegistrationToken m_stopStreamingEventRegister;
	Windows::Foundation::EventRegistrationToken m_FailedEventRegister;
	Windows::Foundation::EventRegistrationToken m_OnClientConnected;
	



	StationLib::StationSensors ^ m_StationSensors;

	Platform::String ^ m_HostName;
	int m_port;

	Platform::String ^ m_ViewName;
	unsigned int m_FailedConnectionCount;

	int m_nSensorViewSelection;
	int m_nChartViewSelection;

	int m_GPIOHCSR04EchoPin;
	int m_GPIOHCSR04TriggPin;

	int m_BME280SensorId;
	int m_OutPutLedPin;
	int m_OutPutLevel1Pin;
	int m_OutPutLevel2Pin;
	int m_ShutDownPin;

	double m_RefreshRateinSec;
	double m_MaxLevelMillimeterProSec; // max Geamtmaenge an Regen in mm Pro Sekunde;

	bool m_eMailWellFillingLevelMinActiv;
	bool m_eMailWellFillingLevelMaxActiv;
	bool m_eMailFillingLevelMinActiv;

	bool m_OutPutWellFillingLevelMinActiv;
	bool m_OutPutFillingLevelMinActiv;

	bool m_WellFillingLevelMin;
	bool m_WellFillingLevelMax;
	bool m_FillingLevelMin;
	bool m_ListenerMode;

	double m_OutPutLevel1PinActivValue; // Trockenlauf:: running dry protect
	double m_OutPutLevel2PinActivValue; // Wasser Nachlauf: water inlet

	double m_InletFillLevelInPercent;		// Wasser Einlauf in % vom max. Füllstandslevel
	double m_InletMaxActiveTime;			// maximale aktive Einlauf-Zeit in min

	double m_InletCycleFlushingTime;		// Einlauf Ventil Spülen aktiv, alle 1...n Tage
	double m_InletMaxCycleFlushingActiveTime;			// maximale aktive Flushing Time in sec
	bool m_WaterInletActive;
	bool m_WaterInletState;

	bool m_PumpDryProtectionActive;
	bool m_PumpDryProtectionState;
	int m_DoWaterInlet;						// 
	int m_DoPumpDryProtection;						// 

	bool  m_DoFlushingActive;
	UINT64 m_InletSetTime;
	UINT64 m_InletnextCycleFlushingTime;


public:
	StationSocketListener();
	virtual ~StationSocketListener();

	virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;

	Windows::Foundation::IAsyncAction ^ startProcessingPackagesAsync();
	Windows::Foundation::IAsyncAction ^ stopProcessingPackagesAsync();


	
//	virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;

	event Windows::Foundation::TypedEventHandler<Platform::Object^, Windows::Networking::Sockets::StreamSocket^  >^ startStreaming;
	event Windows::Foundation::TypedEventHandler<Platform::Object^, Platform::String ^>^ stopStreaming;
	event Windows::Foundation::TypedEventHandler<Platform::Object^, Platform::String ^> ^ Failed;

	event Windows::Foundation::TypedEventHandler<Platform::Object^, int > ^ NotificationLevel;

	event Windows::Foundation::TypedEventHandler<Platform::Object^, UINT64 > ^ Update; // Update

	property StationLib::StationSensors ^ StationSensors {
		StationLib::StationSensors ^ get() { return m_StationSensors; };
	}


	property bool  WaterInletState {
		bool   get() { return m_WaterInletState; };
	}

	property bool  PumpDryProtectionState {
		bool   get() { return m_PumpDryProtectionState; };
	}


	property bool  WaterInletActive {
		bool   get() { return m_WaterInletActive; };
		void set(bool   value);
	}

	property bool  PumpDryProtectionActive {
		bool   get() { return m_PumpDryProtectionActive; };
		void set(bool   value);
	}


	property double  InletFillLevelInPercent {
		double   get() { return m_InletFillLevelInPercent; };
		void set(double   value);
	}

	property double  InletMaxActiveTime {
		double   get() { return m_InletMaxActiveTime; };
		void set(double   value);
	}

	

	property double  InletMaxCycleFlushingActiveTime {
		double   get() { return m_InletMaxCycleFlushingActiveTime; };
		void set(double   value);
	}

	property double  InletCycleFlushingTime {
		double   get() { return m_InletCycleFlushingTime; };
		void set(double   value);
	}


	property int  SensorViewSelection {
		int   get() { return m_nSensorViewSelection; };
		void set(int   value);
	}

	property int  ChartViewSelection {
		int   get() { return m_nChartViewSelection; };
		void set(int   value);
	}


	property unsigned int  FailedConnectionCount {
		unsigned int   get() { return m_FailedConnectionCount; };
		void set(unsigned int   value);
	}


	property Platform::String^  VisibleKeyName {
		Platform::String^  get();
		void set(Platform::String^  value) {  };
	}
	property Platform::String^  ViewName {
		Platform::String^  get();
		void set(Platform::String^  value) ;
	}


	property Windows::UI::Xaml::Media::ImageSource^  ListenerImage
	{
		Windows::UI::Xaml::Media::ImageSource^   get();
	}

	property Platform::String ^ HostName {
		Platform::String ^  get() { return m_HostName; };
		void set(Platform::String ^ value);
	}

	property double OutPutLevel1PinActivValue {
		double  get() { return m_OutPutLevel1PinActivValue; };
		void set(double value);
	}

	property double OutPutLevel2PinActivValue {
		double  get() { return m_OutPutLevel2PinActivValue; };
		void set(double value);
	}


	property bool ListenerMode {
		bool  get() { return m_ListenerMode; };
		void set(bool value);
	}
	
	property bool eMailWellFillingLevelMinActiv {
		bool  get() { return m_eMailWellFillingLevelMinActiv; };
		void set(bool value);
	}

	property bool eMailWellFillingLevelMaxActiv {
		bool  get() { return m_eMailWellFillingLevelMaxActiv; };
		void set(bool value);
	}

	property bool eMailFillingLevelMinActiv {
		bool  get() { return m_eMailFillingLevelMinActiv; };
		void set(bool value);
	}

	property bool OutPutWellFillingLevelMinActiv {
		bool  get() { return m_OutPutWellFillingLevelMinActiv; };
		void set(bool value);
	}

	property bool OutPutFillingLevelMinActiv {
		bool  get() { return m_OutPutFillingLevelMinActiv; };
		void set(bool value);
	}



	property double RefreshRateinSec {
		double  get() { return m_RefreshRateinSec; };
		void set(double value);
	}

	property double  MaxLevelMillimeterProSec
	{
		double   get();
		void set(double  value);

	}


	property int ShutDownPin {
		int  get() { return m_ShutDownPin; };
		void set(int value);
	}

	property int GPIOHCSR04EchoPin {
		int  get() { return m_GPIOHCSR04EchoPin; };
		void set(int value);
	}

	property int GPIOHCSR04TriggPin {
		int  get() { return m_GPIOHCSR04TriggPin; };
		void set(int value);
	}

	property int BME280SensorId {
		int  get() { return m_BME280SensorId; };
		void set(int value);
	}
	property int OutPutLedPin {
		int  get() { return m_OutPutLedPin; };
		void set(int value);
	}
	property int OutPutLevel1Pin {
		int  get() { return m_OutPutLevel1Pin; };
		void set(int value);
	}
	property int OutPutLevel2Pin {
		int  get() { return m_OutPutLevel2Pin; };
		void set(int value);
	}


	property int Port {
		int  get() { return m_port; };
		void set(int value);
	}


	property Windows::UI::Xaml::Controls::Frame^  MainFrame
	{
		void set(Windows::UI::Xaml::Controls::Frame^  value);
		Windows::UI::Xaml::Controls::Frame^  get() { return this->m_MainFrame; 	};
	}



protected:
	void NotifyPropertyChanged(Platform::String^ prop);

private:

	Windows::UI::Xaml::Controls::Frame^ m_MainFrame;
	Concurrency::task<void> doProcessPackages();
	void cancelPackageAsync();
	void OnFailed(Platform::Object ^sender, Platform::Exception ^args);
	void OnOnClientConnected(Windows::Networking::Sockets::StreamSocket ^sender, int args);
	void OnstartStreaming(Platform::Object ^sender, Windows::Networking::Sockets::StreamSocket ^args);
	void OnstopStreaming(Platform::Object ^sender, Platform::Exception ^exception);
	void ProcessGPIOState(GPIODriver::GPIOPin* pGPIOPin);
	void ProcessGPIOOutPutState(SensorPackage*pPackage);
	bool doProcessOutputControl(UINT64 actualTime);
	bool InitGPIOOutput();
	double getSensorStreuungInFillingLevel();
	double getInletQtyFillingLevel(); // Auffüll Level
	void resetOutValues();
	void startProcessingPackages();
	void stopProcessingPackages();
};

}