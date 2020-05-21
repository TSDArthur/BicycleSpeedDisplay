import java.util.*;
import hypermedia.net.*;

UDP communicationUDP = new UDP(this, Config.COMMUNICATION_UDP_LOCAL_PORT);
enum CommunicationState {OFFLINE, NAME_RECIEVE, DATA_RECIEVE};
long currentTimeMS = 0;

public class ClientDataSet
{
    public int speedRaw;
    public double speedPPS;
    public double speedMPS;
    
    public ClientDataSet()
    {
        speedRaw = 0;
        speedPPS = 0;
        speedMPS = 0;
    }
    
    public void SetSpeedRaw(int _speedRaw)
    {
        speedRaw = _speedRaw;
        speedPPS = speedRaw / (Config.SENSOR_MSPS / 1000.0);
        speedMPS = speedPPS * Config.BIKE_WHEEL_DIAMETER * PI;
    }
};

public class Communication
{
    public String clientName;
    public String clientIPAddress;
    public int clientPort;
    public CommunicationState communicationState;
    public ClientDataSet clientDataSet;
    private long lastRecievedTime;
    
    public Communication(String _clientName, String _clientIPAddress, int _clientPort)
    {
        clientName = new String(_clientName);
        clientIPAddress = new String(_clientIPAddress);
        clientPort = _clientPort;
        communicationState = CommunicationState.OFFLINE;
        clientDataSet = new ClientDataSet();
        lastRecievedTime = currentTimeMS;
    }
    
    private String StreamProcess(String stream)
    {
        StringBuilder retValue = new StringBuilder(stream);
        if (Config.COMMUNICATION_ENCRYEN)
        {
            int streamLength = retValue.length();
            for(int i = 0; i < streamLength; i++)
            {
                retValue.setCharAt(i, 
                  (char)(retValue.charAt(i) ^ Config.COMMUNICATION_ENCRKEY.charAt(i % Config.COMMUNICATION_ENCRKEY_SIZE)));
            }
        }
        return retValue.toString();
    }
    
    private boolean StreamFormatCheck(String stream)
    {
        boolean retValue = false;
        if (stream.charAt(0) == Config.COMMUNICATION_STREAMBEGIN && 
          stream.charAt(stream.length() - 1) == Config.COMMUNICATION_STREAMEND &&
          stream.length() > Config.COMMUNICATION_STREAMCSTL)
        {
            retValue = true;
        }
        return retValue;
    }
    
    private boolean StreamContentCheck(String stream, CommunicationState streamType)
    {
        boolean retValue = false;
        switch(streamType)
        {
          case OFFLINE:
          {
              if (stream.substring(1, 
                Config.COMMUNICATION_STREAMCSTL + 1).equals(Config.COMMUNICATION_REQSTR))
              {
                  retValue = true;
              }
              break;
          }
          case NAME_RECIEVE:
          {
              if (stream.substring(1, 
                Config.COMMUNICATION_STREAMCSTL + 1).equals(Config.COMMUNICATION_REPNAMESTR))
              {
                  retValue = true;
              }
              break;
          }
          case DATA_RECIEVE:
          {
              if (stream.substring(1, 
                Config.COMMUNICATION_STREAMCSTL + 1).equals(Config.COMMUNICATION_SPDSTR))
              {
                  retValue = true;
              }
              break;
          }
        }
        return retValue;
    }
    
    private Object GetDataFromStream(String stream, CommunicationState streamType)
    {
        Object retValue = null;
        try
        {
            switch(streamType)
            {
                case OFFLINE:
                  break;
                case NAME_RECIEVE:
                {
                    retValue = new String(stream.substring(Config.COMMUNICATION_STREAMCSTL + 1, 
                      stream.length() - 1));
                    break;
                }
                case DATA_RECIEVE:
                {
                    retValue = Integer.valueOf(stream.substring(Config.COMMUNICATION_STREAMCSTL + 1,
                      stream.length() - 1));
                    break;
                }
            }
        }
        catch(Exception e)
        {
            println("Error: " + e.toString());
        }
        return retValue;
    }
    
