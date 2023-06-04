#include "../include/player.h"
//#include "../include/keys.h"
#include "../include/time.h"
//#include "../include/camera.h"


Player::Player(const string& name, const MDLManager& MDL, vec3 position)
{
	this->name = name;
	this->MDL = MDL;
//	this->camera = camera;
	this->position = position;
//	this->angle = 180;

	tetraminoType = TETRAMINO_TYPE::NONE;
	tetraminoMode == TETRAMINO_MODE::JUMP;
	form = TETRAMINO;
	form = ELLIPSOID;
//	form = HAMMER;
//	speed = SPEED;
	canMove = 1;
	
	this->MDL.AddTexture(Shadow::GetId(), "shadowMap");
	
	
	PlayAnimation(curAnimation = "fStand");
}

void Player::move(MOVEMENT movementX, MOVEMENT movementZ, float dt, Camera& camera)
{
	Animate();
	UpdateJump(camera);
	
	SetFigure(camera);

	if(canMove)
	{
		if(movementZ == MOVEMENT::FORWARD)
		{
			if(speed < SPEED) speed += SPEED / 100.0f;
			lastMoveZ = MOVEMENT::FORWARD;
		}
		if(movementZ == MOVEMENT::BACK)
		{
			if(speed > -SPEED / 2.0f) speed -= SPEED / 100.0f;
			lastMoveZ = MOVEMENT::BACK;
		}
		if(movementZ == MOVEMENT::NONE)
		{
			if(lastMoveZ == MOVEMENT::FORWARD && speed > 0.0f && !isJumping) speed -= SPEED / 50.0f;
			else if(lastMoveZ == MOVEMENT::BACK && speed < 0.0f) speed += SPEED / 50.0f;
			else
			{
				lastMoveZ = MOVEMENT::NONE;
				if(!isJumping)speed = 0.0f;
			}		
		}
		else timeAFKcur = 0;
		
		if(movementX == MOVEMENT::RIGHT)
		{
			if(strafeSpeed < STRAFE_SPEED) strafeSpeed += STRAFE_SPEED / 100.0f;
			lastMoveX = MOVEMENT::RIGHT;
		}
		if(movementX == MOVEMENT::LEFT)
		{
			if(strafeSpeed > -STRAFE_SPEED) strafeSpeed -= STRAFE_SPEED / 100.0f;
			lastMoveX = MOVEMENT::LEFT;
		}	
		if(movementX == MOVEMENT::NONE)
		{
			if(lastMoveX == MOVEMENT::RIGHT && strafeSpeed > 0.0f) strafeSpeed -= STRAFE_SPEED / 50.0f;
			else if(lastMoveX == MOVEMENT::LEFT && strafeSpeed < 0.0f) strafeSpeed += STRAFE_SPEED / 50.0f;
			else
			{
				lastMoveX = MOVEMENT::NONE;
				strafeSpeed = 0.0f;
			}		
		}
		else timeAFKcur = 0;
	
		float Yaw = camera.Yaw;
		/*move front or back*/
		dPos.x += speed * cos(radians(Yaw)) * Time::dt;
		dPos.z += speed * sin(radians(Yaw)) * Time::dt;	
		/*move left or right*/
		dPos.x += strafeSpeed * cos(radians(Yaw + 90)) * Time::dt;
		dPos.z += strafeSpeed * sin(radians(Yaw + 90)) * Time::dt;
		
		if(!isAbroad((position + dPos).x, (position + dPos).z, World::GetSize() - 1, World::GetSize() - 1) && 
			!isAbroad((position + dPos).x - 1, (position + dPos).z - 1, World::GetSize(), World::GetSize()))	
			position += dPos;
		dPos *= 0;
	}

	timeAFKcur += Time::dt; 
}

void Player::checkInputs(GLFWwindow *window, float dt)
{	

}

void Player::Drop()
{
	this->form = FORM::ELLIPSOID;
	this->tetraminoType = TETRAMINO_TYPE::NONE;
	this->tetraminoMode = TETRAMINO_MODE::NONE;
	this->canMove = 1;
}

void Player::Swap()
{
	if(form == FORM::TETRAMINO)
	{
		form = FORM::HAMMER;
	}
	else if(form == FORM::HAMMER)
	{
		form = FORM::TETRAMINO;
	}
}

