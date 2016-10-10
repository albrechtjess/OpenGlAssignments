#include "Bunny.h"
using namespace std;
using namespace glm;

Bunny::Bunny(OBJLoader loader)
{
	vector<int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<int> vIndices;
	std::vector<glm::vec3> normals;
	vertices = loader.getVertices();
	normals = loader.getNormals();
	indices = loader.getVertexIndices();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	numInd = indices.size();

}

void Bunny::render() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, numInd, GL_UNSIGNED_INT, 0);
}


