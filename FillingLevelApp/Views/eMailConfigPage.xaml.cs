using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;
using EmailSupport;
// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238
using FillingLevelApp.Helpers;


using Telerik.Data.Core;

namespace FillingLevelApp.Views
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    ///


    public sealed partial class eMailConfigPage : Page
    {
        EmailSupport.eMailing m_eMailing;
        SettingsToStorage m_SettingsToStorage;


        public eMailConfigPage()
        {
            m_SettingsToStorage = null;
            m_eMailing = null;
            this.InitializeComponent();
        }




        public EmailSupport.eMailing eMailing
        {
            get { return m_eMailing; }
            set { m_eMailing = value; }
        }

        protected override void OnNavigatedTo(NavigationEventArgs e)
        {

            if (e.Parameter is FillingLevelApp.App)
            {

                FillingLevelApp.App AppEnvironment = e.Parameter as FillingLevelApp.App;

                if (AppEnvironment != null)
                {

                    m_eMailing = AppEnvironment.eMailing;
                    m_SettingsToStorage = AppEnvironment.SettingsToStorage;
                    if (m_eMailing != null)
                    {
                    }

                }



            }

            base.OnNavigatedTo(e);
        }

        protected override void OnNavigatingFrom(NavigatingCancelEventArgs e)
        {
            //       this.stopProcessing();

            if (m_SettingsToStorage != null)
            {
                m_SettingsToStorage.writeDatatoLocalStorage();
            }
            base.OnNavigatingFrom(e);

        }

        async void Testmail_Click(Object sender, Windows.UI.Xaml.RoutedEventArgs e)
        {

            string body = string.Format("<< Configuration >> Server: {0} Port: {1} SSL: {2} User: {3}", m_eMailing.Server, m_eMailing.Port,m_eMailing.SSL, m_eMailing.UserName);

            var ret = await m_eMailing.SendMailAsync("Testmail from FillingLevelApp", body);
            if (ret)
            {

            }

        }


    }
}