void Player::TakeFigure(Figure figure)
{
	speed = 0.0f;
	strafeSpeed = 0.0f;
	canMove = 0;
	lastMoveZ = MOVEMENT::NONE;
	lastMoveX = MOVEMENT::NONE;
	angle = 0.0f;
	position = figure.position;
	position.y = World::GetLevel();
	PlayAnimation(curAnimation = "fStand");
	
	SetForm(FORM::TETRAMINO);
	SetTetraminoType(figure.GetType());
	tetraminoMode = TETRAMINO_MODE::JUMP;
	
	tetraminoColor = figure.color;
	tetraminoTextureType = figure.textureType;	
	
	this->figure = figure;
}

FORM Player::GetForm()
{
	return form;
}

TETRAMINO_TYPE Player::GetTetraminoType()
{
	return tetraminoType;
}

void Player::SetForm(FORM form)
{
	this->form = form;
}

void Player::SetTetraminoType(int type)
{
	
	this->tetraminoType = TETRAMINO_TYPE(type);
}

vec3 Player::GetJumpLength()
{
	return vec3(jumpLengthX, 0, jumpLengthZ);
}

bool Player::IsBetween(float mid, float start, float end)
{
	end = (end - start) < 0.0f ? end - start + 360.f : end - start;
	mid = (mid - start) < 0.0f ? mid - start + 360.f : mid - start;
	
	return (mid < end);
}

void Player::culcJumpLength(Camera& camera)
{	
	if(tetraminoType == TETRAMINO_TYPE::S || tetraminoType == TETRAMINO_TYPE::L)
		jumpsByAngleCurrent = jumpsByAngleSL;
		
	if(tetraminoType == TETRAMINO_TYPE::I)
		jumpsByAngleCurrent = jumpsByAngleI;		
		
	if(tetraminoType == TETRAMINO_TYPE::O)
		jumpsByAngleCurrent = jumpsByAngleO;
		
	if(tetraminoType == TETRAMINO_TYPE::T)
		jumpsByAngleCurrent = jumpsByAngleT;	

	float angle = camera.GetAxisYAngle();
	float segmentHalf = 10.0f;


	if(!isJumping)
	{
		for(auto jump : jumpsByAngleCurrent)
		{
			if(IsBetween(jump.first, angle - segmentHalf, angle + segmentHalf))
			{
				jumpLengthX = jump.second.x;
				jumpLengthZ = jump.second.z;
				break;
			}
			else
			{
				jumpLengthX = 0;
				jumpLengthZ = 0;			
			}	
		}	
	}
}

Figure* Player::Set(Camera& camera, World& world)
{
	float angle_ = camera.GetAxisYAngle();;
	if(tetraminoType != TETRAMINO_TYPE::NONE && tetraminoMode == TETRAMINO_MODE::SET && form == TETRAMINO)
	{		
		for(auto set : setPositionsByAngle)
		{
			vec3 pos = vec3(set.second.c1[1], 0, set.second.c1[0]);
			if( !isAbroad((position + pos).x, (position + pos).z, World::GetSize(), World::GetSize()) && !isAbroad(position.x + set.second.c2[1], position.z + set.second.c2[0], World::GetSize(), World::GetSize()) )
			{
				if(IsBetween(set.first, angle_ - 10, angle_ + 10))
				{				
					if(pos.x == setLengthX && pos.z == setLengthZ)
					{			
						if(set.first == 0 && angle_ > 180) angle_ -= 360;
						
						if( (IsBetween(angle_, set.first, set.first + 10) && set.second.da == -1) || (IsBetween(angle_, set.first - 10, set.first) && set.second.da == 1) )
							continue;

						world.SetFigure(tetraminoType, vec2(position.x, position.z), set.first, set.second.da);
			world.print();
							
						
						
						figure.isDropped = 0;
						figure.angleX = 0;
						figure.angleZ = 0;
						vec3 dpos = vec3(set.second.dpos[1], 0.5, set.second.dpos[0]);
						figure.position = position + dpos;
						
						figure.scale *= vec3(set.second.scaleX, 1, set.second.scaleZ) / figure.scale;
						figure.angleY = set.second.angle;
cout << figure.position.y << endl;

						Drop();
						
						return &figure;
					}
				}
			}
		}	
	}
	return nullptr;	
}

