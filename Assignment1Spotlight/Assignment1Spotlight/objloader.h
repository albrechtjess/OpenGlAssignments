#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <vector>
#include <glm/glm.hpp>


	class OBJLoader {
	public:
		//! Constructor
		//!
		OBJLoader();

		//! Destructor
		//!
		~OBJLoader();

		
		bool load(const char *filename);

		std::vector<glm::vec3> const &getVertices() const;
		std::vector<glm::vec3> const &getNormals() const;
		std::vector<int> const &getVertexIndices() const;
		std::vector<int> const &getNormalIndices() const;
		glm::vec3 &OBJLoader::getBottom();
		 

		void computeNormals(std::vector<glm::vec3> const &vertices,
			std::vector<int> const &indices,
			std::vector<glm::vec3> &normals);

		void OBJLoader::unitize(std::vector<glm::vec3> &vertices);

	private:
		std::vector<glm::vec3> mVertices;
		std::vector<glm::vec3> mNormals;
		std::vector<int> vIndices;
		std::vector<int> nIndices;
		glm::vec3 object_bottom;
	};

#endif

