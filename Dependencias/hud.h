#include "variables.h"
#include "map_variable.h"

#ifndef HUD_H
#define HUD_H

class HUD {
  private:
    TTF_Font* font;

    TTF_Font* time_font;  // Fuente tiempo
    SDL_Color time_color; // Color tiempo
    GLuint time_texture;  // Textura tiempo
    int time_w, time_h;   // Ancho y alto de la textura
    float time_elapsed;   // El tiempo que paso desde inicio juego
    float color_timer;    // Por agregar algo mas xd pongo que cambie de color

    TTF_Font* apple_font;
    SDL_Color apple_color;
    GLuint apple_texture;
    GLuint apple_text_texture;
    int apple_w, apple_h; // Ancho y alto de la textura
    float total_apples;
    float eaten_apples;

    int level_number;
    SDL_Color level_color;
    GLuint level_number_texture;
    int level_w, level_h;

    int speed_number;
    TTF_Font* speed_font;
    GLuint speed_texture;
    SDL_Color speed_color;
    int speed_w, speed_h;

    GLuint carta_texture;
    int carta_w, carta_h;

    // Textura cambio nivel
    TTF_Font* next_level_font;
    GLuint next_level_texture;
    SDL_Color next_level_color;
    int next_level_w, next_level_h;
    bool show_next; // Para indicar si dibujo next level o no
    GLuint next_level_frame;
    int next_level_frame_w, next_level_frame_h;

    GLuint crear_textura_texto(const char* texto, TTF_Font* fuente, SDL_Color color, int& w, int& h);
    TTF_Font* cargar_fuente(const char* ruta, int tamano);

    GLuint cargarTexturaApple(const char* archivo);
    GLuint cargarTexturaCarta(const char* archivo);
    GLuint cargarTexturaNextLevelFrame(const char* archivo);

  public:
    HUD();
    ~HUD();

    // Para el tiempo
    void cargar_fuente_time(const char* ruta, int tamano); // Carga this->time_font desde archivo
    void create_time_text(const char* text);
    void set_time_text_color(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha); // no uso pero lo dejo mientras
    void update_time();

    // Para las manzanas
    void cargar_fuente_apple(const char* ruta, int tamano);
    void cargar_textura_apple();                                       // Carga this->apple_texture desde archivo
    void create_apple_text(const char* text);                          // Despues hacer una sola para ambos
    void set_apple_text_color(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha); // no uso pero lo dejo mientras
    void set_total_apples(int value);
    void update_remaining_apples();

    // Para el game speed
    void create_speed_text(const char* text);
    void update_speed();

    // Para el nivel
    void create_level_text(const char* text);

    // Para frame/carta
    void cargar_textura_carta();

    // Dibuja todo
    void draw();

    void update();

    // Para textura de cambio de nivel
    void create_next_level_text(const char* text);
    void show_next_level();
    void hide_next_level();
    void cargar_textura_next_level_frame();

    // NO IMPLEMENTADO AUN
    void reset(); // Por si pierde, ahi se detectaria y se invoca reset
};

HUD::HUD() {
    std::cout << "Inicializando HUD..." << std::endl;
    level_number = level_map.level_number();
    speed_number = 1;
    total_apples = level_map.apple_quantity();
    eaten_apples = 0;
    time_elapsed = 0.0f;
    time_color = {205, 133, 63, 255};
    apple_color = {205, 133, 63, 255};
    level_color = {188, 143, 143, 255};
    speed_color = {210, 105, 30, 255};
    font = cargar_fuente("../Dependencias/Fonts/albert-text/AlbertText-Bold.ttf", 24);
    time_font = font;
    apple_font = font;
    speed_font = font;
    create_time_text("0");
    std::string lvl = "LEVEL " + std::to_string(level_number);
    create_level_text(lvl.c_str());
    std::string speed = "Speed: x " + std::to_string(speed_number);
    create_speed_text(speed.c_str());
    cargar_textura_apple();
    cargar_textura_carta();
    cargar_textura_next_level_frame();
    next_level_font = cargar_fuente("../Dependencias/Fonts/royal-acidbath/Royalacid_o.ttf", 48);

    create_next_level_text("WELL DONE!");
    next_level_color = {255, 255, 255, 255};
}

HUD::~HUD() {
    TTF_CloseFont(time_font);
    TTF_Quit();
}

TTF_Font* HUD::cargar_fuente(const char* ruta, int tamano) {
    TTF_Font* font = TTF_OpenFont(ruta, tamano);
    if (font == nullptr) {
        std::cerr << "Error al cargar la fuente: " << TTF_GetError() << std::endl;
    }
    return font;
}

void HUD::cargar_fuente_time(const char* ruta, int tamano) {
    TTF_Font* fuente = TTF_OpenFont(ruta, tamano);
    if (fuente == nullptr) {
        std::cerr << "Error al cargar la fuente: " << TTF_GetError() << std::endl;
    }
    this->time_font = fuente;
}

