#include "Game.h"

#include <vector>
#include <string>
#include <fstream>

std::string ReadFile(const std::string &fileName)
{
	std::ifstream inStream(fileName);
	if (!inStream.good())
	{
		std::cout << "Shader File not found:\"" << fileName << "\"\n";
		return std::string();
	}
	std::cout << "read success\n";
	std::string data(std::istreambuf_iterator<char>(inStream), (std::istreambuf_iterator<char>()));
	return data;
}

Game::Game()
{
	updateTimer = new Timer();
}

Game::~Game()
{
	delete updateTimer;
}

void Game::initializeGame()
{
	meshSphere.initMeshSphere(512U, 512U);
	meshLight.initMeshSphere(6U, 6U);
	meshPlane.initMeshPlane(512U, 512U);
	meshWater.initMeshPlane(512U, 512U);
	ShaderProgram::initDefault();

	texturePumpkinAlbedo.load("pumpkinAlbedo.png");
	texturePumpkinEmissive.load("pumpkinEmissive.png");
	texturePumpkinSpecular.load("pumpkinSpecular.png");

	heightMap.load("heightMap.png");
	rock.load("rock.jpg");
	grass.load("grass.jpg");
	sand.load("sand.jpg");
	water.load("water.png");

	// Load shaders and compile them into a single Program object...
	// First, create the Program object handle.
	shader.load("../assets/shaders/shader.vert", "../assets/shaders/shader.frag");
	shaderTexture.load("../assets/shaders/shader.vert", "../assets/shaders/shaderTexture.frag");
	waterShader.load("../assets/shaders/water.vert", "../assets/shaders/water.frag");
	

	// Cleanup our state. If we left our buffers bound, someone might accidentally modify them later.
	// This is not technically required, but can help reduce obscure errors.
	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);

	// These Render flags can be set once at the start (No reason to waste time calling these functions every frame).
	// Tells OpenGL to respect the depth of the scene. Triangles will not render when they are behind other geometry.
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	   
	// Basic clear color used by glClear().
	glClearColor(0, 0, 0, 0); // Black.

	// Setup our main scene objects...
	float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
	camera.perspective(90.0f, aspect, 1, 1000.0f);
	camera.m_pLocalPosition = vec3(0.0f, 14.0f, 14.0f);
	camera.setRotationAngleX(-45.0f);

	meshLight.setPosition(vec3(0, 0, 10));
	meshLight.setScale(0.2f);
	meshPlane.setScale(100.0f);
	meshPlane.setPosition(vec3(-meshPlane.m_pScale/2, -2, -meshPlane.m_pScale / 2));

	meshWater.setScale(100.0f);
	meshWater.setPosition(vec3(-meshPlane.m_pScale / 2, 1.5, -meshPlane.m_pScale / 2));
}

void Game::update()
{
	// update our clock so we have the delta time since the last update
	updateTimer->tick();

	float deltaTime = updateTimer->getElapsedTimeSeconds();
	TotalGameTime += deltaTime;

	// Give the cube some motion over time.
	meshSphere.setRotationAngleY(TotalGameTime * 15.0f);

	// Give our Transforms a chance to compute the latest matrices
	camera.update(deltaTime);
	meshSphere.update(deltaTime);
	meshPlane.update(deltaTime);
	meshLight.update(deltaTime);
	meshWater.update(deltaTime);
}

