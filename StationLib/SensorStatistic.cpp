#include "pch.h"
#include "SensorStatistic.h"

namespace StationLib {
	const UINT64 nano100SecInSec = (UINT64)10000000 * 1;

	SensorStatistic::SensorStatistic()
	{
		m_packetQueue = new std::vector<SensorPackage*>();
		InitializeCriticalSection(&m_CritLock);

		m_MaxPackageSize = 1500;
		m_MaxLevelMillimeterProSec = 3;
		m_RefreshRateinSec = 1;
		m_MaxDistance = 350;	// 350 cm Max
		m_MinDistance = 3;		// 3cm Min
	}


	SensorStatistic::~SensorStatistic()
	{
		this->Flush();
		delete m_packetQueue;
		DeleteCriticalSection(&m_CritLock);

	}

	bool SensorStatistic::checkPackageforPlausibility(SensorPackage* ppacket) {
		bool plausible = true;
		this->Lock();
		if (ppacket->getHCSR04MeasTime() != -1) // nicht gemessen
		{
			if ((m_packetQueue->size() > 0) && (m_MaxLevelMillimeterProSec > 0))
			{
				SensorPackage* ppLastpacket = m_packetQueue->at(m_packetQueue->size() - 1);
				double time = double (ppacket->getUnixTime() - ppLastpacket->getUnixTime());

				double distance = ppacket->getDistance();
				double lastdistance = ppLastpacket->getDistance();
				double streuung = ppacket->getSensorStreuungByDistance();
				double distanceMin = m_MinDistance - streuung;
				double distanceMax = m_MaxDistance + streuung;

				if (distance < distanceMin) {m_MinDistance = distanceMin;}
				if (distance > distanceMax) {m_MaxDistance = distanceMax;}

				if (time >= nano100SecInSec * m_RefreshRateinSec) //ist time >) upaterate, dann ok
				{
					double deltaValueinMM = 10 * (distance - lastdistance);
					double detaTime = double(time / nano100SecInSec);
					if (fabs(deltaValueinMM) > fabs(m_MaxLevelMillimeterProSec * detaTime))
					{
						double offsetinMM;
						if (deltaValueinMM > 0) offsetinMM = m_MaxLevelMillimeterProSec * detaTime;
						else offsetinMM = m_MaxLevelMillimeterProSec * detaTime * (-1);
						offsetinMM = offsetinMM * 0.1;
						ppacket->setHCSR04MeasTimeByDistance(lastdistance + offsetinMM);
						ppacket->setPlausible(false);
					}
				}
				else
				{ // Info mit gleicher Zeit
					plausible = false;
				}
			}
		}
		else
		{
			plausible = false;
		}
		this->UnLock();
		return plausible;
	}

	void SensorStatistic::PushPacket(SensorPackage* ppacket) {

		this->Lock();
		bool insert = true;

		m_packetQueue->push_back(ppacket);
		while (m_packetQueue->size() > m_MaxPackageSize) { // ersten  löschen, wenn überfüllt
			SensorPackage* Packet = PopPacket();
			delete Packet;

		}
		this->UnLock();

	};

	SensorPackage* SensorStatistic::GetAverage(SensorPackage* package)
	{
		SensorPackage * pInsertPackage = nullptr;
		this->Lock();
		bool checkfor = checkPackageforPlausibility(package);
		if (checkfor) {
			pInsertPackage = new SensorPackage(*package);
			package->setPlausible(checkfor);

			this->PushPacket(pInsertPackage);

			this->AveragePackage(pInsertPackage);
		}



		this->UnLock();

		return pInsertPackage;
	
	}

	bool SensorStatistic::AveragePackage(SensorPackage* ppacket) {

		this->Lock();
		size_t Idx = 0;
		size_t tempIdx = 0;
		if (!m_packetQueue->empty() && m_packetQueue->size() > 1)
		{
			Idx = 0;
			tempIdx = 0;
			double PressureStart = 0;
			double TempeatureStart = 0;
			double HumidityStart = 0;
			double DistanceStart = 0;
			double HCSR04MeasTimeStart = 0;


			for (size_t i = 0; i < m_packetQueue->size(); i++) {
				SensorPackage* packet = m_packetQueue->at(i);

				if (packet->getTemperature() != -1)
				{
					tempIdx++;
					TempeatureStart = TempeatureStart + packet->getTemperature();
					PressureStart	= PressureStart + packet->getPressure();
					HumidityStart	= HumidityStart + packet->getHumidity();
				}
				HCSR04MeasTimeStart = HCSR04MeasTimeStart + packet->getHCSR04MeasTime();
	//			DistanceStart = DistanceStart + packet->getDistance(); wird aus HCSR04MeasTime berechnet
				Idx++;
			}

			if (tempIdx > 0) {
				TempeatureStart /= tempIdx;
				PressureStart /= tempIdx;
				HumidityStart /= tempIdx;
				(ppacket)->setTemperature(TempeatureStart);
				(ppacket)->setPressure(PressureStart);
				(ppacket)->setHumidity(HumidityStart);
			}


			DistanceStart /= Idx;
			HCSR04MeasTimeStart /= Idx;


			(ppacket)->setHCSR04MeasTime(HCSR04MeasTimeStart);
	//		(ppacket)->setHCSR04MeasTimeByDistance(DistanceStart);

		}

		this->UnLock();
		return (Idx > 0);

	};

	SensorPackage* SensorStatistic::PopPacket() {
		SensorPackage*pPacketRet = nullptr;
		this->Lock();
		if (!m_packetQueue->empty())
		{
			pPacketRet = m_packetQueue->front();
			//	avPacket = m_packetQueue.front();
			m_packetQueue->erase(m_packetQueue->begin());


		}
		this->UnLock();
		return pPacketRet;
	};
	void SensorStatistic::Flush()
	{
		this->Lock();
		while (!m_packetQueue->empty())
		{
			SensorPackage* Packet = PopPacket();
			delete Packet;
		}
		this->UnLock();
	};

	void SensorStatistic::Lock() {
		EnterCriticalSection(&m_CritLock);
	}

	void SensorStatistic::UnLock() {
		LeaveCriticalSection(&m_CritLock);
	}

}