#pragma once


#include "SensorPackage.h"

using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Interop;

namespace StationLib {

	[Windows::UI::Xaml::Data::Bindable]
	[Windows::Foundation::Metadata::WebHostHidden]

	
	public ref class StationSensor sealed : public INotifyPropertyChanged
	{
	
		double m_Height;

		unsigned int m_DevId;
		double m_pressure;
		/*! Compensated temperature */
		double m_temperature;
		/*! Compensated humidity */
		double m_humidity;
		double m_HCSR04MeasTime;
		double m_HCSR04MeasTimeDistance;
		double m_Distance;

		double m_FillingLevel;
		UINT64 m_UnixTime;
		UINT64 m_DiffUnixTime;
		double m_FillingVolume; // in ltr.
		bool m_OutPutFillingLevelMin;
		bool m_OutPutWellFillingLevelMin;

		bool m_SensorDistanceError;
		bool m_SensorTemperatureError;
	public:
		StationSensor();

		virtual ~StationSensor();
		virtual event Windows::UI::Xaml::Data::PropertyChangedEventHandler^ PropertyChanged;

		property Windows::UI::Xaml::Media::ImageSource^  TemperatureImage
		{
			//	Platform::String^ get() override;
			Windows::UI::Xaml::Media::ImageSource^   get();
		//	void set(Windows::UI::Xaml::Media::ImageSource^  value);
		}
		property Windows::UI::Xaml::Media::ImageSource^  PressureImage
		{
			//	Platform::String^ get() override;
			Windows::UI::Xaml::Media::ImageSource^   get();
			//	void set(Windows::UI::Xaml::Media::ImageSource^  value);
		}
		property Windows::UI::Xaml::Media::ImageSource^  HumidityImage
		{
			//	Platform::String^ get() override;
			Windows::UI::Xaml::Media::ImageSource^   get();
			//	void set(Windows::UI::Xaml::Media::ImageSource^  value);
		}

		property Platform::String^  SensorName
		{
			//	Platform::String^ get() override;
			Platform::String^   get();

		}
		property Platform::String^  ActSensorPackageDateTime
		{

			Platform::String^   get();

		}



		property bool SensorDistanceError
		{
			
			bool get();
			void set(bool value);
		}

		property bool SensorTemperatureError
		{

			bool get();
			void set(bool value);
		}


		property bool OutPutFillingLevelMin
		{
			//	Platform::String^ get() override;
			bool get();
			void set(bool value);
		}
		property bool OutPutWellFillingLevelMin
		{
			//	Platform::String^ get() override;
			bool get();
			void set(bool value);
		}

		property double Pressure
		{
			//	Platform::String^ get() override;
			double get();
			void set(double value);
		}

		property double HCSR04MeasTime
		{
			//	Platform::String^ get() override;
			double get();
			void set(double value);
		}

		property double FillingVolume
		{
			double get();
			void set(double value);

		}
		property Platform::String^  FillingVolumeasString
		{
			Platform::String^  get();


		}


		property double FillingLevel
		{
			double get();
			void set(double value);
	
		}

		property double HCSR04MeasTimeDistance
		{
			double get();
			void set(double value);

		}

		property double Distance
		{
			double get();
			void set(double value);

		}

		property double Height
		{
			//	Platform::String^ get() override;
			double get();
			void set(double value);
		}

		property double PressurehPa
		{
			double get();
		}
		
		property double PressurehPaSeaLevel
		{

			double get();
		}

		property Platform::String^  HCSR04MeasTimeasString
		{
			//	Platform::String^ get() override;
			Platform::String^   get();

		}

		property Platform::String^  FillingLevelasString
		{
			//	Platform::String^ get() override;
			Platform::String^   get();

		}

		property Platform::String^  PressurehPaSeaLevelasString
		{
			//	Platform::String^ get() override;
			Platform::String^   get();

		}

		property Platform::String^  PressurehPaasString
		{
			//	Platform::String^ get() override;
			Platform::String^   get();

		}


		property Platform::String^  PressureasString
		{
			//	Platform::String^ get() override;
			Platform::String^   get();

		}

		property double Temperature
		{
			//	Platform::String^ get() override;
			double get();
			void set(double value);
		}

		property Platform::String^  TemperatureasString
		{
			//	Platform::String^ get() override;
			Platform::String^   get();

		}


		property double Humidity
		{
			//	Platform::String^ get() override;
			double get();
			void set(double value);
		}

		property Platform::String^  HumidityasString
		{
			//	Platform::String^ get() override;
			Platform::String^   get();

		}


		property UINT64 UnixTime
		{
			//	Platform::String^ get() override;
			UINT64 get();
			void set(UINT64 value);
		}

		property UINT64 DiffUnixTime
		{
			//	Platform::String^ get() override;
			UINT64 get();
			void set(UINT64 value);
		}



		property unsigned int DevId
		{
			//	Platform::String^ get() override;
			unsigned int get();
			void set(unsigned int value);
		}

	internal:
		StationSensor( StationLib::SensorPackage* package);
	protected:
		void NotifyPropertyChanged(Platform::String^ prop);
		Windows::UI::Xaml::Media::ImageSource^  getImage(Platform::String^ UriRefName);

	};

}