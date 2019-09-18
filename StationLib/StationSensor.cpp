#include "pch.h"

#include "StationSensor.h"
#include "TimeConversion.h"

using namespace Platform;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace Windows::Foundation;


namespace StationLib {

	StationSensor::StationSensor()
	{

		m_Height = 0;
		m_pressure =-1;
		m_temperature = -1;
		m_humidity = -1;

		m_UnixTime = 0;
		m_DiffUnixTime = 0;

		m_HCSR04MeasTime = 0;
		m_FillingLevel = 0;
		m_Distance = 0;
		m_FillingVolume = 00;
		m_OutPutFillingLevelMin = false;
		m_OutPutWellFillingLevelMin =false;
		m_SensorDistanceError = false;
		m_SensorTemperatureError = false;
		m_HCSR04MeasTimeDistance = 0;

	}

	StationSensor::StationSensor( StationLib::SensorPackage* package)
	{

	//	m_Height = package->m_Height;
		m_pressure = package->getPressure();

		m_temperature = package->getTemperature();

		m_humidity = package->getHumidity();
		m_UnixTime = package->getUnixTime ();
		m_HCSR04MeasTime = package->getHCSR04MeasTime();
		m_HCSR04MeasTimeDistance = package->getDistance();
		m_Distance = package->getDistance();


	}



	StationSensor::~StationSensor()
	{

	}

	Windows::UI::Xaml::Media::ImageSource^  StationSensor::getImage(String^ UriRefName)
	{

		Uri^  ref = ref new Uri(UriRefName);

		Windows::UI::Xaml::Media::Imaging::BitmapImage^  bitmapImage = ref new BitmapImage(ref);
		return bitmapImage;



	}

	Windows::UI::Xaml::Media::ImageSource^  StationSensor::TemperatureImage::get()
	{
	
		String^ UriRefName = "ms-appx:///" + "images/cold-1293305_1280.png";
		return getImage(UriRefName);
	}

	Windows::UI::Xaml::Media::ImageSource^  StationSensor::PressureImage::get()
	{

		String^ UriRefName = "ms-appx:///" + "images/atmospheric-pressure-2027048_1280.png";
		return getImage(UriRefName);


	}
	Windows::UI::Xaml::Media::ImageSource^  StationSensor::HumidityImage::get()
	{


		String^ UriRefName = "ms-appx:///" + "images/drops-2087006_1920.png";
		return getImage(UriRefName);
	}

	

	

	Platform::String^ StationSensor::SensorName::get()
	{

		wchar_t buffer[50];
		swprintf(&buffer[0], sizeof(buffer) / sizeof(buffer[0]), L"SensorId: %d", m_DevId);
		return ref new Platform::String(buffer);

	}



	bool StationSensor::SensorTemperatureError::get()
	{
		return  m_SensorTemperatureError;

	}

	void StationSensor::SensorTemperatureError::set(bool value)
	{
		m_SensorTemperatureError = value;
		NotifyPropertyChanged("SensorTemperatureError");

	}


	bool StationSensor::SensorDistanceError::get()
	{
		return  m_SensorDistanceError;

	}

	void StationSensor::SensorDistanceError::set(bool value)
	{
		m_SensorDistanceError = value;
		NotifyPropertyChanged("SensorDistanceError");

	}



	double StationSensor::Height::get()
	{
		return  m_Height;

	}

	
	void StationSensor::Height::set(double value)
	{
		m_Height = value;
		NotifyPropertyChanged("Height");
		NotifyPropertyChanged("PressurehPaSeaLevel");
		NotifyPropertyChanged("PressurehPaSeaLevelasString");
	}

	

	bool StationSensor::OutPutFillingLevelMin::get()
	{
		//		m_FillingLevel = m_DistanceMax - m_Distance;
		return m_OutPutFillingLevelMin;
	}

	bool StationSensor::OutPutWellFillingLevelMin::get()
	{
		//		m_FillingLevel = m_DistanceMax - m_Distance;
		return m_OutPutWellFillingLevelMin;
	}

	double StationSensor::FillingLevel::get()
	{
//		m_FillingLevel = m_DistanceMax - m_Distance;
		return m_FillingLevel;
	}

	

