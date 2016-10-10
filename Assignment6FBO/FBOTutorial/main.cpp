#include <GL/glew.h>
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <map>
#include <set>

#define GLM_FORCE_RADIANS 
#define TWOPI 2*3.1415926535897932384626433832795

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

#include "objloader.h"
#include "shader.h"

using namespace glm;
using namespace std;

mat4 view;
mat4 model;
mat4 projection;
mat4 model_view;
mat4 transformation_matrix;
mat4 mvp;
mat3 normalmatrix;

GLuint projection_matrix_loc;
GLuint model_view_matrix_loc;
GLuint program;
GLuint programOne;
float aspect = 0.0;
GLfloat angle = 0.0;
int viewport[4];
// Properties
GLuint screenWidth = 800, screenHeight = 600;

ILboolean success = true;
bool mipmap = true;
unsigned int imageID, imageID1;

// Set the object data (buffers, vertex attributes)
GLfloat cubeVertices[] = {
	// Positions          // Texture Coords
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
	0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
	-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
	-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};
GLfloat floorVertices[] = {
	// Positions          // Texture Coords 
	5.0f, -0.5f, 5.0f,   2.0f, 0.0f,
	-5.0f, -0.5f, 5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

	5.0f, -0.5f, 5.0f,   2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	5.0f, -0.5f, -5.0f,    2.0f, 2.0f
};
GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// Positions   // TexCoords
	-1.0f, 1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 1.0f, 0.0f,

	-1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, -1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 1.0f, 1.0f
};

// Setup screen VAO
GLuint quadVAO, quadVBO;

GLuint  bunnyVAO, bunnyVBO;
// Framebuffers
GLuint framebuffer;
GLuint rbo;
GLuint textureColorbuffer;

static const double kPI = 3.1415926535897932384626433832795;
static int gViewportWidth, gViewportHeight;
static double gCameraScale = 1.0;
static double gCameraTranslationX = 0;
static double gCameraTranslationY = 0;
static bool gIsRotatingCamera = false;
static bool gIsScalingCamera = false;
static bool gIsTranslatingCamera = false;
static int gLastMouseX, gLastMouseY;
mat4 gCameraRotation;



void Initialize();
void display(void);
void makeIdentity();
void Reshape(int width, int height);
void glutMouse(int button, int state, int x, int y);
void glutMotion(int x, int y);
double projectToTrackball(double radius, double x, double y);
unsigned int loadTexture(std::string filename);
void setMatrices();
vector<int> indices;
std::vector<glm::vec3> vertices;
std::vector<glm::vec2> textures;
std::vector<glm::vec3> normals;

// Generates a texture that is suited for attachments to a framebuffer
GLuint generateAttachmentTexture()
{
	

	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

unsigned int loadTexture(std::string filename) {

	ILboolean success;
	unsigned int imageID;

	// Load Texture Map
	ilGenImages(1, &imageID);

	ilBindImage(imageID); /* Binding of DevIL image name */
	ilEnable(IL_ORIGIN_SET);
	ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
	success = ilLoadImage((ILstring)filename.c_str());

	if (!success) {
		std::cout << "Couldn't load the following texture file: " << filename.c_str() << std::endl;
		// The operation was not sucessfull hence free image and texture 
		ilDeleteImages(1, &imageID);
		return 0;
	}

	// add information to the log
	ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

	GLuint tid;
	glGenTextures(1, &tid);
	glBindTexture(GL_TEXTURE_2D, tid);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0,
		GL_RGBA, GL_UNSIGNED_BYTE, ilGetData());

	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,  GL_REPEAT);	// Use GL_MIRRORED_REPEAT to prevent white borders. Due to interpolation it takes value from next repeat 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glBindTexture(GL_TEXTURE_2D, 0);

	/* Because we have already copied image data into texture data
	we can release memory used by image. */
	ilDeleteImages(1, &imageID);
	return tid;
}



