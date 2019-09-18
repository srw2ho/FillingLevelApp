using System;
using System.ComponentModel;
using System.Runtime.CompilerServices;

using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Navigation;

using StationLib;
using Windows.UI.WebUI;
using System.Collections.Generic;
using FillingLevelApp.Models;
using System.Collections.ObjectModel;
namespace FillingLevelApp.Views
{
    public sealed partial class MainPage : Page, INotifyPropertyChanged
    {
        private  StationLib.StationEnvironment m_Environment;

        private StationLib.SocketStationListeners  m_StationListeners;

        private IDictionary<StationLib.StationSensors, FrameToPageType> m_SensorToFrame;

        private ObservableCollection<SensorViewType> m_SensorViewType;

        private bool m_WaterInletActivate;
        private bool m_PumpDryProtectionActive;

        private bool m_WaterInletActivateEnable;
        private bool m_PumpDryProtectionActiveEnable;



        public MainPage()
        {
            InitializeComponent();
            m_SensorToFrame = new Dictionary<StationLib.StationSensors, FrameToPageType>();
            m_SensorViewType = new ObservableCollection<SensorViewType>();
            m_SensorViewType.Add(new SensorViewType("SensorGauche", typeof(ViewGaugeSensorPage)));
            m_SensorViewType.Add(new SensorViewType("SensorList", typeof(ViewSensorPage)));
            m_WaterInletActivate = false;
            m_PumpDryProtectionActive = false;
            m_PumpDryProtectionActiveEnable = false;
            m_WaterInletActivateEnable = false;


        }

        protected override void OnNavigatingFrom(NavigatingCancelEventArgs e)
        {

            for (int i = 0; i < StationListeners.SocketListeners.Count; i++)
            {
                StationLib.StationSocketListener plistener = m_StationListeners.SocketListeners[i];
                plistener.Update -= Plistener_Update;

            }
            base.OnNavigatingFrom(e);
       


        }

        public bool PumpDryProtectionActive
        {
            get
            {
                return m_PumpDryProtectionActive;
            }
            set
            {
                
                StationLib.StationSocketListener plistener = m_StationListeners.getSelectedItem();
                if (plistener != null)
                {
                    if (m_PumpDryProtectionActive != value)
                    {
                        plistener.PumpDryProtectionActive = value;
                        m_PumpDryProtectionActive = value;
                        OnPropertyChanged("PumpDryProtectionActive");
                    }
   
                }

            }
        }

        public bool WaterInletActivate
        {
            get {
                return m_WaterInletActivate;
            }
            set {
                StationLib.StationSocketListener plistener = m_StationListeners.getSelectedItem();
                if (plistener != null)
                {
                    if (m_WaterInletActivate != value)
                    {
                        plistener.WaterInletActive = value;
                        m_WaterInletActivate = value;
                        OnPropertyChanged("WaterInletActivate");
                    }

                }

            }

            
        }

        public bool WaterInletActivateEnable
        {
            get
            {
                return m_WaterInletActivateEnable;
            }
            set
            {
                if (m_WaterInletActivateEnable != value)
                {
                    m_WaterInletActivateEnable = value;
                    OnPropertyChanged("WaterInletActivateEnable");
                }

            }

        }

        public bool PumpDryProtectionActiveEnable
        {
            get
            {
                return m_PumpDryProtectionActiveEnable;
            }
            set
            {
                if (m_PumpDryProtectionActiveEnable != value)
                {
                    m_PumpDryProtectionActiveEnable = value;
                    OnPropertyChanged("PumpDryProtectionActiveEnable");
                }

            }

        }




        public ObservableCollection<SensorViewType> SensorViewType
        {
            get { return m_SensorViewType; }

        }


        public StationLib.StationSensors SelectedSensorValues
        {
            get {

                StationLib.StationSocketListener plistener = m_StationListeners.getSelectedItem();
                if (plistener != null)
                {
                    return  plistener.StationSensors;
                }

                return null;
            }

        }

