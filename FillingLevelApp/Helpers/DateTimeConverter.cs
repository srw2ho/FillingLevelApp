using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FillingLevelApp.Helpers
{
    class DateTimeConverter
    {
        public static DateTime UnixTimeStampToUTCDateTime(double unixTimeStamp)
        {


            DateTime unixStart = new DateTime(1001, 1, 1, 0, 0, 0, 0, System.DateTimeKind.Utc);// c++ SYSTEMTIME begins with 1001, 1. Januar

            long unixTimeStampInTicks = (long)(unixTimeStamp * 1);
            return new DateTime(unixTimeStampInTicks + unixStart.Ticks, System.DateTimeKind.Utc);


        }
    }
}
