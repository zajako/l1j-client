#ifndef __SDL_FONT_H_
#define __SDL_FONT_H_

#define COLORKEY 255, 0, 255 //Your Transparent colour

class sdl_font
{
	public:
		sdl_font();
		bool init(const char *name);
		int draw(SDL_Surface *surf, int x, int y, char val, Uint32 color);
		int draw(SDL_Surface *surf, int x, int y, char *str, Uint32 color);
		void draw_cursor(SDL_Surface *surf, int x, int y, Uint32 color);
	private:
		SDL_Surface *final;
};

#endif