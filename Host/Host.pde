void setup()
{      
    // Load Window Settings
    LoadWindow();
      
    // Load Communication System
    CommunicationInitialize();
    
    /* Render Init Code Here */
}

void settings()
{
    SetWindow();
}

void draw()
{
    // Draw Window
    PaintWindow();
    
    // Run Communication Handler
    CommunicationHandler(millis());
    
    /* Render Code Here */
    
    // Display Speed List
    DisplaySpeedList();
    
    // Delay
    delay(Config.APP_ANIMATION_REFRESH_TIME);
}
