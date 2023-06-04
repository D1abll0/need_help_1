#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/dual_quaternion.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../include/stb_image.h"
#include "../include/shader.h"
#include "../include/camera.h"
#include "../include/animator.h"
#include "../include/model_animation.h"
#include "../include/model_manager.h"
#include "../include/skybox.h"
#include "../include/time.h"
#include "../include/keys.h"
#include "../include/entity.h"
#include "../include/player.h"
#include "../include/figure.h"
#include "../include/shadow.h"

#include <cmath>


#include <iostream>
#include <vector>

using namespace std;
using namespace glm;

const int W = 800;
const int H = 600;

Camera camera(vec3(0.0f, 5.0f, 3.0f));
vec3 lightPos(1.0f, 20.0f, -1.0f);

World world(10, 10);
vector<Player> players;
vector<Figure> droppedFigures;
vector<Figure> settedFigures;

float lastX = W / 2;
float lastY = H / 2;
bool firstMouse = 1;
float ox;
float oy;

vector<MDLManager>MDLS;

int cIndex = 0;

bool isDirLight = 1;

int figY = 3;

bool isJumping = 0;
float jumpHeight = 2.0f;
float jumpVelocity = 0.0f;
float gravity = 0.001f;
float modelY = 0.0f;
float modelX = 0;
float modelZ = 0;
float timeInAir;

float angle_x = 0;
float angle_y = 0;
float angle_z = 0;


