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

/*********************NEW ADDITIONS*********************/
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
//#include <AL/alut.h>

using namespace glm;

#define PARTSNUM 25
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
#define LEFTCONNECTOR 11
#define LEFTINSIDEBIGWING 12
#define LEFTGUN 13
#define LEFTOUTSIDEBIGWING 14
#define LEFTOUTSIDESMALLWING 15
#define LEFTMIDDLESMALLWING 16
#define LEFTINSIDESMALLWING 17
#define RIGHTCONNECTOR 18
#define RIGHTINSIDEBIGWING 19
#define RIGHTGUN 20
#define RIGHTOUTSIDEBIGWING 21
#define RIGHTOUTSIDESMALLWING 22
#define RIGHTMIDDLESMALLWING 23
#define RIGHTINSIDESMALLWING 24

void updateModels();

void init();

void ChangeSize(int w,int h);
void display();
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);

void menuEvents(int option);
void ActionMenuEvents(int option);
void ModeMenuEvents(int option);
void PPSMenuEvents(int option);

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
GLuint skybox_VAO;
GLuint skybox_VBO;
GLuint skybox_matrices_ubo;
GLuint screen_quad_VAO;
GLuint screen_quad_VBO;
GLuint VBOs[PARTSNUM];
GLuint uVBOs[PARTSNUM];
GLuint nVBOs[PARTSNUM];
GLuint gundaun_shader;
GLuint skybox_shader;
GLuint gray_shader;
GLuint uniform_shader;
GLuint gaussian_shader;
#define ORIGIN 0
#define GRAY 1
#define UNIFORM 2
#define GAUSSIAN 3
int pps;
bool JumpingJack_beginpose = true;
bool GangnanStyle_rightfoot = false;
int GangnanStyle_footcount = 0;
bool GangnanStyle_handdown = false;
bool GangnanStyle_handprepare = false;
int GangnanStyle_temple = 0;
bool GangnanStyle_beginpose = true;

float angles[PARTSNUM][3];
float positions[PARTSNUM][3];
float eyeAngley = 0.0;
float eyeAngleyGoal = 0.0;
float eyedistance = 20.0;
float eyeheight = 0.0;
float eyeX = 0.0;
float eyeXGoal = 0.0;
float size = 1;
float fly_position = 0.0f;
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
#define JumpingJack 2
#define Squat 3
#define MoonWalk 4
#define GangnanStyle 5
#define YoBattle 6
int mode = 1;
int action;
const int fps = 60;
int second_current = 0;
bool reset_action = false;

std::vector<std::string> skybox_textures_faces;
GLuint cubemap_texture;

void initSkybox();
GLuint loadCubemap(std::vector<std::string> skybox_textures_faces);
void drawSkybox();
vec3 light_pos = vec3(-10, 0, 0);

GLuint screen_id;
bool load_screen = false;
int screen_width = 800;
int screen_height = 600;
void initScreenRender();
void renderScreenBegin();
void renderScreenEnd();
GLuint screen_fbo;	//frame buffer
GLuint screen_rbo;	//attach to depth and stencil
void initScreenQuad();
void drawScreenQuad();

//void initOpenAL();
//ALCdevice* device = nullptr;
//ALCcontext* context = nullptr;
//ALuint source;
//ALuint buffer;