        public StationLib.SocketStationListeners StationListeners  {
            get { return m_StationListeners; }
            set { m_StationListeners = value; }
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {
   
            if (e.Parameter is FillingLevelApp.App)
            {

                FillingLevelApp.App AppEnvironment = e.Parameter as FillingLevelApp.App;

 
                if (AppEnvironment != null)
                {
                    m_Environment = AppEnvironment.Environment;

                    this.StationListeners = m_Environment.getWeatherStationListeners();


                    for ( int i = 0; i < StationListeners.SocketListeners.Count; i++)
                    {
                        StationLib.StationSocketListener plistener = m_StationListeners.SocketListeners[i];
                        plistener.MainFrame = this.Frame;

                        plistener.Update += Plistener_Update;

                    }

                    if (m_StationListeners.SelectedIndex == -1) m_StationListeners.SelectedIndex = 0;

                }

          
   
                this.startProcessing();


            }

            base.OnNavigatedTo(e);
        }

        private void UpdateListener(StationLib.StationSocketListener updateplistener)
        {

            if (updateplistener != null)
            {

                StationLib.StationSocketListener plistener = m_StationListeners.getSelectedItem();
                if (plistener == updateplistener)
                {
                    if (updateplistener.WaterInletActive != m_WaterInletActivate)
                    {
                        this.m_WaterInletActivate = updateplistener.WaterInletActive;
                        OnPropertyChanged("WaterInletActivate");
                    }

                    if (updateplistener.PumpDryProtectionActive != m_PumpDryProtectionActive)
                    {
                        this.m_PumpDryProtectionActive = updateplistener.PumpDryProtectionActive;
                        OnPropertyChanged("PumpDryProtectionActive");
                    }

                    WaterInletActivateEnable = !updateplistener.ListenerMode && updateplistener.OutPutWellFillingLevelMinActiv;// && !updateplistener.WaterInletState; ;


                    PumpDryProtectionActiveEnable = !updateplistener.ListenerMode && updateplistener.OutPutFillingLevelMinActiv;// && !updateplistener.PumpDryProtectionState;
                }

            }
        }


        private void Plistener_Update(object sender, ulong args)
        {
            StationLib.StationSocketListener plistener = sender as StationLib.StationSocketListener;
            if (plistener != null)
            {
                UpdateListener(plistener);
            }
        }

        public void StationListenersSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            Pivot pivot = sender as Pivot;

            if (pivot != null)
            {
                SelectSensorView();
            }

        }

        void stopProcessing()
        {

            if (this.m_StationListeners == null) return;

            this.m_StationListeners.DeInitialization(); // Processing stoppen

            


     
        }
        void startProcessing()
        {
            if (this.m_StationListeners == null) return;

            this.m_StationListeners.Initialization(); // Processing starten



        }

        async void startRecording_Click(Object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {

            StationLib.StationSocketListener plistener = m_StationListeners.getSelectedItem();
            if (plistener != null)
            {
               await plistener.startProcessingPackagesAsync();
            }


        }
        async void stopRecording_Click(Object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            StationLib.StationSocketListener plistener = m_StationListeners.getSelectedItem();
            if (plistener != null)
            {
                await plistener.stopProcessingPackagesAsync();
            }


        }

        async void RestartRecording_Click(Object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            StationLib.StationSocketListener plistener = m_StationListeners.getSelectedItem();
            if (plistener != null)
            {
                await plistener.stopProcessingPackagesAsync();
                await plistener.startProcessingPackagesAsync();
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

        private void SensorViewFrame_Loaded(object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {
            Frame frame = sender as Frame;
            if (frame!=null)
            {
                StationLib.StationSensors sensorValues =frame.DataContext as StationLib.StationSensors;
                if (sensorValues!=null)
                {
                    SensorViewType viewType = (SensorViewType)SensorViewCombo.SelectedItem;
                    Type pageType = null; // erste mal immer setzen
                    FrameToPageType frameToPageType = new FrameToPageType(frame, pageType);

                    m_SensorToFrame.Add(sensorValues, frameToPageType);
                    SelectSensorView();

                }


            }
            //   this.SensorViewFrame.Navigate(typeof(ViewSensorPage), sensorValues);
        }

        public void SelectSensorView()
        {


            SensorViewType viewType = (SensorViewType) SensorViewCombo.SelectedItem;
            if (viewType == null) return;

            StationLib.StationSocketListener plistener = m_StationListeners.getSelectedItem();
            if (plistener != null)
            {
                StationLib.StationSensors sensorValoues = plistener.StationSensors;
                //  Frame frame = null;
                FrameToPageType frameToPageType;

                m_SensorToFrame.TryGetValue(sensorValoues, out frameToPageType);
                if (frameToPageType != null)
                {
                    // Check if active Type !=
                    if (frameToPageType.PageType!= viewType.PageType)
                    {
                        frameToPageType.PageFrame.Navigate(viewType.PageType, sensorValoues);
                        frameToPageType.PageType = viewType.PageType;
                        OnPropertyChanged("SelectedSensorValues");
                    }

                 }

             }

         

        }

        private void SensorViewType_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            ComboBox combo = sender as ComboBox;
            if (combo!=null)
            {
                SelectSensorView();

            }
        }
    }
}
