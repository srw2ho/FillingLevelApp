
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using StationLib;

namespace FillingLevelApp.Helpers
{
    public class SettingsToStorage
    {
        private LocalStorageSettings m_LocalStorageSettings;

        private LocalStorageItem m_localStorage;
        private StationEnvironment m_StationEnvironment;

        private EmailSupport.eMailing m_eMailing;

        public SettingsToStorage(FillingLevelApp.App app)
        {
  //          WeatherStationEnvironment weatherStationEnvironment
            m_LocalStorageSettings = new LocalStorageSettings("FillingLevelAppEnvironment");
   
            m_localStorage = new LocalStorageItem("StationSocketListener");
            m_StationEnvironment = app.Environment;
            m_eMailing = app.eMailing;


        }

        public bool writeSensorDatatoLocalStorage(StationLib.StationSensors sensorValues, Windows.Storage.ApplicationDataCompositeValue composite, int sensorValuesIdx)
        {

            bool bok = m_localStorage.writeSettingsToLocalStorage(composite, sensorValuesIdx);

            bok = m_localStorage.writeStringSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.SensorName", sensorValuesIdx), sensorValues.SensorName);
            bok = m_localStorage.writeStringSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.SensorViewName", sensorValuesIdx), sensorValues.SensorViewName);
            bok = m_localStorage.writeIntegerSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.DevId", sensorValuesIdx), sensorValues.DevId);
            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.RecordingHours", sensorValuesIdx), sensorValues.RecordingHours);
            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.RecordingSamplePerHour", sensorValuesIdx), sensorValues.RecordingSamplePerHour);
            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.Height", sensorValuesIdx), sensorValues.Height);

            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.TemperatureMin", sensorValuesIdx), sensorValues.TemperatureMin);
            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.TemperatureMax", sensorValuesIdx), sensorValues.TemperatureMax);

            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellTemperatureMax", sensorValuesIdx), sensorValues.WellTemperatureMax);
            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellTemperatureMin", sensorValuesIdx), sensorValues.WellTemperatureMin);


            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.HumidityMin", sensorValuesIdx), sensorValues.HumidityMin);
            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.HumidityMax", sensorValuesIdx), sensorValues.HumidityMax);

            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellHumidityMax", sensorValuesIdx), sensorValues.WellHumidityMax);
            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellHumidityMin", sensorValuesIdx), sensorValues.WellHumidityMin);

            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.PressureMin", sensorValuesIdx), sensorValues.PressureMin);
            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.PressureMax", sensorValuesIdx), sensorValues.PressureMax);

            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellPressureMax", sensorValuesIdx), sensorValues.WellPressureMax);
            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellPressureMin", sensorValuesIdx), sensorValues.WellPressureMin);

            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.FillingLevelMin", sensorValuesIdx), sensorValues.FillingLevelMin);
            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.FillingLevelMax", sensorValuesIdx), sensorValues.FillingLevelMax);

            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellFillingLevelMin", sensorValuesIdx), sensorValues.WellFillingLevelMin);
            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellFillingLevelMax", sensorValuesIdx), sensorValues.WellFillingLevelMax);


            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.SensorDistanceMin", sensorValuesIdx), sensorValues.SensorDistanceMin);
            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.SensorDistanceMax", sensorValuesIdx), sensorValues.SensorDistanceMax);
 
            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.CysternDiameter", sensorValuesIdx), sensorValues.CysternDiameter);

            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.CysternVolumeProCm", sensorValuesIdx), sensorValues.CysternVolumeProCm);

            return bok;
        }

