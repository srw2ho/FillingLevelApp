using System;

using FillingLevelApp.Services;

using Windows.ApplicationModel.Activation;
using Windows.UI.Xaml;

using StationLib;
using Windows.ApplicationModel.Core;
using Windows.UI.Core;
using FillingLevelApp.Helpers;
using Windows.UI.Popups;
using Windows.System.Threading;
using EmailSupport;
namespace FillingLevelApp
{
    public sealed partial class App : Application
    {
        private Lazy<ActivationService> _activationService;
        int m_displayRequestCnt;
        Windows.System.Display.DisplayRequest m_displayRequest;
		StationLib.StationEnvironment m_Environment;
        EmailSupport.eMailing m_eMailing;

        SettingsToStorage m_SettingsToStorage;

        private ActivationService ActivationService
        {
            get { return _activationService.Value; }
        }


        public SettingsToStorage SettingsToStorage
        {
            get { return m_SettingsToStorage; }
        }

        public StationLib.StationEnvironment Environment
        {
            get { return m_Environment; }
        }
        public EmailSupport.eMailing eMailing
        {
            get { return m_eMailing; }
        }

        public App()
        {
            InitializeComponent();

            m_Environment = new StationEnvironment();

            m_eMailing = new EmailSupport.eMailing();
 


   
            m_displayRequest = new Windows.System.Display.DisplayRequest();
            m_displayRequestCnt = 0;
            // Deferred execution until used. Check https://msdn.microsoft.com/library/dd642331(v=vs.110).aspx for further info on Lazy<T> class.
            _activationService = new Lazy<ActivationService>(CreateActivationService);

            m_SettingsToStorage = new SettingsToStorage(this);




            this.Suspending += App_Suspending;
        }

        private void App_Suspending(object sender, Windows.ApplicationModel.SuspendingEventArgs e)
        {
            var deferral = e.SuspendingOperation.GetDeferral();
            ReleaseAllRequestedDisplay();

            if (m_Environment != null)
            {
                m_SettingsToStorage.writeDatatoLocalStorage(); // alle Daten Speichern
                //		m_OpenCVEnvironment->getDataFaceReaderWriter()->writeDataFaces();
                //		m_OpenCVEnvironment->getDataFaces()->deleteDataFaces();
                m_Environment.DeInitialization();


            }




            deferral.Complete();
        }

        ~App()
        {
            if (m_Environment != null)
            {
                //	m_OnVifCameraViewModel->writeDatatoLocalStorage();
                m_Environment.DeInitialization();

            }

         }
        private  void InitEnvironment()
        {

            m_SettingsToStorage.readDatafromLocalStorage();
            
            // Callback and Eventing
            // Create a AppShell to act as the navigation context and navigate to the first page
            m_Environment.StationSocketListeners.Failed += SocketListener_Failed;
            m_Environment.StationSocketListeners.startStreaming += SocketListener_startStreaming;
            m_Environment.StationSocketListeners.stopStreaming += SocketListener_stopStreaming;

            m_Environment.StationSocketListeners.NotificationLevel += StationSocketListeners_NotificationLevel;
            m_Environment.Initialization();




        }

        private void StationSocketListeners_NotificationLevel(object sender, int args)
        {
            StationLib.StationSocketListener SocketListener = sender as StationLib.StationSocketListener;


            if (SocketListener != null)
            {// email to
   
                notificationEmail(SocketListener, args);
            }

        }

        private  void SocketListener_Failed(object sender, string args)
        { // Failed after Trying Connection

            StationLib.StationSocketListener SocketListener = sender as StationLib.StationSocketListener;
            if (SocketListener.FailedConnectionCount <= 1)
            {
                ShowMessage(args);
                notificationEmail(SocketListener, args);
            }


            if (SocketListener!=null)
            {
                ThreadPoolTimer timer = ThreadPoolTimer.CreateTimer((t) =>
                {
                    SocketListener.stopProcessingPackagesAsync();// Restart Processing Packages
                    SocketListener.startProcessingPackagesAsync();
                    //do some work \ dispatch to UI thread as needed
                }, TimeSpan.FromSeconds(5));
            }

            
        }
        private  void SocketListener_stopStreaming(object sender, string args)
        {
            StationLib.StationSocketListener SocketListener = sender as StationLib.StationSocketListener;
            if (args.Length>0)
            {

                if (SocketListener != null)
                {
                    ThreadPoolTimer timer = ThreadPoolTimer.CreateTimer((t) =>
                    {
                        SocketListener.stopProcessingPackagesAsync();// Restart Processing Packages
                        SocketListener.startProcessingPackagesAsync();
                        //do some work \ dispatch to UI thread as needed
                    }, TimeSpan.FromSeconds(20));
                }


                if (SocketListener.FailedConnectionCount <= 1)
                {
                    ShowMessage(args);
                    notificationEmail(SocketListener, args);
                }



            }

            ReleaseDisplay(SocketListener);

        }
        private void SocketListener_startStreaming(object sender, Windows.Networking.Sockets.StreamSocket args)
        {
            StationLib.StationSocketListener SocketListener = sender as StationLib.StationSocketListener;
            ActivateDisplay(SocketListener);
        }
        protected override async void OnLaunched(LaunchActivatedEventArgs args)
        {
            InitEnvironment();
            if (!args.PrelaunchActivated)
            {
                await ActivationService.ActivateAsync(args);
            }
        }

