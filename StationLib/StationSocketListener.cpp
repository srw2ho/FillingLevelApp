#include "pch.h"
#include "StationSocketListener.h"
#include "ServiceChunkReceiver.h"
#include "TimeConversion.h"

using namespace GPIODriver;


using namespace concurrency;

using namespace Platform;
using namespace Platform::Collections;
using namespace std;
using namespace Windows::Foundation;
using namespace Windows::Media::Capture;
using namespace Windows::Networking;
using namespace Windows::Networking::Connectivity;
using namespace Windows::Networking::Sockets;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Core;
using namespace Windows::System::Threading;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace StreamSocketComm;


const UINT64 nano100SecInSec = (UINT64)10000000 * 1;


namespace StationLib {

	StationSocketListener::StationSocketListener()
	{
	  m_pSocketListener = ref new StreamSocketComm::SocketListener();

	  m_StationSensors = ref new StationLib::StationSensors();

	  m_pGPIOEventPackageQueue = new GPIODriver::GPIOEventPackageQueue();

	  m_GPIOClientInOut = ref new GPIODriver::GPIOInOut(m_pGPIOEventPackageQueue);

	//  m_pBME280DataQueue = new WeatherStationData::BME280DataQueue();

	  m_ViewName = ref  new Platform::String(L"");
//	  m_pSensorStatistics = new SensorStatistics();




	  m_RecSensorPackage =  new StationLib::SensorPackage();

	  m_pSensorStatistic = new SensorStatistic();
//	  m_BME280SensorsValues = ref new WeatherStationLib::BME280SensorsValues();
	  
	  m_bProcessingPackagesStarted = false;

	  m_pPackageCancelTaskToken = nullptr;
	  m_FailedEventRegister,m_pSocketListener->Failed += ref new Windows::Foundation::TypedEventHandler<Platform::Object ^, Platform::Exception ^>(this, &StationLib::StationSocketListener::OnFailed);

	  m_OnClientConnected, m_pSocketListener->OnClientConnected += ref new Windows::Foundation::TypedEventHandler<Windows::Networking::Sockets::StreamSocket ^, int>(this, &StationLib::StationSocketListener::OnOnClientConnected);

	  m_startStreamingEventRegister,m_pSocketListener->startStreaming += ref new Windows::Foundation::TypedEventHandler<Platform::Object ^, Windows::Networking::Sockets::StreamSocket ^>(this, &StationLib::StationSocketListener::OnstartStreaming);

	  m_stopStreamingEventRegister,m_pSocketListener->stopStreaming += ref new Windows::Foundation::TypedEventHandler<Platform::Object ^, Platform::Exception^ >(this, &StationLib::StationSocketListener::OnstopStreaming);


	  m_MainFrame = nullptr;

	  m_nSensorViewSelection = 0;
	  m_nChartViewSelection = 0;



	  m_GPIOHCSR04EchoPin = 12;
	  m_GPIOHCSR04TriggPin = 20;
	  m_BME280SensorId = 0x76;
	  m_OutPutLedPin = 21;

	  m_OutPutLevel1Pin = 17;

	  m_OutPutLevel2Pin = 27;

	  m_ShutDownPin = 16;
	  m_MaxLevelMillimeterProSec = 3;

	  m_pBME280Sensor = nullptr;
	  m_pGPIOHCSR04 = nullptr;
	  m_pOutPutLed = nullptr;
	  m_pOutPutLevel1Pin = nullptr;
	  m_pOutPutLevel2Pin = nullptr;
	  m_pOutPutPinsEnable = nullptr;
	  m_pShutDownPin = nullptr;

	  m_RefreshRateinSec = 1;

	  m_eMailWellFillingLevelMinActiv = false;
	  m_eMailWellFillingLevelMaxActiv = false;;
	  m_eMailFillingLevelMinActiv = false;;

	  m_OutPutWellFillingLevelMinActiv = false;;
	  m_OutPutFillingLevelMinActiv = false;;

	  m_WellFillingLevelMin = true; // Meldung wenn Level unterschritten ist
	  m_WellFillingLevelMax = true;
	  m_FillingLevelMin = true;;

	  m_ListenerMode = false;

	  m_OutPutLevel2PinActivValue = 0;
	  m_OutPutLevel1PinActivValue = 0;

	  m_InletFillLevelInPercent = 2;		// Wasser Einlauf in % vom max. Füllstandslevel
	  m_InletMaxActiveTime = 0.0;				// maximale aktive Einlauf-Zeit in Sec

	  m_InletMaxCycleFlushingActiveTime = 1; // maximale aktive Inlet Flushing-Zeit in Sec

	  m_InletCycleFlushingTime = 5;		// Einlauf Ventil Spülen aktiv, alle 1...n Tage

	  m_WaterInletActive = false;
	  m_PumpDryProtectionActive = false;
	  m_DoWaterInlet = 0;
	  m_PumpDryProtectionState = false;
	  m_DoPumpDryProtection = 0;

	}


