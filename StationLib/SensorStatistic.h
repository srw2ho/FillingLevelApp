#pragma once
#include "SensorPackage.h"
namespace StationLib {
	class SensorStatistic
	{
		std::vector<SensorPackage*>* m_packetQueue;
		CRITICAL_SECTION m_CritLock;
		size_t m_MaxPackageSize;
		double m_MaxLevelMillimeterProSec;
		double m_RefreshRateinSec;
		double m_MaxDistance;
		double m_MinDistance;
	public:
		SensorStatistic();
		virtual ~SensorStatistic();
		void setMaxPackageSize(size_t Value) { m_MaxPackageSize = Value; };
		size_t getMaxPackageSize() { return m_MaxPackageSize; };

		void setMaxLevelMillimeterProSec(double Value) { m_MaxLevelMillimeterProSec = Value; };
		double getMaxLevelMillimeterProSec() { return m_MaxLevelMillimeterProSec; };
		
		void setRefreshRateinSec(double Value) { m_RefreshRateinSec = Value; };
		double getRefreshRateinSec() { return m_RefreshRateinSec; };

		void setMaxDistance(double Value) { m_MaxDistance = Value; }; // in cm
		double getMaxDistance() { return m_MaxDistance; };			 // in cm

		void setMinDistance(double Value) { m_MinDistance = Value; }; // in cm
		double getMinDistance() { return m_MinDistance; };			 // in cm

		SensorPackage* GetAverage(SensorPackage* package);
		void PushPacket(SensorPackage* ppacket);

		virtual void Flush();
		SensorPackage* PopPacket();
		void Lock();

		void UnLock();
	protected:
		bool checkPackageforPlausibility(SensorPackage* ppacket);
		bool AveragePackage(SensorPackage* ppacket);
	};
}
