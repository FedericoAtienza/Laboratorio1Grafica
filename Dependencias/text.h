#ifndef TEXT_H
#define TEXT_H

GLuint generate_text(TTF_Font* font, string texto, SDL_Color color) {

	SDL_Surface* surface = TTF_RenderText_Blended(font, texto.c_str(), color);

	int w, h;

	w = round(pow(2, ceil(log(surface->w) / log(2))));
	h = round(pow(2, ceil(log(surface->h) / log(2))));

	SDL_Surface* intermediary = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_BlitSurface(surface, 0, intermediary, 0);

	GLuint textura;
	glGenTextures(1, &textura);
	glBindTexture(GL_TEXTURE_2D, textura);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, intermediary->pixels);

	SDL_FreeSurface(surface);

	return textura;
}

#endif