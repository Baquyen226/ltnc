#include "UI.h"

//Text
Text::Text() {
	CONTENTS = "";
	x = 0; y = 0;
}

Text::Text(std::string &content, int _x, int _y, float FontSize) {
	CONTENTS = content;
	x = _x;
	y = _y;
	fontSize = FontSize;
	textureManager.loadFont("asset/font/8-bit.ttf", fontSize);
}

void Text::Render(SDL_Renderer* renderer) {
	textureManager.loadTextToTexture(CONTENTS, {255, 255, 255, 255}, renderer);
	textureManager.Render(x, y, renderer, 0);
}


//Button
//Constructor
Button::Button() {
	textureManager = TextureManager();
	name = "default";
	x = 0;
	y = 0;
	w = 0;
	h = 0;
	text = "default";
}

//Initialize button with TEXT CONTENTS
//Param:
//- name: in-game id to be called when debugging
//- x,y,w,h : text placeholder SDL_FRect
//- path : font path
//- text : text content
Button::Button(const std::string &_name, const int _x, const int _y, const int _w, const int _h, const std::string &path, const std::string &_text){
	textureManager = TextureManager();
	name = _name;
	x = _x;
	y = _y;
	w = _w;
	h = _h;

	text = _text;
	

	if (!textureManager.loadFont(path, 32)) {
		std::cerr << "Failed to load font! No VALID font path provided for " << name << " in the provided path: "<< path << " , falling back to default font!\n";
		textureManager.loadFont("asset/font/8-bit.ttf", 32);
	}
}

//Initialize button with NO TEXT CONTENTS
//Param:
//- name: in-game id for debugging
//- path : sprite path
//Note: use Render(SDL_Renderer*, SDL_FRect* src, SDL_FRect* dest) for this
Button::Button(const std::string& _name, const std::string &sprite_path, SDL_Renderer* renderer) {
	//what am i evne doing
	text = "";
	bool loaded = textureManager.loadFromFile(sprite_path, renderer);
	if (!loaded) std::cerr << "Failed to load image. IMG_Error:" << SDL_GetError() << "\n";
}

//Use Render(SDL_Renderer*) if there are text contents
//Use Render(SDL_Renderer*, SDL_FRect* src, SDL_FRect* dest) if button uses a sprite
void Button::Render(SDL_Renderer* renderer) {
	//button placeholder
		SDL_FRect button = { x, y, w, h };
		SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
		SDL_RenderFillRect(renderer, &button);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

		//text
		int text_W = textureManager.get_width() / 2,
			text_H = textureManager.get_height() / 2,
			text_X = x + (w - text_W) / 2 - 15,
			text_Y = y + (h - text_H) / 4 + 10;


		SDL_Color color = { 255, 255, 255, 255 };

		if (!textureManager.loadTextToTexture(text, color, renderer)) {
			std::cerr << "Failed to load text to texture!" << std::endl;
		}
		SDL_FRect textRect = { text_X, text_Y, text_W, text_H };
		textureManager.Render(text_X, text_Y, renderer, 0);
}

void Button::Render(SDL_Renderer* renderer, SDL_FRect* src, SDL_FRect* dest) {
		x = dest->x;
		y = dest->y;
		w = dest->w;
		h = dest->h;
		textureManager.PartialRender(renderer, src, dest);
}


bool Button::isClicked(int x, int y) {
	return (x >= this->x && x <= this->x + w && y >= this->y && y <= this->y + h);
}


//CheckBox functions
CheckBox::CheckBox() {
	TITLE = "";
	x = 0; y = 0; size = 0;
	checked = false;
}

CheckBox::CheckBox(std::string& _TITLE, int _x, int _y, int _size, bool _checked) {
	TITLE = _TITLE;
	x = _x, y = _y, size = _size;
	checked = _checked;
}

bool CheckBox::isClicked(int x, int y) {
	return (x >= this->x && x <= this->x + size && y >= this->y && y <= this->y + size);
}

void CheckBox::Render(SDL_Renderer* renderer) {
	SDL_FRect checkBox = { x, y, size, size };
	SDL_RenderRect(renderer, &checkBox);

	if (checked) {
		int x1 = x, x2 = x + size, y1 = y, y2 = y + size;
		SDL_RenderLine(renderer, x1, y1, x2, y2);
		SDL_RenderLine(renderer, x2, y1, x1, y2);
	}
}


//Slider
Slider::Slider() {
	TITLE = "";
	x = 0;
	y = 0;
	minValue = 0;
	maxValue = 0;
	currentValue = 0;
	dragging = false;
};

Slider::Slider(std::string& t, int _x, int _y, float _min, float _max, float _cur) {
	TITLE = t;
	x = _x;
	y = _y;
	minValue = _min;
	maxValue = _max;
	currentValue = _cur;
	dragging = false;
}

void Slider::Render(SDL_Renderer* renderer) {
	SDL_FRect bar = { x, y, w, h };
	Uint8 r, g, b, a;
	SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	SDL_RenderFillRect(renderer, &bar);
	int knobX = x + ((currentValue - minValue) * w) / (maxValue - minValue);
	SDL_FRect knob = { knobX - 5, y - 5, 10, 20 };
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &knob);

	std::string s = std::to_string((int)currentValue);
	textureManager.loadFont("asset/font/8-bit.ttf", 24);
	textureManager.loadTextToTexture(s, { 255, 255, 255, 255 }, renderer);
	textureManager.Render(x + 400, y, renderer, 0);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Slider::handleEvents(SDL_Event& e) {
	if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		int mx = e.button.x;
		int my = e.button.y;
		if (mx >= x && mx <= x + w && my >= y && my <= y + h) {
			dragging = true;
		}
	}
	else if (e.type == SDL_EVENT_MOUSE_BUTTON_UP) {
		dragging = false;
	}
	else if (e.type == SDL_EVENT_MOUSE_MOTION && dragging) {
		int mx = e.motion.x;
		currentValue = minValue + ((mx - x) * (maxValue - minValue)) / w;
		if (currentValue < minValue) currentValue = minValue;
		if (currentValue > maxValue) currentValue = maxValue;
	}
}

float Slider::getValue() {
	return currentValue;
}