	StationSocketListener::~StationSocketListener()
	{
		// alle CallBacks zurücksetzen
		m_pSocketListener->Failed -= m_FailedEventRegister;
		m_pSocketListener->startStreaming -= m_startStreamingEventRegister;
		m_pSocketListener->stopStreaming -= m_stopStreamingEventRegister;
		m_pSocketListener->OnClientConnected -= m_OnClientConnected;

		m_GPIOClientInOut->deleteAllGPIOPins();


		delete m_pSocketListener;
		delete m_pGPIOEventPackageQueue;
		delete m_StationSensors;
		delete m_RecSensorPackage;
		delete m_pSensorStatistic;

		m_StationSensors = nullptr;
		m_pSocketListener = nullptr;

		m_pGPIOEventPackageQueue = nullptr;

	
	

	}

	bool StationSocketListener::InitGPIOOutput() {

		m_GPIOClientInOut->deleteAllGPIOPins();


		m_pBME280Sensor = new BME280Sensor(m_pGPIOEventPackageQueue, m_BME280SensorId, 0); // BME280_I2C_ADDR_PRIM
		m_pBME280Sensor->setActivateOutputProcessing(!m_ListenerMode);
		m_GPIOClientInOut->addGPIOPin(m_pBME280Sensor);

		m_pOutPutLevel1Pin = new GPIOOutputPin(m_pGPIOEventPackageQueue, m_OutPutLevel1Pin, (m_OutPutLevel1PinActivValue == 0) ? 1 : 0); // LED Output GPIO21, InitValue = 1
		m_pOutPutLevel1Pin->setActivateOutputProcessing(!m_ListenerMode);
		m_GPIOClientInOut->addGPIOPin(m_pOutPutLevel1Pin);

	
		m_pOutPutLevel2Pin = new GPIOOutputPin(m_pGPIOEventPackageQueue, m_OutPutLevel2Pin, (m_OutPutLevel2PinActivValue == 0) ? 1 : 0); // LED Output GPIO21, InitValue = 1
		m_pOutPutLevel2Pin->setActivateOutputProcessing(!m_ListenerMode);
		m_GPIOClientInOut->addGPIOPin(m_pOutPutLevel2Pin);

		// muss nach den Output-Pins  erstes gesetzt werden !!!
		// LED Output ULN2803-OutPut-Pin GPIO16 for Output-Pins Enable, InitValue = 0

		m_pOutPutPinsEnable = new GPIOOutputPin(m_pGPIOEventPackageQueue, 16, 0);
		m_pOutPutPinsEnable->setActivateOutputProcessing(!m_ListenerMode);
		m_GPIOClientInOut->addGPIOPin(m_pOutPutPinsEnable);
		// m_pGPIOHCSR04 muss nach m_pOutPutPinsEnable - Pin gesetzt werden

		m_pGPIOHCSR04 = new GPIOHCSR04(m_pGPIOEventPackageQueue, m_GPIOHCSR04EchoPin, m_GPIOHCSR04TriggPin, 0); // Echo-Eingang = 12, Trigger = 20
		m_pGPIOHCSR04->setActivateOutputProcessing(!m_ListenerMode); // erst aktivieren, wenn m_GPIOClientInOut sicher aktiv ist
		m_GPIOClientInOut->addGPIOPin(m_pGPIOHCSR04);

		m_pOutPutLed = new GPIOOutputPin(m_pGPIOEventPackageQueue, m_OutPutLedPin, 1); // LED Output GPIO21, InitValue = 1
		m_pOutPutLed->setActivateOutputProcessing(!m_ListenerMode);
		m_GPIOClientInOut->addGPIOPin(m_pOutPutLed);
		// LED Output ULN2803-OutPut-Pin GPIO16 for Output-Pins Enable, InitValue = 0

		m_pShutDownPin = new GPIOInputPinShutDown(m_pGPIOEventPackageQueue, m_ShutDownPin, 1); // ShutDown-Pin InitValue
		m_pShutDownPin->setActivateOutputProcessing(!m_ListenerMode);
		m_GPIOClientInOut->addGPIOPin(m_pShutDownPin);

		



		m_WaterInletActive = false;
		m_PumpDryProtectionActive = false;
		m_WaterInletState = false;
		m_PumpDryProtectionState = false;
		m_DoWaterInlet = 0;
		m_DoPumpDryProtection = 0;

		m_InletSetTime = 0;
		m_DoFlushingActive = false;

		return true;
	}

	Platform::String^ StationSocketListener::VisibleKeyName::get()
	{
		//const wchar_t *wdata = m_IDName->Data();
		wchar_t buffer[200];
		if (m_ViewName->Length() > 0)
		{
			swprintf(&buffer[0], sizeof(buffer) / sizeof(buffer[0]), L"%s (%s:%03d)", m_ViewName->Data(), HostName->Data(), m_port);
			return ref new Platform::String(buffer);
		}
		else{
			swprintf(&buffer[0], sizeof(buffer) / sizeof(buffer[0]), L"%s:%03d", HostName->Data(), m_port);
		}
		return ref new Platform::String(buffer);

	}

