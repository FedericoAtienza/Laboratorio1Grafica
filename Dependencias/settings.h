#ifndef SETTINGS_H
#define SETTINGS_H

#include "text.h"

struct frame {
	float minX, maxX, minY, maxY;
	float textureMinX, textureMaxX, textureMinY, textureMaxY;
	GLuint texture;
};

class Settings {
public:

	void init_settings() {

		TTF_Init();
		TTF_Font* font = TTF_OpenFont("../Dependencias/yoster.ttf", 16);

		wireframeTexture = generate_text(font, "Wireframe", { 0, 0, 0 });

		shadeModelTexture = generate_text(font, "Facetado", { 0, 0, 0 });

		shadeModelTexture2 = generate_text(font, "Interpolado", { 0, 0, 0 });

		speedTexture = generate_text(font, "Velocidad", { 0, 0, 0 });

		speed1Texture = generate_text(font, "x1", { 0,0,0 });

		speed2Texture = generate_text(font, "x2", { 0,0,0 });

		speed4Texture = generate_text(font, "x4", { 0,0,0 });

		showTexturesTexture = generate_text(font, "Texturas", { 0, 0, 0 });

		lightTexture = generate_text(font, "Luz", { 0,0,0 });

		titleTexture = generate_text(font, "Opciones", { 0, 0, 0 });

		pauseTexture = generate_text(font, "Pausa", { 255, 255, 255 });

		frameTexture = LoadTexture("../Dependencias/MarcoSettings.png");

		frame2Texture = LoadTexture("../Dependencias/prueba.png");

		boxTexture = LoadTexture("../Dependencias/BotonesSettings.png");

	}

	void draw_pause() {
		glDisable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0.0, 1.0, 1.0, 0.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		//Dibujo "Pausa"
		draw_frame({ 0.42f, 0.58f, 0.525f, 0.475f, 0.0f, 1.0f, 0.0f, 1.0f, pauseTexture });

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glEnable(GL_DEPTH_TEST);
	}

	void draw_settings() {

		glDisable(GL_DEPTH_TEST);

		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0.0, 1.0, 1.0, 0.0);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();

		//Dibujo el marco del menu
		draw_frame({ 0.05f, 0.95f, 0.25f, 0.70f, 0.0f, 1.0f, 0.0f, 1.0f, frameTexture });

		//Dibujo el marco de "Opciones"
		draw_frame({ 0.40f, 0.60f, 0.19f, 0.32f, 0.0f, 1.0f, 0.0f, 1.0f, frame2Texture });

		//Dibujo "Opciones"
		draw_frame({ 0.42f, 0.67f, 0.32f, 0.22f, 0.0f, 1.0f, 0.0f, 1.0f, titleTexture });

		//Dibujo "Velocidad"
		draw_frame({ 0.55f, 0.80f, 0.40f, 0.35f, 0.0f, 1.0f, 0.0f, 1.0f, speedTexture });

		//Dibujo niveles de "Velocidad"

		//Opcion "x1"
		draw_frame({ 0.15f, 0.21f, 0.40f, 0.35f, 0.0f, 1.0f, 0.0f, 1.0f, speed1Texture });
		if (game_speed == 2.0f) {
			draw_frame({ 0.10f, 0.14f, 0.35f, 0.40f, 0.0f, 0.5f, 0.0f, 1.0f, boxTexture });
		}
		else {
			draw_frame({ 0.10f, 0.14f, 0.35f, 0.40f, 0.5f, 1.0f, 0.0f, 1.0f, boxTexture });
		}

		//Opcion "x2"
		draw_frame({ 0.30f, 0.36f, 0.40f, 0.35, 0.0f, 1.0f, 0.0f, 1.0f, speed2Texture });
		if (game_speed == 4.0f) {
			draw_frame({ 0.25f, 0.29f, 0.35f, 0.40f, 0.0f, 0.5f, 0.0f, 1.0f, boxTexture });
		}
		else {
			draw_frame({ 0.25f, 0.29f, 0.35f, 0.40f, 0.5f, 1.0f, 0.0f, 1.0f, boxTexture });
		}

		//Opcion "x4"
		draw_frame({ 0.45f, 0.51f, 0.40f, 0.35, 0.0f, 1.0f, 0.0f, 1.0f, speed4Texture });
		if (game_speed == 8.0f) {
			draw_frame({ 0.40f, 0.44f, 0.35f, 0.40f, 0.0f, 0.5f, 0.0f, 1.0f, boxTexture });
		}
		else {
			draw_frame({ 0.40f, 0.44f, 0.35f, 0.40f, 0.5f, 1.0f, 0.0f, 1.0f, boxTexture });
		}

