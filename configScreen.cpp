#include "ConfigScreen.h"
#include <fstream>
#include <sstream>

ConfigScreen::ConfigScreen(){
    BACK = Button("Back", 50, 400, 100, 50, "asset/font/8-bit.ttf", "Back");
    SAVE = Button("Save", 250, 400, 100, 50, "asset/font/8-bit.ttf", "Save");
    std::string tmp = "DAS(lower = faster)";
    DAS = Slider(tmp, 50, 100, 1.0f, 300.0f, 150.0f);
    DAS_T = Text(tmp, 50, 80, 20),
    tmp = "Repeat Rate(lower = faster)";
    RepeatRate = Slider(tmp, 50, 150, 1.0f, 300.0f, 22.0f);
    RepeatRate_T = Text(tmp, 50, 130, 20),
    tmp = "Volume(does nothing yet...)";
    Volume = Slider(tmp, 50, 200, 0.0f, 100.0f, 50.0f);
    Volume_T = Text(tmp, 50, 180, 20);
    loadConfig();
}

// Render function
void ConfigScreen::Render(SDL_Renderer* renderer) {
    BACK.Render(renderer);
    SAVE.Render(renderer);
    DAS.Render(renderer);
    RepeatRate.Render(renderer);
    Volume.Render(renderer);
    DAS_T.Render(renderer);
    RepeatRate_T.Render(renderer);
    Volume_T.Render(renderer);
}

// Handle input events
void ConfigScreen::handleEvents(SDL_Event& event) {
    if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_UP || event.type == SDL_EVENT_MOUSE_MOTION) {
        DAS.handleEvents(event);
        RepeatRate.handleEvents(event);
        Volume.handleEvents(event);
    }
}

// Load config from file
void ConfigScreen::loadConfig() {
    std::ifstream file("config.txt");
    if (!file.is_open()) return;

    float dasValue, repeatRateValue, volumeValue;
    file >> dasValue >> repeatRateValue >> volumeValue;

    std::string tmp = "DAS";
    DAS = Slider(tmp, 50, 100, 1.0f, 300.0f, dasValue);
    tmp = "Repeat Rate";
    RepeatRate = Slider(tmp, 50, 150, 1.0f, 300.0f, repeatRateValue);
    tmp = "Volume";
    Volume = Slider(tmp, 50, 200, 0.0f, 100.0f, volumeValue);

    file.close();
}

// Save config to file
void ConfigScreen::saveConfig() {
    std::ofstream file("config.txt");
    if (!file.is_open()) return;

    file << DAS.getValue() << " "
        << RepeatRate.getValue() << " "
        << Volume.getValue() << " ";

    file.close();
}