	Platform::String^ StationSocketListener::ViewName::get()
	{
		return m_ViewName;

	}

	void StationSocketListener::ViewName::set(Platform::String ^ value)
	{
		m_ViewName = value;
		NotifyPropertyChanged("ViewName");
	}

	void StationSocketListener::HostName::set(Platform::String ^ value) {
			m_HostName = value;
			NotifyPropertyChanged("HostName");
			NotifyPropertyChanged("VisibleKeyName");
		}





	double StationSocketListener::MaxLevelMillimeterProSec::get()
	{
		return  m_MaxLevelMillimeterProSec;

	}

	void StationSocketListener::MaxLevelMillimeterProSec::set(double value)
	{
		m_MaxLevelMillimeterProSec = value;

		NotifyPropertyChanged("MaxLevelMillimeterProSec");


	}

	void StationSocketListener::OutPutLevel1PinActivValue::set(double value) {
		m_OutPutLevel1PinActivValue = value;
		NotifyPropertyChanged("OutPutLevel1PinActivValue");
	}

	void StationSocketListener::OutPutLevel2PinActivValue::set(double value) {
		m_OutPutLevel2PinActivValue = value;
		NotifyPropertyChanged("OutPutLevel2PinActivValue");
	}


	void StationSocketListener::ListenerMode::set(bool value) {
		m_ListenerMode = value;
		NotifyPropertyChanged("ListenerMode");
	}



	void StationSocketListener::eMailWellFillingLevelMinActiv::set(bool value) {
		m_eMailWellFillingLevelMinActiv = value;
		NotifyPropertyChanged("eMailWellFillingLevelMinActiv");
	}


	void StationSocketListener::eMailWellFillingLevelMaxActiv::set(bool value) {
		m_eMailWellFillingLevelMaxActiv = value;
		NotifyPropertyChanged("eMailWellFillingLevelMaxActiv");
	}

	void StationSocketListener::eMailFillingLevelMinActiv::set(bool value) {
		m_eMailFillingLevelMinActiv = value;
		NotifyPropertyChanged("eMailFillingLevelMinActiv");
	}

	void StationSocketListener::OutPutWellFillingLevelMinActiv::set(bool value) {
		m_OutPutWellFillingLevelMinActiv = value;
		NotifyPropertyChanged("OutPutWellFillingLevelMinActiv");
	}


	void StationSocketListener::OutPutFillingLevelMinActiv::set(bool value) {
		m_OutPutFillingLevelMinActiv = value;
		NotifyPropertyChanged("OutPutFillingLevelMinActiv");
	}



	void StationSocketListener::RefreshRateinSec::set(double value) {
		if (value < 0.5) value = 0.5;
		m_RefreshRateinSec = value;
		NotifyPropertyChanged("RefreshRateinSec");
	}



	void StationSocketListener::ShutDownPin::set(int value) {
		m_ShutDownPin = value;
		NotifyPropertyChanged("ShutDownPin");
	}

	void StationSocketListener::GPIOHCSR04EchoPin::set(int value) {
		m_GPIOHCSR04EchoPin = value;
		NotifyPropertyChanged("GPIOHCSR04EchoPin");
	}

	void StationSocketListener::GPIOHCSR04TriggPin::set(int value) {
		m_GPIOHCSR04TriggPin = value;
		NotifyPropertyChanged("GPIOHCSR04TriggPin");
	}

	void StationSocketListener::BME280SensorId::set(int value) {
		m_BME280SensorId = value;
		NotifyPropertyChanged("BME280SensorId");
	}

	void StationSocketListener::OutPutLedPin::set(int value) {
		m_OutPutLedPin = value;
		NotifyPropertyChanged("OutPutLedPin");
	}

	void StationSocketListener::OutPutLevel1Pin::set(int value) {
		m_OutPutLevel1Pin = value;
		NotifyPropertyChanged("OutPutLevel1Pin");
	}

	void StationSocketListener::OutPutLevel2Pin::set(int value) {
		m_OutPutLevel2Pin = value;
		NotifyPropertyChanged("OutPutLevel2Pin");
	}


	void StationSocketListener::Port::set(int value) {
		m_port = value; 
		NotifyPropertyChanged("Port");
		NotifyPropertyChanged("VisibleKeyName");
	}

	void StationSocketListener::MainFrame::set(Windows::UI::Xaml::Controls::Frame^ value) {
		this->m_MainFrame = value;
		NotifyPropertyChanged("MainFrame");
	}


	void StationSocketListener::FailedConnectionCount::set(unsigned int value) {
		m_FailedConnectionCount = value;

		NotifyPropertyChanged("FailedConnectionCount");
	}


	void StationSocketListener::WaterInletActive::set(bool value) {
		m_WaterInletActive = value;
		NotifyPropertyChanged("WaterInletActive");

	}

	void StationSocketListener::PumpDryProtectionActive::set(bool value) {
		m_PumpDryProtectionActive = value;
		NotifyPropertyChanged("PumpDryProtectionActive");

	}


