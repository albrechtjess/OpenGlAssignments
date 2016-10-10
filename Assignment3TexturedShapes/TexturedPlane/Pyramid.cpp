#include "Pyramid.h"


Pyramid::Pyramid()
{
	int const vert = 16, ind = 18;
	float vertices[vert * 3] = {
		// Front
		1.0f, -1.0f, 1.0f,  // front //0
		-1.0f, -1.0f, 1.0f,	//1
		0.0f, 0.0f, 0.0f,	//2

		1.0f, -1.0f, -1.0f,  // back //3
		-1.0f, -1.0f, -1.0f,	 //4 
		0.0f, 0.0f, 0.0f,	//2

		1.0f, -1.0f, 1.0f,  // right //0
		1.0f, -1.0f, -1.0f, //3
		0.0f, 0.0f, 0.0f, //2 

		-1.0f, -1.0f, 1.0f,  // left//1
		-1.0f, -1.0f, -1.0f, //4
		0.0f, 0.0f, 0.0f, //2

		1.0f, -1.0f, 1.0f, // bottom //0
		1.0f, -1.0f, -1.0f,	// 3
		-1.0f, -1.0f, 1.0f,	//1
		-1.0f, -1.0f, -1.0f, //4

	};

	float normals[vert * 3] = {
		0.0f, 0.0f, 1.0f, // v0,v1,v2,v3 (front)
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,		
		
		0.0f, 0.0f, -1.0f, // v4,v7,v6,v5 (back)
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		
		1.0f, 0.0f, 0.0f, // v0,v3,v4,v5 (right)
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		
		-1.0f, 0.0f, 0.0f,// v1,v6,v7,v2 (left)
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,

		0.0f, -1.0f, 0.0f, // v7,v4,v3,v2 (bottom)
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f //*/
	};

	float textures[16 * 2] = {
		// Front
		0.0f, 0.0f,  // front //0
		1.0f, 0.0f, 	//1
		0.5f, 1.0f, 	//2

		0.0f, 0.0f, 
		1.0f, 0.0f,
		0.5f, 1.0f, 

		0.0f, 0.0f, 
		1.0f, 0.0f,
		0.5f, 1.0f,

		0.0f, 0.0f,
		1.0f, 0.0f,
		0.5f, 1.0f,

		1.0f, 0.0f, // bottom
		1.0f, 1.0f, 
		0.0f, 0.0f,
		0.0f, 1.0f, 
};	

	/*
		0.0, 0.0,      //-2.0f, -2.0f,  //  
		2.0, 0.0,      // 4.0f, -2.0f,  //  
		1.0f, 2.0f,    // 4.0f, 4.0f, //   //
		0.0f, 2.0f,

		0.0, 0.0,      //-2.0f, -2.0f,  //  
		2.0, 0.0,      // 4.0f, -2.0f,  //  
		1.0f, 2.0f,    // 4.0f, 4.0f, //   //
		0.0f, 2.0f,

		0.0, 0.0,      //-2.0f, -2.0f,  //  
		2.0, 0.0,      // 4.0f, -2.0f,  //  
		1.0f, 2.0f,    // 4.0f, 4.0f, //  
		0.0f, 2.0f,

		0.0, 0.0,      //-2.0f, -2.0f,  //  
		2.0, 0.0,      // 4.0f, -2.0f,  //  
		2.0f, 2.0f,    // 4.0f, 4.0f, //   //
		0.0, 2.0,      // -2.0f, 4.0f,  // 
	}; // */

	GLuint indices[] = {
		0, 1, 2,            // front
		3, 4, 5,
		6, 7, 8,
		9, 10, 11,
		12, 13, 14, 
		14, 13, 15

	};

	for (int x = 0; x < vert * 3; x+=3)
	{
		vertices[x] += 2;
		vertices[x + 1] -= 2;
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, vert * 3 * sizeof(float), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, vert * 3 * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, 16 * 2 * sizeof(float), textures, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ind * sizeof(GLuint), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);



}

void Pyramid::render() {
	glBindVertexArray(vao);
	
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