        public bool readSensorDatafromLocalStorage(StationLib.StationSensors sensorValues , Windows.Storage.ApplicationDataCompositeValue composite, int sensorValuesIdx)
        {
            string StringValue;
            int IntValue;
            double doubleValue;
            bool bStoreok = m_localStorage.readSettingsfromLocalStorage(composite, sensorValuesIdx);
            if (bStoreok)
            {
                bool bok = m_localStorage.readStringSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.SensorName", sensorValuesIdx), out StringValue);
            //    sensorValues.SensorName = StringValue;
                bok = m_localStorage.readStringSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.SensorViewName", sensorValuesIdx), out StringValue);

                sensorValues.SensorViewName = StringValue;
                bok = m_localStorage.readIntegerSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.DevId", sensorValuesIdx), out IntValue);
                sensorValues.DevId = IntValue;
                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.RecordingHours", sensorValuesIdx), out doubleValue);
                sensorValues.RecordingHours = doubleValue;
                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.RecordingSamplePerHour", sensorValuesIdx), out doubleValue);
                sensorValues.RecordingSamplePerHour = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.Height", sensorValuesIdx), out doubleValue);
                sensorValues.Height = (uint)doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.TemperatureMin", sensorValuesIdx), out doubleValue);
                sensorValues.TemperatureMin = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.TemperatureMax", sensorValuesIdx), out doubleValue);
                sensorValues.TemperatureMax = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellTemperatureMax", sensorValuesIdx), out doubleValue);
                sensorValues.WellTemperatureMax = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellTemperatureMin", sensorValuesIdx), out doubleValue);
                sensorValues.WellTemperatureMin = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.HumidityMin", sensorValuesIdx), out doubleValue);
                sensorValues.HumidityMin = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.HumidityMax", sensorValuesIdx), out doubleValue);
                sensorValues.HumidityMax = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellHumidityMax", sensorValuesIdx), out doubleValue);
                sensorValues.WellHumidityMax = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellHumidityMin", sensorValuesIdx), out doubleValue);
                sensorValues.WellHumidityMin = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.PressureMin", sensorValuesIdx), out doubleValue);
                sensorValues.PressureMin = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.PressureMax", sensorValuesIdx), out doubleValue);
                sensorValues.PressureMax = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellPressureMax", sensorValuesIdx), out doubleValue);
                sensorValues.WellPressureMax = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellPressureMin", sensorValuesIdx), out doubleValue);
                sensorValues.WellPressureMin = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.FillingLevelMin", sensorValuesIdx), out doubleValue);

                sensorValues.FillingLevelMin = doubleValue;
                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.FillingLevelMax", sensorValuesIdx), out doubleValue);
                sensorValues.FillingLevelMax = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellFillingLevelMin", sensorValuesIdx), out doubleValue);
                sensorValues.WellFillingLevelMin = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.WellFillingLevelMax", sensorValuesIdx), out doubleValue);
                sensorValues.WellFillingLevelMax = doubleValue;


                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.SensorDistanceMin", sensorValuesIdx), out doubleValue);
                sensorValues.SensorDistanceMin = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.SensorDistanceMax", sensorValuesIdx), out doubleValue);
                sensorValues.SensorDistanceMax = doubleValue;

          
                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.CysternDiameter", sensorValuesIdx), out doubleValue);
                sensorValues.CysternDiameter = doubleValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("sensorValues.CysternVolumeProCm", sensorValuesIdx), out doubleValue);
                sensorValues.CysternVolumeProCm = doubleValue;


            }


            return bStoreok;
        }




