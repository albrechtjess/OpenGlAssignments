#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
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
GLuint program;
GLuint program_gouraud;
float aspect = 0.0;
GLfloat angle = 0.0;
GLuint vao;
GLuint vbo[2];
GLuint ebo;
std::vector<glm::vec3> vertices;
std::vector<int> vIndices;
std::vector<glm::vec3> normals; 
vec3 bottom;
vector<int> indices;
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
bool switchShader = true;

void Initialize();
void display(void);
void makeIdentity();
void Reshape(int width, int height);
void glutMouse(int button, int state, int x, int y);
void glutMotion(int x, int y);
double projectToTrackball(double radius, double x, double y);

//cone
#define NumConePoints  18
#define NumTriangles   18
#define NumIndices     3*NumTriangles

GLuint  cone_vao;
unsigned int cone_vbo[2];
//GLuint cone_vbo;
GLuint  cone_ebo;

GLuint cone_position_loc;
GLuint cone_normal_loc;

GLuint cone_indices[NumIndices];
vec4 cone_points[NumConePoints + 1];
vec3 cone_normals[NumConePoints + 1];

int index = 0;

void updateVertexNormals();

void initializeCone() {

	cone_points[index][0] = 0.0;
	cone_points[index][1] = bottom[1];
	cone_points[index][2] = 0.0;
	cone_points[index][3] = 1.0;

	cone_normals[index][0] = 0.0;
	cone_normals[index][1] = 0.0;
	cone_normals[index][2] = 0.0;

	index++;

	int i;
	float theta;
	int tIndices = 0;


	for (i = 0; i < NumConePoints; ++i) {

		theta = i*20.0f*kPI / 180.0f;

		cone_points[index][0] = 10*cos(theta);
		cone_points[index][1] = bottom[1];
		cone_points[index][2] = 10*-sin(theta);
		cone_points[index][3] = 1.0;

		cone_normals[index][0] = 0.0;
		cone_normals[index][1] = 0.0;
		cone_normals[index][2] = 0.0;

		if (i <= (NumConePoints - 2)) {

			cone_indices[tIndices] = 0; tIndices++;
			cone_indices[tIndices] = index; tIndices++;
			cone_indices[tIndices] = index + 1; tIndices++;

		}
		else {
			cone_indices[tIndices] = 0; tIndices++;
			cone_indices[tIndices] = index; tIndices++;
			cone_indices[tIndices] = 1; tIndices++;
		}
		index++;
	}

	updateVertexNormals();

}

void updateVertexNormals()
{
	// Compute per-vertex normals here!
	for (int i = 0; i < NumIndices; i += 3) {
		//calculates normals of side and adds normal onto each vertex normal
		vec3 A = vec3(cone_points[cone_indices[i]]);
		vec3 B = vec3(cone_points[cone_indices[i + 1]]);
		vec3 C = vec3(cone_points[cone_indices[i + 2]]);
		vec3 normal = normalize(cross(B - A, C - A));
		cone_normals[cone_indices[i]] += normal;
		cone_normals[cone_indices[i + 1]] += normal;
		cone_normals[cone_indices[i + 2]] += normal;
	}
	//normalizes each vertex normal 
	for (int i = 0; i < NumConePoints + 1; i++)
	{
		normalize(cone_normals[i]);
	}

}

