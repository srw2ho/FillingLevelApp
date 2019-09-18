#include "pch.h"

#include "StationSensors.h"
#include "TimeConversion.h"


const UINT64 nano100SecInHour = (UINT64)10000000 * 60 * 60;
const UINT64 nano100SecInMin = (UINT64)10000000 * 60;
const UINT64 nano100SecInSec = (UINT64)10000000 * 1;

const double MAX_QUEUESIZE	=	2000;

namespace StationLib {

StationSensors::StationSensors()
{
	this->m_SensorValues = ref new Platform::Collections::Vector < StationLib::StationSensor^ >();

	m_Height = 235;

	this->m_ActSensorValue = ref new StationLib::StationSensor();
	this->m_ActSensorValue->Height = m_Height;


	//	Windows::UI::Xaml::Interop::IBindableObservableVector

	m_DevId = -1;
	m_RecordingSamplePerHour = 50;
	m_RecordingHours = 24;
	m_MaxQueueSize = m_RecordingHours * m_RecordingSamplePerHour; // 24h alle 3600 sec x- Werte
	m_StartUnixTime = ConversionHelpers::TimeConversion::getActualUnixTime();
	m_SensorViewName = "";

	//	m_LastInsertedUnixTime = WeatherStationData::TimeConversion::getActualUnixTime();

	m_LastInsertedUnixTime = 0;

	m_TemperatureMin = -15;
	m_TemperatureMax = 40;

	m_PressureMin = 500;
	m_PressureMax = 1500;

	m_HumidityMin = 0;
	m_HumidityMax = 100;

	m_WellTemperatureMin = 20;
	m_WellTemperatureMax = 23;

	m_WellHumidityMin = 20;
	m_WellHumidityMax = 60;

	m_WellPressureMin = 600;
	m_WellPressureMax = 1400;

	m_FillingLevelMin = 10;
	m_FillingLevelMax = 100;

	m_WellFillingLevelMin = 20;
	m_WellFillingLevelMax = 80;

	m_FillingLevelMax = 350; // max. Sensordistanz in cm
	m_FillingLevelMin = 10;	//	min. Sensordistanz in cm

	m_SensorDistanceMin = 0;

	m_SensorDistanceMax = 0;

	m_CysternDiameter = 0;
	m_CysternVolumeProCm = 0;


}



StationSensors::~StationSensors()
{

	deleteSensorValues();
	delete this->m_SensorValues;
	//	delete m_pBME280AverageDataQueue;
}

void StationSensors::RecordingSamplePerHour::set(double  value)
{
	if (value >= 1 && value <= 3600)
	{


		m_RecordingSamplePerHour = value;

		NotifyPropertyChanged("RecordingSamplePerHour");

		this->MaxQueueSize = m_RecordingSamplePerHour * m_RecordingHours;
	}
	else
	{

	}

}

	double StationSensors::RecordingSamplePerHour::get()
	{
		return m_RecordingSamplePerHour;

	}

	double StationSensors::RecordingHoursForChartMajorStep::get()
	{
		double size = double(this->m_SensorValues->Size) / double(m_MaxQueueSize);
		double value = (size* m_RecordingHours);

		double acthours =  (size * m_RecordingHours);
		double ChartMajorStep = 1;

		while ( (acthours / ChartMajorStep) > 12)
		{
			ChartMajorStep = ChartMajorStep++;
		}
		return ChartMajorStep;
	}

	void StationSensors::RecordingHours::set(double value)
	{
	
		m_RecordingHours = value; 
		NotifyPropertyChanged("RecordingHours");

		this->MaxQueueSize =  m_RecordingSamplePerHour * m_RecordingHours;

	}

	double StationSensors::RecordingHours::get()
	{
		return m_RecordingHours;

	}

	double StationSensors::Height::get()
	{
		return  m_Height;

	}


	void StationSensors::Height::set(double value)
	{
		m_Height = value;
		this->m_ActSensorValue->Height = m_Height;
		NotifyPropertyChanged("Height");

	}


	

	double StationSensors::CysternVolumeProCm::get()
	{
		return  m_CysternVolumeProCm;

	}

	double StationSensors::CysternDiameter::get()
	{
		return  m_CysternDiameter;

	}

	double StationSensors::SensorDistanceMin::get()
	{
		return  m_SensorDistanceMin;

	}

	double StationSensors::SensorDistanceMax::get()
	{
		return  m_SensorDistanceMax;

	}



