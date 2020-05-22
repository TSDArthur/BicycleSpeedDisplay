import java.text.DecimalFormat; 
 
PGraphics gBackgroundImage;
PGraphics gClientsListTexts;
List<String> textList = new ArrayList();
PImage backgroundImage;

void LoadWindow()
{
    surface.setTitle(Config.SCREEN_TITLE);
    
    // Create PGraphics
    gBackgroundImage = createGraphics(width, height);
    gClientsListTexts = createGraphics(width, height);
    
    gBackgroundImage.smooth();
    
    if (Config.SCREEN_BACKGROUND_IMAGE && backgroundImage != null)
    {
        gBackgroundImage.beginDraw();
        gBackgroundImage.background(backgroundImage);
        gBackgroundImage.endDraw();
    }
    else
    {
        gBackgroundImage.beginDraw();
        gBackgroundImage.background((int)(Config.SCREEN_BACKGROUND_RGB.x),
          (int)(Config.SCREEN_BACKGROUND_RGB.y), (int)(Config.SCREEN_BACKGROUND_RGB.z));
        gBackgroundImage.endDraw();
    }
    
    if (Config.SCREEN_DISPLAY_SPEEDLIST)
    {
        gClientsListTexts.beginDraw();
        gClientsListTexts.background(0, 0, 0, 0);
        gClientsListTexts.fill((int)(Config.FONT_COLOR_RGB.x),
          (int)(Config.FONT_COLOR_RGB.y), (int)(Config.FONT_COLOR_RGB.z));
        gClientsListTexts.noStroke();
        gClientsListTexts.smooth();
        gClientsListTexts.endDraw();
    }
    
    frameRate(Config.APP_ANIMATION_FPS);
        
    UserRenderInit();
}

void SetWindow()
{
    if (Config.SCREEN_BACKGROUND_IMAGE)
    {
        backgroundImage = loadImage(Config.SCREEN_BACKGROUND_IMAGEPATH);
        size(backgroundImage.width, backgroundImage.height);
    }
    else
    {
        size((int)(Config.SCREEN_SIZE.x), (int)(Config.SCREEN_SIZE.y));
    }
}

void PaintWindow()
{
    image(gBackgroundImage, 0, 0);
    
    UserRender();
    
    if (Config.SCREEN_DISPLAY_SPEEDLIST)
    {
        RenderSpeedListGraphics();
        image(gClientsListTexts, 0, 0);
    }
}

void RenderSpeedListGraphics()
{
    // Prepare TextList
    textList.clear();
    textList.add("Application Time: " + millis() + "ms.");
    for(int i = 0; i < Clients.GetClientsCount(); i++)
    {
        String clientName = Clients.GetClientName(i);
        String clientIPAddress = Clients.GetClientIPAddress(i);
        String clientSpeed = new DecimalFormat("0.00").format(Clients.GetClientSpeed(i));
        if (!clientIPAddress.equals(""))
        {
            textList.add("Client: " + clientName 
              + " IP: " + clientIPAddress + " Speed: " + clientSpeed + "m/s.");
        }
    }
    
    String clientMaxSpeed = new DecimalFormat("0.00").format(Clients.GetClientsMaxSpeed());
    String clientMinSpeed = new DecimalFormat("0.00").format(Clients.GetClientsMinSpeed());
    String clientAvgSpeed = new DecimalFormat("0.00").format(Clients.GetClientsAvgSpeed());
    
    textList.add("Max Speed: " + clientMaxSpeed + 
      "m/s Min Speed: " + clientMinSpeed + 
      "m/s Avg Speed: " + clientAvgSpeed + "m/s.");
    
    // Render TextList
    gClientsListTexts.beginDraw();
    gClientsListTexts.clear();
    for (int i = 0; i < textList.size(); i++)
    {
        float currentPosX = Config.SCREEN_DISPLAY_SPEEDLIST_POS.x;
        float currentPosY = (i + 1) * Config.SCREEN_DISPLAY_SPEEDLIST_POS.y;
        gClientsListTexts.text(textList.get(i), currentPosX, currentPosY);
    }
    gClientsListTexts.endDraw();
}
