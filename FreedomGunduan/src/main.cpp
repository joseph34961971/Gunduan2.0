#include "main.h"

vec3 camera = vec3(0, 0, 20);
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(4, 3);//以OpenGL version4.3版本為基準
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);//是否向下相容,GLUT_FORWARD_COMPATIBLE不支援(?
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//multisample for golygons smooth
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL 4.3 - Freedom Gunduan");

	glewExperimental = GL_TRUE; //置於glewInit()之前
	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
		exit(EXIT_FAILURE);
	}

	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(Keyboard);
	int ActionMenu, ModeMenu, ShaderMenu;
	ActionMenu = glutCreateMenu(ActionMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("idle", 0);
	glutAddMenuEntry("walk", 1);
	glutAddMenuEntry("reset Model", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	ModeMenu = glutCreateMenu(ModeMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("Line", 0);
	glutAddMenuEntry("Fill", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	glutCreateMenu(menuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddSubMenu("action", ActionMenu);
	glutAddSubMenu("mode", ModeMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	glutMouseFunc(Mouse);
	glutTimerFunc(1000.0f / fps, idle, 0);
	glutMainLoop();
	return 0;
}

void ChangeSize(int w, int h)
{
	if (h == 0) h = 1;
	glViewport(0, 0, w, h);
	Projection = perspective(80.0f, (float)w / h, 0.1f, 100.0f);
}

void Mouse(int button, int state, int x, int y)
{
	if (button == 2) isFrame = false;
}

void idle(int dummy)
{
	isFrame = true;
	int out = 0;
	if (action == WALK || action == IDLE)
	{
		updateObj(dummy);
		out = dummy + 1;
		if (out >= fps)
		{
			out = 0;
			second_current++;
			if (second_current > 2147483645)
				second_current = 0;
		}
	}
	glutPostRedisplay();

	glutTimerFunc(1000.0f / fps, idle, out); // fps 60
}

void resetObj(int f)
{
	for (int i = 0; i < PARTSNUM; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			angles[i][j] = 0.0f;
		}
	}

	positions[BODY][X] = 0.0f;
	positions[BODY][Y] = 0.0f; // 34.0f
	positions[BODY][Z] = 0.0f;

	positions[HEAD][X] = 0;
	positions[HEAD][Y] = 26.0f;
	positions[HEAD][Z] = 0;

	positions[LEFTSHOULDER][X] = -15.0f;
	positions[LEFTSHOULDER][Y] = 19.0f;
	positions[LEFTSHOULDER][Z] = 0.0f;

	positions[LEFTARM][X] = -7.0f;
	positions[LEFTARM][Y] = -11.0f;
	positions[LEFTARM][Z] = 0.0f;

	positions[RIGHTSHOULDER][X] = 14.0f;
	positions[RIGHTSHOULDER][Y] = 21.0f;
	positions[RIGHTSHOULDER][Z] = -1.0f;

	positions[RIGHTARM][X] = 7.0f;
	positions[RIGHTARM][Y] = -8.0f;
	positions[RIGHTARM][Z] = 0.0f;

	positions[WING][X] = 0.0f;
	positions[WING][Y] = 25.0f;
	positions[WING][Z] = -9.0f;

	positions[LEFTLEG][X] = -9.0f;
	positions[LEFTLEG][Y] = -8.0f;
	positions[LEFTLEG][Z] = 0.0f;

	positions[LEFTFOOT][X] = -4.0f;
	positions[LEFTFOOT][Y] = -21.0f;
	positions[LEFTFOOT][Z] = 0.0f;

	positions[RIGHTLEG][X] = 8.0f;
	positions[RIGHTLEG][Y] = -9.0f;
	positions[RIGHTLEG][Z] = 0.0f;

	positions[RIGHTFOOT][X] = 5.0f;
	positions[RIGHTFOOT][Y] = -20.0f;
	positions[RIGHTFOOT][Z] = 12.0f;
}

void updateObj(int frame)
{
	/*if (frame < 10)
	{
		angles[RIGHTSHOULDER][X] += 5.0f;
	}
	else if (frame < 20)
	{
		angles[RIGHTSHOULDER][X] -= 5.0f;
	}*/
	fly_position = 1.0f * sin((((float)frame + second_current * fps) / fps * 3.1415));
}

 GLuint M_KaID;
 GLuint M_KdID;
 GLuint M_KsID;

 void init()
 {
	 isFrame = false;
	 pNo = 0;
	 resetObj(0); // initial angles array

	 //VAO
	 glGenVertexArrays(1, &VAO);
	 glBindVertexArray(VAO);

	 ShaderInfo shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/DSPhong_Material.vp" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/DSPhong_Material.fp" },//fragment shader
		 { GL_NONE, NULL } };
	 program = LoadShaders(shaders);//讀取shader

	 glUseProgram(program);//uniform參數數值前必須先use shader

	 MatricesIdx = glGetUniformBlockIndex(program, "MatVP");
	 ModelID = glGetUniformLocation(program, "Model");
	 M_KaID = glGetUniformLocation(program, "Material.Ka");
	 M_KdID = glGetUniformLocation(program, "Material.Kd");
	 M_KsID = glGetUniformLocation(program, "Material.Ks");
	 //or
	 M_KdID = M_KaID + 1;
	 M_KsID = M_KaID + 2;

	 Projection = glm::perspective(80.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	 //glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates

	 // Camera matrix
	 View = glm::lookAt(
		 glm::vec3(0, 10, 25), // Camera is at (0,10,25), in World Space
		 glm::vec3(0, 0, 0), // and looks at the origin
		 glm::vec3(0, 1, 0)  // Head is up (set to 0,1,0 to look upside-down)
	 );

	 Obj2Buffer();

	 //UBO
	 glGenBuffers(1, &UBO);
	 glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	 glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 2, NULL, GL_DYNAMIC_DRAW);
	 //get uniform struct size
	 int UBOsize = 0;
	 glGetActiveUniformBlockiv(program, MatricesIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &UBOsize);
	 //bind UBO to its idx
	 glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, UBOsize);
	 glUniformBlockBinding(program, MatricesIdx, 0);

	 glClearColor(0.0, 0.0, 0.0, 1);//black screen
 }

#define DOR(body_angle) (body_angle*3.1415/180);
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindVertexArray(VAO);
	glUseProgram(program);//uniform參數數值前必須先use shader
	float eyey = DOR(eyeAngley);
	View = lookAt(
		vec3(eyedistance * sin(eyey), 2 + eyeheight, eyedistance * cos(eyey)), // Camera is at (0,0,20), in World Space
		vec3(0, eyeheight, 0), // and looks at the origin
		vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);
	updateModels();
	//update data to UBO for MVP
	glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), &View);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), &Projection);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	GLuint offset[3] = { 0,0,0 };//offset for vertices , uvs , normals
	for (int i = 0; i < PARTSNUM; i++) {
		glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Models[i][0][0]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,				//location
			3,				//vec3
			GL_FLOAT,			//type
			GL_FALSE,			//not normalized
			0,				//strip
			(void*)offset[0]);//buffer offset
