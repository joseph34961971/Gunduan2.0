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

#define PARTSNUM 36
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
#define LEFTLEGARMOR 25
#define LEFTLEGGUNSTOCK 26
#define LEFTLEGGUNBARREL 27
#define LEFTLEGGUNPOINT 28
#define LEFTLEGSABER 29
#define RIGHTLEGARMOR 30
#define RIGHTLEGGUNSTOCK 31
#define RIGHTLEGGUNBARREL 32
#define RIGHTLEGGUNPOINT 33
#define RIGHTLEGSABER 34
#define LEFTARMGUN 35


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
GLuint asteroids_VAO;
GLuint gundaun_shader;
GLuint skybox_shader;
GLuint gray_shader;
GLuint uniform_shader;
GLuint gaussian_shader;
GLuint basic_shader;
GLuint diamond_shader;
GLuint texture_shader;
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

#define DIAMONDREFLECT 2
#define DIAMONDREFRACT 3
#define DIAMOND 4

#define WALK 1
#define IDLE 0
#define JumpingJack 2
#define Squat 3
#define MoonWalk 4
#define GangnanStyle 5
#define YoBattle 6
#define Opening 7
#define Shoot 8
#define AllShoot 9
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
vec3 light_pos = vec3(0, 50, 0);

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

void drawEarth();
GLint loadTexture(string path);
GLuint sphere_vao;
size_t sphere_indices_size;
GLuint sphereGenerator(int subdivision_level);
void VAOProcess(GLuint& VertexArrayID, vector<vec3> indexed_vertices, vector<vec3> indexed_normals, vector<vec2> indexed_uvs, std::vector<unsigned short> indices, size_t& indices_size);
void drawShpere(GLuint VertexArrayID, int indices_size);
GLint earth_texture;
vec3 earth_pos;
vec3 earth_pos_begin = vec3(70, 0, -50);

void drawRifleBeam();
void drawCannonBeam();
void drawRailgunBeam();
GLint rifle_beam_texture;
vec3 rifle_beam_pos;
vec3 rifle_beam_offset;
vec3 rifle_beam_scale;
bool rifle_shooting = false;
float beam_speed = 15.0f;
GLint cannon_beam_texture;
vec3 lcannon_beam_pos;
vec3 lcannon_beam_offset;
vec3 lcannon_beam_scale;
vec3 rcannon_beam_pos;
vec3 rcannon_beam_offset;
vec3 rcannon_beam_scale;
bool cannon_shooting = false;
GLint railgun_beam_texture;
vec3 lrailgun_beam_pos;
vec3 lrailgun_beam_offset;
vec3 lrailgun_beam_scale;
vec3 rrailgun_beam_pos;
vec3 rrailgun_beam_offset;
vec3 rrailgun_beam_scale;
bool railgun_shooting = false;

#define ASTEROIDNUM 6
std::vector<std::string> asteroids_mtls[ASTEROIDNUM];//use material
std::vector<unsigned int> asteroids_faces[ASTEROIDNUM];//face count
map<string, vec3> asteroids_KDs;//mtl-name&Kd
GLuint asteroids_VBOs[ASTEROIDNUM];
GLuint asteroids_uVBOs[ASTEROIDNUM];
GLuint asteroids_nVBOs[ASTEROIDNUM];
int asteroids_vertices_size[ASTEROIDNUM];
int asteroids_uvs_size[ASTEROIDNUM];
int asteroids_normals_size[ASTEROIDNUM];
void load2AsteroidBuffer(char* obj, int i);
GLint asteroids_textures[ASTEROIDNUM];
void drawAsteroids();
#define ASTEROIDAMOUNT 500
vec3 asteroids_pos[ASTEROIDAMOUNT];
vec3 asteroids_scale[ASTEROIDAMOUNT];
int asteroids_species[ASTEROIDAMOUNT];
float gundam_speed = 0.5f;

//void initOpenAL();
//ALCdevice* device = nullptr;
//ALCcontext* context = nullptr;
//ALuint source;
//ALuint buffer;