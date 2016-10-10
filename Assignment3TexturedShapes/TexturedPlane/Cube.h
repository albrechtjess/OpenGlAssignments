#ifndef CUBE_H
#define CUBE_H


#include <GL/glew.h>


class Cube
{

private:
	unsigned int vao;

public:
	Cube();

	void render();
};

#endif //PLANE_H
