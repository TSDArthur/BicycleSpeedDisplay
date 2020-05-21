static public class Config
{
    // WiFi & UDP Settings
    static public int COMMUNICATION_UDP_LOCAL_PORT = 3333;
    static public String COMMUNICATION_UDP_LOCAL_IP = "192.168.137.1";
    
    // Protocol Settings
    static public int COMMUNICATION_PACK_MAXSIZE = 64;
    static public boolean COMMUNICATION_ENCRYEN = false;
    static public int COMMUNICATION_ENCRKEY_SIZE = 8;
    static public String COMMUNICATION_ENCRKEY = "uA%SVRmQ";
    static public char COMMUNICATION_STREAMBEGIN = '#';
    static public char COMMUNICATION_STREAMEND = '!';
    static public char COMMUNICATION_STREAMDELIMITER = ';';
    static public int COMMUNICATION_REQUIRE_INV = 200;
    static public int COMMUNICATION_TIMEOUT = 2000;
    static public int COMMUNICATION_STREAMCSTL = 4;
    static public String COMMUNICATION_RESPONSESTR = "succ";
    static public String COMMUNICATION_REPNAMESTR = "name";
    static public String COMMUNICATION_SPDSTR = "sped";
    static public String COMMUNICATION_REQSTR = "requ";
    
    // Hardware Settings
    static public int SENSOR_MSPS = 4000;
    static public double BIKE_WHEEL_DIAMETER = 0.0254 * 26;
    
    // Application Settings
    static public String APP_VERSION = "0.1a";
    static public boolean APP_DEBUG_MODE = true;
    static public boolean APP_SHOW_STREAM = false;
    static public int APP_ANIMATION_REFRESH_TIME = 100;
    static public String APP_COPYRIGHT_STR = "ZAOWU Hardware Team [v" + APP_VERSION + "]";
    
    // Screen Settings
    static public PVector SCREEN_SIZE = new PVector(1024, 576);
    static public PVector SCREEN_BACKGROUND_RGB = new PVector(0, 0, 0);
    static public PVector FONT_COLOR_RGB = new PVector(255, 255, 255);
    static public boolean SCREEN_DISPLAY_SPEEDLIST = true;
    static public PVector SCREEN_DISPLAY_SPEEDLIST_POS = new PVector(20, 20);
    static public String SCREEN_TITLE = "Render Window [v" + APP_VERSION + "]";
    static public boolean SCREEN_BACKGROUND_IMAGE = true;
    static public String SCREEN_BACKGROUND_IMAGEPATH = "Background.jpg";
};