	double StationSensors::FillingLevelMin::get()
	{
		return  m_FillingLevelMin;

	}

	double StationSensors::FillingLevelMax::get()
	{
		return  m_FillingLevelMax;

	}


	double StationSensors::PressureMin::get()
	{
		return  m_PressureMin;

	}

	double StationSensors::PressureMax::get()
	{
		return  m_PressureMax;

	}

	void StationSensors::CysternDiameter::set(double value)
	{
		m_CysternDiameter = value;

		NotifyPropertyChanged("CysternDiameter");


	}



	void StationSensors::CysternVolumeProCm::set(double value)
	{
		m_CysternVolumeProCm = value;

		NotifyPropertyChanged("CysternVolumeProCm");


	}



	void StationSensors::SensorDistanceMin::set(double value)
	{
		m_SensorDistanceMin = value;

		NotifyPropertyChanged("SensorDistanceMin");

	}



	void StationSensors::SensorDistanceMax::set(double value)
	{
		m_SensorDistanceMax = value;

		NotifyPropertyChanged("SensorDistanceMax");


	}


	void StationSensors::FillingLevelMax::set(double value)
	{
		m_FillingLevelMax = value;

		NotifyPropertyChanged("FillingLevelMax");
		NotifyPropertyChanged("FillingLevelScaleMin");
		NotifyPropertyChanged("FillingLevelScaleAvg");
		NotifyPropertyChanged("FillingLevelScaleMax");

	}

	void StationSensors::FillingLevelMin::set(double value)
	{
		m_FillingLevelMin = value;

		NotifyPropertyChanged("FillingLevelMin");
		NotifyPropertyChanged("FillingLevelScaleMin");
		NotifyPropertyChanged("FillingLevelScaleAvg");
		NotifyPropertyChanged("FillingLevelScaleMax");

	}

	void StationSensors::WellFillingLevelMin::set(double value)
	{
		m_WellFillingLevelMin = value;

		NotifyPropertyChanged("WellFillingLevelMin");
		NotifyPropertyChanged("FillingLevelScaleMin");
		NotifyPropertyChanged("FillingLevelScaleAvg");
		NotifyPropertyChanged("FillingLevelScaleMax");

	}
	void StationSensors::WellFillingLevelMax::set(double value)
	{
		m_WellFillingLevelMax = value;

		NotifyPropertyChanged("WellFillingLevelMax");
		NotifyPropertyChanged("FillingLevelScaleMin");
		NotifyPropertyChanged("FillingLevelScaleAvg");
		NotifyPropertyChanged("FillingLevelScaleMax");

	}

	double StationSensors::WellFillingLevelMax::get()
	{
		return  m_WellFillingLevelMax;

	}

	double StationSensors::WellFillingLevelMin::get()
	{
		return  m_WellFillingLevelMin;

	}



	void StationSensors::PressureMax::set(double value)
	{
		m_PressureMax = value;

		NotifyPropertyChanged("PressureMax");
		NotifyPropertyChanged("PressureScaleMin");
		NotifyPropertyChanged("PressureScaleAvg");
		NotifyPropertyChanged("PressureScaleMax");

	}

	void StationSensors::PressureMin::set(double value)
	{
		m_PressureMin = value;

		NotifyPropertyChanged("PressureMin");
		NotifyPropertyChanged("PressureScaleMin");
		NotifyPropertyChanged("PressureScaleAvg");
		NotifyPropertyChanged("PressureScaleMax");

	}

	void StationSensors::WellPressureMin::set(double value)
	{
		m_WellPressureMin = value;

		NotifyPropertyChanged("WellPressureMin");
		NotifyPropertyChanged("PressureScaleMin");
		NotifyPropertyChanged("PressureScaleAvg");
		NotifyPropertyChanged("PressureScaleMax");

	}
	double StationSensors::WellPressureMin::get()
	{
		return  m_WellPressureMin;

	}

	void StationSensors::WellPressureMax::set(double value)
	{
		m_WellPressureMax = value;

		NotifyPropertyChanged("WellPressureMax");
		NotifyPropertyChanged("PressureScaleMin");
		NotifyPropertyChanged("PressureScaleAvg");
		NotifyPropertyChanged("PressureScaleMax");

	}
	double StationSensors::WellPressureMax::get()
	{
		return  m_WellPressureMax;

	}


	double StationSensors::HumidityMin::get()
	{
		return  m_HumidityMin;

	}

