#include "Button.h"
#include "texture.h"

//button class
Button::Button(){
	Contents = NULL;
}

Button::~Button(){
	if (Contents) {
		delete Contents;
		Contents = NULL;
	}
}

void Button::InitWithText(std::string _text, SDL_Color _color, SDL_Renderer* renderer, int _x, int _y) {
	if (Contents) delete Contents;
	text = _text;
	color = _color;
	Contents = new Text;
	Contents->loadFont("assets/fonts/8-bit.ttf", 32);
	Contents->loadTextToTexture(text, color, renderer);
	x = _x; y = _y;
	Contents->getSize(&w, &h);
}

void Button::handleEvent(Game& game, SDL_Event &event) {
	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	isHovered = (mouseX > x && mouseX < x + w && mouseY > y && mouseY < y + h);
	isClicked = (isHovered && event.type == SDL_EVENT_MOUSE_BUTTON_DOWN);

	//if (isClicked) {
	//	std::cout << "Button clicked!\n";  // Debugging click events
	//}
}

void Button::Update(Game& game, SDL_Renderer* renderer) {}

void Button::Render(Game& game, SDL_Renderer* renderer) {
	SDL_Color renderColor = isHovered ? SDL_Color{ 96, 0, 0, color.a } : color;

	// Only reload texture if needed
	static bool lastHover = false;
	if (lastHover != isHovered) {
		Contents->loadTextToTexture(text, renderColor, renderer);
		lastHover = isHovered;
	}

	Contents->Render(renderer, x, y);
}