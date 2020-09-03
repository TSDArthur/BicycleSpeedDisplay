using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace BicycleHost.Devices
{
    public class SystemTimer
    {
        static public int systemTimeMS = 0;

        static public System.Threading.Timer threadTimer;

        static public void InitTimer()
        {
            threadTimer = new System.Threading.Timer(new TimerCallback(TimerUp), null, 0, 100);
        }

        static private void TimerUp(object value)
        {
            systemTimeMS += 100;
            Devices.DeviceCommunicationHandler(systemTimeMS);
        }
    }
}