	void StationSensors::HumidityMin::set(double value)
	{
		m_HumidityMin = value;
		NotifyPropertyChanged("HumidityMin");
		NotifyPropertyChanged("HumidityScaleMin");
		NotifyPropertyChanged("HumidityScaleAvg");
		NotifyPropertyChanged("HumidityScaleMax");

	}



	void StationSensors::HumidityMax::set(double value)
	{
		m_HumidityMax = value;
		NotifyPropertyChanged("HumidityMax");
		NotifyPropertyChanged("HumidityScaleMin");
		NotifyPropertyChanged("HumidityScaleAvg");
		NotifyPropertyChanged("HumidityScaleMax");

	}

	double StationSensors::HumidityMax::get()
	{
		return  m_HumidityMax;

	}


	void StationSensors::WellHumidityMin::set(double value)
	{
		m_WellHumidityMin = value;

		NotifyPropertyChanged("WellHumidityMin");
		NotifyPropertyChanged("HumidityScaleMin");
		NotifyPropertyChanged("HumidityScaleAvg");
		NotifyPropertyChanged("HumidityScaleMax");

	}
	double StationSensors::WellHumidityMin::get()
	{
		return  m_WellHumidityMin;

	}



	void StationSensors::WellHumidityMax::set(double value)
	{
		m_WellHumidityMax = value;
		NotifyPropertyChanged("WellHumidityMax");
		NotifyPropertyChanged("HumidityScaleMin");
		NotifyPropertyChanged("HumidityScaleAvg");
		NotifyPropertyChanged("HumidityScaleMax");

	}

	double StationSensors::WellHumidityMax::get()
	{
		return  m_WellHumidityMax;

	}

	void StationSensors::WellTemperatureMin::set(double value)
	{
		m_WellTemperatureMin = value;
		NotifyPropertyChanged("WellTemperatureMin");
		NotifyPropertyChanged("TemperatureScaleMin");
		NotifyPropertyChanged("TemperatureScaleAvg");
		NotifyPropertyChanged("TemperatureScaleMax");

	}
	double StationSensors::WellTemperatureMin::get()
	{
		return  m_WellTemperatureMin;

	}

	void StationSensors::WellTemperatureMax::set(double value)
	{
		m_WellTemperatureMax = value;
		NotifyPropertyChanged("WellTemperatureMax");
		NotifyPropertyChanged("TemperatureScaleMin");
		NotifyPropertyChanged("TemperatureScaleAvg");
		NotifyPropertyChanged("TemperatureScaleMax");

	}

	double StationSensors::WellTemperatureMax::get()
	{
		return  m_WellTemperatureMax;

	}


	void StationSensors::TemperatureMin::set(double value)
	{
		m_TemperatureMin = value;
		NotifyPropertyChanged("TemperatureMin");
		NotifyPropertyChanged("TemperatureScaleMin");
		NotifyPropertyChanged("TemperatureScaleAvg");
		NotifyPropertyChanged("TemperatureScaleMax");
	}
	double StationSensors::TemperatureMin::get()
	{
		return  m_TemperatureMin;

	}

	void StationSensors::TemperatureMax::set(double value)
	{
		m_TemperatureMax = value;
		NotifyPropertyChanged("TemperatureMax");
		NotifyPropertyChanged("TemperatureScaleMin");
		NotifyPropertyChanged("TemperatureScaleAvg");
		NotifyPropertyChanged("TemperatureScaleMax");
	}

	double StationSensors::TemperatureMax::get()
	{
		return  m_TemperatureMax;

	}

	double StationSensors::PressureScaleMin::get()
	{

		double diff = ((this->m_WellPressureMax + this->m_WellPressureMin) / 2) - (this->m_PressureMax + this->m_PressureMin) / 2;

		double max = fabs(this->m_PressureMax);

		double ret;

		double avg = this->PressureScaleAvg;
		double Minmax = 1;
		if (avg != 0) Minmax = (avg / fabs(this->m_WellPressureMax - this->m_WellPressureMin));

		if (this->m_PressureMin < 0) {
			ret = abs(max - avg + this->m_PressureMin) / 2 + diff * Minmax;
		}
		else {
			ret = abs(max - avg) / 2 + diff * Minmax;
		}

		return ret;


	}

	double StationSensors::PressureScaleAvg::get()
	{
		double MaxMin = 1;
		if (this->m_PressureMin > 0) MaxMin = this->m_PressureMax / (this->m_PressureMax - this->m_PressureMin);

		double span = fabs(this->m_WellPressureMax - this->m_WellPressureMin);
		return  span * MaxMin;

	}