//(location,vec3,type,固定點,連續點的偏移量,buffer point)
		offset[0] += vertices_size[i] * sizeof(vec3);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);//location 1 :vec2 UV
		glBindBuffer(GL_ARRAY_BUFFER, uVBO);
		glVertexAttribPointer(1,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)offset[1]);
		//(location,vec2,type,固定點,連續點的偏移量,point)
		offset[1] += uvs_size[i] * sizeof(vec2);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(2);//location 2 :vec3 Normal
		glBindBuffer(GL_ARRAY_BUFFER, nVBO);
		glVertexAttribPointer(2,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)offset[2]);
		//(location,vec3,type,固定點,連續點的偏移量,point)
		offset[2] += normals_size[i] * sizeof(vec3);

		int vertexIDoffset = 0;//glVertexID's offset 
		string mtlname;//material name
		vec3 Ks = vec3(1, 1, 1);//because .mtl excluding specular , so give it here.
		for (int j = 0; j < mtls[i].size(); j++) {//
			mtlname = mtls[i][j];
			//find the material diffuse color in map:KDs by material name.
			glUniform3fv(M_KdID, 1, &KDs[mtlname][0]);
			glUniform3fv(M_KsID, 1, &Ks[0]);
			//          (primitive   , glVertexID base , vertex count    )
			glDrawArrays(GL_TRIANGLES, vertexIDoffset, faces[i][j + 1] * 3);
			//we draw triangles by giving the glVertexID base and vertex count is face count*3
			vertexIDoffset += faces[i][j + 1] * 3;//glVertexID's base offset is face count*3
		}//end for loop for draw one part of the robot	

	}//end for loop for updating and drawing model
	glFlush();//強制執行上次的OpenGL commands
	glutSwapBuffers();//調換前台和後台buffer ,當後臺buffer畫完和前台buffer交換使我們看見它
}

