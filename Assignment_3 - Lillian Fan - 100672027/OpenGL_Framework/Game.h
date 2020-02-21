#pragma once
#include <windows.h>
#include "GL/glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include "Timer.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"

#define WINDOW_SCREEN_WIDTH		640
#define WINDOW_SCREEN_HEIGHT	432
#define WINDOW_WIDTH			800
#define WINDOW_HEIGHT			432
#define FRAMES_PER_SECOND		60

const int FRAME_DELAY_SPRITE = 1000 / FRAMES_PER_SECOND;

class Game
{
public:
	Game();
	~Game();

	void initializeGame();
	void update();
	void draw();

	/* input callback functions */
	void keyboardDown(unsigned char key, int mouseX, int mouseY);
	void keyboardUp(unsigned char key, int mouseX, int mouseY);
	void keyboardSpecialDown(int key, int mouseX, int mouseY);
	void mouseClicked(int button, int state, int x, int y);
	void mouseMoved(int x, int y);

	/* Data Members */
	Timer *updateTimer	= nullptr;
	float TotalGameTime = 0.0f;

private:
	// Scene Objects.
	Camera camera;
	Mesh meshSphere;
	Mesh meshLight;
	Mesh meshPlane;
	Mesh meshCube;
	Mesh meshWater;

	Texture texturePumpkinAlbedo;
	Texture texturePumpkinEmissive;
	Texture texturePumpkinSpecular;

	Texture heightMap;
	Texture rock;
	Texture grass;
	Texture sand;
	Texture water;

	// OpenGL Handles
	ShaderProgram shader;
	ShaderProgram shaderTexture;
	ShaderProgram terrainShader;
	ShaderProgram waterShader;
};