	double StationSensors::PressureScaleMax::get()
	{

		double diff = ((this->m_WellPressureMax + this->m_WellPressureMin) / 2) - (this->m_PressureMax + this->m_PressureMin) / 2;
		double max = fabs(this->m_PressureMax);
		double ret;

		double avg = this->PressureScaleAvg;
		double Minmax = 1;
		if (avg != 0) Minmax = (avg / fabs(this->m_WellPressureMax - this->m_WellPressureMin));

		if (this->m_PressureMin < 0) {
			ret = abs(max - avg - this->m_PressureMin) / 2 - diff * Minmax;
		}
		else {
			ret = abs(max - avg) / 2 - diff * Minmax;
		}

		return ret;


	}


	double StationSensors::TemperatureScaleMin::get()
	{

		double diff = ((this->m_WellTemperatureMax + this->m_WellTemperatureMin) / 2) - (this->m_TemperatureMax + this->m_TemperatureMin) / 2;

		double max = fabs(this->m_TemperatureMax);

		double avg = this->TemperatureScaleAvg;
		double Minmax = 1;
		if (avg !=0) Minmax = avg / fabs(this->m_WellTemperatureMax - this->m_WellTemperatureMin);

		double ret;

		if (this->m_TemperatureMin < 0) {
			ret = abs(max - avg + this->m_TemperatureMin) / 2 + diff * Minmax;
		} else{
			ret = abs(max - avg) / 2 + diff* Minmax;
		}

		return ret;


	}

	double StationSensors::TemperatureScaleAvg::get()
	{
	
		double MaxMin = 1;
		if (this->m_TemperatureMin > 0) MaxMin = this->m_TemperatureMax / (this->m_TemperatureMax - this->m_TemperatureMin);

		double span = fabs(this->m_WellTemperatureMax - this->m_WellTemperatureMin);
		return  span*MaxMin;

	}

	
	double StationSensors::TemperatureScaleMax::get()
	{
	
		double diff = ((this->m_WellTemperatureMax + this->m_WellTemperatureMin) / 2) - (this->m_TemperatureMax + this->m_TemperatureMin) / 2;
		double max = fabs(this->m_TemperatureMax);
		double ret;

		double avg = this->TemperatureScaleAvg;
		double Minmax = 1;
		if (avg !=0) Minmax = avg / fabs(this->m_WellTemperatureMax - this->m_WellTemperatureMin);

		if (this->m_TemperatureMin < 0) {
			ret = abs(max - avg - this->m_TemperatureMin) / 2 - diff* Minmax;
		}
		else {
			ret = abs(max - avg) / 2 - diff* Minmax;
		}

		return ret;


	}

	double StationSensors::HumidityScaleMin::get()
	{
		double diff = ((this->m_WellHumidityMax + this->m_WellHumidityMin) / 2) - (this->HumidityMax + this->m_HumidityMin) / 2;

		double max = fabs(this->HumidityMax);

		double avg = this->HumidityScaleAvg;
		double Minmax = 1;
		if (avg !=0) Minmax = avg / fabs(this->m_WellHumidityMax - this->m_WellHumidityMin);

		double ret;

		if (this->m_HumidityMin < 0) {
			ret = fabs(max - avg + this->m_HumidityMin) / 2 + diff* Minmax;
		}
		else
		{
			ret = fabs(max - avg) / 2 + diff* Minmax;
		}
		return ret;


	}

	double StationSensors::HumidityScaleMax::get()
	{
		double diff = ( (this->m_WellHumidityMax + this->m_WellHumidityMin) / 2) -  (this->HumidityMax + this->m_HumidityMin )  / 2;

		double max = fabs(this->HumidityMax);

		double avg = this->HumidityScaleAvg;
		double Minmax = 1;
		if (avg !=0)  Minmax = avg / fabs(this->m_WellHumidityMax - this->m_WellHumidityMin);

		double ret;

		if (this->m_HumidityMin < 0) {
			ret = fabs(max - avg - this->m_HumidityMin) / 2 - diff* Minmax;
		}
		else
		{
			ret = fabs(max - avg) / 2 - diff* Minmax;
		}

		return ret;

	}



