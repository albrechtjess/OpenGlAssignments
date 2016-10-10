#include "Bunny.h"
using namespace std;
using namespace glm;

Bunny::Bunny(OBJLoader loader)
{
	vector<int> indices;
	std::vector<glm::vec3> vertices;
	std::vector<int> vIndices;
	//std::vector <array<float,2>> textures;
	std::vector<glm::vec2> textures;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tan1;
	std::vector<glm::vec3> tan2;
	std::vector<glm::vec4> tangents;
	vec3 bottom;
	vertices = loader.getVertices();
	normals = loader.getNormals();
	textures = loader.getTextures();
	indices = loader.getVertexIndices();
	bottom = loader.getBottom();
	//tan1.resize(vertices.size(), vec3(0));
	//tan2.resize(vertices.size(), vec3(0));
	//tangents.resize(vertices.size(), vec4(0));
	
	for (long x = 0; x < vertices.size(); x+=3)
	{
		
		long i0 = indices[x];
		//printf("i0: %f\n", i0);
		long i1 = indices[x+1];
		long i2 = indices[x+2];

		vec3 v1 = vertices[i0];
		vec3 v2 = vertices[i1];
		vec3 v3 = vertices[i2];

		vec2 w1 = textures[i0];
		vec2 w2 = textures[i1];
		vec2 w3 = textures[i2];

		float x1 = v2.x - v1.x;
		float x2 = v3.x - v1.x;
		float y1 = v2.y - v1.y;
		float y2 = v3.y - v1.y;
		float z1 = v2.z - v1.z;
		float z2 = v3.z - v1.z;

		float s1 = w2.x - w1.x;
		float s2 = w3.x - w1.x;
		float t1 = w2.y - w1.y;
		float t2 = w3.y - w1.y;

		
		float r = 1.0f / ((s1 * t2) - (s2 * t1)); //occasionally infinity 
		
		//printf("r: %f\n", r);

		vec3 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
			(t2 * z1 - t1 * z2) * r);
		vec3 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
			(s1 * z2 - s2 * z1) * r);
		
		/*
		tan1[i0] += sdir;
		tan1[i1] += sdir;
		tan1[i2] += sdir;

		tan2[i0] += tdir;
		tan2[i1] += tdir;
		tan2[i2] += tdir;
		*/
		tan1.push_back(sdir);
		tan1.push_back(sdir);
		tan1.push_back(sdir);

		tan2.push_back(tdir);
		tan2.push_back(tdir);
		tan2.push_back(tdir);

	}

	//Gram-Schmidt orthogonalization
	//Next B is a cross 

	for (long a = 0; a < vertices.size(); a++)
	{
		//printf("a: %d\n", a);
		const vec3 n = normals[a];
		const vec3 t = tan1[a];
		
		// Gram-Schmidt orthogonalize
		tangents.push_back(normalize(vec4(t - n * dot(n, t), 0)));
		/*if (isnan(tangents[a].x))
		{
			printf("Nand Point: %d\nn: %f %f %f\nt: %f, %f, %f\n", a, n.x, n.y, n.z, t.x, t.y, t.z);
			a = vertices.size();
		}
		// Calculate handedness
		else 
		*/
		tangents.back().w = (dot(cross(n, t), tan2[a]) < 0.0f) ? -1.0F : 1.0F;
		
		//printf("x: %f, y: %f, z: %f, w: %f\n", tangents[a].x, tangents[a].y, tangents[a].z, tangents[a].w);
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int handle[5];
	glGenBuffers(5, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	//glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(textures[0]), &textures[0], GL_STATIC_DRAW);
	glBufferData(GL_ARRAY_BUFFER, textures.size() * sizeof(glm::vec2), &textures[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, handle[4]);
	glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(glm::vec4), &tangents[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)3, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);  // Vertex normal

	glBindVertexArray(0);

	numInd = indices.size();

}

void Bunny::render() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES,numInd, GL_UNSIGNED_INT, 0);
}