bool isRun = 0;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int moveZ, moveX;
int pressCount = 0;
void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    	glfwSetWindowShouldClose(window, true);
    	
			
	if(players[0].form == ELLIPSOID)
	{	
		moveX = (int)MOVEMENT::NONE;
		moveZ = (int)MOVEMENT::NONE;
		
		if(glfwGetKey(window, keys["FORWARD"]) == GLFW_PRESS)
		{
			moveZ += (int)MOVEMENT::FORWARD;
		}
		
		if(glfwGetKey(window, keys["BACK"]) == GLFW_PRESS)
		{
			moveZ += (int)MOVEMENT::BACK;
		}

		if(glfwGetKey(window, keys["LEFT"]) == GLFW_PRESS)
		{
			moveX += (int)MOVEMENT::LEFT;
		}

		if(glfwGetKey(window, keys["RIGHT"]) == GLFW_PRESS)
		{
			moveX += (int)MOVEMENT::RIGHT;
		}

		if(glfwGetKey(window, keys["JUMP"]) == GLFW_PRESS)
		{
			if(!pressedKeys[keys["JUMP"]])
			{
				pressedKeys[keys["JUMP"]] = 1;
				players[0].Jump(camera);
			}			
		}
		if(glfwGetKey(window, keys["JUMP"]) == GLFW_RELEASE) pressedKeys[keys["JUMP"]] = 0;
	}
	
	if(players[0].form == TETRAMINO)
	{
		if(glfwGetKey(window, keys["JUMP"]) == GLFW_PRESS)
		{
			if(!pressedKeys[keys["JUMP"]])
			{
				pressedKeys[keys["JUMP"]] = 1;
				players[0].Jump(camera);
			}			
		}
		if(glfwGetKey(window, keys["JUMP"]) == GLFW_RELEASE) pressedKeys[keys["JUMP"]] = 0;

		
		if(glfwGetKey(window, keys["DROP"]) == GLFW_PRESS)
		{
			if(!pressedKeys[keys["DROP"]])
			{
				pressedKeys[keys["DROP"]] = 1;
				players[0].Drop();
			}			
		}
		if(glfwGetKey(window, keys["DROP"]) == GLFW_RELEASE) pressedKeys[keys["DROP"]] = 0;


		if(glfwGetKey(window, keys["SET"]) == GLFW_PRESS)
		{
			if(!pressedKeys[keys["SET"]])
			{
				pressedKeys[keys["SET"]] = 1;
				Figure* figure;
				figure = players[0].Set(camera, world);
				if(figure != nullptr)
					settedFigures.push_back(*figure);
			}			
		}
		if(glfwGetKey(window, keys["SET"]) == GLFW_RELEASE) pressedKeys[keys["SET"]] = 0;

		
		if(glfwGetKey(window, keys["SWAP"]) == GLFW_PRESS)
		{
			if(!pressedKeys[keys["SWAP"]])
			{
				pressedKeys[keys["SWAP"]] = 1;

			}			
		}
		if(glfwGetKey(window, keys["SWAP"]) == GLFW_RELEASE) pressedKeys[keys["SWAP"]] = 0;
		
		
		if(glfwGetKey(window, keys["MODE"]) == GLFW_PRESS)
		{
			if(!pressedKeys[keys["MODE"]])
			{
				pressedKeys[keys["MODE"]] = 1;
				players[0].SwapTetraminoMode();
//				players[0].SetFigure(camera);
			}			
		}
		if(glfwGetKey(window, keys["MODE"]) == GLFW_RELEASE) pressedKeys[keys["MODE"]] = 0;		
	}
	
	if(players[0].form == HAMMER)
	{
		if(glfwGetKey(window, keys["SWAP"]) == GLFW_PRESS)
		{
			if(!pressedKeys[keys["SWAP"]])
			{
				pressedKeys[keys["SWAP"]] = 1;
				
			}			
		}
		if(glfwGetKey(window, keys["SWAP"]) == GLFW_RELEASE) pressedKeys[keys["SWAP"]] = 0;

		
		if(glfwGetKey(window, keys["ATTACK"]) == GLFW_PRESS)
		{
			
		}	
	}
	
	if(glfwGetKey(window, keys["SCORE"]) == GLFW_PRESS)
	{
		
	}	
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		int index = 4;
		droppedFigures.push_back(Figure(MDLS[index], vec3(ceil(players[0].position.x), 0, ceil(players[0].position.z)), 0, vec3(1), index));
		droppedFigures[droppedFigures.size()-1].isDropped = 1;
		droppedFigures[droppedFigures.size()-1].isTetramino = 1;
		
	} 
	if(button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{

	}
	
	if(button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
	{

	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if(firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = 0;
	}
	ox = xpos - lastX;
	oy = lastY - ypos;
	lastX = xpos;
	lastY = ypos;
	
	camera.ProcessMouseMovement(ox, oy);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void print(mat4 m)
{
	printf("%f\t%f\t%f\t%f\n", m[0][0], m[0][1], m[0][2], m[0][3]);
	printf("%f\t%f\t%f\t%f\n", m[1][0], m[1][1], m[1][2], m[1][3]);
	printf("%f\t%f\t%f\t%f\n", m[2][0], m[2][1], m[2][2], m[2][3]);
	printf("%f\t%f\t%f\t%f\n", m[3][0], m[3][1], m[3][2], m[3][3]);
	printf("\n-----------------------\n\n");
}

bool checkCollisionPlayerFigure()
{
	return 0;
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(W, H, "First Person Tetris", NULL, NULL);
//    GLFWwindow* window = glfwCreateWindow(W, H, "First Person Tetris", glfwGetPrimaryMonitor(), NULL);
	
//	GLFWwindow* window = glfwCreateWindow(glfwGetVideoMode(glfwGetPrimaryMonitor())->width, glfwGetVideoMode(glfwGetPrimaryMonitor())->height, "My Title", glfwGetPrimaryMonitor(), nullptr);


    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
//    glfwSetKeyCallback(window, key_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	


    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    
	glViewport(0, 0, W, H);	
	
	/*load*/
    Shader shaderMDLA("shaders/ModelA/vertex.glsl", "shaders/ModelA/fragment.glsl");
    Shader shaderMDLS("shaders/ModelS/vertex.glsl", "shaders/ModelS/fragment.glsl");
	Shader shaderSKY("shaders/SkyBox/skybox.vs", "shaders/SkyBox/skybox.fs");
	Shader shaderSUN("shaders/Light/vertex.glsl", "shaders/Light/fragment.glsl");
	Shader shaderTRANSPERENT("shaders/Transperent/vertex.glsl", "shaders/Transperent/fragment.glsl");
	Shader shaderSHADOW("shaders/Shadow/vertex.glsl", "shaders/Shadow/fragment.glsl");


	/*init*/;

	MDLManager MDL_0(MDLType::DYNAMIC, shaderMDLA, "resources/3D/tetris/models/NObody/model.glb", animationsList);
	MDLManager MDL_E(MDLType::DYNAMIC, shaderMDLA, "resources/3D/tetris/models/Ellipsoid/model.glb", animationsList);
	MDLManager MDL_H(MDLType::DYNAMIC, shaderMDLA, "resources/3D/tetris/models/Hammer/model.glb", animationsList);
	MDLManager MDL_I(MDLType::DYNAMIC, shaderMDLA, "resources/3D/tetris/models/I/model.glb", animationsList);
	MDLManager MDL_O(MDLType::DYNAMIC, shaderMDLA, "resources/3D/tetris/models/O/model.glb", animationsList);
	MDLManager MDL_S(MDLType::DYNAMIC, shaderMDLA, "resources/3D/tetris/models/S/model.glb", animationsList);
	MDLManager MDL_T(MDLType::DYNAMIC, shaderMDLA, "resources/3D/tetris/models/T/model.glb", animationsList);
	MDLManager MDL_L(MDLType::DYNAMIC, shaderMDLA, "resources/3D/tetris/models/L/model.glb", animationsList);
	
	MDLManager MDL1(MDLType::STATIC, shaderMDLS, "resources/3D/tetris/figures/I/model.obj");
	MDLManager MDL2(MDLType::STATIC, shaderMDLS, "resources/3D/tetris/figures/O/model.obj");
	MDLManager MDL3(MDLType::STATIC, shaderMDLS, "resources/3D/tetris/figures/S/model.obj");
	MDLManager MDL4(MDLType::STATIC, shaderMDLS, "resources/3D/tetris/figures/L/model.obj");
	MDLManager MDL5(MDLType::STATIC, shaderMDLS, "resources/3D/tetris/figures/T/model.obj");
	
	MDLManager CUBE(MDLType::STATIC, shaderMDLS, "resources/3D/cube/model.obj");
	
	MDLManager FLOOR(MDLType::STATIC, shaderMDLS, "resources/3D/floor/model.obj");
	MDLManager ZONE(MDLType::STATIC, shaderTRANSPERENT, "resources/3D/zone/model.obj");
	
	MDLManager JUMP(MDLType::STATIC, shaderTRANSPERENT, "resources/3D/toJump/model.obj");

	SkyBox SKY(shaderSKY, "resources/img/sky1", 10000.0f);

	Shadow SHADOW(shaderSHADOW, 1024 * 1);
	
	
	MDLS.push_back(MDL1);
	MDLS.push_back(MDL2);
	MDLS.push_back(MDL3);
	MDLS.push_back(MDL4);
	MDLS.push_back(MDL5);
	
	
	world.AddSky(&SKY);
	world.AddFloor(&FLOOR, &ZONE);
		
	players.push_back(Player("player", MDL_0, vec3(9, World::GetLevel(), 9)));

	Figure TARGET(JUMP, vec3(0), 0, vec3(1));
	

	int time = Time::cTime;	
    while (!glfwWindowShouldClose(window))
    {   	
        Time::calculate();
		glfwSetWindowTitle(window, ("First Person Tetris    FPS: " + to_string(Time::FPS)).c_str());   
     	
        glClearColor(0.21, 0.21, 0.21, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        processInput(window);

//---------------------------
	/*spawn random figures*/
	
	if(droppedFigures.size() < players.size() * 5)
	{
		if(Time::cTime - time > 1)
		{
			int seed1 = int(Time::FPS * (Time::dt * 1000000));
			int seed2 = int(Time::cTime * (Time::dt * 1000000));
			int tetraminoIndex = int(seed1 + seed2) % 5;
			int x = seed1 % world.GetSize();
			int z = seed2 % world.GetSize();
//			tetraminoIndex = 0;
			
			if(world.AddDroppedTetramino(x, z, tetraminoIndex))
			{
				droppedFigures.push_back(Figure(MDLS[tetraminoIndex], vec3(x, 0, z), 0, vec3(1), tetraminoIndex));
				droppedFigures[droppedFigures.size()-1].isDropped = 1;
				droppedFigures[droppedFigures.size()-1].isTetramino = 1;
			}
			time = Time::cTime;
		}
	}


//---------------------------
	/*move players*/
	for(int i = 0; i < 1; i++)
	{
		players[0].move((MOVEMENT)moveX, (MOVEMENT)moveZ, Time::dt, camera);
	}

//---------------------------
	/*render shadow map*/
	SHADOW.SetLightMatrix(lightPos);

//---------------------------
	/*render players shadow*/
	for(int i = 0; i < 1; i++)
	{
		players[0].RenderShadow(camera);
	}
	
//---------------------------
	/*render figures dropped shadow*/

	for(int i = 0; i < droppedFigures.size(); i++)
	{
		droppedFigures[i].RenderShadow(camera);
	}

//---------------------------
	/*render figures setted shadow*/
	
	for(int i = 0; i < settedFigures.size(); i++)
	{
		settedFigures[i].RenderShadow(camera);
	}
	
//---------------------------
	/*disable shadow map*/
	SHADOW.Disable();	


//---------------------------
	/*render players*/
	
	for(int i = 0; i < 1; i++)
	{
		players[0].Draw(camera);
	}	
//---------------------------
	/*render figures*/

	for(int i = 0; i < droppedFigures.size(); i++)
	{
		if(droppedFigures[i].isVisible)
		{
			droppedFigures[i].Draw(camera);			
		}
	}

//---------------------------
	/*render figures*/	

	for(int i = 0; i < settedFigures.size(); i++)
	{
		settedFigures[i].Draw(camera);
	}
	
//---------------------------
	/*render player targets jump*/

	if(!players[0].isJumping && players[0].tetraminoMode == TETRAMINO_MODE::JUMP)
	{
		for(auto jump : players[0].jumpsByAngleCurrent)
		{
			vec3 pos = vec3(jump.second.x, 0, jump.second.z);
			if(!isAbroad((players[0].position + pos).x, (players[0].position + pos).z, world.GetSize(), world.GetSize()))
			{
				vec3 jumpLength = players[0].GetJumpLength();
				TARGET.position = players[0].position + pos;
				
				if(pos.x == jumpLength.x && pos.z == jumpLength.z)
					TARGET.scale = vec3(1-abs(sin(Time::cTime*4)/2));
				else
					TARGET.scale = vec3(1-abs(sin(Time::cTime*1)/2)) / 2.0f;
					
				TARGET.Draw(camera);				
			}
		}
	}

//---------------------------
	/*render player targets set*/
	float a = camera.GetAxisYAngle();
	if(players[0].tetraminoMode == TETRAMINO_MODE::SET)
	{
		for(auto set : players[0].setPositionsByAngle)
		{
			vec3 pos = vec3(set.second.c1[1], 0, set.second.c1[0]);
			if(!isAbroad((players[0].position + pos).x, (players[0].position + pos).z, world.GetSize(), world.GetSize()))
			{
				TARGET.position = players[0].position + pos;

				if(pos.x == players[0].setLengthX && pos.z == players[0].setLengthZ)
				{
					if(set.first == 0 && a > 180) a -= 360;
					
					if( (players[0].IsBetween(a, set.first, set.first + 10) && set.second.da == -1) || (players[0].IsBetween(a, set.first - 10, set.first) && set.second.da == 1) )
						continue;					
				
					TARGET.scale = vec3(1-abs(sin(Time::cTime*4)/2));
//printf("da: %d\n", set.second.da);
//					int c1X = set.second.c1[1];
//					int c1Z = set.second.c1[0];
//					int c2X = set.second.c2[1];
//					int c2Z = set.second.c2[0];
//					int c3X = set.second.c3[1];
//					int c3Z = set.second.c3[0];					
//					printf("a: %.0f\tc1X: %d, c1Z: %d; c2X: %d, c2Z: %d; c3X: %d, c3Z: %d\n", a, c1X, c1Z, c2X, c2Z, c3X, c3Z);
				}
				else
					TARGET.scale = vec3(1-abs(sin(Time::cTime*1)/2)) / 2.0f;
					
				TARGET.Draw(camera);				
			}
			

			
			
				
		}
	}


//---------------------------
	/*check collision player-figure*/

	
	for(int pi = 0; pi < players.size(); pi++)
	{
		if(players[pi].GetForm() == FORM::ELLIPSOID && players[pi].GetTetraminoType() == TETRAMINO_TYPE::NONE)
		{
			vec3 pPos = players[pi].position;
//			if( (pPos.x - int(pPos.x) <= 0.6 || pPos.x - int(pPos.x) >= 0.4) && (pPos.z - int(pPos.z) <= 0.6 || pPos.z - int(pPos.z) >= 0.4))

			int cpx = round(pPos.x);
			int cpz = round(pPos.z);
			
			for(int fi = 0 ; fi < droppedFigures.size(); fi++)
			{
				int cfx = round(droppedFigures[fi].position.x);
				int cfz = round(droppedFigures[fi].position.z);	
				
				if(cpx == cfx && cpz == cfz)
				{					
					players[pi].TakeFigure(droppedFigures[fi]);
//					settedFigures.push_back();
					droppedFigures.erase(droppedFigures.begin() + fi);
				}
			}
		}			
	}



//---------------------------
	/*render world*/

	world.Draw(camera);

//---------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
    }
	
	glfwTerminate();
	return 0;
}