	double StationSensor::HCSR04MeasTimeDistance::get()
	{
		return m_HCSR04MeasTimeDistance;
	}

	double StationSensor::Distance::get()
	{
		return m_Distance;
	}

	double StationSensor::FillingVolume::get()
	{
		return m_FillingVolume;
	}
	/*

	/*
	double StationSensor::DistanceMax::get()
	{

		return m_DistanceMax;
	}
	*/

	


	double StationSensor::HCSR04MeasTime::get() // Meastime in µsec
	{
		return m_HCSR04MeasTime;

	}

	double StationSensor::Pressure::get()
	{
		return   m_pressure;

	}

	double StationSensor::PressurehPa::get()
	{
		return  m_pressure ;

	}

	double StationSensor::PressurehPaSeaLevel::get()
	{
		// barometrische Höhenformel 
		//
		

		double pressure_0 = m_pressure/100;

		if (m_Height == 0) return pressure_0;
		if (m_temperature == -1) return pressure_0;


		double E = 1;
		double temp_t = m_temperature;
		double temp_T = temp_t + 237.15;

		if (temp_t < 9.1)
		{
			E = 5.6402 * (-0.0916 + exp(0.06*temp_t));
		}
		else
		{
			E = 18.2194 * (1.0463 -1 * exp(-0.0666*temp_t));
		}

		

		double x = (9.80665* m_Height) / (287.05 * (temp_T + 0.12 * E + 0.0065 * (m_Height / 2 ))  );

		double retValue = pressure_0 * exp(x);

		return   retValue;

	}


	Platform::String^ StationSensor::PressurehPaSeaLevelasString::get()
	{

		wchar_t buffer[50];
		swprintf(&buffer[0], sizeof(buffer) / sizeof(buffer[0]), L"%2.1f", PressurehPaSeaLevel );
		return ref new Platform::String(buffer);

	}


	Platform::String^ StationSensor::PressureasString::get()
	{

		wchar_t buffer[50];
		swprintf(&buffer[0], sizeof(buffer) / sizeof(buffer[0]), L"%2.1f", m_pressure);
		return ref new Platform::String(buffer);

	}

	Platform::String^ StationSensor::PressurehPaasString::get()
	{

		wchar_t buffer[50];
		swprintf(&buffer[0], sizeof(buffer) / sizeof(buffer[0]), L"%2.1f", m_pressure);
		return ref new Platform::String(buffer);

	}


	

	void StationSensor::HCSR04MeasTime::set(double value)
	{
		m_HCSR04MeasTime = value; 
		/*
		double temp;
		if (m_temperature != -1) temp = m_temperature;
		else temp = 15;

		double VinMeterProSec = 331.5 + (0.6 * temp);
		m_Distance = (VinMeterProSec * m_HCSR04MeasTime) / (10000 * 2);
		*/
		NotifyPropertyChanged("HCSR04MeasTime");
		NotifyPropertyChanged("HCSR04MeasTimeasString");
		/*
		NotifyPropertyChanged("Distance");

		NotifyPropertyChanged("FillingLevel");
		NotifyPropertyChanged("FillingLevelasString");
		*/

	}
	/*
	void StationSensor::DistanceMax::set(double value)
	{
		m_DistanceMax = value;
		if (m_DistanceMax != value) {
			NotifyPropertyChanged("m_DistanceMax");
			NotifyPropertyChanged("FillingLevel");
		}

	}
	*/

	

	Platform::String^ StationSensor::HCSR04MeasTimeasString::get()
	{

		wchar_t buffer[50];
		swprintf(&buffer[0], sizeof(buffer) / sizeof(buffer[0]), L"%5.0f", m_HCSR04MeasTime);
		return ref new Platform::String(buffer);

	}
	Platform::String^ StationSensor::FillingLevelasString::get()
	{

		wchar_t buffer[50];
		swprintf(&buffer[0], sizeof(buffer) / sizeof(buffer[0]), L"%2.1f", m_FillingLevel);
		return ref new Platform::String(buffer);

	}

	Platform::String^ StationSensor::FillingVolumeasString::get()
	{

		wchar_t buffer[50];
		swprintf(&buffer[0], sizeof(buffer) / sizeof(buffer[0]), L"%2.0f", m_FillingVolume);
		return ref new Platform::String(buffer);

	}

	

