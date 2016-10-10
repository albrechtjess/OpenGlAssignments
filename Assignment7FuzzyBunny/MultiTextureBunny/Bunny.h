#ifndef BUNNY_H
#define BUNNY_H


#include <GL/glew.h>
#include <vector>
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "objloader.h"

class Bunny
{

private:
	unsigned int vao;

public:
	Bunny(OBJLoader);
	int numInd;
	void render();
};

#endif 
