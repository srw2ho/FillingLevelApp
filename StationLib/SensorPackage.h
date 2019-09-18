#pragma once
namespace StationLib {
	class SensorPackage
	{
		BYTE m_DevId;
		double m_pressure;
		/*! Compensated temperature */
		double m_temperature;
		/*! Compensated humidity */
		double m_humidity;
		UINT64 m_UnixTime;
		double m_HC_SR04MeasTime;
		double m_Distance;
		bool m_isPlausible;
		int m_DistanceErrCnt;
		int m_TempErrCnt;

	public:
		SensorPackage();
		SensorPackage(const SensorPackage&right);
		virtual ~SensorPackage();
		// getter setters
		void setPressure(double Pressure) { m_pressure = Pressure; };
		void setTemperature(double Temperature) ;
		void setHumidity(double Humidity) { m_humidity = Humidity; };
		void setUnixTime(UINT64 UnixTime) { m_UnixTime = UnixTime; };
		void setDevId(BYTE DevId) { m_DevId = DevId; };
		void setHCSR04MeasTime(double value) ;
		void setHCSR04MeasTimeByDistance(double Distance); // korrigiere setHCSR04MeasTime by Distance
		void setDistance(double Distance);
		void setPlausible(bool plausible) { m_isPlausible = plausible; };
		void setTempErrCnt(int value) { m_TempErrCnt = value; };
		void setDistanceErrCnt(int value) { m_DistanceErrCnt = value; };

		double getPressure() { return m_pressure; };
		double getTemperature() { return m_temperature; };
		double getHumidity() { return m_humidity; };
		UINT64 getUnixTime() { return m_UnixTime; };
		double getHCSR04MeasTime() { return m_HC_SR04MeasTime; };// int µs
		double getDistance(); // into cm
		bool getPlausible() { return m_isPlausible; };
		int getTempErrCnt() { return m_TempErrCnt; };
		int getDistanceErrCnt() { return m_DistanceErrCnt; };
		BYTE getDevId() { return m_DevId; };
		double getSensorStreuungByDistance();
	};

}

