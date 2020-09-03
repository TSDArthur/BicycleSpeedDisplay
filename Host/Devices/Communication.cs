using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WebSocketSharp;
using WebSocketSharp.Net.WebSockets;
using WebSocketSharp.Server;

namespace BicycleHost.Devices
{
    class Communication
    {
        static public int serverPort = 3333;

        static public int connectionTimeout = 2000;

        static public WebSocketServer webSocketServer;

        public class WebsocketServer : WebSocketBehavior
        {
            protected override void OnMessage(MessageEventArgs e)
            {
                Devices.DeviceEvent(Context.UserEndPoint.ToString(), e.Data);
            }
        }

        static public void SendStream(string ipAddress, string dataStream)
        {
            foreach (IWebSocketSession webSocketSession in webSocketServer.WebSocketServices["/"].Sessions.Sessions)
            {
                if (webSocketSession.Context.UserEndPoint.ToString() == ipAddress)
                    webSocketSession.Context.WebSocket.Send(dataStream);
            }
        }

        static public void StartServer(int serverPort)
        {
            webSocketServer = new WebSocketServer(serverPort);
            webSocketServer.AddWebSocketService<WebsocketServer>("/");
            webSocketServer.Start();
        }
    }
}