void Obj2Buffer()
{
	std::vector<vec3> Kds;
	std::vector<vec3> Kas;
	std::vector<vec3> Kss;
	std::vector<std::string> Materials;//mtl-name
	std::string texture;
	loadMTL("../FreedomGunduan/objs/freedom.mtl", Kds, Kas, Kss, Materials, texture);
	//printf("%d\n",texture);
	for (int i = 0; i < Materials.size(); i++) {
		string mtlname = Materials[i];
		//  name            vec3
		KDs[mtlname] = Kds[i];
	}

	load2Buffer("../FreedomGunduan/objs/body.obj", BODY);

	load2Buffer("../FreedomGunduan/objs/head.obj", HEAD);

	load2Buffer("../FreedomGunduan/objs/left_arm.obj", LEFTSHOULDER);
	load2Buffer("../FreedomGunduan/objs/left_hand.obj", LEFTARM);
	load2Buffer("../FreedomGunduan/objs/right_arm.obj", RIGHTSHOULDER);
	load2Buffer("../FreedomGunduan/objs/right_hand.obj", RIGHTARM);

	load2Buffer("../FreedomGunduan/objs/wing.obj", WING);

	load2Buffer("../FreedomGunduan/objs/left_leg.obj", LEFTLEG);
	load2Buffer("../FreedomGunduan/objs/left_foot.obj", LEFTFOOT);
	load2Buffer("../FreedomGunduan/objs/right_leg.obj", RIGHTLEG);
	load2Buffer("../FreedomGunduan/objs/right_foot.obj", RIGHTFOOT);

	GLuint totalSize[3] = { 0,0,0 };
	GLuint offset[3] = { 0,0,0 };
	for (int i = 0; i < PARTSNUM; i++) {
		totalSize[0] += vertices_size[i] * sizeof(vec3);
		totalSize[1] += uvs_size[i] * sizeof(vec2);
		totalSize[2] += normals_size[i] * sizeof(vec3);
	}
	//generate vbo
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &uVBO);
	glGenBuffers(1, &nVBO);
	//bind vbo ,第一次bind也同等於 create vbo 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, totalSize[0], NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uVBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, totalSize[1], NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, nVBO);//VBO的target是GL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, totalSize[2], NULL, GL_STATIC_DRAW);


	for (int i = 0; i < PARTSNUM; i++) {
		glBindBuffer(GL_COPY_WRITE_BUFFER, VBO);
		glBindBuffer(GL_COPY_READ_BUFFER, VBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, offset[0], vertices_size[i] * sizeof(vec3));
		offset[0] += vertices_size[i] * sizeof(vec3);
		glInvalidateBufferData(VBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

		glBindBuffer(GL_COPY_WRITE_BUFFER, uVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, uVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, offset[1], uvs_size[i] * sizeof(vec2));
		offset[1] += uvs_size[i] * sizeof(vec2);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);

		glBindBuffer(GL_COPY_WRITE_BUFFER, nVBO);
		glBindBuffer(GL_COPY_READ_BUFFER, nVBOs[i]);
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 0, offset[2], normals_size[i] * sizeof(vec3));
		offset[2] += normals_size[i] * sizeof(vec3);
		glInvalidateBufferData(uVBOs[i]);//free vbo
		glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
	}
	glBindBuffer(GL_COPY_WRITE_BUFFER, 0);
}