        public bool writeSocketListenerDatatoLocalStorage(StationLib.StationSocketListener listener, Windows.Storage.ApplicationDataCompositeValue composite, int ListenerIdx)
        {

            if (m_localStorage == null) return false;
            m_localStorage.SetSourceIDName("StationSocketListener", ListenerIdx);

            int Idx = -1;

            bool bok = m_localStorage.writeSettingsToLocalStorage(composite, Idx);

            bok = m_localStorage.writeStringSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.HostName", Idx), listener.HostName);
            bok = m_localStorage.writeIntegerSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.Port", Idx), listener.Port);
            bok = m_localStorage.writeStringSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.ViewName", Idx), listener.ViewName);


            bok = m_localStorage.writeIntegerSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.SensorViewSelection", Idx), listener.SensorViewSelection);



            bok = m_localStorage.writeIntegerSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.ChartViewSelection", Idx), listener.ChartViewSelection);

            bok = m_localStorage.writeIntegerSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.BME280SensorId", Idx), listener.BME280SensorId);

            bok = m_localStorage.writeIntegerSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.GPIOHCSR04EchoPin", Idx), listener.GPIOHCSR04EchoPin);

            bok = m_localStorage.writeIntegerSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.GPIOHCSR04TriggPin", Idx), listener.GPIOHCSR04TriggPin);

            bok = m_localStorage.writeIntegerSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.OutPutLedPin", Idx), listener.OutPutLedPin);

            bok = m_localStorage.writeIntegerSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.OutPutLevel1Pin", Idx), listener.OutPutLevel1Pin);

            bok = m_localStorage.writeIntegerSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.OutPutLevel2Pin", Idx), listener.OutPutLevel2Pin);

            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.RefreshRateinSec", Idx), listener.RefreshRateinSec);

            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.MaxLevelMillimeterProSec", Idx), listener.MaxLevelMillimeterProSec);

            bok = m_localStorage.writeBoolSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.eMailWellFillingLevelMinActiv", Idx), listener.eMailWellFillingLevelMinActiv);

            bok = m_localStorage.writeBoolSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.eMailWellFillingLevelMaxActiv", Idx), listener.eMailWellFillingLevelMaxActiv);

            bok = m_localStorage.writeBoolSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.eMailFillingLevelMinActiv", Idx), listener.eMailFillingLevelMinActiv);

            bok = m_localStorage.writeBoolSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.OutPutWellFillingLevelMinActiv", Idx), listener.OutPutWellFillingLevelMinActiv);


            bok = m_localStorage.writeBoolSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.OutPutFillingLevelMinActiv", Idx), listener.OutPutFillingLevelMinActiv);

            bok = m_localStorage.writeBoolSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.ListenerMode", Idx), listener.ListenerMode);


            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.OutPutLevel1PinActivValue", Idx), listener.OutPutLevel1PinActivValue);

            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.OutPutLevel2PinActivValue", Idx), listener.OutPutLevel2PinActivValue);

            bok = m_localStorage.writeIntegerSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.ShutDownPin", Idx), listener.ShutDownPin);


            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.InletFillLevelInPercent", Idx), listener.InletFillLevelInPercent);
        
            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.InletMaxActiveTime", Idx), listener.InletMaxActiveTime);
    
            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.InletCycleFlushingTime", Idx), listener.InletCycleFlushingTime);

            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.InletMaxCycleFlushingActiveTime", Idx), listener.InletMaxCycleFlushingActiveTime);


            StationSensors sensorValue = listener.StationSensors;
            m_localStorage.ResetCompositeName();
            m_localStorage.DataCompositeIDName = m_localStorage.getCompositePropertyIDName("StationSensorValues", -1);


            int SensorsIdx = 0;
            bok = writeSensorDatatoLocalStorage(sensorValue, composite, SensorsIdx);



            return bok;

        }

