using BicycleHost.Devices;
using System;
using System.Windows.Forms;

namespace BicycleHost
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            Communication.StartServer(Communication.serverPort);
            SystemTimer.InitTimer();
        }

        private void RefreshTimer_Tick(object sender, EventArgs e)
        {
            try
            {
                if (listBoxDevices.Items.Count != Devices.Devices.deviceInfos.Count)
                {
                    listBoxDevices.Items.Clear();
                    for (int i = 0; i < Devices.Devices.deviceInfos.Count; i++)
                        listBoxDevices.Items.Add(Devices.Devices.deviceInfos[i].deviceName);
                }

                if (listBoxDevices.SelectedItem != null)
                {
                    for (int i = 0; i < Devices.Devices.deviceInfos.Count; i++)
                    {
                        if (listBoxDevices.SelectedItem.ToString() == Devices.Devices.deviceInfos[i].deviceName)
                        {
                            labelDeviceState.Text = "设备状态：" +
                                (Devices.Devices.deviceInfos[i].deviceState == Devices.Devices.DeviceState.OFFLINE ? "离线" : "在线");
                            labelDeviceName.Text = "设备名称：" + Devices.Devices.deviceInfos[i].deviceName;
                            labelDeviceIP.Text = "设备IP：" + Devices.Devices.deviceInfos[i].deviceIPAddress;
                            labelDeviceSensorSpeed.Text = "设备传感器速度：" + Devices.Devices.deviceInfos[i].deviceSensorSpeed.ToString() + " 圈/分钟";
                            labelDeviceVersion.Text = "设备版本：" + Devices.Devices.deviceInfos[i].deviceVersion.ToString();
                            break;
                        }
                    }
                }
                else
                {
                    labelDeviceState.Text = "设备状态：";
                    labelDeviceName.Text = "设备名称：";
                    labelDeviceIP.Text = "设备IP：";
                    labelDeviceSensorSpeed.Text = "设备传感器速度：";
                    labelDeviceVersion.Text = "设备版本：";
                }

                if (DebugInfo.debugInfo.Count >= 500)
                {
                    DebugInfo.debugInfo.Clear();
                    listBoxConsole.Items.Clear();
                }

                if (listBoxConsole.Items.Count != DebugInfo.debugInfo.Count)
                {
                    listBoxConsole.Items.Clear();
                    for (int i = 0; i < DebugInfo.debugInfo.Count; i++)
                        listBoxConsole.Items.Add(DebugInfo.debugInfo[i]);
                    if (listBoxConsole.Items.Count > 0)
                        listBoxConsole.SelectedIndex = listBoxConsole.Items.Count - 1;
                }
            }
            catch (Exception) { };
        }
    }
}
