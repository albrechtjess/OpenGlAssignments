#include "Cube.h"


Cube::Cube()
{
	float side = 1.0f;
	float side2 = side / 2.0f;

	float vertices[24 * 3] = {
		// Front
		1.0f, 1.0f, 1.0f,  // v0,v1,v2,v3 (front)
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,  // v0,v3,v4,v5 (right)
		1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f, // v0,v5,v6,v1 (top)
		1.0f, 1.0f, -1.0,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,  // v1,v6,v7,v2 (left)
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, // v7,v4,v3,v2 (bottom)
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f, // v4,v7,v6,v5 (back)
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f
	};

	float normals[24 * 3] = {
		0.0f, 0.0f, 1.0f, // v0,v1,v2,v3 (front)
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, // v0,v3,v4,v5 (right)
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, // v0,v5,v6,v1 (top)
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,// v1,v6,v7,v2 (left)
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, // v7,v4,v3,v2 (bottom)
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, 0.0f, -1.0f, // v4,v7,v6,v5 (back)
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f
	};

	float textures[24 * 2] = {
		// Front
		0.0, 0.0,      //-2.0f, -2.0f,  //  
		2.0, 0.0,      // 4.0f, -2.0f,  //  
		2.0f, 2.0f,    // 4.0f, 4.0f, //   //
		0.0, 2.0,      // -2.0f, 4.0f,  // 

		0.0, 0.0,      //-2.0f, -2.0f,  //  
		2.0, 0.0,      // 4.0f, -2.0f,  //  
		2.0f, 2.0f,    // 4.0f, 4.0f, //   //
		0.0, 2.0,      // -2.0f, 4.0f,  // 

		0.0, 0.0,      //-2.0f, -2.0f,  //  
		2.0, 0.0,      // 4.0f, -2.0f,  //  
		2.0f, 2.0f,    // 4.0f, 4.0f, //   //
		0.0, 2.0,      // -2.0f, 4.0f,  // 

		0.0, 0.0,      //-2.0f, -2.0f,  //  
		2.0, 0.0,      // 4.0f, -2.0f,  //  
		2.0f, 2.0f,    // 4.0f, 4.0f, //   //
		0.0, 2.0,      // -2.0f, 4.0f,  // 

		0.0, 0.0,      //-2.0f, -2.0f,  //  
		2.0, 0.0,      // 4.0f, -2.0f,  //  
		2.0f, 2.0f,    // 4.0f, 4.0f, //   //
		0.0, 2.0,      // -2.0f, 4.0f,  // 

		0.0, 0.0,      //-2.0f, -2.0f,  //  
		2.0, 0.0,      // 4.0f, -2.0f,  //  
		2.0f, 2.0f,    // 4.0f, 4.0f, //   //
		0.0, 2.0,      // -2.0f, 4.0f,  // 
	};

	GLuint indices[] = {
		0, 1, 2, 2, 3, 0,            // front
		4, 5, 6, 6, 7, 4,            // right
		8, 9, 10, 10, 11, 8,         // top
		12, 13, 14, 14, 15, 12,      // left
		16, 17, 18, 18, 19, 16,      // bottom
		20, 21, 22, 22, 23, 20
	};

	for (int x = 0; x < 24 * 3; x += 3)
	{
		vertices[x] -= 2;
		vertices[x + 1] -= 2;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, 24 * 3 * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, 24 * 2 * sizeof(float), textures, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);



}

void Cube::render() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