        public bool readSocketListenerDatafromLocalStorage(StationLib.StationSocketListener listener, Windows.Storage.ApplicationDataCompositeValue composite, int ListenerIdx)
        {

            if (m_localStorage == null) return false;
            m_localStorage.SetSourceIDName("StationSocketListener", ListenerIdx);


            int Idx = -1;
            bool bStoreOk = m_localStorage.readSettingsfromLocalStorage(composite, Idx);

            if (bStoreOk)
            {
                string StringValue;
                int IntValue;
                double dblValue;
                bool   boolValue;

                bool bok = m_localStorage.readStringSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.HostName", Idx), out StringValue);
                listener.HostName = StringValue;

                bok = m_localStorage.readIntegerSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.Port", Idx), out IntValue);
                listener.Port = IntValue;

                bok = m_localStorage.readStringSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.ViewName", Idx), out StringValue);
                listener.ViewName = StringValue;

                bok = m_localStorage.readIntegerSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.SensorViewSelection", Idx), out IntValue);
                listener.SensorViewSelection = IntValue;


                bok = m_localStorage.readIntegerSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.ChartViewSelection", Idx), out IntValue);
                listener.ChartViewSelection = IntValue;

                bok = m_localStorage.readIntegerSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.BME280SensorId", Idx), out IntValue);
                listener.BME280SensorId = IntValue;

                bok = m_localStorage.readIntegerSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.GPIOHCSR04EchoPin", Idx), out IntValue);
                listener.GPIOHCSR04EchoPin = IntValue;

                bok = m_localStorage.readIntegerSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.GPIOHCSR04TriggPin", Idx), out IntValue);
                listener.GPIOHCSR04TriggPin = IntValue;

                bok = m_localStorage.readIntegerSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.OutPutLedPin", Idx), out IntValue);
                listener.OutPutLedPin = IntValue;

                bok = m_localStorage.readIntegerSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.OutPutLevel1Pin", Idx), out IntValue);
                listener.OutPutLevel1Pin = IntValue;

                bok = m_localStorage.readIntegerSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.OutPutLevel2Pin", Idx), out IntValue);

                listener.OutPutLevel2Pin = IntValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.RefreshRateinSec", Idx), out dblValue);

                listener.RefreshRateinSec = dblValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.MaxLevelMillimeterProSec", Idx), out dblValue);

                listener.MaxLevelMillimeterProSec = dblValue;

                bok = m_localStorage.readBoolSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.eMailWellFillingLevelMinActiv", Idx), out boolValue);

                listener.eMailWellFillingLevelMinActiv = boolValue;

                bok = m_localStorage.readBoolSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.eMailWellFillingLevelMaxActiv", Idx), out boolValue);

                listener.eMailWellFillingLevelMaxActiv = boolValue;

                bok = m_localStorage.readBoolSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.eMailFillingLevelMinActiv", Idx), out boolValue);

                listener.eMailFillingLevelMinActiv = boolValue;

                bok = m_localStorage.readBoolSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.OutPutWellFillingLevelMinActiv", Idx), out boolValue);

                listener.OutPutWellFillingLevelMinActiv = boolValue;

                bok = m_localStorage.readBoolSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.OutPutFillingLevelMinActiv", Idx), out boolValue);

                listener.OutPutFillingLevelMinActiv = boolValue;


                bok = m_localStorage.readBoolSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.ListenerMode", Idx), out boolValue);

                listener.ListenerMode = boolValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.OutPutLevel1PinActivValue", Idx), out dblValue);

                listener.OutPutLevel1PinActivValue = dblValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.OutPutLevel2PinActivValue", Idx), out dblValue);

                listener.OutPutLevel2PinActivValue = dblValue;


                bok = m_localStorage.readIntegerSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.ShutDownPin", Idx), out IntValue);
                listener.ShutDownPin = IntValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.InletFillLevelInPercent", Idx), out dblValue);
                listener.InletFillLevelInPercent = dblValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.InletMaxActiveTime", Idx), out dblValue);
                listener.InletMaxActiveTime = dblValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.InletCycleFlushingTime", Idx), out dblValue);
                listener.InletCycleFlushingTime = dblValue;

                bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_localStorage.getCompositePropertyIDName("listener.InletMaxCycleFlushingActiveTime", Idx), out dblValue);
                listener.InletMaxCycleFlushingActiveTime = dblValue;

                


                StationSensors sensorValue = listener.StationSensors;
                int SensorsIdx = 0;

                m_localStorage.ResetCompositeName();
                m_localStorage.DataCompositeIDName = m_localStorage.getCompositePropertyIDName("StationSensorValues", -1);
                bool bfound = readSensorDatafromLocalStorage(sensorValue, composite, SensorsIdx);


/*
                int SensorsIdx = 0;
                while (true) // vorhandene Profiles aus den Settings lesen
                {
                    StationLib.StationSensors SensorValues =  new StationSensors();
                    listener.StationSensors.AddSensorValues(SensorValues);
                    m_localStorage.ResetCompositeName();
                    m_localStorage.DataCompositeIDName = m_localStorage.getCompositePropertyIDName("StationSensorValues", -1);

                    bool bfound = readSensorDatafromLocalStorage(SensorValues, composite, SensorsIdx);
                    if (!bfound)
                    {// wiede loeschen, wenn nicht gefunden
                        listener.BME280SensorsValues.deleteSensorValues(SensorValues);
                        break;
                    }
        
       
                    SensorsIdx++;
                }
*/

            }