void updateModels()
{
	mat4 Rotatation[PARTSNUM];
	mat4 Translation[PARTSNUM];
	mat4 Scaling[PARTSNUM];
	for(int i = 0 ; i < PARTSNUM;i++)
	{
		Models[i] = mat4(1.0f);
		Scaling[i] = scale(mat4(1.0f), vec3(0.125f, 0.125f, 0.125f));
		Rotatation[i] = mat4(1.0f);
		Translation[i] = mat4(1.0f);
	}

	//Body
	Rotatation[BODY] = rotate(angles[BODY][X], 0, 1, 0);
	Translation[BODY] = translate(positions[BODY][X], positions[BODY][Y] + fly_position, positions[BODY][Z]);
	Models[BODY] = Translation[BODY] * Rotatation[BODY] * Scaling[BODY];
	//============================================================

	//左肩膀
	Rotatation[LEFTSHOULDER] = rotate(angles[LEFTSHOULDER][X], 1, 0, 0) * rotate(angles[LEFTSHOULDER][Z], 0, 0, 1);//向前旋轉*向右旋轉
	Translation[LEFTSHOULDER] = translate(positions[LEFTSHOULDER][X], positions[LEFTSHOULDER][Y], positions[LEFTSHOULDER][Z]);//位移到左上手臂處
	Models[LEFTSHOULDER] = Models[BODY] * Translation[LEFTSHOULDER] * Rotatation[LEFTSHOULDER];
	//============================================================
	
	//左下手臂
	Rotatation[LEFTARM] = rotate(angles[LEFTARM][X], 1, 0, 0);
	Translation[LEFTARM] = translate(positions[LEFTARM][X], positions[LEFTARM][Y], positions[LEFTARM][Z]); // 0 -3 0
	Models[LEFTARM] = Models[LEFTSHOULDER] * Translation[LEFTARM] * Rotatation[LEFTARM];
	//============================================================
	
	//頭
	Translation[HEAD] = translate(positions[HEAD][X], positions[HEAD][Y], positions[HEAD][Z]);
	Models[HEAD] = Models[BODY] * Translation[HEAD] * Rotatation[HEAD];
	//============================================================

	//右肩膀(面對畫面右邊)
	Rotatation[RIGHTSHOULDER] = rotate(angles[RIGHTSHOULDER][X], 1, 0, 0) * rotate(angles[RIGHTSHOULDER][Z], 0, 0, 1);
	Translation[RIGHTSHOULDER] = translate(positions[RIGHTSHOULDER][X], positions[RIGHTSHOULDER][Y], positions[RIGHTSHOULDER][Z]);
	Models[RIGHTSHOULDER] = Models[BODY] * Translation[RIGHTSHOULDER] * Rotatation[RIGHTSHOULDER];
	//============================================================
	
	//右手(面對畫面右邊)
	Rotatation[RIGHTARM] = rotate(angles[RIGHTARM][X], 1, 0, 0);
	Translation[RIGHTARM] = translate(positions[RIGHTARM][X], positions[RIGHTARM][Y], positions[RIGHTARM][Z]);
	Models[RIGHTARM] = Models[RIGHTSHOULDER] * Translation[RIGHTARM] * Rotatation[RIGHTARM];
	//=============================================================
	
	//wing
	Translation[WING] = translate(positions[WING][X], positions[WING][Y], positions[WING][Z]);
	Models[WING] = Models[BODY] * Translation[WING] * Rotatation[WING];
	//=============================================================
	
	//左腿(面對畫面左邊)
	Rotatation[LEFTLEG] = rotate(angles[LEFTLEG][X], 1, 0, 0) * rotate(angles[LEFTLEG][Z], 0, 0, 1);
	Translation[LEFTLEG] = translate(positions[LEFTLEG][X], positions[LEFTLEG][Y], positions[LEFTLEG][Z]);
	Models[LEFTLEG] = Models[BODY] * Translation[LEFTLEG] * Rotatation[LEFTLEG];
	//=============================================================

	//左腳(面對畫面左邊)
	Rotatation[LEFTFOOT] = rotate(angles[LEFTFOOT][X], 1, 0, 0);
	Translation[LEFTFOOT] = translate(positions[LEFTFOOT][X], positions[LEFTFOOT][Y], positions[LEFTFOOT][Z]);
	Models[LEFTFOOT] = Models[LEFTLEG] * Translation[LEFTFOOT] * Rotatation[LEFTFOOT];
	//=============================================================
	
	//右腿
	Rotatation[RIGHTLEG] = rotate(angles[RIGHTLEG][X], 1, 0, 0) * rotate(angles[RIGHTLEG][Z], 0, 0, 1);
	Translation[RIGHTLEG] = translate(positions[RIGHTLEG][X], positions[RIGHTLEG][Y], positions[RIGHTLEG][Z]);
	Models[RIGHTLEG] = Models[BODY] * Translation[RIGHTLEG] * Rotatation[RIGHTLEG];
	//=============================================================

	//右腳
	Rotatation[RIGHTFOOT] = rotate(angles[RIGHTFOOT][X], 1, 0, 0);
	Translation[RIGHTFOOT] = translate(positions[RIGHTFOOT][X], positions[RIGHTFOOT][Y], positions[RIGHTFOOT][Z]);
	Models[RIGHTFOOT] = Models[RIGHTLEG] * Translation[RIGHTFOOT] * Rotatation[RIGHTFOOT];
	//=============================================================
}