	double StationSensors::HumidityScaleAvg::get()
	{
	
		double MaxMin = 1;

		if (this->m_HumidityMin > 0) MaxMin = this->HumidityMax / (this->HumidityMax - this->m_HumidityMin);

		double span = fabs (this->m_WellHumidityMax - this->m_WellHumidityMin);
		return  span * MaxMin;

	}

	double StationSensors::FillingLevelScaleMin::get()
	{
	

		double diff = ((this->m_WellFillingLevelMax + this->m_WellFillingLevelMin) / 2) - (this->m_FillingLevelMax + this->m_FillingLevelMin) / 2;

		double max = fabs(this->m_FillingLevelMax);

		double avg = this->FillingLevelScaleAvg;
		double Minmax = 1;
	


		if (avg != 0) Minmax = avg / fabs(this->m_WellFillingLevelMax - this->m_WellFillingLevelMin);

		double ret;

		if (this->m_FillingLevelMin < 0) {
			ret = fabs(max - avg + this->m_FillingLevelMin) / 2 + diff * Minmax;
		}
		else
		{
			ret = fabs(max - avg) / 2 + diff * Minmax;
		}
		return ret;


	}

	double StationSensors::FillingLevelScaleMax::get()
	{


		double diff = ((this->m_WellFillingLevelMax + this->m_WellFillingLevelMin) / 2) - (this->m_FillingLevelMax + this->m_FillingLevelMin) / 2;

		double max = fabs(this->m_FillingLevelMax);

		double avg = this->FillingLevelScaleAvg;
		double Minmax = 1;
		if (avg != 0)  Minmax = avg / fabs(this->m_WellFillingLevelMax - this->m_WellFillingLevelMin);

		double ret;

		if (this->m_FillingLevelMin < 0) {
			ret = fabs(max - avg - this->m_FillingLevelMin) / 2 - diff * Minmax;
		}
		else
		{
			ret = fabs(max - avg) / 2 - diff * Minmax;
		}

		return ret;

	}



	double StationSensors::FillingLevelScaleAvg::get()
	{

		double MaxMin = 1;

		if (this->m_FillingLevelMin > 0) MaxMin = this->FillingLevelMax / (this->FillingLevelMax - this->m_FillingLevelMin);

		double span = fabs(this->m_WellFillingLevelMax - this->m_WellFillingLevelMin);
		return  span * MaxMin;

	}


	Platform::String^ StationSensors::SensorViewName::get()
	{

		if (!m_SensorViewName->IsEmpty() )
		{
			return m_SensorViewName;

		}
		else return SensorName;
	}

	void StationSensors::SensorViewName::set(Platform::String^  value)
	{
		m_SensorViewName = value; NotifyPropertyChanged("SensorViewName");

	}

	Platform::String^ StationSensors::SensorName::get()
	{

		wchar_t buffer[50];
		swprintf(&buffer[0], sizeof(buffer) / sizeof(buffer[0]), L"SensorId: %d", m_DevId);
		return ref new Platform::String(buffer);

	}

	void StationSensors::DevId::set(int value)
	{
		m_DevId = value; 
		this->m_ActSensorValue->DevId = value;
		NotifyPropertyChanged("DevId");

	}

	void StationSensors::StartUnixTime::set(UINT64 value)
	{
		 m_StartUnixTime = value; NotifyPropertyChanged("StartUnixTime");

	}


	void StationSensors::MaxQueueSize::set(double  value){

		if (m_RecordingHours * m_RecordingSamplePerHour > MAX_QUEUESIZE) {
			value = MAX_QUEUESIZE;
		}
		m_MaxQueueSize = value; 
		NotifyPropertyChanged("MaxQueueSize");
	}



	void StationSensors::startRecording() {
		// alle löschen

		auto lock = m_lock.LockExclusive();

		UINT64 timeinPast = ( (UINT64)m_RecordingHours)*nano100SecInHour;

		deleteSensorValuesolderThen(timeinPast);

		m_StartUnixTime = ConversionHelpers::TimeConversion::getActualUnixTime();

	//	m_LastInsertedUnixTime = WeatherStationData::TimeConversion::getActualUnixTime();
	//	m_LastInsertedUnixTime = 0;


	}





	void StationSensors::deleteSensorValuesolderThen(UINT64 timeinPast) {
		// alle löschen


	//	UINT64 actualTime = WeatherStationData::TimeConversion::getActualUnixTime(); // actuelle Uhrzeit

		UINT64 actualTime = m_LastInsertedUnixTime;




		while (this->m_SensorValues->Size > 0)
		{
			StationSensor ^  sensor = this->m_SensorValues->GetAt(0);


			if ( (actualTime - timeinPast) > sensor->UnixTime) // alle ältere löschen, welche älter als aktulle Zeit- xhours sind
			{
				this->m_SensorValues->RemoveAt(0);

				delete sensor;
			}
			else
			{
				break;
			}

		}

	}


