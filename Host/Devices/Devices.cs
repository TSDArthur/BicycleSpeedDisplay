using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using System.Text;
using System.Threading.Tasks;

namespace BicycleHost.Devices
{
    public class Devices
    {
        public enum DeviceState { OFFLINE, ONLINE };

        public enum CommunicationState { DISCONNECTED, WAITFORDATA, SENDRESP };

        public enum CommandList { RESP_OK = 0,RESP_ERROR = 1, CONNECT_REQ = 2, SENSOR_DATA = 3 };

        public enum RoleList { SERVER = 0, CLIENT = 1 };

        public enum Result { OK, ERROR };

        public class CommunicationFrame
        {
            public int role;
            public int cmd;
            public int data_size;
            public List<object> data;

            public CommunicationFrame()
            {
                data = new List<object>();
                Clear();
            }

            public void Clear()
            {
                role = (int)RoleList.SERVER;
                cmd = (int)CommandList.RESP_OK;
                data_size = 0;
                data.Clear();
            }
        };

        public class DeviceInfo
        {
            public string deviceName;
            public string deviceVersion;
            public string deviceIPAddress;
            public DeviceState deviceState;
            public CommunicationState communicationState;
            public int deviceSensorSpeed;
            public int lastConnectTimeMS;
            CommunicationFrame communicationFrame;

            public DeviceInfo(string _deviceIPAddresss, int connectTime)
            {
                communicationFrame = new CommunicationFrame();
                ResetDevice();
                deviceIPAddress = _deviceIPAddresss;
                lastConnectTimeMS = connectTime;
            }

            public void ResetDevice()
            {
                deviceName = string.Empty;
                deviceVersion = string.Empty;
                deviceIPAddress = string.Empty;
                deviceState = DeviceState.OFFLINE;
                communicationState = CommunicationState.DISCONNECTED;
                deviceSensorSpeed = 0;
                lastConnectTimeMS = 0;
                communicationFrame.Clear();
            }

            public void ProcessStream()
            {
                try
                {
                    switch(communicationFrame.cmd)
                    {
                        case (int)CommandList.CONNECT_REQ:
                            {
                                deviceName = communicationFrame.data[0].ToString();
                                deviceVersion = communicationFrame.data[1].ToString();
                                deviceState = DeviceState.ONLINE;
                                break;
                            }
                        case (int)CommandList.SENSOR_DATA:
                            {
                                deviceSensorSpeed = Int32.Parse(communicationFrame.data[1].ToString());
                                deviceState = DeviceState.ONLINE;
                                break;
                            }
                    }
                }
                catch (Exception) { };
            }

            public string StreamBuilder(CommandList builderType)
            {
                string retValue = string.Empty;
                fastJSON.JSONParameters jsonParameters = new fastJSON.JSONParameters
                {
                    UseExtensions = false
                };
                communicationFrame.Clear();
                try
                {
                    switch(builderType)
                    {
                        case CommandList.RESP_OK:
                            {
                                communicationFrame.role = (int)RoleList.SERVER;
                                communicationFrame.cmd = (int)CommandList.RESP_OK;
                                communicationFrame.data_size = 1;
                                communicationFrame.data.Add("Server");
                                break;
                            }
                        case CommandList.RESP_ERROR:
                            {
                                communicationFrame.role = (int)RoleList.SERVER;
                                communicationFrame.cmd = (int)CommandList.RESP_ERROR;
                                communicationFrame.data_size = 1;
                                communicationFrame.data.Add("Server");
                                break;
                            }
                    }
                    retValue = fastJSON.JSON.ToJSON(communicationFrame, jsonParameters);
                }
                catch (Exception) { };
                return retValue;
            }

            public void DoEvents(int callTime, string dataStream)
            {
                try
                {
                    lastConnectTimeMS = callTime;
                    communicationFrame.Clear();
                    communicationFrame = fastJSON.JSON.ToObject<CommunicationFrame>(dataStream);
                    if (communicationState == CommunicationState.DISCONNECTED ||
                        communicationState == CommunicationState.WAITFORDATA)
                    {
                        ProcessStream();
                        communicationState = CommunicationState.SENDRESP;
                    }
                    if (communicationState == CommunicationState.SENDRESP)
                    {
                        string sendStream = StreamBuilder(CommandList.RESP_OK);
                        Communication.SendStream(deviceIPAddress, sendStream);
                        DebugInfo.debugInfo.Add("设备" + deviceName + " (" + deviceIPAddress + ") 发送: " + sendStream);
                        communicationState = CommunicationState.WAITFORDATA;
                    }
                }
                catch (Exception) { };
            }
        };

        static public List<DeviceInfo> deviceInfos = new List<DeviceInfo>();

        static public string GetStreamClientName(string dataStream)
        {
            string retValue = string.Empty;
            try
            {
                CommunicationFrame communicationFrame = new CommunicationFrame();
                communicationFrame = fastJSON.JSON.ToObject<CommunicationFrame>(dataStream);
                retValue = communicationFrame.data[0].ToString();
            }
            catch (Exception) { };
            return retValue;
        }

        static public void DeviceEvent(string ipAddress, string dataStream)
        {
            try
            {
                bool deviceExisted = false;
                string clientName = GetStreamClientName(dataStream);
                for (int i = 0; i < deviceInfos.Count; i++)
                {
                    if (deviceInfos[i].deviceName == clientName)
                    {
                        deviceExisted = true;
                        deviceInfos[i].deviceIPAddress = ipAddress;
                        deviceInfos[i].deviceState = DeviceState.ONLINE;
                        deviceInfos[i].DoEvents(SystemTimer.systemTimeMS, dataStream);
                        DebugInfo.debugInfo.Add("设备" + clientName + " (" + ipAddress + ") 接收: " + dataStream);
                        break;
                    }
                }
                if (!deviceExisted)
                {
                    DeviceInfo deviceInfo = new DeviceInfo(ipAddress, SystemTimer.systemTimeMS);
                    deviceInfo.deviceState = DeviceState.ONLINE;
                    deviceInfo.DoEvents(SystemTimer.systemTimeMS, dataStream);
                    deviceInfos.Add(deviceInfo);
                    DebugInfo.debugInfo.Add("设备" + clientName + " (" + ipAddress + "): " + "已经加入。");
                    DebugInfo.debugInfo.Add("设备" + clientName + " (" + ipAddress + ") 接收: " + dataStream);
                }
            }
            catch (Exception) { };
        }

        static public void DeviceCommunicationHandler(int callTime)
        {
            for (int i = 0; i < deviceInfos.Count; i++)
            {
                if (deviceInfos[i].deviceState == DeviceState.ONLINE &&
                    callTime - deviceInfos[i].lastConnectTimeMS >= Communication.connectionTimeout)
                {
                    deviceInfos[i].communicationState = CommunicationState.DISCONNECTED;
                    deviceInfos[i].deviceState = DeviceState.OFFLINE;
                    DebugInfo.debugInfo.Add("设备" + deviceInfos[i].deviceName + " (" + deviceInfos[i].deviceIPAddress + "): " + "超时离线。");
                }
            }
        }
    }
}