	void StationSocketListener::InletFillLevelInPercent::set(double value) {
		m_InletFillLevelInPercent = value;
		NotifyPropertyChanged("InletFillLevelInPercent");

	}

	void StationSocketListener::InletMaxActiveTime::set(double value) {
		m_InletMaxActiveTime = value;
		NotifyPropertyChanged("InletMaxActiveTime");

	}

	void StationSocketListener::InletMaxCycleFlushingActiveTime::set(double value) {
		m_InletMaxCycleFlushingActiveTime = value;
		NotifyPropertyChanged("InletMaxCycleFlushingActiveTime");

	}
	


	void StationSocketListener::InletCycleFlushingTime::set(double value) {
		m_InletCycleFlushingTime = value;
		NotifyPropertyChanged("InletCycleFlushingTime");

	}


	void StationSocketListener::SensorViewSelection::set(int value) {
		m_nSensorViewSelection = value;

		NotifyPropertyChanged("SensorViewSelection");

	}


	 void StationSocketListener::ChartViewSelection::set(int value) {
		 m_nChartViewSelection = value;
		 NotifyPropertyChanged("ChartViewSelection");

	 }




	Windows::UI::Xaml::Media::ImageSource^  StationSocketListener::ListenerImage::get()
	{


		String^ UriRefName = "ms-appx:///" + "images/drops-2087006_1920.png";

		Uri^  ref = ref new Uri(UriRefName);

		Windows::UI::Xaml::Media::Imaging::BitmapImage^  bitmapImage = ref new BitmapImage(ref);
		return bitmapImage;

	
	}

	void StationSocketListener::resetOutValues()
	{
		m_pOutPutLevel2Pin->setSetValue((m_OutPutLevel2PinActivValue == 0) ? 1 : 0); // Inlet Valve abschalten
		m_pOutPutLevel1Pin->setSetValue((m_OutPutLevel1PinActivValue == 0) ? 1 : 0); // Inlet Valve abschalten
		Windows::Storage::Streams::IBuffer^ buf = m_GPIOClientInOut->GetGPIClientSendStateBuf();
		if (buf != nullptr) {
			m_pSocketListener->SendDataToClients(buf);
		}

		//Platform::String^ state = m_GPIOClientInOut->GetGPIClientSendState(); // Status aller Ausgänge
		//if (state->Length() > 0) {
		//	Windows::Storage::Streams::IBuffer^ buf = SocketHelpers::createPayloadBufferfromSendData(state);
		//	m_pSocketListener->SendDataToClients(buf);
		//}

	}


	bool StationSocketListener::doProcessOutputControl(UINT64 actualTime)
	{

		bool update = false;

		switch (m_DoWaterInlet) {
		case 0: // Wait for Water Inlet Activ


			if (m_WaterInletState) { // Inlet Control Level unterschritten

				m_WaterInletActive = true;
				if (m_InletMaxActiveTime > 0) { // Zeitüberwachung aktiv
					m_InletSetTime = actualTime + nano100SecInSec * 60 * m_InletMaxActiveTime;
				}
				else {
					m_InletSetTime = 0; // keine Zeitüberwachung
				}
				m_DoFlushingActive = false;
				m_DoWaterInlet++;
			}
			else
			{

				if (m_InletCycleFlushingTime > 0) {
					if (actualTime > m_InletnextCycleFlushingTime) {
						m_WaterInletActive = true;
						m_InletSetTime = actualTime + nano100SecInSec * m_InletMaxCycleFlushingActiveTime; // 1..80 sec Ventil einschalten;
						m_DoWaterInlet++;
						m_DoFlushingActive = true;
					}
				}
			}


			break;
		case 1:
			if (!m_WaterInletState || !m_WaterInletActive) // Inlet Control Level erreicht
			{
				m_DoWaterInlet = 0;
				m_WaterInletActive = false;
			}
			else if (m_InletSetTime > 0) {
				if (actualTime > m_InletSetTime) // Überwachungs-Zeit hat angesprochen
				{
					m_WaterInletActive = false;
					if (!m_DoFlushingActive) { // Timeout Überwachung aktiv
						m_DoWaterInlet = 3;
						this->NotificationLevel(this, 6);//Water Inlet Filling Time Over
					}
					else
					{
						m_DoFlushingActive = false;
						m_DoWaterInlet = 0;
					}
		
				}
			}
		
			break;

		case 3: // Error-Case
			if (!m_WaterInletState || m_WaterInletActive) // einschalten über Level manual vom Display
			{
				m_DoWaterInlet = 0;
			}
			break;
		}

		switch (m_DoPumpDryProtection) {
		case 0: // Wait for Water Inlet Activ

			if (m_PumpDryProtectionState) { // Innlet Control Level unterschritten

				m_DoPumpDryProtection++;
				m_PumpDryProtectionActive = true;
			}

			break;
		case 1:
			if (!m_PumpDryProtectionState) // Inlet Control Level erreicht
			{
				m_DoPumpDryProtection = 0;
				m_PumpDryProtectionActive = false;
			}


			break;


		}

		// wenn in aktiv dann au 0 setzen oder wenn Fehler Distance Sensor
		//if (!m_OutPutFillingLevelMinActiv || m_StationSensors->ActSensorValue->SensorDistanceError)
		//srw2ho,08.04.2020: Inlet activate also in case of Display-Button
		if (!m_OutPutFillingLevelMinActiv)
		{	// inaktiv setzen
			m_PumpDryProtectionActive = false;
		}
	//	if (!m_OutPutWellFillingLevelMinActiv || m_StationSensors->ActSensorValue->SensorDistanceError)
		//srw2ho,08.04.2020: Inlet activate also in case of Display-Button
		if (!m_OutPutWellFillingLevelMinActiv)
		{
			m_WaterInletActive = false;
		}


		bool WaterInletActive = (m_pOutPutLevel2Pin->getSetValue() == m_OutPutLevel2PinActivValue);
		
		bool PumpDryProtectionActive = (m_pOutPutLevel1Pin->getSetValue() == m_OutPutLevel1PinActivValue);

		if (m_WaterInletActive != WaterInletActive) update = true;
		else if (m_PumpDryProtectionActive != PumpDryProtectionActive) update = true;


		return update;

	}



