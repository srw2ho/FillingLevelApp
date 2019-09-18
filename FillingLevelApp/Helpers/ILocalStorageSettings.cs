﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FillingLevelApp.Helpers
{
    interface ILocalStorageSettings
    {

        string getCompositePropertyIDName(string Property);
        bool writeCompositeValuetoLocalStorage();
        Windows.Storage.ApplicationDataCompositeValue getCompositeValue();
        Windows.Storage.ApplicationDataCompositeValue deleteCompositeValue();
    }

    interface ILocalStorageItem
    {

        bool writeSettingsToLocalStorage(Windows.Storage.ApplicationDataCompositeValue composite, int Idx);
        bool readSettingsfromLocalStorage(Windows.Storage.ApplicationDataCompositeValue composite, int Idx);
    }


    
    
}