		//Dibujo "Wireframe"
		draw_frame({ 0.15f, 0.40f, 0.49f, 0.44f, 0.0f, 1.0f, 0.0f, 1.0f, wireframeTexture });
		//Dibujo opcion de "Wireframe"
		if (wireframe) {
			draw_frame({ 0.1f, 0.14f, 0.44f, 0.49f, 0.0f, 0.5f, 0.0f, 1.0f, boxTexture });
		}
		else {
			draw_frame({ 0.1f, 0.14f, 0.44f, 0.49f, 0.5f, 1.0f, 0.0f, 1.0f, boxTexture });
		}

		//Dibujo "Luz"
		draw_frame({ 0.15f, 0.21f, 0.58f, 0.53f, 0.0f, 1.0f, 0.0f, 1.0f, lightTexture });
		//Dibujo opcion de "Luz"
		if (light) {
			draw_frame({ 0.10f, 0.14f, 0.53f, 0.58f, 0.0f, 0.5f, 0.0f, 1.0f, boxTexture });
		}
		else {
			draw_frame({ 0.10f, 0.14f, 0.53f, 0.58f, 0.5f, 1.0f, 0.0f, 1.0f, boxTexture });
		}

		//Dibujo "Texturas"
		draw_frame({ 0.50f, 0.75f, 0.49f, 0.44f, 0.0f, 1.0f, 0.0f, 1.0f, showTexturesTexture });
		//Dibujo opcion de "Texturas"
		if (textures) {
			draw_frame({ 0.45f, 0.49f, 0.44f, 0.49f, 0.0f, 0.5f, 0.0f, 1.0f, boxTexture });
		}
		else {
			draw_frame({ 0.45f, 0.49f, 0.44f, 0.49f, 0.5f, 1.0f, 0.0f, 1.0f, boxTexture });
		}

		//Dibujo "Facetado"
		draw_frame({ 0.15f, 0.40f, 0.67f, 0.62f, 0.0f, 1.0f, 0.0f, 1.0f, shadeModelTexture });
		//Dibujo opcion de "Facetado"
		if (shadeFlat) {
			draw_frame({ 0.10f, 0.14f, 0.62f, 0.67f, 0.0f, 0.5f, 0.0f, 1.0f, boxTexture });
		}
		else {
			draw_frame({ 0.10f, 0.14f, 0.62f, 0.67f, 0.5f, 1.0f, 0.0f, 1.0f, boxTexture });
		}

		//Dibujo "Interpolado"
		draw_frame({ 0.41f, 0.66f, 0.72f, 0.62f, 0.0f, 1.0f, 0.0f, 1.0f, shadeModelTexture2 });
		//Dibujo opcion de "Interpolado"
		if (!shadeFlat) {
			draw_frame({ 0.36f, 0.40f, 0.62f, 0.67f, 0.0f, 0.5f, 0.0f, 1.0f, boxTexture });
		}
		else {
			draw_frame({ 0.36f, 0.40f, 0.62f, 0.67f, 0.5f, 1.0f, 0.0f, 1.0f, boxTexture });
		}

		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();

		glEnable(GL_DEPTH_TEST);
	}
private:
	GLuint wireframeTexture, shadeModelTexture, shadeModelTexture2,
		speedTexture, showTexturesTexture, titleTexture, frameTexture, frame2Texture,
		boxTexture, lightTexture, speed1Texture, speed2Texture, speed4Texture, pauseTexture;

	void draw_frame(frame data) {

		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, data.texture);  
		glColor3f(1.0f, 1.0f, 1.0f);

		glBegin(GL_QUADS);
		glTexCoord2f(data.textureMinX, data.textureMaxY);           //(Sup. Izq)
		glVertex2f(data.minX, data.minY);							//SUPERIOR IZQUIERDA
		glTexCoord2f(data.textureMaxX, data.textureMaxY);			//(Sup. Der)
		glVertex2f(data.maxX, data.minY);							//SUPERIOR DERECHA
		glTexCoord2f(data.textureMaxX, data.textureMinY);			//(Inf. Der)
		glVertex2f(data.maxX, data.maxY);							//INFERIOR DERECHA
		glTexCoord2f(data.textureMinX, data.textureMinY);			//(Inf. Izq)
		glVertex2f(data.minX, data.maxY);							//INFERIOR IZQUIERDA
		glEnd();

		glDisable(GL_TEXTURE_2D);
	}
};

#endif