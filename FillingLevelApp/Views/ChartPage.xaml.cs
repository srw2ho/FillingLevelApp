using System;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Runtime.CompilerServices;

using FillingLevelApp.Models;
//using FillingLevelApp.Core.Services;

using Windows.UI.Xaml.Controls;

using Windows.UI.Xaml.Navigation;
using StationLib;
using System.Collections.Generic;
using Telerik.UI.Xaml.Controls.Chart;
using Windows.System.Threading;
using Windows.UI.Core;
using Windows.ApplicationModel.Core;
using Windows.Foundation.Collections;
using Telerik.Charting;
using FillingLevelApp.Helpers;
namespace FillingLevelApp.Views
{
    public sealed partial class ChartPage : Page, INotifyPropertyChanged
    {
        // TODO WTS: Change the chart as appropriate to your app.
        // For help see http://docs.telerik.com/windows-universal/controls/radchart/getting-started
        private StationLib.StationEnvironment m_Environment;

        private StationLib.SocketStationListeners m_SocketStationListeners;
        
        private IList<Models.SensorMapToScatterLine> m_SensorMaps;



    //    private IDictionary<StationLib.StationSensorsValues, FrameToPageType> m_SensorToFrame;

        private ObservableCollection<ChartViewType> m_ChartViewType;
        public ChartPage()
        {
            InitializeComponent();
            m_SensorMaps = new  List< Models.SensorMapToScatterLine >  () ;
            m_ChartViewType = new ObservableCollection<ChartViewType>();
            m_ChartViewType.Add(new ChartViewType("FillingLevel", "FillingLevel"));
            m_ChartViewType.Add(new ChartViewType("FillingVolume", "FillingVolume"));
            m_ChartViewType.Add(new ChartViewType("HCSR04MeasTime", "HCSR04MeasTime"));
            m_ChartViewType.Add(new ChartViewType("HCSR04MeasTimeDistance", "HCSR04MeasTimeDistance"));
            m_ChartViewType.Add(new ChartViewType("Distance", "Distance"));
            
            m_ChartViewType.Add(new ChartViewType("Temperature", "Temperature"));
            m_ChartViewType.Add(new ChartViewType("Pressure", "Pressure"));
            m_ChartViewType.Add(new ChartViewType("Humidity", "Humidity"));
  

        }

   



        public ObservableCollection<ChartViewType> ChartViewType
        {
            get { return m_ChartViewType; }

        }

        public StationLib.SocketStationListeners StationListeners
        {
            get { return m_SocketStationListeners; }
            set { m_SocketStationListeners = value; }
        }

        void startProcessing()
        {
            if (this.m_SocketStationListeners == null) return;

            this.m_SocketStationListeners.Initialization(); // Processing starten



        }
        protected override void OnNavigatedTo(NavigationEventArgs e)
        {

            if (e.Parameter is FillingLevelApp.App)
            {

                FillingLevelApp.App AppEnvironment = e.Parameter as FillingLevelApp.App;

                if (AppEnvironment != null)
                {


                    m_Environment = AppEnvironment.Environment;

                    if (m_Environment != null)
                    {

                        this.m_SocketStationListeners = m_Environment.StationSocketListeners;

                        for (int i = 0; i < this.m_SocketStationListeners.SocketListeners.Count; i++)
                        {

                            StationLib.StationSocketListener plistener = this.m_SocketStationListeners.SocketListeners[i];
                            plistener.StationSensors.SensorValues.VectorChanged += SensorValues_VectorChanged;
   

                        }

                        if (m_SocketStationListeners.SelectedIndex == -1) m_SocketStationListeners.SelectedIndex = 0;

                    }



                    this.startProcessing();
                }
            }

            base.OnNavigatedTo(e);
        }


        protected override void OnNavigatingFrom(NavigatingCancelEventArgs e)
        {

            if (m_Environment != null)
            {

                for (int i = 0; i < this.m_SocketStationListeners.SocketListeners.Count; i++)
                {
                    StationLib.StationSocketListener plistener = this.m_SocketStationListeners.SocketListeners[i];

                    for (int j = 0; j < plistener.StationSensors.SensorValues.Count; j++)
                    {
                        plistener.StationSensors.SensorValues.VectorChanged -= SensorValues_VectorChanged;

                     }

                }


            }


            base.OnNavigatingFrom(e);

        }
        private void SensorValues_VectorChanged(Windows.Foundation.Collections.IObservableVector<StationSensor> sender, Windows.Foundation.Collections.IVectorChangedEventArgs @event)
        {
            uint idx = @event.Index;
            Windows.Foundation.Collections.IObservableVector<StationSensor> sensors = sender  as Windows.Foundation.Collections.IObservableVector<StationSensor>;
            if (sensors != null)
            {
                updateChartAsync(sensors, @event);
            }

        }

