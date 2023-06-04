#ifndef PLAYER_H
#define PLAYER_H

#include "model_manager.h"
#include "world.h"
#include "entity.h"
#include "camera.h"
#include "constants.h"
#include "figure.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <map>

using namespace std;
using namespace glm;

//const float SPEED 2.5;
#define STRAFE_SPEED 7
#define JUMP_POWER 30

class Player : public Entity
{
public:
	int ID;
	bool isYou;
	string name;
//	MDLManager MDL;
//	Camera* camera;
	FORM form;
	TETRAMINO_TYPE tetraminoType = TETRAMINO_TYPE::NONE;
	TETRAMINO_MODE tetraminoMode = TETRAMINO_MODE::JUMP;
	string curAnimation = "fStand";
	
	map<int, JumpLength> jumpsByAngleCurrent;
	multimap<float, Cells> setPositionsByAngle;
	
	Figure figure;
	
	vec3 position;
	vec3 dPos = vec3(0);
	float angle = 0;
	float vX = 0.0;
	float vZ = 0.0;
	float vY = 0.0;
	float jumpForce = 0;

float speed = 0.0;
bool speedF = 0;
bool speedB = 0;
float strafeSpeed = 0;

	MOVEMENT lastMoveX;
	MOVEMENT lastMoveZ;

	float sensitivity = 3.0f;
	
	bool canMove = 1;
	
	float jumpTimeout = 0.0f;
	bool isJump = 0;
	bool isJumping = 0;;
	float jumpHeight = 1.0f;
	float jumpSpeed = 0.1f;
	float jumpVelocity = 0.0f;
	int jumpLengthX = 0;
	int jumpLengthZ = 0;
	float timeInAir = 0;
	
	float timeAFKcur = 0;
	float timeAFKmax = 30;
	
	
	int score = 0;
	
	int setLengthX = 0;
	int setLengthZ = 0;
	
	float tetraminoColor = 0;
	int tetraminoTextureType = 0;
	

	Player() = default;
	Player(const string& name, const MDLManager& MDL, vec3 position);
	
	void move(MOVEMENT movementX, MOVEMENT movementZ, float dt, Camera& camera);
	void Jump(Camera& camera);
	void UpdateJump(Camera& camera);
	
	bool IsBetween(float mid, float start, float end);
	void checkInputs(GLFWwindow *window, float dt);
	
	FORM GetForm();
	TETRAMINO_TYPE GetTetraminoType();
	void SetForm(FORM form);
	void SetTetraminoType(int type);	
	
	void Drop();
	void Swap();
	void TakeFigure(Figure figure);
	void SetFigure(Camera& camera);
	Figure* Set(Camera& camera, World& world);
	
	void SwapTetraminoMode();
	void culcJumpLength(Camera& camera);

	void RenderShadow(Camera& camera);
	
	void Draw(Camera& camera);
	void PlayAnimation(const string& animName);
	void Animate();
	
	vec3 GetJumpLength();
};

#endif