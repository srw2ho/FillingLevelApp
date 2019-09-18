using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using Telerik.UI.Xaml.Controls.Chart;
using StationLib;
using Windows.Foundation.Collections;
using Windows.UI.Xaml.Controls;
using System.ComponentModel;

namespace FillingLevelApp.Models
{
    // TODO WTS: This is used by the Sample Chart Data. Remove this once your chart page is displaying real data
    public class DataPoint
    {
        public double Value { get; set; }

        public string Category { get; set; }
    }


    public  class SensorMapToScatterLine
    {
        LineSeries m_LineSeries;
        IObservableVector<StationSensor> m_Sensors;

     

        //public SensorMapToScatterLine(ScatterLineSeries lineSeries, IObservableVector<StationSensor> sensors )
        public SensorMapToScatterLine(LineSeries lineSeries, IObservableVector<StationSensor> sensors)
        {
            //m_ScatterLineSeries = lineSeries;
            m_LineSeries = lineSeries;
            m_Sensors = sensors;
        }

        public IObservableVector<StationSensor>  Sensors
        {
            get { return m_Sensors; }
        }
        

        public LineSeries LineSeries
        {
            get { return m_LineSeries; }
        }
    }

    public sealed class SensorViewType
    {
        string m_Name;

        Type m_PageType;


        public SensorViewType(string Name, Type Type)
        {
            //m_ScatterLineSeries = lineSeries;
            m_Name = Name;
            m_PageType = Type;
        }

        public string Name
        {
            get { return m_Name; }
        }


        public Type PageType
        {
            get { return m_PageType; }
        }
    }

    public sealed class FrameToPageType
    {
           Type m_PageType;
           Frame m_PageFrame;

        public FrameToPageType(Frame frame, Type Type)
        {
            //m_ScatterLineSeries = lineSeries;
            m_PageFrame = frame;
            m_PageType = Type;
        }

        public Frame PageFrame
        {
            get { return m_PageFrame; }
            set { m_PageFrame = value; }
        }


        public Type PageType
        {
            get { return m_PageType; }
            set { m_PageType = value; }
        }
    }


    public  class ChartViewType : INotifyPropertyChanged
    {
        string m_Name;

        string m_ViewPropertyName;

        double m_ActValue;

        public event PropertyChangedEventHandler PropertyChanged;

        public ChartViewType(string Name, string PropertyName)
        {
            //m_ScatterLineSeries = lineSeries;
            m_Name = Name;
            m_ViewPropertyName = PropertyName;
            m_ActValue = 0;
        }

   
        public string ActValueasString
        {
            get {
                string body = string.Format(" {0:00.00}", m_ActValue);
                return body;
            }

        }


        public double ActValue
        {
            get { return m_ActValue; }
            set {
                m_ActValue = value;
                OnPropertyChanged("ActValue");
                OnPropertyChanged("ActValueasString");
            }

        }


        public string Name
        {
            get { return m_Name; }
        }


        public string ViewPropertyName
        {
            get {
                return m_ViewPropertyName;
            }
        }
        // Create the OnPropertyChanged method to raise the event
        protected void OnPropertyChanged(string name)
        {
            PropertyChangedEventHandler handler = PropertyChanged;
            if (handler != null)
            {
                handler(this, new PropertyChangedEventArgs(name));
            }
        }

    }


}
