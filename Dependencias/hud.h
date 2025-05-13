#ifndef HUD_H
#define HUD_H

class HUD {
private:
    TTF_Font* font;

    TTF_Font* time_font; // Fuente tiempo
    SDL_Color time_color; // Color tiempo
    GLuint time_texture; // Textura tiempo
    int time_w, time_h; // Ancho y alto de la textura
    float time_elapsed; // El tiempo que paso desde inicio juego
    float color_timer; // Por agregar algo mas xd pongo que cambie de color

    TTF_Font* apple_font;
    SDL_Color apple_color;
    GLuint apple_texture;
    GLuint apple_text_texture;
    int apple_w, apple_h; // Ancho y alto de la textura
    float total_apples;
    float eaten_apples;

    int level_number;

    GLuint crear_textura_texto(const char* texto, TTF_Font* fuente, SDL_Color color, int& w, int& h);
    TTF_Font* cargar_fuente(const char* ruta, int tamano);


    GLuint cargarTextura(const char* archivo);

    public:
    HUD(int apple_total, int lvl_number);
    ~HUD();

    // Para el tiempo
    void cargar_fuente_time(const char* ruta, int tamano); // Carga this->time_font desde archivo
    void create_time_text(const char* text);
    void set_time_text_color(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha); // no uso pero lo dejo mientras
    void update_time(float delta_time);

    // Para las manzanas
    void cargar_fuente_apple(const char* ruta, int tamano);
    void cargar_textura_apple(); // Carga this->apple_texture desde archivo
    void create_apple_text(const char* text); // Despues hacer una sola para ambos
    void set_apple_text_color(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha); // no uso pero lo dejo mientras
    void draw_apple();
    void set_total_apples(int value);
    void update_remaining_apples(int valor);

    // Dibuja todo
    void draw();

    void reset(); // Por si pierde!!!!! ahi se detectaria y se invoca reset
};

HUD::HUD(int apple_total, int lvl_number){
    std::cout << "Inicializando HUD..." << std::endl; 
    level_number = lvl_number;
    total_apples = apple_total;
    eaten_apples = 0;
    time_elapsed = 0.0f;
    time_color = {255, 255, 255, 255};
    apple_color = {255, 255, 255, 255};
    font = cargar_fuente("../Dependencias/Fonts/albert-text/AlbertText-Bold.ttf", 24);
    time_font = font;
    apple_font = font;
    time_texture = crear_textura_texto("0", this->time_font, this->time_color, this->time_w, this->time_h);
    apple_texture = crear_textura_texto("0", this->apple_font, this->apple_color, this->apple_w, this->apple_h);
    cargar_textura_apple();
}

HUD::~HUD(){
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

void HUD::set_time_text_color(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha){
    this->time_color = {r,g,b,alpha};
}

void HUD::set_apple_text_color(Uint8 r, Uint8 g, Uint8 b, Uint8 alpha){
    this->apple_color = {r,g,b,alpha};
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

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // Mejor calidad
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 formattedSurface->w, formattedSurface->h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, formattedSurface->pixels);

    w = formattedSurface->w;
    h = formattedSurface->h;

    SDL_FreeSurface(textSurface);
    SDL_FreeSurface(formattedSurface);

    return textura;
}

void HUD::create_time_text(const char* text){
    this->time_texture = crear_textura_texto(text, this->time_font, this->time_color, this->time_w, this->time_h);
}

void HUD::create_apple_text(const char* text){
    this->apple_text_texture = crear_textura_texto(text, this->apple_font, this->apple_color, this->apple_w, this->apple_h);
}

void HUD::update_time(float delta_time) {
    // Aumenta el tiempo total acumulado
    time_elapsed += delta_time;
    color_timer += delta_time;

    // Se convierte todo el tiempo el string a dibujar segun el tiempo elapsed
    std::ostringstream ss;
    ss << "Tiempo: " << std::fixed << std::setprecision(0) << time_elapsed;

    // Regenerar la textura del tiempo
    if (time_texture) {
        glDeleteTextures(1, &time_texture);
    }

    create_time_text(ss.str().c_str());
}

void HUD::draw_apple(){
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 600, 0);  // PONER window_width, window_height

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, apple_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    int desired_width_apple = 70;  // tamaño en pantalla de la manzana
    int desired_height_apple = 70;

    glBegin(GL_QUADS); 
        glTexCoord2f(0, 0); glVertex2f(-10, 45);
        glTexCoord2f(1, 0); glVertex2f(-10 + desired_width_apple, 45);
        glTexCoord2f(1, 1); glVertex2f(-10 + desired_width_apple, 45 + desired_height_apple);
        glTexCoord2f(0, 1); glVertex2f(-10, 45 + desired_height_apple);
    glEnd();

    int desired_width_apple_text = 50;  // tamaño en pantalla de la manzana
    int desired_height_apple_text = 50;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, apple_text_texture);
    glBegin(GL_QUADS); 
        glTexCoord2f(0, 0); glVertex2f(50, 60);
        glTexCoord2f(1, 0); glVertex2f(50 + desired_width_apple_text, 60);
        glTexCoord2f(1, 1); glVertex2f(50 + desired_width_apple_text, 60 + desired_height_apple_text);
        glTexCoord2f(0, 1); glVertex2f(50, 60 + desired_height_apple_text);
    glEnd();


    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);


    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING); 

    glPopMatrix(); 
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); 
    glMatrixMode(GL_MODELVIEW);
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
    glDisable(GL_LIGHTING);

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, time_texture);

    // Uso GL_REPLACE para que utilice el color de mi textura, no el del ultimo glColor
    // que se seteo anteriormente como el color actual de opengl
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2f(10, 10);
        glTexCoord2f(1, 0); glVertex2f(10 + time_w, 10);
        glTexCoord2f(1, 1); glVertex2f(10 + time_w, 10 + time_h);
        glTexCoord2f(0, 1); glVertex2f(10, 10 + time_h);
    glEnd();

    // Restauro esto a como estaba antes
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    // Y tambien restauro estos parametros anteriores
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING); 

    glPopMatrix(); 
    glMatrixMode(GL_PROJECTION);
    glPopMatrix(); 
    glMatrixMode(GL_MODELVIEW);
}


GLuint HUD::cargarTextura(const char* archivo) {
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

void HUD::set_total_apples(int value){
    this->total_apples = value;
}

void HUD::cargar_textura_apple(){
    this->apple_texture = cargarTextura("/Users/manuelrv/Desktop/Laboratorio1Grafica/Dependencias/TexturasHUD/apple.png");
}

void HUD::update_remaining_apples(int value){
    // Regenerar la textura del tiempo
    if (apple_text_texture) {
        glDeleteTextures(1, &apple_text_texture);
    }

    this->eaten_apples = value;

    std::ostringstream ss;
    ss << eaten_apples << "/" << total_apples;
    create_apple_text(ss.str().c_str());
}

#endif