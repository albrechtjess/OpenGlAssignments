#include "Sphere.h"
#include <math.h>
#include <vector>
#include <array>

#define GLM_FORCE_RADIANS 
#include <glm/gtc/matrix_transform.inl>

using namespace glm;
using namespace std;
Sphere::Sphere()
{
	static const double kPI = 3.1415926535897932384626433832795;
	int const vert = 19, ind = vert+1;
	float x, y, z, theta = 0, beta, phi = 0, radius = 1.0, step = (2*kPI)/18, phiStep = (kPI)/18;
	array<float, 18*18*3> vertices;
	array<GLuint, 18*18*6> indices;
	int index = 0, index2 = 0;
	for (int i = 0; i < 18; i++)//possibly set <=
	{
		for (int j = 0; j < 18; j++)//possibly set <=]]]
		{
			x = radius*sin(theta)*sin(phi);
			y = radius*cos(phi);
			z = radius*cos(theta)*sin(phi);

			theta += step;

			vertices[index] = x;
			vertices[index + 1] = y;
			vertices[index + 2] = z;

			if(i == 17 && j == 17)
			{
				indices[index2] = j + 18 * i;
				indices[index2 + 1] = 18 * i;
				indices[index2 + 2] = j + 18 * i;

				indices[index2 + 3] = j + 18 * i;
				indices[index2 + 4] = 18 + 18 * i;
				indices[index2 + 5] = j + 18 * i;
			}
			else if(i == 17)
			{
				indices[index2] = j + 18 * i;
				indices[index2 + 1] = j + 1 + 18 * i;
				indices[index2 + 2] = j + 18 * i;


				indices[index2 + 3] = j + 18 * i;
				indices[index2 + 4] = j + 1 + 18 * i;
				indices[index2 + 5] = j + 1 + 18 * i;
				
				
				/*indices[index2] = j + 18 * i;
				indices[index2 + 1] = j + 1 + 18 * i;
				indices[index2 + 2] = j + 18 * i;


				indices[index2 + 3] = j + 18 * i;
				indices[index2 + 4] = j + 1 + 18 * i;
				indices[index2 + 5] = j + 18 * i;*/
			}
			else if(j == 17)
			{
				indices[index2] = j + 18 * i;
				indices[index2 + 1] = 18 * i;
				indices[index2 + 2] = j + 18 + 18 * i;

				indices[index2 + 3] = j + 18 + 18 * i;
				indices[index2 + 4] = 18 + 18 * i;
				indices[index2 + 5] = j + 1 + 18 * i;
			}
			else
			{
				indices[index2] = j + 18 * i;
				indices[index2 + 1] = j + 1 + 18 * i;
				indices[index2 + 2] = j + 18 + 18 * i;

				
				indices[index2 + 3] = j + 18 + 18 * i;
				indices[index2 + 4] = j + 19 + 18 * i;
				indices[index2 + 5] = j + 1 + 18 * i;
			}
			index+=3;
			index2 += 6;
		}
		phi += phiStep;
	}

	array<float,(18*18*3)>normals;
	for (int x = 0; x < normals.size(); x++)
	{
		normals[x] = 0;
	}
	
		// Compute per-vertex normals here!
		for (int i = 0; i < indices.size(); i += 9) {
			//calculates normals of side and adds normal onto each vertex normal
			vec3 A = vec3(vertices[indices[i]], vertices[indices[i+1]], vertices[indices[i+2]]);
			vec3 B = vec3(vertices[indices[i+3]], vertices[indices[i + 4]], vertices[indices[i + 5]]);
			vec3 C = vec3(vertices[indices[i + 6]], vertices[indices[i + 7]], vertices[indices[i + 8]]);
			vec3 normal = normalize(cross(B - A, C - A));
			normals[indices[i]] += normal.x;
			normals[indices[i + 1]] += normal.y;
			normals[indices[i + 2]] += normal.z;

			normals[indices[i + 3]] += normal.x;
			normals[indices[i + 4]] += normal.y;
			normals[indices[i + 5]] += normal.z;

			normals[indices[i + 6]] += normal.x;
			normals[indices[i + 7]] += normal.y;
			normals[indices[i + 8]] += normal.z;
		}
		//normalizes each vertex normal 
		for (int i = 0; i < normals.size(); i+=3)
		{
			normals[i] += normalize(normals[i]);
			normals[i + 1] += normalize(normals[i + 1]);
			normals[i + 2] += normalize(normals[i + 2]);
			//printf("normal[%i]: %f, %f , %f\n", i, normals[i].x, normals[i].y, normals[i].z);
		}
	//*/
	array<float,(18* 18 * 2)>textures;
	for (int u = 0; u < textures.size(); u++)
	{
		textures[u] = 0.0;
		textures[u++] = 0.0;
		textures[u++] = 1.0;
		textures[u++] = 0.0;
		textures[u++] = 1.0;
		textures[u++] = 1.0;
		textures[u++] = 0.0;
		textures[u++] = 1.0;

	};

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int handle[4];
	glGenBuffers(4, handle);

	glBindBuffer(GL_ARRAY_BUFFER, handle[0]);
	glBufferData(GL_ARRAY_BUFFER, 18 * 18 * 3 * sizeof(float), vertices.data(), GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, handle[1]);
	glBufferData(GL_ARRAY_BUFFER, 18 * 18 * 3 * sizeof(float), normals.data(), GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal

	glBindBuffer(GL_ARRAY_BUFFER, handle[2]);
	glBufferData(GL_ARRAY_BUFFER, 18 * 18 * 2 * sizeof(float), textures.data(), GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);  // texture coords

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, handle[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18 * 18 *6 * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Sphere::render() {
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES,18*18*6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}


