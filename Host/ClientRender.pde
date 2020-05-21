PImage backgroundImage;

void LoadWindow()
{
    surface.setTitle(Config.SCREEN_TITLE);
    smooth();
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
        background((int)(Config.SCREEN_BACKGROUND_RGB.x),
          (int)(Config.SCREEN_BACKGROUND_RGB.y), (int)(Config.SCREEN_BACKGROUND_RGB.z));
    }
}

void PaintWindow()
{
    if (Config.SCREEN_BACKGROUND_IMAGE)
    {
        background(backgroundImage);
    }
}

void DisplaySpeedList()
{
    if(Config.SCREEN_DISPLAY_SPEEDLIST)
    {
        fill((int)(Config.SCREEN_BACKGROUND_RGB.x),
          (int)(Config.SCREEN_BACKGROUND_RGB.y), (int)(Config.SCREEN_BACKGROUND_RGB.z));
        noStroke();
        rect(0, 0, width, 50);
        fill((int)(Config.FONT_COLOR_RGB.x),
          (int)(Config.FONT_COLOR_RGB.y), (int)(Config.FONT_COLOR_RGB.z));
        text(Config.APP_COPYRIGHT_STR, Config.SCREEN_DISPLAY_SPEEDLIST_POS.x, Config.SCREEN_DISPLAY_SPEEDLIST_POS.y);
        text("Application Time: " + millis(), 20, 40);
    }
}
