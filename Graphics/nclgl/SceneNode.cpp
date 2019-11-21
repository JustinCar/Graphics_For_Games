#include "SceneNode.h"

SceneNode::SceneNode(Mesh* mesh, Vector4 colour) {
	this -> mesh = mesh;
	this -> colour = colour;
	parent = NULL;
	modelScale = Vector3(1, 1, 1);
	boundingRadius = 1.0f;
	distanceFromCamera = 0.0f;
	castShadows = false;
	recieveShadows = false;
	elapsedTime = 0;
}

SceneNode ::~SceneNode(void) {
	for (unsigned int i = 0; i < children.size(); ++i) {
		delete children[i];
	}
}

void SceneNode::AddChild(SceneNode* s) {
	if (s != this) 
	{
		children.push_back(s);
		s -> parent = this;
	}
}

void SceneNode::RemoveChild(SceneNode* s)
{
	std::vector<SceneNode*>::iterator pos = std::find(children.begin(), children.end(), s);
	if (pos != children.end())
		children.erase(pos);
}

void SceneNode::Draw(OGLRenderer& r, float msec, GLuint shadowTex, int drawCount) {
	
	if (mesh) { 
		mesh->Draw();
	}
}

void SceneNode::Update(float msec) {
	if (parent) { // This node has a parent ...
		worldTransform = parent -> worldTransform * transform;
	}
	else { // Root node , world transform is local transform !
		worldTransform = transform;
	}

	for (vector < SceneNode* >::iterator i = children.begin();
		i != children.end(); ++i) {
		(*i) -> Update(msec);
	}
}