        public event PropertyChangedEventHandler PropertyChanged;

        private void Set<T>(ref T storage, T value, [CallerMemberName]string propertyName = null)
        {
            if (Equals(storage, value))
            {
                return;
            }

            storage = value;
            OnPropertyChanged(propertyName);
        }


        private void OnPropertyChanged(string propertyName) => PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));


        private void insertintoChart(Windows.Foundation.Collections.IObservableVector<StationSensor> SennsorValue, LineSeries lineseries, int Idx)
        {

            if (Idx >= SennsorValue.Count) return;


            StationSensor sensor = SennsorValue[Idx];
            double YValue = 0 ;
            
            Telerik.UI.Xaml.Controls.Chart.PropertyNameDataPointBinding Ybinding = lineseries.ValueBinding as Telerik.UI.Xaml.Controls.Chart.PropertyNameDataPointBinding;
            if (Ybinding != null)
            {
                if (Ybinding.PropertyName == "Temperature") YValue = sensor.Temperature;
                else if (Ybinding.PropertyName == "FillingLevel") YValue = sensor.FillingLevel;
                else if (Ybinding.PropertyName == "FillingVolume") YValue = sensor.FillingVolume;
                else if (Ybinding.PropertyName == "Pressure") YValue = sensor.PressurehPaSeaLevel;
                else if (Ybinding.PropertyName == "HCSR04MeasTime") YValue = sensor.HCSR04MeasTime;
                else if (Ybinding.PropertyName == "HCSR04MeasTimeDistance") YValue = sensor.HCSR04MeasTimeDistance;
                else if (Ybinding.PropertyName == "Distance") YValue = sensor.Distance;
                else if (Ybinding.PropertyName == "Humidity") YValue = sensor.Humidity;
                else YValue = 0;
            }

            ChartViewType viewType = (ChartViewType)ChartViewCombo.SelectedItem;
            if (viewType!=null) viewType.ActValue = YValue;

            DateTime dt = FillingLevelApp.Helpers.DateTimeConverter.UnixTimeStampToUTCDateTime(sensor.UnixTime).ToLocalTime();

            lineseries.DataPoints.Add(new CategoricalDataPoint() { Category = dt, Value = YValue });



        }


        private void updateChartAsync(Windows.Foundation.Collections.IObservableVector<StationSensor> SennsorValue, Windows.Foundation.Collections.IVectorChangedEventArgs @event)
        {

            { // your code should be here
              //must be same instance, so quit if it doesn't exist

                uint idx = @event.Index;

                // StationLib.StationSensorValues SennsorValue;

                for (int i = 0; i < m_SensorMaps.Count; i++)
                {
                    SensorMapToScatterLine map = m_SensorMaps[i];

                    if (map.Sensors == SennsorValue)
                    {
                        CollectionChange ch = @event.CollectionChange;
                        switch (ch)
                        {
                            case CollectionChange.ItemChanged:
                                break;
                            case CollectionChange.ItemInserted:
                                insertintoChart(SennsorValue, map.LineSeries, (int)idx);
                                break;
                            case CollectionChange.ItemRemoved:
                                {
                                    //
                                    if (idx < map.LineSeries.DataPoints.Count)
                                    {
                                        map.LineSeries.DataPoints.RemoveAt((int)idx);
                                    }

                                }
                                break;
                            case CollectionChange.Reset:
                                break;

                        }

                    }
                }

            }



        }


        private void fillChart(IObservableVector<StationSensor> SennsorValue, LineSeries lineseries)
        {
            int nSensorValue = 0;

      
            lineseries.DataPoints.Clear();

            Telerik.UI.Xaml.Controls.Chart.PropertyNameDataPointBinding Ybinding = lineseries.ValueBinding as Telerik.UI.Xaml.Controls.Chart.PropertyNameDataPointBinding;

    
            if (Ybinding != null)
            {
                if (Ybinding.PropertyName == "Temperature") nSensorValue = 0;
                else if (Ybinding.PropertyName == "Pressure") nSensorValue = 1;
                else if (Ybinding.PropertyName == "FillingLevel") nSensorValue = 2;
                else if (Ybinding.PropertyName == "FillingVolume") nSensorValue = 3;
                else if (Ybinding.PropertyName == "HCSR04MeasTime") nSensorValue = 4;
                else if (Ybinding.PropertyName == "HCSR04MeasTimeDistance") nSensorValue = 5;
                else if (Ybinding.PropertyName == "Distance") nSensorValue = 6;
                else if (Ybinding.PropertyName == "Humidity") nSensorValue = 7;
                else nSensorValue = 8;
            }
            

            double YValue;
            for (int i = 0; i < SennsorValue.Count; i++)
            {
                StationSensor sensor = SennsorValue[i];

                if (nSensorValue == 0) YValue = sensor.Temperature;
                else if (nSensorValue == 1) YValue = sensor.PressurehPaSeaLevel;
                else if (nSensorValue == 2) YValue = sensor.FillingLevel;
                else if (nSensorValue == 3) YValue = sensor.FillingVolume;
                else if (nSensorValue == 4) YValue = sensor.HCSR04MeasTime;
                else if (nSensorValue == 5) YValue = sensor.HCSR04MeasTimeDistance;
                else if (nSensorValue == 6) YValue = sensor.Distance;
                else if (nSensorValue == 7) YValue = sensor.Humidity;
                else YValue = 0;

                DateTime dt = FillingLevelApp.Helpers.DateTimeConverter.UnixTimeStampToUTCDateTime(sensor.UnixTime).ToLocalTime();
                //          new CategoricalDataPoint(9, DateTime.Now)
                lineseries.DataPoints.Add(new CategoricalDataPoint() { Category = dt, Value= YValue });
            }


        }

        private void Chart_Loaded(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {

            Telerik.UI.Xaml.Controls.Chart.RadCartesianChart chart = sender as RadCartesianChart;
            if (chart!=null)
            {
                StationLib.StationSocketListener plistener = chart.DataContext as StationLib.StationSocketListener;
                if (plistener != null)
                {
                    StationLib.StationSensors SennsorValue = plistener.StationSensors;
                    if (SennsorValue != null)
                    {
                        for (int i = 0; i < chart.Series.Count; i++)
                        {
                            //   ScatterLineSeries lineseries = chart.Series[i] as ScatterLineSeries;
                            LineSeries lineseries = chart.Series[i] as LineSeries;
                            if (lineseries != null)
                            {

                                Telerik.UI.Xaml.Controls.Chart.PropertyNameDataPointBinding Ybinding = lineseries.ValueBinding as Telerik.UI.Xaml.Controls.Chart.PropertyNameDataPointBinding;
                                ChartViewType viewType = (ChartViewType)ChartViewCombo.SelectedItem;
                                Ybinding.PropertyName = viewType.ViewPropertyName;
                                //m_MapTemperatureChart.Add(SennsorValue.SensorValues, lineseries);
                                fillChart(SennsorValue.SensorValues, lineseries);
                                m_SensorMaps.Add(new SensorMapToScatterLine(lineseries, SennsorValue.SensorValues));
                            }
                        }


                    }
                }


            }



        }


        void DeleteDataSensors_Click(Object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {

            StationLib.StationSocketListener plistener = m_SocketStationListeners.getSelectedItem();
            if (plistener != null)
            {
                plistener.StationSensors.deleteSensorValues();
          

            }


        }

        async void startRecording_Click(Object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {

            StationLib.StationSocketListener plistener = m_SocketStationListeners.getSelectedItem();
            if (plistener != null)
            {
                await plistener.startProcessingPackagesAsync();
            }


        }
        async void stopRecording_Click(Object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            StationLib.StationSocketListener plistener = m_SocketStationListeners.getSelectedItem();
            if (plistener != null)
            {
                await plistener.stopProcessingPackagesAsync();
            }


        }

        public void StationListenersSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Pivot pivot = sender as Pivot;

            if (pivot != null)
            {

            }

        }

        private void ChartView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {

            StationLib.StationSocketListener plistener;

            {
           
                ComboBox combo = sender as ComboBox;

                if (combo == null) return;
                plistener = combo.DataContext as StationLib.StationSocketListener;
                if (plistener != null)
                {
                    for (int i = 0; i < m_SensorMaps.Count; i++)
                    {
                        SensorMapToScatterLine map = m_SensorMaps[i];
                        LineSeries lineseries = map.LineSeries;
                        IObservableVector<StationSensor> SennsorValue = map.Sensors;

                        for (int k = 0; k < plistener.StationSensors.SensorValues.Count;k++)
                        {
                            StationLib.StationSensors sensors = plistener.StationSensors;
                            if (sensors.SensorValues== SennsorValue)
                            {
                                Telerik.UI.Xaml.Controls.Chart.PropertyNameDataPointBinding Ybinding = lineseries.ValueBinding as Telerik.UI.Xaml.Controls.Chart.PropertyNameDataPointBinding;
                                ChartViewType viewType = (ChartViewType)combo.SelectedItem;

                                if (Ybinding.PropertyName!= viewType.ViewPropertyName)
                                {
                                    Ybinding.PropertyName = viewType.ViewPropertyName;
                                    fillChart(SennsorValue, lineseries);
                                }

                                break;

                            }

                        }
                    }
                }


            }
        }
    }
}