void HUD::cargar_fuente_apple(const char* ruta, int tamano) {
    TTF_Font* fuente = TTF_OpenFont(ruta, tamano);
    if (fuente == nullptr) {
        std::cerr << "Error al cargar la fuente: " << TTF_GetError() << std::endl;
    }
    this->apple_font = fuente;
}

void HUD::set_time_text_color(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha) {
    this->time_color = {r, g, b, alpha};
}

void HUD::set_apple_text_color(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha) {
    this->apple_color = {r, g, b, alpha};
}

GLuint HUD::crear_textura_texto(const char* texto, TTF_Font* fuente, SDL_Color color, int& w, int& h) {
    // Renderizado con calidad antialias y alpha
    SDL_Surface* textSurface = TTF_RenderUTF8_Blended(fuente, texto, color);
    if (!textSurface) {
        SDL_Log("No se pudo renderizar el texto: %s", TTF_GetError());
        exit(1);
    }

    // Convertir a formato compatible con OpenGL
    SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat(textSurface, SDL_PIXELFORMAT_RGBA32, 0);

    GLuint textura;
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Mejor calidad
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, formattedSurface->w, formattedSurface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, formattedSurface->pixels);

    w = formattedSurface->w;
    h = formattedSurface->h;

    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(formattedSurface);

    return textura;
}

void HUD::create_time_text(const char* text) {
    this->time_texture = crear_textura_texto(text, this->time_font, this->time_color, this->time_w, this->time_h);
}

void HUD::create_apple_text(const char* text) {
    this->apple_text_texture = crear_textura_texto(text, this->apple_font, this->apple_color, this->apple_w, this->apple_h);
}

void HUD::create_level_text(const char* text) {
    this->level_number_texture = crear_textura_texto(text, this->font, this->level_color, this->level_w, this->level_h);
}

void HUD::create_speed_text(const char* text) {
    this->speed_texture = crear_textura_texto(text, this->speed_font, this->speed_color, this->speed_w, this->speed_h);
}

void HUD::create_next_level_text(const char* text){
    if (!text) {
        printf("text es NULL\n");
    }
    if (!this->next_level_font) {
        printf("Fuente es NULL\n");
    }
    printf("Color: (%d, %d, %d, %d)\n", this->next_level_color.r, this->next_level_color.g, this->next_level_color.b, this->next_level_color.a);
    printf("Ancho: %d, Alto: %d\n", this->next_level_w, this->next_level_h);
    this->next_level_texture = crear_textura_texto(text, this->next_level_font, this->next_level_color, this->next_level_w, this->next_level_h);
}

void HUD::show_next_level(){
    this->show_next = true;
}

void HUD::hide_next_level(){
    this->show_next = false;
}

void HUD::update_time() {
    // Aumenta el tiempo total acumulado
    time_elapsed += deltaTime;
    color_timer += deltaTime;

    // Se convierte todo el tiempo el string a dibujar segun el tiempo elapsed
    std::ostringstream ss;
    ss << "Time: " << std::fixed << std::setprecision(0) << time_elapsed;

    // Regenerar la textura del tiempo
    if (time_texture) {
        glDeleteTextures(1, &time_texture);
    }

    create_time_text(ss.str().c_str());
}

void HUD::update_speed() {
    this->speed_number = game_speed;
    if (speed_texture) {
        glDeleteTextures(1, &speed_texture);
    }

    std::ostringstream ss;
    ss << "Speed: x " << std::fixed << std::setprecision(0) << speed_number;

    create_speed_text(ss.str().c_str());
}

void HUD::draw() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 600, 0); // PONER window_width, window_height

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    int x_frame = 10;
    int x = 20;
    int y = 10;
    int separacion = 10;

    // 0. Dibujo carta/marco
    glBindTexture(GL_TEXTURE_2D, carta_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x_frame, y);
    glTexCoord2f(1, 0);
    glVertex2f(x_frame + 120, y);
    glTexCoord2f(1, 1);
    glVertex2f(x_frame + 120, y + 180);
    glTexCoord2f(0, 1);
    glVertex2f(x_frame, y + 180);
    glEnd();

    // 1. Dibujo Level
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, level_number_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + level_w, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + level_w, y + level_h);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + level_h);
    glEnd();

    // 2. Dibujo time
    y += level_h + separacion;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, time_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + time_w, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + time_w, y + time_h);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + time_h);
    glEnd();

    // 3. Dibujo speed
    y += time_h + separacion;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, speed_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + speed_w, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + speed_w, y + speed_h);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + speed_h);
    glEnd();

    // 4.1. Dibujo apple
    y += speed_h + separacion;
    int desired_width_apple = 50; // tamaño en pantalla de la manzana
    int desired_height_apple = 50;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, apple_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + desired_width_apple, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + desired_width_apple, y + desired_height_apple);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + desired_height_apple);
    glEnd();

    // 4.2. Dibujo apple text
    x += desired_width_apple;
    y += (desired_height_apple - apple_h) / 2;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, apple_text_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex2f(x, y);
    glTexCoord2f(1, 0);
    glVertex2f(x + apple_w, y);
    glTexCoord2f(1, 1);
    glVertex2f(x + apple_w, y + apple_h);
    glTexCoord2f(0, 1);
    glVertex2f(x, y + apple_h);
    glEnd();

    // 5. Si corresponde, dibujo mensaje de next level y su marco
    if (show_next) {
        int next_level_frame_w = 500;
        int next_level_frame_h = 133;
        x = -15;
        y = 450;

        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, next_level_frame);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0); glVertex2f(x, y);
            glTexCoord2f(1, 0); glVertex2f(x + next_level_frame_w, y);
            glTexCoord2f(1, 1); glVertex2f(x + next_level_frame_w, y + next_level_frame_h);
            glTexCoord2f(0, 1); glVertex2f(x, y + next_level_frame_h);
        glEnd();

        x = 10;
        y = 485;
        
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, next_level_texture);
        glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(x, y);
        glTexCoord2f(1, 0);
        glVertex2f(x + next_level_w, y);
        glTexCoord2f(1, 1);
        glVertex2f(x + next_level_w, y + next_level_h);
        glTexCoord2f(0, 1);
        glVertex2f(x, y + next_level_h);
        glEnd();
    }

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