void load2Buffer(char* obj,int i)
{
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals; // Won't be used at the moment.
	std::vector<unsigned int> materialIndices;

	bool res = loadOBJ(obj, vertices, uvs, normals, faces[i], mtls[i]);
	if(!res) printf("load failed\n");

	//glUseProgram(program);

	glGenBuffers(1,&VBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(vec3),&vertices[0],GL_STATIC_DRAW);
	vertices_size[i] = vertices.size();

	//(buffer type,data起始位置,data size,data first ptr)
	//vertices_size[i] = glm_model->numtriangles;
	
	//printf("vertices:%d\n",vertices_size[);

	glGenBuffers(1,&uVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, uVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);
	uvs_size[i] = uvs.size();

	glGenBuffers(1,&nVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, nVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
	normals_size[i] = normals.size();
}

mat4 translate(float x, float y, float z)
{
	vec4 t = vec4(x, y, z, 1);//w = 1 ,則x,y,z=0時也能translate
	vec4 c1 = vec4(1, 0, 0, 0);
	vec4 c2 = vec4(0, 1, 0, 0);
	vec4 c3 = vec4(0, 0, 1, 0);
	mat4 M = mat4(c1, c2, c3, t);
	return M;
}

mat4 scale(float x, float y, float z)
{
	vec4 c1 = vec4(x, 0, 0, 0);
	vec4 c2 = vec4(0, y, 0, 0);
	vec4 c3 = vec4(0, 0, z, 0);
	vec4 c4 = vec4(0, 0, 0, 1);
	mat4 M = mat4(c1, c2, c3, c4);
	return M;
}

mat4 rotate(float body_angle, float x, float y, float z)
{
	float r = DOR(body_angle);
	mat4 M = mat4(1);

	vec4 c1 = vec4(cos(r) + (1 - cos(r)) * x * x, (1 - cos(r)) * y * x + sin(r) * z, (1 - cos(r)) * z * x - sin(r) * y, 0);
	vec4 c2 = vec4((1 - cos(r)) * y * x - sin(r) * z, cos(r) + (1 - cos(r)) * y * y, (1 - cos(r)) * z * y + sin(r) * x, 0);
	vec4 c3 = vec4((1 - cos(r)) * z * x + sin(r) * y, (1 - cos(r)) * z * y - sin(r) * x, cos(r) + (1 - cos(r)) * z * z, 0);
	vec4 c4 = vec4(0, 0, 0, 1);
	M = mat4(c1, c2, c3, c4);
	return M;
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'u':
	case 'U':
		angles[BODY][X] += 5;
		if (angles[BODY][X] >= 360) angles[BODY][X] = 0;
		printf("beta:%f\n", angles[BODY][X]);
		break;
	case 'o':
	case 'O':
		angles[BODY][X] -= 5;
		if (angles[BODY][X] <= 0) angles[BODY][X] = 360;
		printf("beta:%f\n", angles[BODY][X]);
		break;
	case 'i':
	case 'I':
		eyeheight -= 2;
		break;
	case 'k':
	case 'K':
		eyeheight += 2;
		break;
	case 'j':
	case 'J':
		positions[BODY][X] += 1;
		break;
	case 'l':
	case 'L':
		positions[BODY][X] -= 1;
		break;
	case 'w':
	case 'W':
		eyedistance -= 0.2;
		break;
	case 's':
	case 'S':
		eyedistance += 0.2;
		break;
	case 'a':
	case 'A':
		eyeAngley -= 10;
		break;
	case 'd':
	case 'D':
		eyeAngley += 10;
		break;
	case 'r':
	case 'R':
		/*angles[1] -= 5;
		if(angles[1] == -360) angles[1] = 0;
		movey = 0;
		movex = 0;*/
		break;
	case 't':
	case 'T':
		/*angles[2] -= 5;
		if(angles[2] == -360) angles[2] = 0;
		movey = 0;
		movex = 0;*/
		break;
	case 'q':
		break;
	case 'e':
		break;
	}
	glutPostRedisplay();
}

void menuEvents(int option){}

void ActionMenuEvents(int option)
{
	switch(option)
	{
	case 0:
		action = IDLE;
		break;
	case 1:
		action = WALK;
		break;
	case 2:
		resetObj(0);
		break;
	}
}

void ModeMenuEvents(int option)
{
	switch(option)
	{
	case 0:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 1:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}
}

void ShaderMenuEvents(int option)
{
	pNo = option;
}