    private String GetDataToSend(CommunicationState streamType, Object dataValue)
    {
        StringBuilder retValue = new StringBuilder("");
        retValue.append(Config.COMMUNICATION_STREAMBEGIN);
        switch(streamType)
        {
            case OFFLINE:
            case NAME_RECIEVE:
            case DATA_RECIEVE:
            {
                retValue.append(Config.COMMUNICATION_RESPONSESTR);
                break;
            }
        }
        retValue.append(Config.COMMUNICATION_STREAMEND);
        return retValue.toString();
    }
    
    public long GetLastRecievedTime()
    {
        long retValue = lastRecievedTime;
        return retValue;
    }
    
    public double GetSpeedData()
    {
        double retValue = clientDataSet.speedMPS;
        return retValue;
    }
    
    private void SendData(String stream)
    {
        PrintStreamInfo("Stream: Send " + stream + " To Client " + clientIPAddress + "."); 
        communicationUDP.send(stream);
    }
    
    public void DoEvents(String streamRecieved, long callTime)
    {
        String streamDecoded = StreamProcess(streamRecieved);
        boolean postbackEnable = false;
        if (StreamFormatCheck(streamDecoded))
        {
            PrintStreamInfo("Stream: Recieved " + streamDecoded + " From Client " + clientIPAddress + "."); 
            // Reset All
            if (StreamContentCheck(streamDecoded, CommunicationState.OFFLINE))
            {
                communicationState = CommunicationState.NAME_RECIEVE;
                clientDataSet.SetSpeedRaw(0);
                postbackEnable = true;
            }
            else if (communicationState == CommunicationState.NAME_RECIEVE)
            {
                if (StreamContentCheck(streamDecoded, CommunicationState.NAME_RECIEVE))
                {
                    clientName = (String)GetDataFromStream(streamDecoded, 
                      CommunicationState.NAME_RECIEVE);
                    communicationState = CommunicationState.DATA_RECIEVE;
                    postbackEnable = true;
                }
            }
            else if (communicationState == CommunicationState.DATA_RECIEVE)
            {
                if (StreamContentCheck(streamDecoded, CommunicationState.DATA_RECIEVE))
                {
                    clientDataSet.SetSpeedRaw(
                      ((Integer)GetDataFromStream(streamDecoded, 
                      CommunicationState.NAME_RECIEVE)).intValue());
                    postbackEnable = true;
                }
            }
            if (postbackEnable)
            {
                lastRecievedTime = callTime;
                SendData(StreamProcess(GetDataToSend(
                  CommunicationState.DATA_RECIEVE, null)));
            }
        }
        return;
    }
};

static public Map<String, Communication> ipAddressToCommunication = new HashMap();
static public List<String> ipAddressList = new ArrayList();

void CommunicationInitialize()
{
    ipAddressToCommunication.clear();
    ipAddressList.clear();
    communicationUDP.listen(true);
    communicationUDP.setReceiveHandler("UDPRecieveHandler");
}

void CommunicationHandler(long currentTime)
{
    currentTimeMS = currentTime;
    for (Map.Entry<String, Communication> entry : ipAddressToCommunication.entrySet())
    {
        if(currentTime - ((Communication)(entry.getValue())).lastRecievedTime > Config.COMMUNICATION_TIMEOUT)
        {
            PrintDebugInfo("Debug: Client " + entry.getKey() + " Disconnected.");
            ipAddressList.remove(entry.getKey());
            ipAddressToCommunication.remove(entry.getKey());
            PrintDebugInfo("Debug: Client Count: " + ipAddressToCommunication.size() + ".");
        }
    }
}

void UDPRecieveHandler(byte[] data, String ip, int port)
{
    String streamRecieved = new String(data);
    if (ipAddressToCommunication.get(ip) == null)
    {
        ipAddressList.add(ip);
        Communication newCommunication = new Communication(ip, ip, port);
        ipAddressToCommunication.put(ip, newCommunication);
        PrintDebugInfo("Debug: Client " + ip + ":" + port + " Connected.");
        PrintDebugInfo("Debug: Client Count: " + ipAddressToCommunication.size() + ".");
    }
    ((Communication)ipAddressToCommunication.get(ip)).DoEvents(streamRecieved, currentTimeMS);
}
