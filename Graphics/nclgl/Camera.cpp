#include "Camera.h"

/*
Polls the camera for keyboard / mouse movement.
Should be done once per frame! Pass it the msec since
last frame (default value is for simplicities sake...)
*/
void Camera::UpdateCamera(float msec)	{

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_F) && !manualMovement) {
		manualMovement = true;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_G) && manualMovement) {
		manualMovement = false;
		position = getPoint(segmentIndex, t - 0.1);
	}
	
	if (manualMovement)
		manualMove(msec);
	else
		autoMove(msec);
}

void Camera::constructSegments(Vector3 p0, Vector3 p1, Vector3 p2, Vector3 p3)
{
	float t0 = 0.0f;
	float t1 = t0 + pow(distance(p0, p1), alpha);
	float t2 = t1 + pow(distance(p1, p2), alpha);
	float t3 = t2 + pow(distance(p2, p3), alpha);

	Vector3 m1 = mult((1.0f - tension), mult((t2 - t1), ((p1 - p0) / (t1 - t0) - (p2 - p0) / (t2 - t0) + (p2 - p1) / (t2 - t1))));

	Vector3 m2 = mult((1.0f - tension), mult((t2 - t1), ((p2 - p1) / (t2 - t1) - (p3 - p1) / (t3 - t1) + (p3 - p2) / (t3 - t2))));

	Segment segment;
	segment.a = mult(2.0f, (p1 - p2)) + m1 + m2;
	segment.b = mult(-3.0f, (p1 - p2)) - m1 - m1 - m2;
	segment.c = m1;
	segment.d = p1;
	segments.push_back(segment);
}

Vector3 Camera::getPoint(int segmentIndex, float t)
{
	return segments[segmentIndex].a * t * t * t +
		segments[segmentIndex].b * t * t +
		segments[segmentIndex].c * t +
		segments[segmentIndex].d;
}

float Camera::distance(Vector3 first, Vector3 second)
{
	// Calculating distance 
	return sqrt(pow((first.x - second.x), 2) + pow((first.y - second.y), 2) + pow((first.z - second.z), 2));
}

void Camera::autoMove(float msec)
{
	if (distance(position, getPoint(segmentIndex, t)) < 1.0f)
	{
		t += 0.1f;
		if (t > 1.0f)
		{
			t = 0;
			segmentIndex++;

			if (segmentIndex >= segments.size())
				segmentIndex = 0;
		}
	}

	Vector3 target = getPoint(segmentIndex, t);

	//Vector3 target = positions[1];

	Vector3 heading = target - position;
	heading = heading / distance(position, target);

	position += heading * 1.0f;
}

void Camera::manualMove(float msec)
{
	//Update the mouse by how much
	pitch -= (Window::GetMouse()->GetRelativePosition().y);
	yaw -= (Window::GetMouse()->GetRelativePosition().x);

	//Bounds check the pitch, to be between straight up and straight down ;)
	pitch = min(pitch, 90.0f);
	pitch = max(pitch, -90.0f);

	if (yaw < 0) {
		yaw += 360.0f;
	}
	if (yaw > 360.0f) {
		yaw -= 360.0f;
	}

	msec *= 5.0f;

	// Roll
	if (Window::GetKeyboard()->KeyDown(KEYBOARD_Q)) {
		roll -= 1.0f;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_E)) {
		roll += 1.0f;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_5)) {
		xSpeed += 0.1f;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_6)) {
		xSpeed -= 0.1f;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_7)) {
		ySpeed += 0.1f;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_8)) {
		ySpeed -= 0.1f;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_W)) {
		position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * (msec * xSpeed);
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_S)) {
		position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(0, 0, -1) * (msec * xSpeed);
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_A)) {
		position += Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * (msec * xSpeed);
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_D)) {
		position -= Matrix4::Rotation(yaw, Vector3(0, 1, 0)) * Vector3(-1, 0, 0) * (msec * xSpeed);
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SHIFT)) {
		position.y += (msec * ySpeed);
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_SPACE)) {
		position.y -= (msec * ySpeed);
	}
}

/*
Generates a view matrix for the camera's viewpoint. This matrix can be sent
straight to the shader...it's already an 'inverse camera' matrix.
*/
Matrix4 Camera::BuildViewMatrix()	{

	if (manualMovement) 
	{
		//Why do a complicated matrix inversion, when we can just generate the matrix
		//using the negative values ;). The matrix multiplication order is important!
		return	Matrix4::Rotation(-pitch, Vector3(1,0,0)) * 
				Matrix4::Rotation(-yaw, Vector3(0,1,0)) * 
				Matrix4::Rotation(-roll, Vector3(0, 0, 1)) *
				Matrix4::Translation(-position);
	}
	else 
	{
		return Matrix4::lookAt(position, look, Vector3(0.0f, 1.0f, 0.0f));
	}
};
