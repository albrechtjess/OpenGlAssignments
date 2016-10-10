#include <iostream>    // std::cout  
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <string>         // std::string
#include <cstddef>         // std::size_t
#include "objloader.h"
#include <glm/gtc/matrix_transform.inl>

using namespace glm;

void OBJLoader:: computeNormals(std::vector<glm::vec3> const &vertices, std::vector<int> const &indices, std::vector<glm::vec3> &normals){
	    //sets normals to zero and size to vertices size
		normals.resize(vertices.size(), glm::vec3(0.0f, 0.0f, 0.0f));

		// Compute per-vertex normals here!
		for (int i = 0; i < indices.size(); i += 3) {
			//calculates normals of side and adds normal onto each vertex normal
			vec3 A = vec3(vertices[indices[i]]);
			vec3 B = vec3(vertices[indices[i + 1]]);
			vec3 C = vec3(vertices[indices[i + 2]]);
			vec3 normal = normalize(cross(B - A, C - A));
			normals[indices[i]] += normal;
			normals[indices[i+1]] += normal;
			normals[indices[i+2]] += normal;
		}
		//normalizes each vertex normal 
		for (int i = 0; i < normals.size(); i++)
		{
			normals[i] = normalize(normals[i]);
		}
}



//find bounding box find lowest and highest x, y, z
//center of the model, take that and subtract the center from the vertex 
// 2/ max(max(width,height), depth) is our scale factor 
//unitize model so that it fits in a box (-1,-1,-1) to (1,1,1)
//first find center of the bounding box 
	
	//scale = max(maxx, max y, max z) (max x - min x,  max y - min y, max z - min z)
		//devide all of our points by scale factor 
void OBJLoader::unitize(std::vector<glm::vec3> &vertices) {
	//decltype(vertices[0][0]);
	float max_x = vertices[0][0], min_x = vertices[0][0], max_y = vertices[0][1], min_y = vertices[0][1], max_z = vertices[0][2], min_z = vertices[0][2];
	for (int i = 1; i < vertices.size(); i++) {
		if (vertices[i][0] > max_x)max_x = vertices[i][0];
		if (vertices[i][0] < min_x)min_x = vertices[i][0];
		if (vertices[i][1] > max_y)max_y = vertices[i][1];
		if (vertices[i][1] < min_y)min_y = vertices[i][1];
		if (vertices[i][2] > max_z)max_z = vertices[i][2];
		if (vertices[i][2] < min_z)min_z = vertices[i][2];
	}
	float width = abs(max_x - min_x), 
		height = abs(max_y - min_y), 
		depth = abs(max_z - min_z)

		, mid_x = (max_x + min_x)/2,
		mid_y = (max_y + min_y)/2, 
		mid_z = (max_z + min_z)/2

		, scale_factor = 2 / glm::max(glm::max(width, height), depth);
	//float largest_axis = glm::max(glm::max(width, height), depth);
	printf("Scale Factor: %f\n", scale_factor);

	//mat4 translate_mat4 = mat4(vec4(mid_x, 0, 0, 0), vec4(0, mid_y, 0, 0),vec4(0, 0, mid_z, 0),vec4(0, 0, 0, 0));
	for (int i = 0; i < vertices.size();i++)
	{
		vertices[i][0] -= mid_x;
		vertices[i][1] -= mid_y;
		vertices[i][2] -= mid_z;
	}
	
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = vertices[i] * scale_factor;
	}
	
	object_bottom = vec3(0.0f, (min_y - mid_y) * scale_factor, 0.0f);
	//printf("Object_bottom in ObjLoader: %f", object_bottom[2]);
	//change the view
	//mat4 scale_mat4 = mat4(vec4(scale_factor, 0, 0, 0),vec4(0, scale_factor, 0, 0), vec4(0, 0, scale_factor, 0), vec4(0, 0, 0, 0));

}

OBJLoader::OBJLoader() :
	mVertices(0),
	mNormals(0),
	vIndices(0),
	object_bottom(vec3(0.0)),
nIndices()
{
	std::cout << "Called OBJFileReader constructor" << std::endl;
}

OBJLoader::~OBJLoader()
{
	std::cout << "Called OBJFileReader destructor" << std::endl;
}

bool OBJLoader::load(const char *filename)
{
	// Open OBJ file
	std::ifstream OBJFile(filename);
	if (!OBJFile.is_open()) {
		std::cerr << "Could not open " << filename << std::endl;
		return false;
	}
	
	// Extract vertices and indices
	std::string line;
	glm::vec3 vertex;
	glm::vec2 uv;
	while (!OBJFile.eof()) {
		std::getline(OBJFile, line);
		if ((line.find('#') == -1) && (line.find('m') == -1)){
			if (line.find('v') != -1) {

				if ((line.find('t') == -1) && (line.find('n') == -1)){
					std::istringstream vertexLine(line.substr(2));
					vertexLine >> vertex.x;
					vertexLine >> vertex.y;
					vertexLine >> vertex.z;
				    mVertices.push_back(vertex);
				}
			}

			else if (line.find("f ") != -1) {
				std::istringstream faceLine(line);
				std::string val1;
				faceLine >> val1;
				int val;
				for (int n = 0; n < 3; n++){
					faceLine >> val;
					
					vIndices.push_back(val- 1);
					nIndices.push_back(val - 1);

				}
			}
	    }
	}

	// Close OBJ file
	OBJFile.close();

	
	// Compute normals
	computeNormals(mVertices, vIndices, mNormals);
	
	// Unitize Object
	unitize(mVertices);

	// Display log message
	return true;
}

std::vector<glm::vec3> const &OBJLoader::getVertices() const
{
	return mVertices;
}

std::vector<glm::vec3> const &OBJLoader::getNormals() const
{
	return mNormals;
}


std::vector<int> const &OBJLoader::getVertexIndices() const
{
	return vIndices;
}

std::vector<int> const &OBJLoader::getNormalIndices() const
{
	return nIndices;
}

glm::vec3 &OBJLoader::getBottom()
{
	return object_bottom;
}