	Concurrency::task<void> StationSocketListener::doProcessPackages()
	{
		auto token = m_pPackageCancelTaskToken->get_token();


		auto tsk = create_task([this, token]() -> void

		{
			bool dowhile = true;
		//	DWORD waitTime = INFINITE; // INFINITE ms Waiting-Time
			DWORD waitTime = 200; // 200 ms Waiting-Time

			// erst nach 2 sec mit dem Update loslegen, damit das System sich einschwingen kann
			UINT64 lastSendTime = ConversionHelpers::TimeConversion::getActualUnixTime()+ 1 * nano100SecInSec;

			m_InletnextCycleFlushingTime = ConversionHelpers::TimeConversion::getActualUnixTime() + nano100SecInSec * 24 * 3600 * m_InletCycleFlushingTime;



			int WaterInlet = 0;
			while (dowhile) {
				try {
	
					GPIOEventPackage* pPacket = nullptr;
					UINT64 actualTime = ConversionHelpers::TimeConversion::getActualUnixTime();

					size_t startsize = m_pGPIOEventPackageQueue->waitForPacket(&pPacket, waitTime); //wait for incoming Packet, INFINITE waiting for incoming Package
					if (pPacket != nullptr)
					{
						if (m_MainFrame != nullptr) {
							m_MainFrame->Dispatcher->RunAsync(CoreDispatcherPriority::High, ref new DispatchedHandler([this, actualTime, token,  pPacket]() {

								GPIOPin* pGPIOPin = (GPIOPin*)pPacket->getAdditional();
								if (pGPIOPin != nullptr)
								{
									ProcessGPIOState(pGPIOPin);
								}

								this->Update(this, actualTime);
								delete pPacket; // Package deleten
							}));
						}
						else
						{
							delete pPacket; // Package deleten
						}

					}
					
			

					if (!m_ListenerMode) { // Listenr-Mode , keine aktives Senden


						bool update = doProcessOutputControl(actualTime); // Control Outputs

						if ( update || (actualTime - lastSendTime) >= nano100SecInSec * m_RefreshRateinSec) // 1 sec Update-Time
						{
			
							m_pOutPutLed->setSetValue(!m_pOutPutLed->getSetValue());

							if (m_WaterInletActive) // Water Inlet active
							{
								m_pOutPutLevel2Pin->setSetValue((m_OutPutLevel2PinActivValue == 0) ? 0 : 1); // Inlet Valve einschalten
								m_InletnextCycleFlushingTime = actualTime + nano100SecInSec * 24 * 3600 * m_InletCycleFlushingTime; //letze eingeschaltete Zweit des Ventils
							}
							else
							{
								m_pOutPutLevel2Pin->setSetValue((m_OutPutLevel2PinActivValue == 0) ? 1 : 0); // Inlet Valve abschalten

							}

							if (m_PumpDryProtectionActive) {
								m_pOutPutLevel1Pin->setSetValue((m_OutPutLevel1PinActivValue == 0) ? 0 : 1); // Inlet Valve abschalten
							}
							else{
								m_pOutPutLevel1Pin->setSetValue((m_OutPutLevel1PinActivValue == 0) ? 1 : 0); // Inlet Valve abschalten
							}

							
							Windows::Storage::Streams::IBuffer^ buf = m_GPIOClientInOut->GetGPIClientSendStateBuf();
							if (buf != nullptr) {
								m_pSocketListener->SendDataToClients(buf);
							}


							// Anforderungen an Service senden
							//Platform::String^ state = m_GPIOClientInOut->GetGPIClientSendState(); // Status aller Ausgänge

							//Windows::Storage::Streams::IBuffer^ buf = SocketHelpers::createPayloadBufferfromSendData(state);
							//m_pSocketListener->SendDataToClients(buf);

							lastSendTime = ConversionHelpers::TimeConversion::getActualUnixTime();
						}
					}

					if (token.is_canceled()) {
						cancel_current_task();
					}

				}
				catch (task_canceled&)
				{

					dowhile = false;

				}
				catch (const exception&)
				{
					dowhile = false;


				}

			}





		}, token);

		return tsk;
	}

