static public class Clients
{
    static public int GetClientsCount()
    {
        int retValue = ipAddressToCommunication.size();
        return retValue;
    }
    
    static public float GetClientSpeed(int clientID)
    {
        float retValue = 0;
        if (clientID < ipAddressList.size() &&
          ipAddressToCommunication.containsKey(ipAddressList.get(clientID)))
        {
            retValue = (float)
              ((Communication)ipAddressToCommunication.get(ipAddressList.get(clientID))).GetSpeedData();
        }
        return retValue;
    }
    
    static public float GetClientSpeed(String clientIPAddress)
    {
        float retValue = 0;
        if (ipAddressToCommunication.containsKey(clientIPAddress))
        {
            retValue = (float)
              ((Communication)ipAddressToCommunication.get(clientIPAddress)).GetSpeedData();
        }
        return retValue;
    }
    
    static public float GetClientsMaxSpeed()
    {
        float retValue = 0;
        for (int i = 0; i < ipAddressList.size(); i++)
        {
            retValue = max(retValue, GetClientSpeed(i));
        }
        return retValue;
    }
    
    static public float GetClientsMinSpeed()
    {
        float retValue = Float.MAX_VALUE;
        for (int i = 0; i < ipAddressList.size(); i++)
        {
            retValue = min(retValue, GetClientSpeed(i));
        }
        return retValue == Float.MAX_VALUE ? 0 : retValue;
    }
    
    static public float GetClientsAvgSpeed()
    {
        float retValue = 0;
        for (int i = 0; i < ipAddressList.size(); i++)
        {
            retValue += (float)GetClientSpeed(i);
        }
        if (ipAddressList.size() > 0)
        {
            retValue /= ipAddressList.size();
        }
        return retValue;
    }
    
    static public String GetClientIPAddress(int clientID)
    {
        String retValue = new String("");
        if (clientID < ipAddressList.size() &&
          ipAddressToCommunication.containsKey(ipAddressList.get(clientID)))
        {
            retValue = 
              ((Communication)ipAddressToCommunication.get(ipAddressList.get(clientID))).clientIPAddress;
        }
        return retValue;
    }
    
    static public String GetClientName(int clientID)
    {
        String retValue = new String("");
        if (clientID < ipAddressList.size() &&
          ipAddressToCommunication.containsKey(ipAddressList.get(clientID)))
        {
            retValue = 
              ((Communication)ipAddressToCommunication.get(ipAddressList.get(clientID))).clientName;
        }
        return retValue;
    }
};

void PrintDebugInfo(String debugContext)
{
    if(Config.APP_DEBUG_MODE)
    {
        println(debugContext);
    }
}

void PrintStreamInfo(String stream)
{
    if(Config.APP_SHOW_STREAM)
    {
        println(stream);
    }
}