        protected override async void OnActivated(IActivatedEventArgs args)
        {
            await ActivationService.ActivateAsync(args);
        }


        private ActivationService CreateActivationService()
        {
            return new ActivationService(this, typeof(Views.MainPage), new Lazy<UIElement>(CreateShell));
        }

        private UIElement CreateShell()
        {
            return new Views.ShellPage(this);
        }


        private async void notificationEmail(StationLib.StationSocketListener SocketListener, string message)
        {
            await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, async () =>
            { // your code should be here
              // FailedConnection in UI-Thread 	NotifyPropertyChanged("FailedConnectionCount"); setzen , sonst Absturz

                string subject = String.Format("FillingLevelApp: {0} Date: {1} Connection Error", SocketListener.VisibleKeyName, SocketListener.StationSensors.ActSensorValue.ActSensorPackageDateTime);

                await m_eMailing.SendMailAsync(subject, message);


            });
        }

        private async void notificationEmail(StationLib.StationSocketListener SocketListener,  int message)
        {
            await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, async () =>
            { // your code should be here
                string errorLevel = "";
            
                string body="";
                switch (message)
                {
                    case 0:
                        {
                            body = String.Format("Level Min overstepped {0} > : {1} ", SocketListener.StationSensors.ActSensorValue.FillingLevel, SocketListener.StationSensors.FillingLevelMin);
                            errorLevel = "warning";
                            break;
                        }
                    case 1:
                        {
                            body = String.Format("Pump dry protection understepped {0} < : {1} ", SocketListener.StationSensors.ActSensorValue.FillingLevel, SocketListener.StationSensors.FillingLevelMin);
                            errorLevel = "error";
                            break;
                        }
                    case 2:
                        {
                            body = String.Format("Well Level Min overstepped {0} > : {1} ", SocketListener.StationSensors.ActSensorValue.FillingLevel, SocketListener.StationSensors.WellFillingLevelMin);
                            errorLevel = "info";
                            break;
                        }
                    case 3:
                        {
                            body = String.Format("Water Inlet Level understepped {0} < : {1} ", SocketListener.StationSensors.ActSensorValue.FillingLevel, SocketListener.StationSensors.WellFillingLevelMin);
                            errorLevel = "info";
                            break;
                        }
                    case 4:
                        {
                            body = String.Format("Well Level Max overstepped {0} > : {1} ", SocketListener.StationSensors.ActSensorValue.FillingLevel, SocketListener.StationSensors.WellFillingLevelMax);
                            errorLevel = "info";
                            break;
                        }
                    case 5:
                        {
                            body = String.Format("Well Level Max understepped {0} < : {1} ", SocketListener.StationSensors.ActSensorValue.FillingLevel, SocketListener.StationSensors.WellFillingLevelMax);
                            errorLevel = "info";
                            break;
                        }
                    case 6:// Water Inlet Filling Time Over
                        {
                            body = String.Format("Water Inlet Time Out Monitoring occured:{1} -> Restart Recording ", SocketListener.StationSensors.ActSensorValue.FillingLevel, SocketListener.InletMaxActiveTime);

                            errorLevel = "error";
                            ShowMessage(body);
                            break;
                        }
                    case 100: // Shutdown: Filling Level Station
                        {
                            body = String.Format("Filling Level Station: Shutdown");
                            errorLevel = "error";
                            break;
                        }
                    case 101:// Restart: Filling Level Station
                        {
                            body = String.Format("Filling Level Station: Restart");
                            errorLevel = "error";
                            break;
                        }

                    default:
                        {
                            body = String.Format("no Level Min Assoziated {0}  ", SocketListener.StationSensors.ActSensorValue.FillingLevel);
                            break;

                        }

                }

                string subject = String.Format("FillingLevelApp: {0} Date: {1} Notifation : {2}", SocketListener.VisibleKeyName, SocketListener.StationSensors.ActSensorValue.ActSensorPackageDateTime, errorLevel);


 
                await m_eMailing.SendMailAsync(subject, body);




            });
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

        private async void ActivateDisplay(StationLib.StationSocketListener SocketListener)
        {

            await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal,() =>
            { // your code should be here
                SocketListener.FailedConnectionCount = SocketListener.FailedConnectionCount;
                m_displayRequest.RequestActive();
                m_displayRequestCnt++;
            });


        }
        private async void ReleaseDisplay(StationLib.StationSocketListener SocketListener)
        {

            await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
            { // your code should be here
                SocketListener.FailedConnectionCount = SocketListener.FailedConnectionCount;

                if (m_displayRequest == null)
                    return;

                if (SocketListener.FailedConnectionCount <= 1) // noch keinen FailedCount
                {
                    if (m_displayRequestCnt > 0)
                    {
                        //undo the request
                        m_displayRequest.RequestRelease();
                        m_displayRequestCnt--;
                    }
                }

            });
 
        }

        private async void ReleaseAllRequestedDisplay()
        {

            await CoreApplication.MainView.CoreWindow.Dispatcher.RunAsync(CoreDispatcherPriority.Normal, () =>
            { // your code should be here
              //must be same instance, so quit if it doesn't exist
                if (m_displayRequest == null)
                    return;


                while (m_displayRequestCnt > 0)
                {
                    //undo the request
                    m_displayRequest.RequestRelease();
                    m_displayRequestCnt--;
                }
            });
        }
    }
}