	Windows::Foundation::IAsyncAction ^ StationSocketListener::startProcessingPackagesAsync() {


		return create_async([this]()
		{
			startProcessingPackages();

		});

	}

	Windows::Foundation::IAsyncAction ^ StationSocketListener::stopProcessingPackagesAsync() {

		return create_async([this]()
		{
			stopProcessingPackages();
		});
	}


	void StationSocketListener::startProcessingPackages( )
	{

		if (m_bProcessingPackagesStarted) return;

		m_bProcessingPackagesStarted = true;

		m_pGPIOEventPackageQueue->Flush();
		m_pSensorStatistic->Flush();

		m_pSensorStatistic->setMaxLevelMillimeterProSec(this->MaxLevelMillimeterProSec);
		m_pSensorStatistic->setMaxPackageSize(5); // 5 gemittelt
		m_pSensorStatistic->setRefreshRateinSec(this->RefreshRateinSec);
		m_pSensorStatistic->setMaxDistance(this->StationSensors->SensorDistanceMax);
		m_pSensorStatistic->setMinDistance(this->StationSensors->SensorDistanceMin);

		m_RecSensorPackage->setDistanceErrCnt(0); // Sensor-Error Counter 0 setzen
		m_RecSensorPackage->setTempErrCnt(0); // Sensor-Error Counter 0 setzen


		InitGPIOOutput(); // alle GPIO-Output erzeugen
		//m_BME280SensorsValues->startRecording(nano100SecInHour*12); // alle löschen, welche älter sind als 12 h




		m_StationSensors->startRecording(); // alle löschen, welche älter sind als 12 h
		


		m_pOutPutPinsEnable->setSetValue(1);


		m_pSocketListener->StartClientAsync(m_HostName, m_port);
		


		if (m_pPackageCancelTaskToken != nullptr)
		{
			delete m_pPackageCancelTaskToken;

		}
		m_pPackageCancelTaskToken = new concurrency::cancellation_token_source();


		m_ProcessingPackagesTsk = create_task(doProcessPackages()).then([this](task<void> previous)
		{
			m_bProcessingPackagesStarted = false;
			try {
				previous.get();
			}
			catch (Exception^ exception)
			{

			}

		});




	}
	void StationSocketListener::cancelPackageAsync()
	{
		if (m_pPackageCancelTaskToken != nullptr) {
			m_pPackageCancelTaskToken->cancel();
		}

		m_pGPIOEventPackageQueue->cancelwaitForPacket();

		//m_pOutPutPinsEnable->setSetValue(0);
		// Anforderungen an Service senden



	}



	void StationSocketListener::stopProcessingPackages()
	{
		if (!m_bProcessingPackagesStarted) return;
		try {

			m_bProcessingPackagesStarted = false;
			cancelPackageAsync();


//			m_pSocketListener->CancelConnections();// alle Connections schliessen

			/*
			create_task(m_ProcessingPackagesTsk).then([this](task<void> previous)
			{
				m_bProcessingPackagesStarted = false;
				try {
					previous.get();
				}
				catch (Exception^ exception)
				{

				}

			}).wait();
			*/
//			Sleep(100);
		// Darf nicht in UI-Thread aufgerufen werden-> Blockiert UI-Thread-> gibt Exception
			m_ProcessingPackagesTsk.wait();

			resetOutValues(); // Reset all Outputs


			m_pSocketListener->CancelConnections();// alle Connections schliessen
		}
		catch (Exception^ exception)
		{
			bool b = true;

		}


	}

	double StationSocketListener::getSensorStreuungInFillingLevel()
	{
		double DeltaStreuungfillingLevel = 0;
		if ((this->StationSensors->SensorDistanceMax - this->StationSensors->SensorDistanceMin) > 0) {
			DeltaStreuungfillingLevel = (m_RecSensorPackage->getSensorStreuungByDistance() * this->StationSensors->FillingLevelMax) / (this->StationSensors->SensorDistanceMax - this->StationSensors->SensorDistanceMin); ;
		}
		return DeltaStreuungfillingLevel;
	}

	double StationSocketListener::getInletQtyFillingLevel() // Auffüll Level
	{
		double DeltaStreuungfillingLevel = getSensorStreuungInFillingLevel();
	

		if (this->StationSensors->FillingLevelMax > 0)
		{
			double InletfillingLevel = ((m_InletFillLevelInPercent * (this->StationSensors->SensorDistanceMax - this->StationSensors->SensorDistanceMin)) / this->StationSensors->FillingLevelMax); ;
			if (InletfillingLevel > DeltaStreuungfillingLevel) {
				DeltaStreuungfillingLevel = InletfillingLevel; // nur, wenn InletfillingLevel> SensorStreuung
			}

		}
		return DeltaStreuungfillingLevel;
	}