            return bStoreOk;


        }



        protected StationSocketListener createDummyListener()
        {

            StationSocketListener listener = new StationSocketListener();
            listener.HostName = "localhost";
            listener.Port = 3005;
            listener.ViewName = "Cysterne";

            listener.GPIOHCSR04EchoPin = 12;
            listener.GPIOHCSR04TriggPin = 23;
            listener.BME280SensorId = 0x76;
            listener.OutPutLedPin = 22;

            listener.OutPutLevel1Pin = 17;

            listener.OutPutLevel2Pin = 27;

            listener.ShutDownPin = 24;

            listener.MaxLevelMillimeterProSec = 3;

            listener.eMailWellFillingLevelMinActiv = false;

            listener.eMailWellFillingLevelMaxActiv = false;
            listener.eMailFillingLevelMinActiv = false;
            listener.OutPutWellFillingLevelMinActiv = false;
            listener.OutPutFillingLevelMinActiv = false;

            listener.OutPutLevel2PinActivValue = 0; // Output ist low aktiv
            listener.OutPutLevel1PinActivValue = 0;

            listener.ListenerMode = false;

            listener.InletFillLevelInPercent = 2;

            listener.InletMaxActiveTime = 1;
            listener.InletMaxCycleFlushingActiveTime = 1;

            listener.InletCycleFlushingTime = 2; // alle 2 Tage


            StationSensors sensorValue = listener.StationSensors;
            sensorValue.DevId = 118;
            sensorValue.SensorViewName = "";
            // 204 stunde Aufzeichnung
            sensorValue.RecordingHours = 204;
            //mit 5 Samples/Sec.
            sensorValue.RecordingSamplePerHour = 5;


            sensorValue.HumidityMax = 100;
            sensorValue.HumidityMin = 0;
            sensorValue.WellHumidityMax = 90;
            sensorValue.WellHumidityMin = 40;

            sensorValue.TemperatureMax = 45;
            sensorValue.TemperatureMin = 0;
            sensorValue.WellTemperatureMax = 25;
            sensorValue.WellTemperatureMin = 20;

            sensorValue.PressureMax = 1500;
            sensorValue.PressureMin = 500;
            sensorValue.WellPressureMax = 1100;
            sensorValue.WellPressureMin = 900;

            sensorValue.FillingLevelMin = 0;

            sensorValue.FillingLevelMax = 100;

            sensorValue.WellFillingLevelMax = 40;

            sensorValue.WellFillingLevelMin = 10;

            sensorValue.SensorDistanceMin = 30; // in cm
            sensorValue.SensorDistanceMax = 180;
            sensorValue.CysternDiameter = 230; // Cysterne Durchmesser in cm
            sensorValue.CysternVolumeProCm = 0;




            return listener;
        }

        public bool writeListenerDatatoLocalStorage()
        {


            m_LocalStorageSettings.SetSourceIDName("WeatherStationData");

            m_LocalStorageSettings.deleteCompositeValue(); // vor jedem Schreiben alles löschen



            Windows.Storage.ApplicationDataCompositeValue composite = m_LocalStorageSettings.getCompositeValue();

            SocketStationListeners listeners = m_StationEnvironment.StationSocketListeners;


            for (int i = 0; i < listeners.SocketListeners.Count; i++)
            {
                StationSocketListener listener = listeners.SocketListeners[i];
                writeSocketListenerDatatoLocalStorage(listener, composite, i);

            }

            if (composite.ContainsKey("SelectedIndex"))
            {
                composite["SelectedIndex"] = listeners.SelectedIndex;
            }
            else
            {
                composite.Add("SelectedIndex", listeners.SelectedIndex);
            }

            m_LocalStorageSettings.writeCompositeValuetoLocalStorage();

            return true;
        }

        public bool readListenerDatafromLocalStorage()
        {


            SocketStationListeners listeners = m_StationEnvironment.StationSocketListeners;


            listeners.clearallListeners();

            m_LocalStorageSettings.SetSourceIDName("WeatherStationData");

            Windows.Storage.ApplicationDataCompositeValue composite = m_LocalStorageSettings.getCompositeValue();
            int Idx = 0;
            while (true)
            {
                StationSocketListener listener = new StationSocketListener();
                listeners.AddListener(listener);


                bool bdata = readSocketListenerDatafromLocalStorage(listener, composite, Idx);
                if (!bdata)
                {
                    // listener wieder aus Queue löshen, wenn nicht benötigt
                    listeners.deleteListener(listener);
                    //    delete listener;
                    break;
                }
                Idx++;

            }


            if (listeners.SocketListeners.Count == 0) // Add Dummy
            {
                StationSocketListener listener = createDummyListener();
                listeners.SocketListeners.Add(listener);
                listeners.SelectedIndex = 0;
            }
            else
            {
                Object outvalue;
                bool bKey = composite.TryGetValue("SelectedIndex", out outvalue);
                if (bKey) listeners.SelectedIndex = (int)outvalue;
            }


            return (listeners.SocketListeners.Count > 0);

        }



        public bool readMailingDatafromLocalStorage()
        {


            m_LocalStorageSettings.SetSourceIDName("eMailing");
            Windows.Storage.ApplicationDataCompositeValue composite = m_LocalStorageSettings.getCompositeValue();


            bool bok;


            string StringValue;

            double doubleValue;

            bool boolValue;

            bok = m_localStorage.readStringSettingsfromLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.Server"), out StringValue);
            this.m_eMailing.Server = StringValue;


            bok = m_localStorage.readDoubleSettingsfromLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.Port"), out doubleValue);
            this.m_eMailing.Port = doubleValue;

            bok = m_localStorage.readStringSettingsfromLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.UserName"), out StringValue);
            this.m_eMailing.UserName = StringValue;


            bok = m_localStorage.readStringSettingsfromLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.Password"), out StringValue);
            this.m_eMailing.Password = StringValue;

            bok = m_localStorage.readBoolSettingsfromLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.SSL"), out boolValue);
            this.m_eMailing.SSL = boolValue;


            bok = m_localStorage.readStringSettingsfromLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.Receipients_00"), out StringValue);
            if ( (StringValue.Length > 0) && (this.m_eMailing.Receipients.Count > 0))
                this.m_eMailing.Receipients[0] =  StringValue;

            bok = m_localStorage.readStringSettingsfromLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.Receipients_01"), out StringValue);
            if ((StringValue.Length > 0) && (this.m_eMailing.Receipients.Count > 1))
                this.m_eMailing.Receipients[1] = StringValue;

            bok = m_localStorage.readStringSettingsfromLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.Receipients_02"), out StringValue);
            if ((StringValue.Length > 0) && (this.m_eMailing.Receipients.Count > 2))
                this.m_eMailing.Receipients[2] = StringValue;

            bok = m_localStorage.readStringSettingsfromLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.Receipients_03"), out StringValue);
            if ((StringValue.Length > 0) && (this.m_eMailing.Receipients.Count > 3))
                this.m_eMailing.Receipients[3] = StringValue;




            return true;
        }

        public bool writeMailingDatatoLocalStorage()
        {


            m_LocalStorageSettings.SetSourceIDName("eMailing");

            m_LocalStorageSettings.deleteCompositeValue(); // vor jedem Schreiben alles löschen

            Windows.Storage.ApplicationDataCompositeValue composite = m_LocalStorageSettings.getCompositeValue();


            bool bok;


            bok = m_localStorage.writeStringSettingsToLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.Server"), this.m_eMailing.Server);
      


            bok = m_localStorage.writeDoubleSettingsToLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.Port"), this.m_eMailing.Port);


            bok = m_localStorage.writeStringSettingsToLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.UserName"), this.m_eMailing.UserName);

            bok = m_localStorage.writeStringSettingsToLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.Password"), this.m_eMailing.Password);

            bok = m_localStorage.writeBoolSettingsToLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.SSL"), this.m_eMailing.SSL);


            if (this.m_eMailing.Receipients.Count > 0)
                bok = m_localStorage.writeStringSettingsToLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.Receipients_00"), this.m_eMailing.Receipients[0]);

            if (this.m_eMailing.Receipients.Count>1)
             bok = m_localStorage.writeStringSettingsToLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.Receipients_01"), this.m_eMailing.Receipients[1]);

            if (this.m_eMailing.Receipients.Count > 2)
                bok = m_localStorage.writeStringSettingsToLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.Receipients_02"), this.m_eMailing.Receipients[2]);

            if (this.m_eMailing.Receipients.Count > 3)
                bok = m_localStorage.writeStringSettingsToLocalStorage(composite, m_LocalStorageSettings.getCompositePropertyIDName("m_eMailing.Receipients_03"), this.m_eMailing.Receipients[3]);


            m_LocalStorageSettings.writeCompositeValuetoLocalStorage();

            return true;
        }

        public bool writeDatatoLocalStorage()
        {


            writeListenerDatatoLocalStorage();

            this.writeMailingDatatoLocalStorage();

            return true;
        }

        public bool readDatafromLocalStorage()
        {

            readListenerDatafromLocalStorage();

            this.readMailingDatafromLocalStorage();

            return true;
   
        }


    }
}