GLuint HUD::cargarTexturaApple(const char* archivo) {
    // Inicializar FreeImage
    FreeImage_Initialise();

    // -> CARGAR IMAGEN
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(archivo);
    FIBITMAP* bitmap = FreeImage_Load(fif, archivo);
    bitmap = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_FlipVertical(bitmap);
    this->apple_w = FreeImage_GetWidth(bitmap);
    this->apple_h = FreeImage_GetHeight(bitmap);
    GLubyte* datos = FreeImage_GetBits(bitmap);

    // -> CREAR LA TEXTURA EN OPENGL
    GLuint textura;
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, this->apple_w, this->apple_h, GL_BGRA, GL_UNSIGNED_BYTE, datos);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();

    return textura;
}

GLuint HUD::cargarTexturaCarta(const char* archivo) {
    // Inicializar FreeImage
    FreeImage_Initialise();

    // -> CARGAR IMAGEN
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(archivo);
    FIBITMAP* bitmap = FreeImage_Load(fif, archivo);
    bitmap = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_FlipVertical(bitmap);
    this->carta_w = FreeImage_GetWidth(bitmap);
    this->carta_h = FreeImage_GetHeight(bitmap);
    GLubyte* datos = FreeImage_GetBits(bitmap);

    // -> CREAR LA TEXTURA EN OPENGL
    GLuint textura;
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, this->carta_w, this->carta_h, GL_BGRA, GL_UNSIGNED_BYTE, datos);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();

    return textura;
}

GLuint HUD::cargarTexturaNextLevelFrame(const char* archivo) {
    // Inicializar FreeImage
    FreeImage_Initialise();

    // -> CARGAR IMAGEN
    FREE_IMAGE_FORMAT fif = FreeImage_GetFIFFromFilename(archivo);
    FIBITMAP* bitmap = FreeImage_Load(fif, archivo);
    bitmap = FreeImage_ConvertTo32Bits(bitmap);
    FreeImage_FlipVertical(bitmap);
    this->next_level_frame_w = FreeImage_GetWidth(bitmap);
    this->next_level_frame_h = FreeImage_GetHeight(bitmap);
    GLubyte* datos = FreeImage_GetBits(bitmap);

    // -> CREAR LA TEXTURA EN OPENGL
    GLuint textura;
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, this->next_level_frame_w, this->next_level_frame_h, GL_BGRA, GL_UNSIGNED_BYTE, datos);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    FreeImage_Unload(bitmap);
    FreeImage_DeInitialise();

    return textura;
}

void HUD::set_total_apples(int value) {
    this->total_apples = value;
}

void HUD::cargar_textura_apple() {
    this->apple_texture = cargarTexturaApple("../Dependencias/TexturasHUD/apple.png");
}

void HUD::cargar_textura_carta() {
    this->carta_texture = cargarTexturaCarta("../Dependencias/TexturasHUD/carta.png");
}

void HUD::cargar_textura_next_level_frame(){ // SEGUIR ESTO
    this->next_level_frame = cargarTexturaNextLevelFrame("../Dependencias/TexturasHUD/banner.png");
}

void HUD::update_remaining_apples() {
    // Regenerar la textura de manzanas restantes
    if (apple_text_texture) {
        glDeleteTextures(1, &apple_text_texture);
    }

    this->eaten_apples = this->total_apples - level_map.apple_quantity();

    std::ostringstream ss;
    ss << eaten_apples << "/" << total_apples;
    create_apple_text(ss.str().c_str());
}

void HUD::update() {
    // Actualizar el tiempo
    update_time();

    // Actualizar las manzanas restantes
    update_remaining_apples();

    // Actualizar la velocidad
    update_speed();
}

#endif