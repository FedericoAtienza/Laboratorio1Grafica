
//Se definen funciones para operar la Skybox.
//Entre ellas queda definida una función para cargar imagenes.

//CARGADO DE IMAGENES
GLuint LoadTexture(const char* filename) {
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
	FIBITMAP* image = FreeImage_Load(format, filename);
	FIBITMAP* image32 = FreeImage_ConvertTo32Bits(image);

	int width = FreeImage_GetWidth(image32);
	int height = FreeImage_GetHeight(image32);
	BYTE* bits = FreeImage_GetBits(image32);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_BGRA, GL_UNSIGNED_BYTE, bits);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	FreeImage_Unload(image32);
	FreeImage_Unload(image);

	return textureID;
};

GLuint skyboxTextures[6];

//CARGADO DE SKYBOX
void LoadSkybox() {
	skyboxTextures[0] = LoadTexture("../Dependencias/Skyboxes/elyvisions/rainbow_rt.png");	//Cara derecha
	skyboxTextures[1] = LoadTexture("../Dependencias/Skyboxes/elyvisions/rainbow_lf.png");	//Cara izquierda
	skyboxTextures[2] = LoadTexture("../Dependencias/Skyboxes/elyvisions/rainbow_up.png");	//Cara arriba
	skyboxTextures[3] = LoadTexture("../Dependencias/Skyboxes/elyvisions/rainbow_dn.png");	//Cara abajo
	skyboxTextures[4] = LoadTexture("../Dependencias/Skyboxes/elyvisions/rainbow_ft.png");	//Cara enfrente
	skyboxTextures[5] = LoadTexture("../Dependencias/Skyboxes/elyvisions/rainbow_bk.png");	//Cara trasera
}

//DIBUJO DE SKYBOX
void DrawSkybox(float size) {

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	// Cada cara del cubo
	for (int i = 0; i < 6; ++i) {
		glBindTexture(GL_TEXTURE_2D, skyboxTextures[i]);
		glBegin(GL_QUADS);
		glColor3f(1.0f, 1.0f, 1.0f);
		switch (i) {
		case 0:
			glTexCoord2f(0, 0); glVertex3f(size, -size, -size);
			glTexCoord2f(1, 0); glVertex3f(size, -size, size);
			glTexCoord2f(1, 1); glVertex3f(size, size, size);
			glTexCoord2f(0, 1); glVertex3f(size, size, -size);
			break;
		case 1:
			glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
			glTexCoord2f(1, 0); glVertex3f(-size, -size, -size);
			glTexCoord2f(1, 1); glVertex3f(-size, size, -size);
			glTexCoord2f(0, 1); glVertex3f(-size, size, size);
			break;
		case 2:
			glTexCoord2f(0, 0); glVertex3f(-size, size, -size);
			glTexCoord2f(1, 0); glVertex3f(size, size, -size);
			glTexCoord2f(1, 1); glVertex3f(size, size, size);
			glTexCoord2f(0, 1); glVertex3f(-size, size, size);
			break;
		case 3:
			glTexCoord2f(0, 0); glVertex3f(-size, -size, size);
			glTexCoord2f(1, 0); glVertex3f(size, -size, size);
			glTexCoord2f(1, 1); glVertex3f(size, -size, -size);
			glTexCoord2f(0, 1); glVertex3f(-size, -size, -size);
			break;
		case 4:
			glTexCoord2f(0, 0); glVertex3f(size, -size, size);
			glTexCoord2f(1, 0); glVertex3f(-size, -size, size);
			glTexCoord2f(1, 1); glVertex3f(-size, size, size);
			glTexCoord2f(0, 1); glVertex3f(size, size, size);
			break;
		case 5:
			glTexCoord2f(0, 0); glVertex3f(-size, -size, -size);
			glTexCoord2f(1, 0); glVertex3f(size, -size, -size);
			glTexCoord2f(1, 1); glVertex3f(size, size, -size);
			glTexCoord2f(0, 1); glVertex3f(-size, size, -size);
			break;
		}
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}