void Initialize(void){
	// Create the program for rendering the model

	OBJLoader loader;

	bool loadfile = loader.load("bunny.obj");
	vertices = loader.getVertices();
	normals = loader.getNormals();
	indices = loader.getVertexIndices();
	bottom = loader.getBottom();


	//create cone
	/*
	initializeCone();

	GLuint offset = 0;
	glGenVertexArrays(1, &cone_vao);
	glBindVertexArray(cone_vao);
	glGenBuffers(1, &cone_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, cone_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cone_points) + sizeof(cone_normals), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(cone_points), cone_points);
	offset += sizeof(cone_points);
	glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(cone_normals), cone_normals);

	glGenBuffers(1, &cone_ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cone_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cone_indices), (cone_indices), GL_STATIC_DRAW);

	cone_position_loc = glGetAttribLocation(program, "VertexPosition");
	glVertexAttribPointer(cone_position_loc, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(cone_position_loc);

	cone_normal_loc = glGetAttribLocation(program, "VertexNormal");
	glVertexAttribPointer(cone_normal_loc, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(cone_points));
	glEnableVertexAttribArray(cone_normal_loc);
	glBindVertexArray(0);
	//*/

	// Create and compile our GLSL program from the shaders
	program = LoadShaders("spotlight.vs", "spotlight.fs");
	program_gouraud = LoadShaders("smoothshader.vs", "smoothshader.fs");

	view = glm::lookAt(vec3(0.0f, 0.0f, 30.5f), vec3(0.0f), vec3(0.0f, 1.0f, 0.0f));//position, looking at, up vector 
	projection = mat4(1.0f);

	//scale bunny
	for (int i = 0; i < vertices.size(); i++)
	{
		vertices[i] = vertices[i] * 7.0f;
	}

	// Initialize shader lighting parameters
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(4, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position
	
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(int), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	//cone attemp using a bigger vbo
	/*
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, (NumConePoints + 1) * sizeof(glm::vec4), &cone_points[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)2, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, (NumConePoints + 1) * sizeof(glm::vec3), &cone_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal

	glGenBuffers(1, &cone_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cone_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumIndices*sizeof(int), &cone_indices[0], GL_STATIC_DRAW);
	//end cone stuff
	//*/

	
	//Cone///////////////////////////////////////////////////////////////////////////
	///*
	initializeCone();


	//Scale Cone
	///*
	//printf("bottom: %f\n", bottom[1]);
	for (int i = 0; i < NumConePoints + 1; i++)
	{
		//printf("cone_points before: %f\n", cone_points[i][1]);
		cone_points[i][1] *= 7.0f;
		//printf("cone_points after: %f\n", cone_points[i][1]);
	}
	//*/

	GLuint offset = 0;
	glGenVertexArrays(1, &cone_vao);
	glBindVertexArray(cone_vao);

	glGenBuffers(2, cone_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, cone_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, (NumConePoints + 1) * sizeof(glm::vec4), &cone_points[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);  // Vertex position

	glBindBuffer(GL_ARRAY_BUFFER, cone_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, (NumConePoints + 1) * sizeof(glm::vec3), &cone_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);  // Vertex normal

	glGenBuffers(1, &cone_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cone_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, NumIndices*sizeof(int), &cone_indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
	//end Cone
	//*/

	vec3 lightintensity = vec3(0.9f, 0.9f, 0.9f);
	vec3 material_ambient(0.9, 0.5, 0.3);
	vec3 material_diffuse(0.9, 0.5, 0.3);
	vec3 material_specular(0.8, 0.8, 0.8);

	glUseProgram(program);
	glUniform3fv(glGetUniformLocation(program, "Spot.intensity"), 1, (GLfloat*)&lightintensity);
	glUniform3fv(glGetUniformLocation(program, "Ka"), 1, (GLfloat*)&material_ambient);
	glUniform3fv(glGetUniformLocation(program, "Kd"), 1, (GLfloat*)&material_diffuse[0]);
	glUniform3fv(glGetUniformLocation(program, "Ks"), 1, (GLfloat*)&material_specular[0]);
	glUniform1f(glGetUniformLocation(program, "Shininess"), 180.0f);
	glUniform1f(glGetUniformLocation(program, "Spot.exponent"), 30.0f);
	glUniform1f(glGetUniformLocation(program, "Spot.cutoff"), 15.0f);
	
	//uniforms for vertex shader
	glUseProgram(program_gouraud); 
	glUniform3fv(glGetUniformLocation(program_gouraud, "Spot.intensity"), 1, (GLfloat*)&lightintensity);
	glUniform3fv(glGetUniformLocation(program_gouraud, "Ka"), 1, (GLfloat*)&material_ambient);
	glUniform3fv(glGetUniformLocation(program_gouraud, "Kd"), 1, (GLfloat*)&material_diffuse[0]);
	glUniform3fv(glGetUniformLocation(program_gouraud, "Ks"), 1, (GLfloat*)&material_specular[0]);
	glUniform1f(glGetUniformLocation(program_gouraud, "Shininess"), 180.0f);
	glUniform1f(glGetUniformLocation(program_gouraud, "Spot.exponent"), 30.0f);
	glUniform1f(glGetUniformLocation(program_gouraud, "Spot.cutoff"), 15.0f);

	glClearColor(1.0, 1.0, 1.0, 1.0);
}
void Display(void)
{
	// Clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	if(switchShader)glUseProgram(program);
	else { glUseProgram(program_gouraud); }

	model = mat4(1.0f);
	vec4 lightPos = vec4(10.0f*cos(angle), 10.0f, 10.0f*sin(angle), 1.0f);
	vec4 spotPos = view*lightPos;
	

	// implement spot direction
	mat3 viewMatrix = mat3(vec3(view[0]), vec3(view[1]), vec3(view[2]));
	vec3 spot_direction = viewMatrix * vec3(-lightPos);

	mat4 scaled = glm::scale(glm::mat4(1.0f), glm::vec3(gCameraScale, gCameraScale, gCameraScale));
	mat4 translated = glm::translate(glm::mat4(1.0f), glm::vec3(gCameraTranslationX, gCameraTranslationY, 0.0));
	mat4 transformation_matrix = translated*gCameraRotation*scaled;
	model *= transformation_matrix;
	model_view = view*model;
	mat3 normalmatrix;
	projection = glm::perspective(70.0f, aspect, 0.3f, 100.0f); 
	mat4 mvp = projection*model_view;

	///*
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	if(switchShader)
	{ 
		glUniform3fv(glGetUniformLocation(program, "Spot.direction"), 1, (GLfloat*)&spot_direction);
		glUniform4fv(glGetUniformLocation(program, "Spot.position"), 1, (GLfloat*)&spotPos);
		glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
		glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normalmatrix[0]);
		glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
		glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	}
	else
	{ 
		glUniform3fv(glGetUniformLocation(program_gouraud, "Spot.direction"), 1, (GLfloat*)&spot_direction); 
		glUniform4fv(glGetUniformLocation(program_gouraud, "Spot.position"), 1, (GLfloat*)&spotPos); 
		glUniformMatrix4fv(glGetUniformLocation(program_gouraud, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
		glUniformMatrix3fv(glGetUniformLocation(program_gouraud, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&normalmatrix[0]);
		glUniformMatrix4fv(glGetUniformLocation(program_gouraud, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
		glUniformMatrix4fv(glGetUniformLocation(program_gouraud, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	}
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
	//*/
	///*
	//begin cone
	mat3 cone_normalmatrix;
	glBindVertexArray(cone_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cone_ebo);
	if (switchShader)
	{
		glUniform3fv(glGetUniformLocation(program, "Spot.direction"), 1, (GLfloat*)&spot_direction);
		glUniform4fv(glGetUniformLocation(program, "Spot.position"), 1, (GLfloat*)&spotPos);
		glUniformMatrix4fv(glGetUniformLocation(program, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
		glUniformMatrix3fv(glGetUniformLocation(program, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&cone_normalmatrix[0]);
		glUniformMatrix4fv(glGetUniformLocation(program, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
		glUniformMatrix4fv(glGetUniformLocation(program, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	}
	else
	{	
		glUniform3fv(glGetUniformLocation(program_gouraud, "Spot.direction"), 1, (GLfloat*)&spot_direction);
		glUniform4fv(glGetUniformLocation(program_gouraud, "Spot.position"), 1, (GLfloat*)&spotPos);
		glUniformMatrix4fv(glGetUniformLocation(program_gouraud, "ModelViewMatrix"), 1, GL_FALSE, (GLfloat*)&model_view[0]);
		glUniformMatrix3fv(glGetUniformLocation(program_gouraud, "NormalMatrix"), 1, GL_FALSE, (GLfloat*)&cone_normalmatrix[0]);
		glUniformMatrix4fv(glGetUniformLocation(program_gouraud, "MVP"), 1, GL_FALSE, (GLfloat*)&mvp[0]);
		glUniformMatrix4fv(glGetUniformLocation(program_gouraud, "ProjectionMatrix"), 1, GL_FALSE, (GLfloat*)&projection[0]);
	}
	glDrawElements(GL_TRIANGLES, NumIndices, GL_UNSIGNED_INT, NULL);
	glBindVertexArray(0);
	//*/
	//glDrawArrays could be used to implement flat shading 
	glutSwapBuffers();
}



void Reshape(int width, int height)
{
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	gViewportWidth = width;
	gViewportHeight = height;
	glViewport(0, 0, width, height);

	
	aspect = float(width) / float(height);
}

void update(int n){
	
		angle += 0.1f;
		if (angle > TWOPI) angle -= TWOPI;

	    glutPostRedisplay();
	    glutTimerFunc(500, update, n);
   
}


void keyboard(unsigned char key, int x, int y){
	switch (key){
	case 'q':case 'Q':
		exit(EXIT_SUCCESS);
		break;
	case 's':case 'S':
		switchShader = !switchShader;
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
	static const double Radius = sqrt(2.0);
	double z;

	double dist = sqrt(x * x + y * y);
	if (dist < radius * Radius / 2.0)
	{
		// Solve for sphere case.
		z = sqrt(radius * radius - dist * dist);
	}
	else
	{
		// Solve for hyperbolic sheet case.
		double t = radius / Radius;
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
/**************************************************************************************************************************************************************/

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
int main(int argc, char** argv){

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);

	glutCreateWindow("Shiny Gloomy Bunny");

	if (glewInit()){
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
	}
	
	Initialize();
	std::cout << glGetString(GL_VERSION) << std::endl;
	glutDisplayFunc(Display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(100, update, 0);
	glutMotionFunc(glutMotion);
	glutMouseFunc(glutMouse);
	glutReshapeFunc(Reshape);
	glutMainLoop();
	return 0;
}