void Player::SetFigure(Camera& camera)
{
	if(tetraminoType == TETRAMINO_TYPE::S)
		setPositionsByAngle = setPositionsByAngleS;
		
	if(tetraminoType == TETRAMINO_TYPE::L)
		setPositionsByAngle = setPositionsByAngleL;
		
	if(tetraminoType == TETRAMINO_TYPE::I)
		setPositionsByAngle = setPositionsByAngleI;		
		
	if(tetraminoType == TETRAMINO_TYPE::O)
		setPositionsByAngle = setPositionsByAngleO;
		
	if(tetraminoType == TETRAMINO_TYPE::T)
		setPositionsByAngle = setPositionsByAngleT;	

	float angle = camera.GetAxisYAngle();
	float segmentHalf = 10.0f;
		
	
//printf("a: %.0f\t", angle);

	for(auto set : setPositionsByAngle)
	{
		if(IsBetween(set.first, angle - segmentHalf, angle + segmentHalf))
		{
			setLengthX = set.second.c1[1];
			setLengthZ = set.second.c1[0];
			break;
		}
		else
		{
			setLengthX = 0;
			setLengthZ = 0;			
		}	
	}	
}

void Player::SwapTetraminoMode()
{
	if(tetraminoMode == TETRAMINO_MODE::JUMP)
	{
		tetraminoMode = TETRAMINO_MODE::SET;
		return;
	}
	if(tetraminoMode == TETRAMINO_MODE::SET)
	{
		tetraminoMode = TETRAMINO_MODE::JUMP;
		return;
	}
}

void Player::Jump(Camera& camera)
{
	if(!isJumping)
	{
		if(form == ELLIPSOID && jumpTimeout == 0.0f)
			isJump = 1;
			
		if(form == TETRAMINO && tetraminoMode == TETRAMINO_MODE::JUMP && 
			!isAbroad(position.x + jumpLengthX, position.z + jumpLengthZ, World::GetSize(), World::GetSize()))
			isJump = 1;	
	}
	timeAFKcur = 0;
}

void Player::UpdateJump(Camera& camera)
{
	if(jumpTimeout > 0.0f) jumpTimeout -= Time::dt;
	if(jumpTimeout < 0.0f) jumpTimeout = 0.0f;
	
	if(!isJump &&  tetraminoMode == TETRAMINO_MODE::JUMP) culcJumpLength(camera);		
	if(isJump)
	{
		if(lastMoveX == MOVEMENT::NONE && lastMoveZ == MOVEMENT::NONE)
		{
			if(curAnimation == "fJump1" && MDL.animator->m_CurrentAnimation->IsFinished())
			{
				isJumping = true;
				isJump = 0;
				jumpTimeout = 2.0f;
				jumpVelocity = sqrt(2.0f * jumpHeight * World::GRAVITY);
				timeInAir = World::GRAVITY / jumpVelocity / 2;	
			}			
		}
		else
		{
			isJumping = true;
			isJump = 0;
			jumpTimeout = 3.0f;
			jumpVelocity = sqrt(2.0f * jumpHeight * World::GRAVITY);
			timeInAir = World::GRAVITY / jumpVelocity / 2;			
		}
	}

	
    if(isJumping)
	{
		position.y += jumpVelocity * Time::dt;
		if(form == TETRAMINO)
		{
			
		position.x += jumpLengthX * timeInAir * Time::dt;
		position.z += jumpLengthZ * timeInAir * Time::dt;
		}
		
		jumpVelocity -= World::GRAVITY * Time::dt;
		if(position.y < (float)World::GetLevel())
		{
		    position.y = (float)World::GetLevel();
		    jumpVelocity = 0.0f;
		    isJumping = false;

			if(form == TETRAMINO)
			{		    
			    position.x = round(position.x);
			    position.z = round(position.z);			    
			}
		}
    }    
}

void Player::RenderShadow(Camera& camera)
{
	Reset();
	Move(position);
	Rotate(vec3(0, camera.GetAxisYAngle() + this->angle, 0));
	MDL.SetOrigin(GetPosition(), GetRotation(), GetScale());
	
	MDL.AssemblyShadow();
}

void Player::Draw(Camera& camera)
{
	MDL.Animate(Time::dt);
	
	Reset();
	Move(position);
	Rotate(vec3(0, camera.GetAxisYAngle() + this->angle, 0));
	vec3 lightPos(.0f, 2.0f, -1.0f);
	
//camera.Position = vec3(GetPosition().x+0, GetPosition().y+3, GetPosition().z+5);
camera.Position = vec3(GetPosition().x, GetPosition().y+7, GetPosition().z);
	
	MDL.SetOrigin(GetPosition(), GetRotation(), GetScale());
	MDL.SetViewMat(camera.GetViewMatrix());
	MDL.SetProjMat(camera.GetProjectionMatrix());
	MDL.SetLightMatrix(Shadow::GetLightSpaceMatrix());
	MDL.ClearLights();
	MDL.SetLight(Direction, vec3(0), lightPos * -1.0f);
	MDL.SetViewPos(camera.Position);
	MDL.Draw();
}

