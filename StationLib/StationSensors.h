#pragma once
#include "pch.h"
#include "StationSensor.h"


#include "SensorPackage.h"



using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;

namespace StationLib {


	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class  StationSensors sealed : public INotifyPropertyChanged {




		Windows::Foundation::Collections::IObservableVector < StationSensor^ > ^ m_SensorValues;
		StationLib::StationSensor^ m_ActSensorValue;
		int m_DevId;
		UINT64 m_StartUnixTime;

		UINT64 m_LastInsertedUnixTime;
		double  m_MaxQueueSize;

		Microsoft::WRL::Wrappers::SRWLock m_lock;

		double m_RecordingHours;
		double  m_RecordingSamplePerHour;
		Platform::String^ m_SensorViewName;
		double m_Height;

		double m_WellTemperatureMin;
		double m_WellTemperatureMax;

		double m_WellHumidityMin;
		double m_WellHumidityMax;

		double m_WellPressureMin;
		double m_WellPressureMax;

		double m_TemperatureMin;
		double m_TemperatureMax;


		double m_HumidityMin;
		double m_HumidityMax;

		double m_PressureMin;
		double m_PressureMax;

		double m_WellFillingLevelMin;
		double m_WellFillingLevelMax;

		double m_FillingLevelMin;
		double m_FillingLevelMax;

		double m_SensorDistanceMax;

		double m_SensorDistanceMin;


		double m_CysternDiameter; // Zysterne Durchmesser in cm

		double m_CysternVolumeProCm; // Zysterne Voklumen pro cm




	public:
		StationSensors();
		virtual ~StationSensors();

		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;
		
		property Windows::Foundation::Collections::IObservableVector<StationSensor^>^ SensorValues {
			Windows::Foundation::Collections::IObservableVector<StationSensor^>^ SensorValues::get() { return m_SensorValues; };
		}
		
		property StationLib::StationSensor^ ActSensorValue {
			StationLib::StationSensor^ ActSensorValue::get() { return m_ActSensorValue; };
		}

		property double Height
		{
			//	Platform::String^ get() override;
			double get();
			void set(double value);
		}
	

		property double   RecordingSamplePerHour
		{

			double   get();
			void set(double   value);

		}


		property double  RecordingHours
		{

			double   get();
			void set(double value);

		}

		property double  RecordingHoursForChartMajorStep // berechnet ChartMajorStep for telerikChart:DateTimeContinuousAxis
		{

			double   get();

		}

		property double  SensorDistanceMin
		{
			double   get();
			void set(double  value);

		}
		property double  SensorDistanceMax
		{
			double   get();
			void set(double  value);

		}

		property double  CysternDiameter
		{
			double   get();
			void set(double  value);

		}
		property double  CysternVolumeProCm
		{
			double   get();
			void set(double  value);

		}



		property double  FillingLevelMin
		{
			double   get();
			void set(double  value);

		}


		property double  FillingLevelMax
		{
			double   get();
			void set(double  value);

		}

		property double  WellFillingLevelMin
		{
			double   get();
			void set(double  value);

		}

		property double  WellFillingLevelMax
		{
			double   get();
			void set(double  value);

		}

		property double  PressureMin
		{
			double   get();
			void set(double  value);

		}

		property double  PressureMax
		{
			double   get();
			void set(double  value);

		}

		property double  WellPressureMin
		{
			double   get();
			void set(double  value);

		}

		property double  WellPressureMax
		{
			double   get();
			void set(double  value);

		}
		property double  PressureScaleMin
		{
			double   get();
		}

		property double  PressureScaleAvg
		{
			double   get();
		}

		property double  PressureScaleMax
		{
			double   get();
		}

		property double  HumidityMin
		{
			double   get();
			void set(double  value);

		}

		property double  HumidityMax
		{
			double   get();
			void set(double  value);

		}

		property double  WellHumidityMin
		{
			double   get();
			void set(double  value);

		}

		property double  WellHumidityMax
		{
			double   get();
			void set(double  value);

		}

		property double  WellTemperatureMin
		{
			double   get();
			void set(double  value);

		}

		property double  WellTemperatureMax
		{
			double   get();
			void set(double  value);

		}

		property double  TemperatureMin
		{
			double   get();
			void set(double  value);

		}

		property double  TemperatureMax
		{
			double   get();
			void set(double  value);

		}


		property double  TemperatureScaleMin
		{
			double   get();
		}

		property double  TemperatureScaleAvg
		{
			double   get();
		}

		property double  TemperatureScaleMax
		{
			double   get();
		}



		property double  HumidityScaleMax
		{
			double   get();
		}

		property double  HumidityScaleMin
		{
			double   get();
		}

		property double  HumidityScaleAvg
		{
			double   get();
		}



		property double  FillingLevelScaleMin
		{
			double   get();
		}

		property double  FillingLevelScaleAvg
		{
			double   get();
		}

		property double  FillingLevelScaleMax
		{
			double   get();
		}


		property Platform::String^  SensorViewName
		{

			Platform::String^   get();
			void set(Platform::String^  value);

		}

		property Platform::String^  SensorName
		{

			Platform::String^   get();

		}

		property int DevId {
			int DevId::get() { return m_DevId; };
			void DevId::set(int value);
		}

		property UINT64 StartUnixTime
		{
			//	Platform::String^ get() override;
			UINT64 get() { return m_StartUnixTime; };
			void set(UINT64 value);
			
		}

		property double  MaxQueueSize
		{
			//	Platform::String^ get() override;
			double  get() { return m_MaxQueueSize; };;
			void set(double);
		}



		void startRecording();

		void deleteSensorValues();

	


	internal:
		StationLib::StationSensor^ InsertSensorValue(SensorPackage* package);

	protected:
		void NotifyPropertyChanged(Platform::String^ prop);
	private:
		void deleteSensorValuesolderThen(UINT64 tineinPast);
		double getUpdateTime();

	};




}