void Initialize(void){
	

	// Create and compile our GLSL program from the shaders
	
	OBJLoader loader;
	bool loadfile = loader.load("bunny.obj");
	
	program = LoadShaders("texture.vs", "texture.fs");
	
	programOne = LoadShaders("framebuffer.vs", "framebuffer.frag");

	glUseProgram(program);
	view = glm::lookAt(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	projection = mat4(1.0f);

	vec3 light_intensity(1.0f, 1.0f, 1.0f);
	vec4 light_position(10.0f, 10.0f, 10.0f, 1.0f);
	vec3 material_ambient(0.9, 0.5, 0.3);
	vec3 material_diffuse(0.9, 0.5, 0.3);
	vec3 material_specular(0.8, 0.8, 0.8);

	GLfloat shininess = 100.0f;
	glUniform3fv(glGetUniformLocation(program, "Light.Intensity"), 1, (GLfloat*)&light_intensity);
	glUniform4fv(glGetUniformLocation(program, "Light.Position"), 1, (GLfloat*)&light_position);
	glUniform3fv(glGetUniformLocation(program, "Material.Ka"), 1, (GLfloat*)&material_ambient);
	glUniform3fv(glGetUniformLocation(program, "Material.Kd"), 1, (GLfloat*)&material_diffuse[0]);
	glUniform3fv(glGetUniformLocation(program, "Material.Ks"), 1, (GLfloat*)&material_specular[0]);
	glUniform1f(glGetUniformLocation(program, "Material.Shininess"), shininess);

	vertices = loader.getVertices();
	indices = loader.getVertexIndices();
	normals = loader.getNormals();

	glGenVertexArrays(1, &bunnyVAO);
	glBindVertexArray(bunnyVAO);
	unsigned int bunnyVBO[2];
	GLuint bunnyEBO;
	glGenBuffers(2, bunnyVBO);

	glBindBuffer(GL_ARRAY_BUFFER, bunnyVBO[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, bunnyVBO[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal

	glGenBuffers(1, &bunnyEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bunnyEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	//*/


	glUseProgram(programOne);
	// screen VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
	
	
	// Framebuffers
	
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Create a color attachment texture
	textureColorbuffer = generateAttachmentTexture();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// Create a renderbuffer object for depth and stencil attachment 

	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
	// Now that created the framebuffer and added all attachments we want to check if it is  complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glEnable(GL_DEPTH_TEST);
	
}
void Display(void)
{
	/////////////////////////////////////////////////////
	// Bind to framebuffer and draw to color texture 
	// as we normally would.
	// //////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Clear all attached buffers        
	glViewport(0, 0, screenWidth, screenHeight);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

	
    glUseProgram(program);
	
	transformation_matrix = mat4(1.0f);
	mat4 scaled = glm::scale(glm::mat4(1.0f), glm::vec3(gCameraScale, gCameraScale, gCameraScale));
	mat4 translated = glm::translate(glm::mat4(1.0f), glm::vec3(gCameraTranslationX, gCameraTranslationY, 0.0));
	transformation_matrix = translated*gCameraRotation*scaled;

	model = mat4(1.0f);

	setMatrices();
	glBindVertexArray(bunnyVAO);
	glDrawElements(GL_TRIANGLES,indices.size(), GL_UNSIGNED_INT,0);
	glBindVertexArray(0);
	//*/

	/////////////////////////////////////////////////////
	// Bind to default framebuffer again and draw the quad plane with attched screen texture.
	// //////////////////////////////////////////////////
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Clear all relevant buffers
	glViewport(0, 0, gViewportWidth, gViewportHeight);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	// Draw Screen
	glUseProgram(programOne);
	projection = glm::perspective(45.0f, aspect, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(programOne, "view"), 1, GL_FALSE, (GLfloat*)&view[0]);
	glUniformMatrix4fv(glGetUniformLocation(programOne, "projection"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	model = mat4(1.0f);
	model *= transformation_matrix;
	glUniformMatrix4fv(glGetUniformLocation(programOne, "model"), 1, GL_FALSE, (GLfloat*)&model[0]);
	
	// Choose whether to draw in wireframe mode or not
	//glPolygonMode(GL_FRONT, GL_LINE);
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// Use the color attachment texture as the texture of the quad plane
	
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	
	glutSwapBuffers();
}

void setMatrices() {
	model *= transformation_matrix;
	normalmatrix = mat3(1.0f);
	mvp = mat4(1.0f);
	model_view = view*model;
	normalmatrix = mat3(vec3(model_view[0]), vec3(model_view[1]), vec3(model_view[2]));
	projection = glm::perspective(70.0f, aspect, 0.3f, 100.0f);
	mvp = projection*model_view;

	glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
	glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normalmatrix[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
}

void Reshape(int width, int height)
{
	
	gViewportWidth = width;
	gViewportHeight = height;
	glViewport(0, 0, width, height);

	
	aspect = float(width) / float(height);
}

bool poly = true;
void keyboard(unsigned char key, int x, int y){
	switch (key){
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;
	
	case 'l':
	case 'L':
		poly = !poly;
		if (poly)
		{
			glPolygonMode(GL_FRONT, GL_LINE);
		}
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;

	case 'r':
	case 'R':
		makeIdentity();
		break;
	}

	glutPostRedisplay();
}

/*********************************************************************************************/
void makeIdentity(){

	gCameraScale = 1.0;
	gCameraTranslationX = 0;
	gCameraTranslationY = 0;
	gCameraRotation = mat4(1.0f);


}
/************************************************************************************************/
/*******************************************************************************/
double projectToTrackball(double radius, double x, double y)
{
	static const double kUnitSphereRadius2D = sqrt(2.0);
	double z;

	double dist = sqrt(x * x + y * y);
	if (dist < radius * kUnitSphereRadius2D / 2.0)
	{
		// Solve for sphere case.
		z = sqrt(radius * radius - dist * dist);
	}
	else
	{
		// Solve for hyperbolic sheet case.
		double t = radius / kUnitSphereRadius2D;
		z = t * t / dist;
	}

	return z;
}

/*******************************************************************************/
/******************************************************************************
GLUT callback for responding to mouse button presses. Detecst whether to
initiate a point snapping, view rotation or view scale.
******************************************************************************/
void glutMouse(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_LEFT_BUTTON)
		{
			gIsRotatingCamera = true;
		}
		else if (button == GLUT_MIDDLE_BUTTON)
		{
			gIsScalingCamera = true;
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			gIsTranslatingCamera = true;
		}

		gLastMouseX = x;
		gLastMouseY = y;
	}
	else
	{
		gIsRotatingCamera = false;
		gIsScalingCamera = false;
		gIsTranslatingCamera = false;
	}
}
/**************************************************************************/
/*******************************************************************************
*******************************************************************************/

void glutMotion(int x, int y)
{
	if (gIsRotatingCamera)
	{
		static const double kTrackBallRadius = 0.8;
		vec3 lastPos;
		vec3 currPos;

		lastPos.x = gLastMouseX * 2.0 / gViewportWidth - 1.0;
		lastPos.y = (gViewportHeight - gLastMouseY) * 2.0 / gViewportHeight - 1.0;
		lastPos.z = projectToTrackball(kTrackBallRadius, lastPos[0], lastPos[1]);

		currPos.x = x * 2.0 / gViewportWidth - 1.0;
		currPos.y = (gViewportHeight - y) * 2.0 / gViewportHeight - 1.0;
		currPos.z = projectToTrackball(kTrackBallRadius, currPos[0], currPos[1]);
		
		currPos = normalize(currPos);
		lastPos = normalize(lastPos);

		vec3 rotateVecCross;

		rotateVecCross = cross(vec3(lastPos), vec3(currPos)) ;
		
		double sin_rotate_angle = sqrt(rotateVecCross.x * rotateVecCross.x + rotateVecCross.y * rotateVecCross.y + rotateVecCross.z * rotateVecCross.z);
		double angle = asin(sqrt(rotateVecCross.x * rotateVecCross.x + rotateVecCross.y * rotateVecCross.y + rotateVecCross.z * rotateVecCross.z));
		double cos_rotate_angle = cos(angle);
		
		rotateVecCross = normalize(rotateVecCross);

		mat4 deltaRotation(1.0f);

		deltaRotation[0][0] = cos_rotate_angle + rotateVecCross[0] * rotateVecCross[0] * (1 - cos_rotate_angle);
		deltaRotation[0][1] = rotateVecCross[0] * rotateVecCross[1] * (1 - cos_rotate_angle) + rotateVecCross[2] * sin_rotate_angle;
		deltaRotation[0][2] = rotateVecCross[0] * rotateVecCross[2] * (1 - cos_rotate_angle) - rotateVecCross[1] * sin_rotate_angle;
		deltaRotation[0][3] = 0.0;
		deltaRotation[1][0] = rotateVecCross[0] * rotateVecCross[1] * (1 - cos_rotate_angle) - rotateVecCross[2] * sin_rotate_angle;
		deltaRotation[1][1] = cos_rotate_angle + rotateVecCross[1] * rotateVecCross[1] * (1 - cos_rotate_angle);
		deltaRotation[1][2] = rotateVecCross[2] * rotateVecCross[1] * (1 - cos_rotate_angle) + rotateVecCross[0] * sin_rotate_angle;
		deltaRotation[1][3] = 0.0;
		deltaRotation[2][0] = rotateVecCross[0] * rotateVecCross[2] * (1 - cos_rotate_angle) + rotateVecCross[1] * sin_rotate_angle;
		deltaRotation[2][1] = rotateVecCross[1] * rotateVecCross[2] * (1 - cos_rotate_angle) - rotateVecCross[0] * sin_rotate_angle;
		deltaRotation[2][2] = cos_rotate_angle + rotateVecCross[2] * rotateVecCross[2] * (1 - cos_rotate_angle);
		

		mat4 tempMatrix;

		
		if (abs(angle) > DBL_EPSILON*20.0){
			tempMatrix = gCameraRotation*deltaRotation;
			gCameraRotation = tempMatrix;
		}
	}
	if (gIsTranslatingCamera)
	{
		gCameraTranslationX += 2 * double(x - gLastMouseX) / gViewportWidth;
		gCameraTranslationY -= 2 * double(y - gLastMouseY) / gViewportHeight;

	}
	else if (gIsScalingCamera)
	{
		float y1 = gViewportHeight - gLastMouseY;
		float y2 = gViewportHeight - y;

		gCameraScale *= 1 + (y1 - y2) / gViewportHeight;
	}

	glutPostRedisplay();

	gLastMouseX = x;
	gLastMouseY = y;
}
/************************************************************************************************************************************************************/

/*******************************************************************************
*******************************************************************************/

/*********/
int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(800, 600);

	glutCreateWindow("FBO Quad");

	if (glewInit()){
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
	}
	ilInit();
	Initialize();
	std::cout << glGetString(GL_VERSION) << std::endl;
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutMotionFunc(glutMotion);
	glutMouseFunc(glutMouse);
	glutReshapeFunc(Reshape);
	glutMainLoop();
	return 0;
}