	void StationSocketListener::ProcessGPIOOutPutState(SensorPackage*pPackage)
	{

		if (pPackage != nullptr) {
			m_StationSensors->InsertSensorValue(pPackage);


			if (pPackage->getPlausible()) {
				double deltafilling = getSensorStreuungInFillingLevel();
				double inletFillLevel = getInletQtyFillingLevel();
				if (!m_WellFillingLevelMax && (m_StationSensors->ActSensorValue->FillingLevel) > m_StationSensors->WellFillingLevelMax + deltafilling)
				{
					m_WellFillingLevelMax = true;
					if (m_eMailWellFillingLevelMaxActiv)
					{
						this->NotificationLevel(this, 4);// Well Level Max überschritten
					}

				}
				else if (m_WellFillingLevelMax && (m_StationSensors->ActSensorValue->FillingLevel) < m_StationSensors->WellFillingLevelMax - deltafilling)
				{
					m_WellFillingLevelMax = false;
					if (m_eMailWellFillingLevelMaxActiv)
					{
						this->NotificationLevel(this, 5);// Well Level Min unterschritten

					}
				}

				if ((m_StationSensors->ActSensorValue->FillingLevel) > m_StationSensors->WellFillingLevelMin + inletFillLevel)
				{
					if (!m_WellFillingLevelMin)
					{
						m_WellFillingLevelMin = true;
						if (m_eMailWellFillingLevelMinActiv)
						{
							this->NotificationLevel(this, 2);// Well Level Min überschritten
						}
					}

					if (m_OutPutWellFillingLevelMinActiv)
					{
						m_WaterInletState = false;
					//	m_pOutPutLevel2Pin->setSetValue((m_OutPutLevel2PinActivValue == 0) ? 1 : 0);

					}
				}
				else if ((m_StationSensors->ActSensorValue->FillingLevel) < m_StationSensors->WellFillingLevelMin - deltafilling)
				{
					if (m_WellFillingLevelMin)
					{
						m_WellFillingLevelMin = false;
						if (m_eMailWellFillingLevelMinActiv)
						{
							this->NotificationLevel(this, 3);// Well Level Min unterschritten
						}
					}
					if (m_OutPutWellFillingLevelMinActiv)
					{
						m_WaterInletState = true;
			//			m_pOutPutLevel2Pin->setSetValue((m_OutPutLevel2PinActivValue == 0) ? 0 : 1);
	
					}

				}

	

				if ((m_StationSensors->ActSensorValue->FillingLevel) > m_StationSensors->FillingLevelMin + deltafilling)
				{
					if (!m_FillingLevelMin) {
						m_FillingLevelMin = true;
						if (m_eMailFillingLevelMinActiv)
						{
							this->NotificationLevel(this, 0);// Level Min überschritten
						}
					}

					if (m_OutPutFillingLevelMinActiv)
					{
		//				m_pOutPutLevel1Pin->setSetValue((m_OutPutLevel1PinActivValue == 0) ? 1 : 0);
						m_PumpDryProtectionState = false;
					}
				}
				else if ((m_StationSensors->ActSensorValue->FillingLevel) < m_StationSensors->FillingLevelMin - deltafilling)
				{
					if (m_FillingLevelMin) {
						m_FillingLevelMin = false;
						if (m_eMailFillingLevelMinActiv)
						{
							this->NotificationLevel(this, 1);// Level Min unterschritten
						}
					}

					if (m_OutPutFillingLevelMinActiv)
					{
						m_PumpDryProtectionState = true;
		//				m_pOutPutLevel1Pin->setSetValue((m_OutPutLevel1PinActivValue == 0) ? 0 : 1);
					}
				}
			}


		}


	}

