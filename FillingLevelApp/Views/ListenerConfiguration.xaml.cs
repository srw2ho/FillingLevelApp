using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using FillingLevelApp.Helpers;
using StationLib;
using Windows.ApplicationModel.Core;
using Windows.Devices.Geolocation;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Core;
using Windows.UI.Popups;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace FillingLevelApp.Views
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class ListenerConfiguration : Page
    {
        private StationLib.StationEnvironment m_Environment;
        private StationLib.SocketStationListeners m_SocketStationListeners;


        private  List<int> m_RecordingHours = new List<int> { 3, 6, 9, 12, 24 };

        SettingsToStorage m_SettingsToStorage;

        public ListenerConfiguration()
        {
            m_SocketStationListeners = null;
            m_Environment = null;
            m_SettingsToStorage = null;

            this.InitializeComponent();
        }

        public IList<int> RecordingHours
        {
            get { return m_RecordingHours; }
  
        }

        public StationLib.SocketStationListeners StationListeners
        {
            get { return m_SocketStationListeners; }
            set { m_SocketStationListeners = value; }
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
                        m_SettingsToStorage = AppEnvironment.SettingsToStorage;

                        this.m_SocketStationListeners = m_Environment.StationSocketListeners;
                        // für alle Listeners Processing stoppen
                        this.m_SocketStationListeners.DeInitialization();

                        if (m_SocketStationListeners.SelectedIndex == -1) m_SocketStationListeners.SelectedIndex = 0;

             

                    }

                }
          


            }

            base.OnNavigatedTo(e);
        }


        protected override void OnNavigatingFrom(NavigatingCancelEventArgs e)
        {
            //       this.stopProcessing();

            if (m_SettingsToStorage!=null)
            {
                m_SettingsToStorage.writeDatatoLocalStorage();
            }
            base.OnNavigatingFrom(e);

        }

 


        public void ListenerSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            GridView gridView = sender as GridView;

            if (gridView != null)
            {
                if (gridView.ItemsSource == this.StationListeners.SocketListeners)
                {
                    StationLib.StationSocketListener  listener = StationListeners.getSelectedItem();
                    if (listener != null)
                    {

                    }

                }

            }

        }


        public void setListenerToDefaultValues(StationLib.StationSocketListener listener)
        {

    

            listener.HostName = "localhost";
            listener.Port = 3005;
            listener.ViewName = "Cysterne";

            listener.GPIOHCSR04EchoPin = 12;
            listener.GPIOHCSR04TriggPin = 23;
            listener.BME280SensorId = 0x76;
            listener.OutPutLedPin = 21;

            listener.OutPutLevel1Pin = 17;

            listener.OutPutLevel2Pin = 27;

            listener.ShutDownPin = 24;

            listener.MaxLevelMillimeterProSec = 3;

            listener.eMailWellFillingLevelMinActiv = false;

            listener.eMailWellFillingLevelMaxActiv = false;
            listener.eMailFillingLevelMinActiv = false;
            listener.OutPutWellFillingLevelMinActiv = false;
            listener.OutPutFillingLevelMinActiv = false;

            listener.ListenerMode = false;

            listener.OutPutLevel1PinActivValue = 0; // Ausgang ist low aktiv

            listener.OutPutLevel2PinActivValue = 0; // Ausgang ist low aktiv

            listener.InletFillLevelInPercent = 2;

            listener.InletMaxActiveTime = 1;
            listener.InletMaxCycleFlushingActiveTime = 1;

            listener.InletCycleFlushingTime = 2; // alle 2 Tage


            StationSensors sensorValue = listener.StationSensors;
            sensorValue.DevId = 118;
            sensorValue.SensorViewName = "";

            sensorValue.RecordingHours = 24;
            sensorValue.RecordingSamplePerHour = 50;

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

            sensorValue.SensorDistanceMin = 20; // in cm
            sensorValue.SensorDistanceMax = 180;
            sensorValue.CysternDiameter = 230; // Cysterne Durchmesser in cm
            sensorValue.CysternVolumeProCm = 0;



        }

        public void ButtonAddListener(object sender, RoutedEventArgs e)
        {
            if (this.StationListeners == null) return;
            StationLib.StationSocketListener listener = new StationSocketListener();

            setListenerToDefaultValues(listener);

            this.StationListeners.AddListener(listener);

        }

        public void ButtonEditListener(object sender, RoutedEventArgs e)
        {
       //     this.CameraViewModel.editDevice(this.devideIPAdress.Text, this.deviceUser.Text, this.devicePassword.Password);
        }


        public void ButtonDeleteListener(object sender, RoutedEventArgs e)
        {

            StationLib.StationSocketListener listener = StationListeners.getSelectedItem();
            if (listener!=null)
            {

                StationListeners.deleteListener(listener);
            }


        }


        public void setToDefault(object sender, RoutedEventArgs e)
        {

            StationLib.StationSocketListener listener = StationListeners.getSelectedItem();
            if (listener != null)
            {
                setListenerToDefaultValues(listener);
            }


        }


        private void ListenerGridViewTapped(object sender, TappedRoutedEventArgs e) // selection for GridView
        {
            GridView gridView = sender as GridView;

            if (gridView != null)
            {
                if (gridView.ItemsSource == this.StationListeners.SocketListeners)
                {
             //       StationSocketListener Listener = gridView.ItemsSource as StationSocketListener;
                    StationLib.StationSocketListener listener = StationListeners.getSelectedItem();

                    if (listener != null && (gridView.SelectedItem != listener)) // already selected
                    {
                        gridView.SelectedItem = listener;
                    }
                }



            }

        }


 

        private async void ShowMessage(string message)
        {
            await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, async () =>
            { // your code should be here
              // FailedConnection in UI-Thread 	NotifyPropertyChanged("FailedConnectionCount"); setzen , sonst Absturz

                //     if (message.Length>0)
                {
                    var messageDialog = new MessageDialog(message);
                    await messageDialog.ShowAsync();
                }


            });
        }


        private void OnKeyUpHandler(object sender, KeyRoutedEventArgs e)
        {
            TextBox TeBox = sender as TextBox;
            if (TeBox != null)
            {

                if (e.Key == Windows.System.VirtualKey.Enter)
                {
                    FocusManager.TryMoveFocus(FocusNavigationDirection.Next);
                    e.Handled = true;
                }
            }
            base.OnKeyUp(e);
        }

    }
}