	void StationSensor::FillingLevel::set(double value)
	{
		m_FillingLevel = value;

		NotifyPropertyChanged("FillingLevel");
		NotifyPropertyChanged("FillingLevelasString");

	}


	void StationSensor::OutPutFillingLevelMin::set(bool value)
	{
		m_OutPutFillingLevelMin = value;

		NotifyPropertyChanged("OutPutFillingLevelMin");

	}

	void StationSensor::OutPutWellFillingLevelMin::set(bool value)
	{
		m_OutPutWellFillingLevelMin = value;

		NotifyPropertyChanged("OutPutWellFillingLevelMin");

	}

	void StationSensor::Distance::set(double value)
	{
		m_Distance = value;
		NotifyPropertyChanged("Distance");

	}

	void StationSensor::HCSR04MeasTimeDistance::set(double value)
	{
		m_HCSR04MeasTimeDistance = value;
		NotifyPropertyChanged("HCSR04MeasTimeDistance");

	}

	


	void StationSensor::FillingVolume::set(double value)
	{
		m_FillingVolume = value;
		NotifyPropertyChanged("FillingVolume");
		NotifyPropertyChanged("FillingVolumeasString");

	}

	void StationSensor::Pressure::set(double value)
	{
		m_pressure = value;

		NotifyPropertyChanged("Pressure");
		NotifyPropertyChanged("PressureasString");
		NotifyPropertyChanged("PressurehPaasString");
		NotifyPropertyChanged("PressurehPaSeaLevel");
		NotifyPropertyChanged("PressurehPaSeaLevelasString");

	}

	double StationSensor::Temperature::get()
	{
		return   m_temperature;

	}

	Platform::String^ StationSensor::TemperatureasString::get()
	{

		wchar_t buffer[50];
		swprintf(&buffer[0], sizeof(buffer) / sizeof(buffer[0]), L"%2.2f", m_temperature);
		return ref new Platform::String(buffer);

	}

	void StationSensor::Temperature::set(double value)
	{
		m_temperature = value;

		NotifyPropertyChanged("Temperature");
		NotifyPropertyChanged("TemperatureasString");
	}

	double StationSensor::Humidity::get()
	{
		return   m_humidity;

	}



	Platform::String^ StationSensor::ActSensorPackageDateTime::get()
	{
		SYSTEMTIME st;

		ConversionHelpers::TimeConversion::NanosToLocalSystemTime(UnixTime, &st);
		//Convertto_String

		std::wstring timestr;
		ConversionHelpers::TimeConversion::ConvertSystemTimeto_String(&st, timestr); // ISO format, time zone designator Z == zero (UTC)

		return ref new Platform::String(timestr.c_str());

	}



	void StationSensor::UnixTime::set(UINT64 value)
	{
		m_UnixTime = value;
		NotifyPropertyChanged("UnixTime");
		NotifyPropertyChanged("ActSensorPackageDateTime");
		

	}


	UINT64 StationSensor::UnixTime::get()
	{
		return  m_UnixTime;

	}

	void StationSensor::DiffUnixTime::set(UINT64 value)
	{
		m_DiffUnixTime = value;
		NotifyPropertyChanged("DiffUnixTime");
	}


	UINT64 StationSensor::DiffUnixTime::get()
	{
		return m_DiffUnixTime;

	}


	void StationSensor::Humidity::set(double value)
	{
		m_humidity = value;
		NotifyPropertyChanged("Humidity");
		NotifyPropertyChanged("HumidityasString");
	}


	Platform::String^ StationSensor::HumidityasString::get()
	{

		wchar_t buffer[50];
		swprintf(&buffer[0], sizeof(buffer) / sizeof(buffer[0]), L"%2.1f", m_humidity);
		return ref new Platform::String(buffer);

	}

	unsigned int StationSensor::DevId::get()
	{
		return m_DevId;

	}

	void StationSensor::DevId::set(unsigned int value)
	{
		m_DevId = value;

		NotifyPropertyChanged("DevId");
	}


	void StationSensor::NotifyPropertyChanged(Platform::String^ prop)
	{
	
		PropertyChanged(this, ref new PropertyChangedEventArgs(prop));

	}

}