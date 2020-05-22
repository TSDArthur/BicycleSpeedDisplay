void setup()
{      
    // Load Window Settings
    LoadWindow();
      
    // Load Communication System
    CommunicationInitialize();
}

void settings()
{
    // Setup Window
    SetWindow();
}

void draw()
{
    // Run Communication Handler
    CommunicationHandler(millis());
    
    // Draw Window
    PaintWindow();
}
