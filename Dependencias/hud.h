#ifndef HUD_H
#define HUD_H

class HUD {
private:
    TTF_Font* time_font; // Fuente tiempo
    SDL_Color time_color; // Color tiempo
    GLuint time_texture; // Textura tiempo
    int time_w, time_h; // Ancho y alto de la textura
    float time_elapsed; // El tiempo que paso desde inicio juego
    float color_timer; // Por agregar algo mas xd pongo que cambie de color

    TTF_Font* apple_font;
    SDL_Color apple_color;
    GLuint apple_texture;
    int apple_w, apple_h;
    float eaten_apples;

    public:
    HUD();
    ~HUD();

    // Para el tiempo
    void crear_textura_time(const char* text);
    void cargar_fuente_time(const char* ruta, int tamano);
    void set_color_fuente_time(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha);
    void update_time(float delta_time);

    void draw();

    void reset(); // Por si pierde!!!!! ahi se detectaria y se invoca reset
};

HUD::~HUD(){
    TTF_CloseFont(time_font);
    TTF_Quit();
}

void HUD::cargar_fuente_time(const char* ruta, int tamano) {
    TTF_Font* fuente = TTF_OpenFont(ruta, tamano);
    if (fuente == nullptr) {
        std::cerr << "Error al cargar la fuente: " << TTF_GetError() << std::endl;
    }
    this->time_font = fuente;
}

void HUD::set_color_fuente_time(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha){
    this->time_color = {r,g,b,alpha};
}

void HUD::crear_textura_time(const char* text){
    // Primero cargo el color desde MAIN
    // Luego cargo fuente desde MAIN 

    // Genero la textura formato sdl renderizando la fuente
    SDL_Surface* textSurface = TTF_RenderText_Blended(this->time_font, text, this->time_color);
    SDL_SetSurfaceBlendMode(textSurface, SDL_BLENDMODE_BLEND);
    if (!textSurface) {
        SDL_Log("No se pudo renderizar el texto: %s", TTF_GetError());
        exit(1);
    }

    GLuint textura_texto;
    glGenTextures(1, &textura_texto);
    glBindTexture(GL_TEXTURE_2D, textura_texto);

    // Parametros textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Formateo la textura formato sdl para que use RGBA
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(textSurface, SDL_PIXELFORMAT_ABGR8888, 0);
    SDL_SetSurfaceBlendMode(formattedSurface, SDL_BLENDMODE_BLEND);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
        formattedSurface->w, formattedSurface->h, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, formattedSurface->pixels);

    this->time_w = textSurface->w;
    this->time_h = textSurface->h;

    glBindTexture(GL_TEXTURE_2D, textura_texto);
    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(formattedSurface);

    this->time_texture = textura_texto;
}

void HUD::update_time(float delta_time) {
    // Aumenta el tiempo total acumulado
    time_elapsed += delta_time;
    color_timer += delta_time;

    // ESTO HAY QUE REVISARLO LOS COLORES SALEN SIEMPRE OPACOS,
    // algun parametro blend?

    // Si paso un segundo -> cambio color
    if (color_timer >= 1.0f) {
        color_timer = 0.0f;

        // Colores aleatorios entre 0-255
        Uint8 r = rand() % 256;
        Uint8 g = rand() % 256;
        Uint8 b = rand() % 256;
        Uint8 a = 255;

        this->time_color = {r,g,b,a};
    }

    // Se convierte todo el tiempo el string a dibujar segun el tiempo elapsed
    std::ostringstream ss;
    ss << "Tiempo: " << std::fixed << std::setprecision(0) << time_elapsed;

    // Regenerar la textura del tiempo
    if (time_texture) {
        glDeleteTextures(1, &time_texture);
    }

    crear_textura_time(ss.str().c_str());
}

void HUD::draw(){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 600, 0);  // PONER window_width, window_height

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Activo el blending y desactivo el depth test
    // para que el texto se dibuje correctamente, sin que sea tapado por otros objetos 3D
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING); // Solo si estás usando luces


    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, time_texture);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(10, 10);
        glTexCoord2f(1, 0); glVertex2f(10 + time_w, 10);
        glTexCoord2f(1, 1); glVertex2f(10 +time_w, 10 + time_h);
        glTexCoord2f(0, 1); glVertex2f(10, 10 + time_h);
    glEnd();

    // Restauro los parametros anteriores
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING); // Solo si estás usando luces


    glPopMatrix(); // ModelView
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); // Projection
    glMatrixMode(GL_MODELVIEW);
}


/* 
    El constructor del hud inicializara la fuente y el color del texto,
    ademas de contadores
*/
HUD::HUD(){
    std::cout << "Entro a constructor HUD" << std::endl; 
    eaten_apples = 0;
    time_elapsed = 0.0f;
}


#endif