	void StationSocketListener::ProcessGPIOState(GPIODriver::GPIOPin* pGPIOPin)
	{
	//	GPIODriver::GPIOPin* pGPIOOut = nullptr;
		double value;
		bool doSendNewState = false;
		bool doOutPutLevel = false;
		switch (pGPIOPin->getGPIOTyp())
		{

			case GPIOTyp::inputShutdown:
			{

				if (pGPIOPin == m_pShutDownPin) {// Daten ShutDown
					if (m_pShutDownPin->getPinValue() == 3) { // Shutdown
						this->NotificationLevel(this, 100);// 
					}
					else if (m_pShutDownPin->getPinValue() == 2) { // Restart
						this->NotificationLevel(this, 101);// 
					}
					bool doOK = true;
				}
				break;
			}
			case GPIOTyp::output:
			{
				if (pGPIOPin == m_pOutPutPinsEnable) {


				}
				if (pGPIOPin == m_pOutPutLevel1Pin)
				{
					m_pOutPutLevel1Pin->setPinValue(pGPIOPin->getPinValue());
					doOutPutLevel = true;
				
				}
				if (pGPIOPin == m_pOutPutLevel2Pin)
				{
					m_pOutPutLevel2Pin->setPinValue(pGPIOPin->getPinValue());
					doOutPutLevel = true;
				}

				if (doOutPutLevel) {
					double Output1 = (m_pOutPutLevel1Pin->getPinValue() == m_OutPutLevel1PinActivValue) ? 1 : 0;
					double Output2 = (m_pOutPutLevel2Pin->getPinValue() == m_OutPutLevel2PinActivValue) ? 1 : 0;
					m_StationSensors->ActSensorValue->OutPutFillingLevelMin = Output1;
					m_StationSensors->ActSensorValue->OutPutWellFillingLevelMin = Output2;
				}

				break;
			}
			case GPIOTyp::PWM:
			{

			}
			case GPIOTyp::HC_SR04:
			{
				if (pGPIOPin == m_pGPIOHCSR04) {


					value = pGPIOPin->getPinValue(); // Zeit in s
					m_RecSensorPackage->setHCSR04MeasTime(value);

					m_RecSensorPackage->setUnixTime(pGPIOPin->getTimeinNanos());
					doSendNewState = true;
					// Sensor Error Counts
					if (m_RecSensorPackage->getDistanceErrCnt() > 3) {
						m_StationSensors->ActSensorValue->SensorDistanceError = true;
					}
					else
					{
						m_StationSensors->ActSensorValue->SensorDistanceError = false;
					}



				}

				break;
			}
			case GPIOTyp::BME280:
			{
				if (pGPIOPin == m_pBME280Sensor) {

					BME280Sensor*pBME280Sensor = (BME280Sensor *)pGPIOPin;

					m_RecSensorPackage->setHumidity(pBME280Sensor->getHumidity());
					m_RecSensorPackage->setPressure(pBME280Sensor->getPressure());
					m_RecSensorPackage->setTemperature(pBME280Sensor->getTemperature());

				//	m_RecSensorPackage->setUnixTime(pGPIOPin->getTimeinNanos());
					doSendNewState = false;
					// Sensor Error Counts
					if (m_RecSensorPackage->getTempErrCnt() > 3) {
						m_StationSensors->ActSensorValue->SensorTemperatureError = true;
					}
					else
					{
						m_StationSensors->ActSensorValue->SensorTemperatureError = false;
					}

					//			DoDisplayInfo(m_DisplayAdress);
				}

				break;
			}
			default:
			{
				break;
			}

		}



		if (m_pSensorStatistic->getMaxDistance() > this->StationSensors->SensorDistanceMax) {

		}

		if (m_pSensorStatistic->getMinDistance() < this->StationSensors->SensorDistanceMin) {

		}

		if (doSendNewState) {
			SensorPackage*pPackage = m_pSensorStatistic->GetAverage(m_RecSensorPackage);
			if (pPackage != nullptr) {
				ProcessGPIOOutPutState(pPackage);
			}
	
		}


	}

	void StationLib::StationSocketListener::OnFailed(Platform::Object ^sender, Platform::Exception ^args)
	{// Trying to connect failed
	//	throw ref new Platform::NotImplementedException();
		Platform::String^ err = ref new String();
		err = args->Message;
		m_FailedConnectionCount = m_FailedConnectionCount + 1;
		this->Failed(this, err);
	}


	void StationLib::StationSocketListener::OnOnClientConnected(Windows::Networking::Sockets::StreamSocket ^sender, int args)
	{// Client connection created --> Add Connection to Listener
		//throw ref new Platform::NotImplementedException();

		StreamSocket^ socket = sender;

		StreamSocketComm::SocketChunkReceiver* pServiceListener = new ServiceChunkReceiver(socket, this->m_GPIOClientInOut);

		SocketChunkReceiverWinRT ^ pBME280ChunkReceiverWinRT = ref new SocketChunkReceiverWinRT(pServiceListener); // wrapper for SocketChunkReceiver and its derived
		m_pSocketListener->AddChunkReceiver(pBME280ChunkReceiverWinRT);
		pBME280ChunkReceiverWinRT->geSocketChunkReceiver()->StartService(); // send "BME280Server.Start" - Copmmand to Client Station-> Start with Communication
//		this->m_FailedConnectionCount = 0; // wieder 0 setzen, damit Fehler wieder neu erfasst werden kann.




	}


	void StationLib::StationSocketListener::OnstartStreaming(Platform::Object ^sender, Windows::Networking::Sockets::StreamSocket ^args)
	{
		m_FailedConnectionCount = 0;
		this->startStreaming(this, args);
		//throw ref new Platform::NotImplementedException();
	}


	void StationLib::StationSocketListener::OnstopStreaming(Platform::Object ^sender, Platform::Exception ^exception)
	{
		Platform::String^ err = ref new String();
		if (exception != nullptr) {
			err = exception->Message;
			m_FailedConnectionCount = m_FailedConnectionCount + 1;
		}
		else err = "";

		this->stopStreaming(this, err);
		//Connection  error by remote or stopped by user (CancelConnections)
		//throw ref new Platform::NotImplementedException();
	}

	void StationLib::StationSocketListener::NotifyPropertyChanged(Platform::String^ prop)
	{

		PropertyChanged(this, ref new PropertyChangedEventArgs(prop));

	}


}