void Game::draw()
{
	// Completely clear the Back-Buffer before doing any work.
	glClearColor(0.05f, 0.05f, 0.05f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mat4 cameraView = camera.getView();

	vec4 lightPos = camera.getView() * vec4(meshLight.m_pLocalPosition, 1.0f);


	waterShader.bind();
	waterShader.sendUniform("uTime", TotalGameTime);
	waterShader.sendUniform("uModel", meshWater.getLocalToWorldMatrix());
	waterShader.sendUniform("uView", cameraView);
	waterShader.sendUniform("uProj", camera.getProjection());
	waterShader.sendUniform("uLightPosition", vec3(lightPos));
	texturePumpkinAlbedo.bind(0);
	texturePumpkinEmissive.bind(1);
	texturePumpkinSpecular.bind(2);
	heightMap.bind(3);
	rock.bind(4);
	grass.bind(5);
	sand.bind(6);
	water.bind(7);
	meshWater.bind();
	meshWater.draw();
	meshWater.unbind();
	waterShader.unbind();

	shaderTexture.bind();
	shaderTexture.sendUniform("uTime", TotalGameTime);

	// Ask for the handles identfying the uniform variables in our shader.
	shaderTexture.sendUniform("uModel", meshSphere.getLocalToWorldMatrix());
	shaderTexture.sendUniform("uView", cameraView);
	shaderTexture.sendUniform("uProj", camera.getProjection());
	shaderTexture.sendUniform("uLightPosition", vec3(lightPos));

	/*texturePumpkinAlbedo.bind(0);
	texturePumpkinEmissive.bind(1);
	texturePumpkinSpecular.bind(2);

	heightMap.bind(3);
	rock.bind(4);
	grass.bind(5);
	sand.bind(6);
	water.bind(7);*/

	meshSphere.bind();
	meshSphere.draw();
	meshSphere.unbind();
	texturePumpkinEmissive.unbind(1);


	shaderTexture.sendUniform("uModel", meshPlane.getLocalToWorldMatrix());

	meshPlane.bind();
	meshPlane.draw();
	Mesh::unbind();
	

	texturePumpkinAlbedo.unbind(0);
	texturePumpkinSpecular.unbind(2);

	heightMap.unbind(3);
	rock.unbind(4);
	grass.unbind(5);
	sand.unbind(6);
	water.unbind(7);

	shaderTexture.unbind();

	ShaderProgram::unbind();
	shader.bind();
	shader.sendUniform("uModel", meshLight.getLocalToWorldMatrix());
	meshLight.bind();
	meshLight.draw();
	meshLight.unbind();

	// Commit the Back-Buffer to swap with the Front-Buffer and be displayed on the monitor.
	glutSwapBuffers();
}

void Game::keyboardDown(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case 27: // the escape key
	case 'q':
		camera.m_pLocalPosition.y -= 0.1f;
		break;
	case 'e':
		camera.m_pLocalPosition.y += 0.1f;
		break;
	case 'a':
		camera.m_pLocalPosition.x -= 0.1f;
		break;
	case 'd':
		camera.m_pLocalPosition.x += 0.1f;
		break;
	case 'w':
		camera.m_pLocalPosition.z -= 0.1f;
		break;
	case 's':
		camera.m_pLocalPosition.z += 0.1f;
		break;
	}
}

void Game::keyboardUp(unsigned char key, int mouseX, int mouseY)
{
	switch(key)
	{
	case 32: // the space bar
		break;
	case 27: // the escape key
		exit(1);
		break;
	}
}

void Game::keyboardSpecialDown(int key, int mouseX, int mouseY)
{
	switch (key)
	{
	case GLUT_KEY_F5:
		shader.reload();
		shaderTexture.reload();
		waterShader.reload();
		break;
	case GLUT_KEY_UP:
		meshLight.m_pLocalPosition.z -= 0.1f;
		break;
	case GLUT_KEY_DOWN:
		meshLight.m_pLocalPosition.z += 0.1f;
		break;
	case GLUT_KEY_RIGHT:
		meshLight.m_pLocalPosition.x += 0.1f;
		break;
	case GLUT_KEY_LEFT:
		meshLight.m_pLocalPosition.x -= 0.1f;
		break;
	case GLUT_KEY_PAGE_UP:
		meshLight.m_pLocalPosition.y += 0.1f;
		break;
	case GLUT_KEY_PAGE_DOWN:
		meshLight.m_pLocalPosition.y -= 0.1f;
		break;
	case GLUT_KEY_END:
		exit(1);
		break;
	}
}

void Game::mouseClicked(int button, int state, int x, int y)
{
	if(state == GLUT_DOWN) 
	{
		switch(button)
		{
		case GLUT_LEFT_BUTTON:

			break;
		case GLUT_RIGHT_BUTTON:
		
			break;
		case GLUT_MIDDLE_BUTTON:

			break;
		}
	}
	else
	{

	}
}

/*
 * mouseMoved(x,y)
 * - this occurs only when the mouse is pressed down
 *   and the mouse has moved.  you are given the x,y locations
 *   in window coordinates (from the top left corner) and thus 
 *   must be converted to screen coordinates using the screen to window pixels ratio
 *   and the y must be flipped to make the bottom left corner the origin.
 */
void Game::mouseMoved(int x, int y)
{
}
