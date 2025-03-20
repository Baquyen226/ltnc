#include "UI.h"

Button::Button() {
	textureManager = TextureManager();
	name = "default";
	x = 0;
	y = 0;
	w = 0;
	h = 0;
	text = "default";
}

Button::Button(const std::string &_name, const int _x, const int _y, const int _w, const int _h, const std::string &fontPath, const std::string &_text){
	textureManager = TextureManager();
	name = _name;
	x = _x;
	y = _y;
	w = _w;
	h = _h;

	text = _text;
	

	if (!textureManager.loadFont(fontPath, 32)) {
		std::cerr << "Failed to load font! No VALID font path provided for " << name << " in the provided path: "<< fontPath << " , falling back to default font!\n";
		textureManager.loadFont("asset/font/8-bit.ttf", 32);
	}
}

//currently x and y are where the button is, while w and y determines the size
//so now i want to center the text based on the size of the button
void Button::Render(SDL_Renderer* renderer) {
	//button placeholder
	SDL_FRect button = { x, y, w, h };
	SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
	SDL_RenderFillRect(renderer, &button);

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	//text
	int text_W = textureManager.get_width()	  / 2, 
		text_H = textureManager.get_height()  / 2,
		text_X = x + (w - text_W) / 2 - 15,
		text_Y = y + (h - text_H) / 4 + 10;


	SDL_Color color = { 255, 255, 255, 255 };

	if (!textureManager.loadTextToTexture(text, color, renderer)) {
		std::cerr << "Failed to load text to texture!" << std::endl;
	}
	SDL_FRect textRect = { text_X, text_Y, text_W, text_H };
	textureManager.Render(text_X, text_Y, renderer, 0);
}

bool Button::isClicked(int x, int y) {
	return (x >= this->x && x <= this->x + w && y >= this->y && y <= this->y + h);
}