	void StationSensors::deleteSensorValues() {
		// alle löschen

		auto lock = m_lock.LockExclusive();

		while (this->m_SensorValues->Size > 0)
		{
			StationSensor ^  sensor = this->m_SensorValues->GetAt(0);


			this->m_SensorValues->RemoveAt(0);

			delete sensor;
		}

	}

//	NanosToLocalTime

	double StationSensors::getUpdateTime() { // calculate Update Time in dependence of Values in Queue

		double UpdateTime = 0;
		if (m_MaxQueueSize != 0) {
			double size = double(this->m_SensorValues->Size) / double(m_MaxQueueSize);
			// bei einer vollen Queue soll die Zeitspannne für ein Update = 20 sec betragen
			double updateTimeinSec = double(3600 / m_RecordingSamplePerHour);
	//		UpdateTime = size * updateTimeinSec * (10 * 1000 * 1000);
			UpdateTime = size * updateTimeinSec * nano100SecInSec;
			

		}


		return UpdateTime;
	}


	StationLib::StationSensor^ StationSensors::InsertSensorValue(SensorPackage* package) {

		// take values with actual timestamp
		if ((package->getUnixTime() < m_LastInsertedUnixTime) ) {
			return nullptr;
		}

		auto lock = m_lock.LockExclusive();

//		m_ActSensorValue->DistanceMax = this->FillingLevelMax - this->FillingLevelMin;
		double HCSR04MeasTimeDistance = package->getDistance();
		double SensorDistance = this->SensorDistanceMax- HCSR04MeasTimeDistance;
		double fillingLevel;
		if (this->SensorDistanceMax - this->SensorDistanceMin > 0) {
			fillingLevel = (SensorDistance*this->FillingLevelMax) / (this->SensorDistanceMax - this->SensorDistanceMin);
		}
		else fillingLevel = (SensorDistance);


		double fillingVolume;
		if (m_CysternVolumeProCm != 0)
		{
			fillingVolume = (m_CysternVolumeProCm * SensorDistance) / (100);

		}
		else
		{
			fillingVolume = ((this->m_CysternDiameter * this->m_CysternDiameter)*3.14 * SensorDistance) / (4 * 1000);
		}


		m_ActSensorValue->Humidity = package->getHumidity();
		m_ActSensorValue->Temperature = package->getTemperature();
		m_ActSensorValue->Pressure = package->getPressure();


		m_ActSensorValue->HCSR04MeasTimeDistance = HCSR04MeasTimeDistance;

		m_ActSensorValue->Distance = SensorDistance;
		m_ActSensorValue->FillingLevel = fillingLevel;
		m_ActSensorValue->HCSR04MeasTime = package->getHCSR04MeasTime();
		m_ActSensorValue->FillingVolume = fillingVolume;

	
		m_ActSensorValue->UnixTime = package->getUnixTime();

	
		if ((package->getUnixTime() - m_LastInsertedUnixTime ) < getUpdateTime() ) { // each 2 sec one value

			return nullptr;
		}
		

		StationLib::StationSensor^ sensor = ref new StationLib::StationSensor(package);
		sensor->Height = this->m_Height;
		sensor->Distance = SensorDistance;
		sensor->FillingLevel = fillingLevel;
		sensor->FillingVolume = fillingVolume;
		sensor->OutPutFillingLevelMin = m_ActSensorValue->OutPutFillingLevelMin;
		sensor->OutPutWellFillingLevelMin = m_ActSensorValue->OutPutWellFillingLevelMin;
		m_LastInsertedUnixTime = package->getUnixTime();

		sensor->DiffUnixTime = m_LastInsertedUnixTime - this->m_StartUnixTime;


		this->m_SensorValues->Append(sensor);
	
		UINT64 timeinPast = nano100SecInHour * (UINT64)this->m_RecordingHours;

		deleteSensorValuesolderThen(timeinPast); // zurückliegende > m_RecordingHours löschen

	


		return sensor;
	}



	void StationLib::StationSensors::NotifyPropertyChanged(Platform::String^ prop)
	{

		PropertyChanged(this, ref new PropertyChangedEventArgs(prop));

	}


}