void Player::PlayAnimation(const string& animName)
{
	curAnimation = animName;
	
	if(MDL.animations.find(animName) != MDL.animations.end())
	{
		if(animName[0] == 'f' && (form == FORM::ELLIPSOID || form == FORM::TETRAMINO))
			MDL.animator->PlayAnimation(MDL.animations[animName]);

		if(animName[0] == 'e' && form == FORM::ELLIPSOID)
			MDL.animator->PlayAnimation(MDL.animations[animName]);

		if(animName[0] == 'h' && form == FORM::HAMMER)
			MDL.animator->PlayAnimation(MDL.animations[animName]);	
	}
}

void Player::Animate()
{	
	if(lastMoveZ == MOVEMENT::FORWARD && !isJumping && !isJump)
	{
		if(curAnimation != "eRun") PlayAnimation(/*curAnimation = */"eRun");
	}
	
	if(lastMoveZ == MOVEMENT::BACK && !isJumping && !isJump)
	{
		if(curAnimation != "eRun") PlayAnimation(/*curAnimation = */"eRun");	
	}
	
	if(lastMoveX == MOVEMENT::LEFT && !isJumping && !isJump)
	{
		if(curAnimation != "eRun") PlayAnimation(/*curAnimation = */"eRun");
		angle = 90;
		
		if(lastMoveZ == MOVEMENT::FORWARD && !isJumping && !isJump)
		{
			if(curAnimation != "eRun") PlayAnimation(/*curAnimation = */"eRun");
			angle = 45;		
		}
		if(lastMoveZ == MOVEMENT::BACK && !isJumping && !isJump)
		{
			if(curAnimation != "eRun") PlayAnimation(/*curAnimation = */"eRun");
			angle = -45;		
		}		
	}

	if(lastMoveX == MOVEMENT::RIGHT && !isJumping && !isJump)
	{
		if(curAnimation != "eRun") PlayAnimation(/*curAnimation = */"eRun");
		angle = -90;
		
		if(lastMoveZ == MOVEMENT::FORWARD && !isJumping && !isJump)
		{
			if(curAnimation != "eRun") PlayAnimation(/*curAnimation = */"eRun");
			angle = -45;		
		}
		if(lastMoveZ == MOVEMENT::BACK && !isJumping && !isJump)
		{
			if(curAnimation != "eRun") PlayAnimation(/*curAnimation = */"eRun");
			angle = 45;		
		}			
	}
	
	if(lastMoveX == MOVEMENT::NONE)
	{
		angle = 0;
	}

	if(isJump && !isJumping)
	{
		if(lastMoveX == MOVEMENT::NONE && lastMoveZ == MOVEMENT::NONE)
		{
			if(curAnimation != "fJump1") PlayAnimation(/*curAnimation = */"fJump1");
		}
		else
		{
			if(curAnimation != "fJump0") PlayAnimation(/*curAnimation = */"fJump0");
		}
	}
		
	if(lastMoveZ == MOVEMENT::NONE && lastMoveX == MOVEMENT::NONE && !isJumping && !isJump && timeAFKcur < timeAFKmax)
	{
		string anim;
		
		if(form == FORM::HAMMER) anim = "hStand";
		else anim = "fStand";
				
		if(curAnimation != anim) PlayAnimation(curAnimation = anim);
	}
	
	if(timeAFKcur >= timeAFKmax)
	{
		string anim0;
		string anim1;
		
		if(form == FORM::HAMMER)
		{
			anim0 = "hIdle0";
			anim1 = "hIdle1";
		}
		else
		{
			anim0 = "fIdle0";
			anim1 = "fIdle1";			
		}

		if(curAnimation != anim1 && curAnimation != anim0) PlayAnimation(curAnimation = anim0);
		
		if(curAnimation == anim0 && MDL.animator->m_CurrentAnimation->IsFinished())
			if(curAnimation != anim1) PlayAnimation(curAnimation = anim1);
	}
}














