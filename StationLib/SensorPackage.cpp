#include "pch.h"
#include "SensorPackage.h"

namespace StationLib {

#define SensorStreuung_50cm		0.5	// 0.5 cm Streuung ab 100 cm
#define SensorStreuung_100cm	1	// 1 cm Streuung ab 100 cm
#define SensorStreuung_150cm	2	// 2 cm Streuung ab 150 cm
#define SensorStreuung_200cm	3	// 3 cm Streuung ab 200 cm

	SensorPackage::SensorPackage()
	{
		m_DevId = -1;
		m_pressure = -1;
		m_temperature = -1;
		m_humidity = -1;
		m_UnixTime = -1;
		m_HC_SR04MeasTime = -1;
		m_Distance = 0;
		m_isPlausible = true;
		m_DistanceErrCnt = 0;
		m_TempErrCnt = 0;
	}

	SensorPackage::SensorPackage(const SensorPackage&right)
	{
		if (&right != this) {
			m_DevId = right.m_DevId;
			m_pressure = right.m_pressure;
			m_temperature = right.m_temperature;
			m_humidity = right.m_humidity;
			m_UnixTime = right.m_UnixTime;
			m_HC_SR04MeasTime = right.m_HC_SR04MeasTime;
			m_Distance = right.m_Distance;
			m_isPlausible = right.m_isPlausible;
			m_DistanceErrCnt = right.m_DistanceErrCnt;
			m_TempErrCnt = right.m_TempErrCnt;

		}

	}

	SensorPackage::~SensorPackage()
	{
	}

	void SensorPackage::setHCSR04MeasTime(double value)
	{ 
		if (value == -1) {//Sensor Error
			if (m_DistanceErrCnt <= 5) {
				m_DistanceErrCnt++;
			}
		}
		else if (m_DistanceErrCnt>0)
		{
			m_DistanceErrCnt--;
		}
		m_HC_SR04MeasTime = value; 
	}

	void SensorPackage::setTemperature(double Temperature)
	{ 
		if (Temperature == -1) { // Sensor Error
			if (m_TempErrCnt <= 5) {
				m_TempErrCnt++;
			}

		}
		else if (m_TempErrCnt > 0)
		{
			m_TempErrCnt--;
		}

		m_temperature = Temperature; 
	}

	void SensorPackage::setDistance(double Distance) 
	{ 
		m_Distance = Distance;

	}
	void SensorPackage::setHCSR04MeasTimeByDistance(double Distance)
	{
		double temp;
		if (m_temperature != -1) temp = m_temperature;
		else temp = 15;

		double VinMeterProSec = 331.5 + (0.6 * temp);
		// Distanz umgerechnet in m_HC_SR04MeasTime
		m_HC_SR04MeasTime = Distance * (10000 * 2) / (VinMeterProSec);
		m_Distance = Distance;

	}

	double SensorPackage::getDistance()
	{ 
		double temp;
		if (m_temperature != -1) temp = m_temperature;
		else temp = 15;

		double VinMeterProSec = 331.5 + (0.6 * temp);
		m_Distance = (VinMeterProSec * m_HC_SR04MeasTime) / (10000 * 2);

		return m_Distance; // into cm
	};

	double SensorPackage::getSensorStreuungByDistance()
	{

		double DeltaStreuungDistance = 0;
		if (m_Distance >= 200)
		{
			DeltaStreuungDistance = (SensorStreuung_200cm);
		}
		else
		if (m_Distance >= 150)
		{
			DeltaStreuungDistance = (SensorStreuung_150cm);
		}
		else if (m_Distance >= 100)
		{
			DeltaStreuungDistance = (SensorStreuung_100cm);
		}
		else
		{
			DeltaStreuungDistance = (SensorStreuung_50cm);
		}
		return DeltaStreuungDistance;
	}
}