#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <map>
#include <string>
using namespace std;
//#include <cmath>

#include "vgl.h"
#include "LoadShaders.h"
#include "objloader.hpp"
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#define PARTSNUM 11
#define BODY 0
#define HEAD 1
#define LEFTSHOULDER 2
#define LEFTARM 3
#define RIGHTSHOULDER 4
#define RIGHTARM 5
#define WING 6
#define LEFTLEG 7
#define LEFTFOOT 8
#define RIGHTLEG 9
#define RIGHTFOOT 10

void updateModels();

void init();

void ChangeSize(int w,int h);
void display();
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);

void menuEvents(int option);
void ActionMenuEvents(int option);
void ModeMenuEvents(int option);
void ShaderMenuEvents(int option);

void idle(int dummy);

mat4 translate(float x, float y, float z);
mat4 scale(float x, float y, float z);
mat4 rotate(float body_angle, float x, float y, float z);

void Obj2Buffer();
void load2Buffer(char* obj, int);

void updateObj(int);
void resetObj(int);

bool isFrame;

GLuint VAO;
GLuint VBO;
GLuint uVBO;
GLuint nVBO;
GLuint mVBO;
GLuint UBO;
GLuint VBOs[PARTSNUM];
GLuint uVBOs[PARTSNUM];
GLuint nVBOs[PARTSNUM];
GLuint program;
int pNo;

float angles[PARTSNUM][3];
float positions[PARTSNUM][3];
float eyeAngley = 0.0;
float eyedistance = 20.0;
float size = 1;
GLfloat movex,movey;
GLint MatricesIdx;
GLuint ModelID;

int vertices_size[PARTSNUM];
int uvs_size[PARTSNUM];
int normals_size[PARTSNUM];
int materialCount[PARTSNUM];

std::vector<std::string> mtls[PARTSNUM];//use material
std::vector<unsigned int> faces[PARTSNUM];//face count
map<string, vec3> KDs;//mtl-name&Kd
map<string, vec3> KSs;//mtl-name&Ks

mat4 Projection;
mat4 View;
mat4 Model;
mat4 Models[PARTSNUM];

#define WALK 1
#define IDLE 0
int mode;
int action;
const int fps = 60;