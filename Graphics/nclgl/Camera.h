/******************************************************************************
Class:Camera
Implements:
Author:Rich Davison	<richard.davison4@newcastle.ac.uk>
Description:FPS-Style camera. Uses the mouse and keyboard from the Window
class to get movement values!

-_-_-_-_-_-_-_,------,   
_-_-_-_-_-_-_-|   /\_/\   NYANYANYAN
-_-_-_-_-_-_-~|__( ^ .^) /
_-_-_-_-_-_-_-""  ""   

*//////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Window.h"
#include "Matrix4.h"
#include "Vector3.h"

class Camera	{
public:
	Camera(void){
		yaw		= 0.0f;
		pitch	= 0.0f;
		roll = 0.0f;
		xSpeed = 0.06f;
		ySpeed = 0.06f;

		look = Vector3(506.605, 116.561, 560.64);

		positions.push_back(Vector3(160.623, 497.495, 1142.51));
		positions.push_back(Vector3(1016.81, 497.495, 1101.25));
		positions.push_back(Vector3(984.046, 497.495, 53.6444));
		positions.push_back(Vector3(32.6108, 185.228, -17.4477));
		positions.push_back(Vector3(-71.6629, 111.208, 540.399));
		positions.push_back(Vector3(356.891, 111.245, 492.283));
		positions.push_back(Vector3(507.036, 145.689, 661.651));
		positions.push_back(Vector3(695.728, 200.252, 1130.64));

		manualMovement = false;

		alpha = 0.5;
		tension = 0;
		t = 0;
		segmentIndex = 0;

		int i = 0;
		while ((i + 3) < positions.size())
		{
			constructSegments(positions[i], positions[i+1], positions[i+2], positions[i+3]);
			i++;
		}

		position = getPoint(0, 0);
	};

	Camera(float pitch, float yaw, float roll, Vector3 position){
		this->pitch		= pitch;
		this->yaw		= yaw;
		this->roll = roll;
		this->position	= position;
		xSpeed = 0.06f;
		ySpeed = 0.06f;
	}

	~Camera(void){};

	void UpdateCamera(float msec = 10.0f);

	void constructSegments(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3);
	float distance(Vector3 first, Vector3 second);
	Vector3 getPoint(int segmentIndex, float t);

	Vector3  mult(float a, Vector3 b){
		return Vector3(b.x * a, b.y * a, b.z * a);
	}

	void manualMove(float msec = 10.0f);
	void autoMove(float msec = 10.0f);

	//Builds a view matrix for the current camera variables, suitable for sending straight
	//to a vertex shader (i.e it's already an 'inverse camera matrix').
	Matrix4 BuildViewMatrix();

	//Gets position in world space
	Vector3 GetPosition() const { return position;}
	//Sets position in world space
	void	SetPosition(Vector3 val) { position = val;}

	//Gets yaw, in degrees
	float	GetYaw()   const { return yaw;}
	//Sets yaw, in degrees
	void	SetYaw(float y) {yaw = y;}

	//Gets pitch, in degrees
	float	GetPitch() const { return pitch;}
	//Sets pitch, in degrees
	void	SetPitch(float p) {pitch = p;}

	//Gets xSpeed
	float	GetXSpeed() const { return xSpeed; }
	//Sets xSpeed
	void	SetXSpeed(float x) { xSpeed = x; }

	//Gets ySpeed
	float	GetYSpeed() const { return ySpeed; }
	//Sets ySpeed
	void	SetYSpeed(float y) { ySpeed = y; }

protected:
	struct Segment
	{
		Vector3 a;
		Vector3 b;
		Vector3 c;
		Vector3 d;
	};

	float xSpeed;
	float ySpeed;
	float	yaw;
	float	pitch;
	float	roll;
	Vector3 position;

	vector<Vector3> positions;
	vector<Segment> segments;

	float alpha;
	float tension;
	Vector3 look;
	float t;
	int segmentIndex;

	bool manualMovement;
};