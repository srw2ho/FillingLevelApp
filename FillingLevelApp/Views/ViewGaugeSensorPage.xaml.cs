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

// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=234238

namespace FillingLevelApp.Views
{
    /// <summary>
    /// An empty page that can be used on its own or navigated to within a Frame.
    /// </summary>
    public sealed partial class ViewGaugeSensorPage : Page
    {
        private StationLib.StationSensors m_SensorValues;

        public ViewGaugeSensorPage()
        {
            this.InitializeComponent();
        }

        protected override void OnNavigatingFrom(NavigatingCancelEventArgs e)
        {


            base.OnNavigatingFrom(e);

        }

        public StationLib.StationSensors SensorsValues
        {
            get { return m_SensorValues; }

        }


        protected override void OnNavigatedTo(NavigationEventArgs e)
        {

            if (e.Parameter is StationLib.StationSensors)
            {
                m_SensorValues = e.Parameter as StationLib.StationSensors;
         //       if (m_SensorValues.SelectedIndex==-1)  m_SensorValues.SelectedIndex = 0;
            }

            base.OnNavigatedTo(e);
        }
    }
}
