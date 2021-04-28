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
	int ActionMenu, ModeMenu, PPMenu;
	ActionMenu = glutCreateMenu(ActionMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("Idle", 0);
	glutAddMenuEntry("Walk", 1);
	glutAddMenuEntry("Jumping Jack", 2);
	glutAddMenuEntry("Squat", 3);
	glutAddMenuEntry("MoonWalk", 4);
	glutAddMenuEntry("Gangnan Style", 5);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	ModeMenu = glutCreateMenu(ModeMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("Line", 0);
	glutAddMenuEntry("Fill", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	PPMenu = glutCreateMenu(PPSMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("Origin", 0);
	glutAddMenuEntry("Gray", 1);
	glutAddMenuEntry("Uniform", 2);
	glutAddMenuEntry("Gaussian", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	glutCreateMenu(menuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddSubMenu("Action", ActionMenu);
	glutAddSubMenu("Mode", ModeMenu);
	glutAddSubMenu("Post-Processing", PPMenu);
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
	screen_width = w;
	screen_height = h;
	initScreenRender();
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

	if (reset_action)
	{
		reset_action = false;
		resetObj(0);
	}
	else
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

	if (action == WALK)
	{
		angles[LEFTSHOULDER][X] = 45.0f;
		angles[LEFTARM][X] = -30.0f;

		angles[RIGHTSHOULDER][X] = -45.0f;
		angles[RIGHTARM][X] = -30.0f;

		angles[LEFTLEG][X] = -30.0f;
		angles[LEFTFOOT][X] = 0.0f;

		angles[RIGHTLEG][X] = 30.0f;
		angles[RIGHTFOOT][X] = 0.0f;
	}

	second_current = 0;
}

void updateObj(int frame)
{
	if (eyeAngley < eyeAngleyGoal - 0.1f)
		eyeAngley += 0.4f;
	else if(eyeAngley > eyeAngleyGoal + 0.1f)
		eyeAngley -= 0.4f;

	if (action == IDLE)
	{
		fly_position = 1.0f * sin((((float)frame + second_current * fps) / fps * 3.1415));
	}
	else if (action == WALK)
	{
		if (second_current % 2 == 0)
		{
			angles[LEFTSHOULDER][X] -= 1.5f;
			angles[LEFTARM][X] -= 1.0f;

			angles[RIGHTSHOULDER][X] += 1.5f;
			angles[RIGHTARM][X] += 1.0f;

			angles[LEFTLEG][X] += 1.0f;
			angles[LEFTFOOT][X] -= 0.25f;

			angles[RIGHTLEG][X] -= 1.0f;
			angles[RIGHTFOOT][X] += 0.25f;
		}
		else
		{
			angles[LEFTSHOULDER][X] += 1.5f;
			angles[LEFTARM][X] += 1.0f;

			angles[RIGHTSHOULDER][X] -= 1.5f;
			angles[RIGHTARM][X] -= 1.0f;

			angles[LEFTLEG][X] -= 1.0f;
			angles[LEFTFOOT][X] += 0.25f;

			angles[RIGHTLEG][X] += 1.0f;
			angles[RIGHTFOOT][X] -= 0.25f;
		}

		fly_position = 1.0f * sin((float)frame / fps * 3.1415);
	}
	else if (action == JumpingJack)
	{

	}
	else if (action == Squat)
	{

	}
	else if (action == MoonWalk)
	{

	}
	else if (action == GangnanStyle)
	{

	}
}

 GLuint M_KaID;
 GLuint M_KdID;
 GLuint M_KsID;

 void init()
 {
	 isFrame = false;
	 pps = 0;
	 action = WALK;
	 resetObj(0); // initial angles array

	 //VAO
	 glGenVertexArrays(1, &VAO);
	 glBindVertexArray(VAO);

	 ShaderInfo shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/DSPhong_Material.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/DSPhong_Material.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 gundaun_shader = LoadShaders(shaders);//讀取shader

	 ShaderInfo skybox_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/skybox.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/skybox.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 skybox_shader = LoadShaders(skybox_shaders);//讀取shader

	 ShaderInfo gray_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/gray.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/gray.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 gray_shader = LoadShaders(gray_shaders);//讀取shader

	 ShaderInfo uniform_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/uniform.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/uniform.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 uniform_shader = LoadShaders(uniform_shaders);//讀取shader

	 ShaderInfo gaussian_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/gaussian.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/gaussian.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 gaussian_shader = LoadShaders(gaussian_shaders);//讀取shader

	 glUseProgram(gundaun_shader);//uniform參數數值前必須先use shader

	 MatricesIdx = glGetUniformBlockIndex(gundaun_shader, "MatVP");
	 ModelID = glGetUniformLocation(gundaun_shader, "Model");
	 M_KaID = glGetUniformLocation(gundaun_shader, "Material.Ka");
	 M_KdID = glGetUniformLocation(gundaun_shader, "Material.Kd");
	 M_KsID = glGetUniformLocation(gundaun_shader, "Material.Ks");
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
	 glGetActiveUniformBlockiv(gundaun_shader, MatricesIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &UBOsize);
	 //bind UBO to its idx
	 glBindBufferRange(GL_UNIFORM_BUFFER, /*binding point*/0, UBO, 0, UBOsize);
	 glUniformBlockBinding(gundaun_shader, MatricesIdx, 0);
	 glBindBuffer(GL_UNIFORM_BUFFER, 0);

	 initSkybox();

	 //skybox_matrices_ubo
	 glGenBuffers(1, &skybox_matrices_ubo);
	 glBindBuffer(GL_UNIFORM_BUFFER, skybox_matrices_ubo);
	 glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 2, NULL, GL_STATIC_DRAW);
	 glBindBufferRange(GL_UNIFORM_BUFFER, /*binding point*/1, skybox_matrices_ubo, 0, sizeof(mat4) * 2);
	 glBindBuffer(GL_UNIFORM_BUFFER, 0);
	 glClearColor(0.0, 1.0, 0.0, 1);//black screen

	 initScreenRender();

	 initScreenQuad();
 }

#define DOR(body_angle) (body_angle*3.1415/180);
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	//==========================

	//update data to UBO for skybox
	glBindBuffer(GL_UNIFORM_BUFFER, skybox_matrices_ubo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &mat4(mat3(View)));
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &Projection);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//==========================

	switch (mode)
	{
	case 0: // LINE
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 1: // FILL
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	}

	if (pps != ORIGIN)
		renderScreenBegin();

	glBindVertexArray(VAO);
	glUseProgram(gundaun_shader);//uniform參數數值前必須先use shader
	GLuint offset[3] = { 0,0,0 };//offset for vertices , uvs , normals
	for (int i = 0; i < PARTSNUM; i++) {
		glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Models[i][0][0]);
		glUniform3fv(glGetUniformLocation(gundaun_shader, "vLightPosition"), 1, &light_pos[0]);

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

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	drawSkybox();

	if (pps != ORIGIN)
	{
		renderScreenEnd();

		drawScreenQuad();
	}

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

	glGenBuffers(1,&VBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(vec3),&vertices[0],GL_STATIC_DRAW);
	vertices_size[i] = vertices.size();

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
		if (eyeAngleyGoal > eyeAngley - 10)
			eyeAngleyGoal -= 10;
		break;
	case 'd':
	case 'D':
		if (eyeAngleyGoal < eyeAngley + 10)
			eyeAngleyGoal += 10;
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
	action = option;
	
	reset_action = true;
}

void ModeMenuEvents(int option)
{
	mode = option;
}

void PPSMenuEvents(int option)
{
	pps = option;
}

void initSkybox()
{
	GLfloat vertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	glGenVertexArrays(1, &skybox_VAO);
	glGenBuffers(1, &skybox_VBO);

	glBindVertexArray(skybox_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, skybox_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Unbind VAO
	glBindVertexArray(0);

	// for test
	/*skybox_textures_faces.push_back("../FreedomGunduan/images/skybox/right.jpg");
	skybox_textures_faces.push_back("../FreedomGunduan/images/skybox/left.jpg");
	skybox_textures_faces.push_back("../FreedomGunduan/images/skybox/top.jpg");
	skybox_textures_faces.push_back("../FreedomGunduan/images/skybox/bottom.jpg");
	skybox_textures_faces.push_back("../FreedomGunduan/images/skybox/back.jpg");
	skybox_textures_faces.push_back("../FreedomGunduan/images/skybox/front.jpg");*/
	skybox_textures_faces.push_back("../FreedomGunduan/images/space/right.bmp");
	skybox_textures_faces.push_back("../FreedomGunduan/images/space/left.bmp");
	skybox_textures_faces.push_back("../FreedomGunduan/images/space/top.bmp");
	skybox_textures_faces.push_back("../FreedomGunduan/images/space/bottom.bmp");
	skybox_textures_faces.push_back("../FreedomGunduan/images/space/front.bmp");
	skybox_textures_faces.push_back("../FreedomGunduan/images/space/back.bmp");
	cubemap_texture = loadCubemap(skybox_textures_faces);
}

GLuint loadCubemap(std::vector<std::string> skybox_textures_faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height;
	unsigned char* image;
	for (GLuint i = 0; i < skybox_textures_faces.size(); i++)
	{
		image = stbi_load(skybox_textures_faces[i].c_str(), &width, &height, 0, 0);
		if (image)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
			stbi_image_free(image);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << skybox_textures_faces[i] << std::endl;
			stbi_image_free(image);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void drawSkybox()
{
	// draw skybox as last
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	glUseProgram(skybox_shader);
	glUniform1i(glGetUniformLocation(skybox_shader, "skybox"), 0);
	// skybox cube
	glBindVertexArray(skybox_VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
}

void initScreenRender()
{
	//************************************************************************
	//
	// * generate a new texture
	//
	//========================================================================
	if (load_screen) // initial in different screen size
		glDeleteTextures(1, &screen_id);
	load_screen = true;
	glGenTextures(1, &screen_id);
	glBindTexture(GL_TEXTURE_2D, screen_id);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void renderScreenBegin()
{
	glGenFramebuffers(1, &screen_fbo);
	glGenRenderbuffers(1, &screen_rbo);
	glBindFramebuffer(GL_FRAMEBUFFER, screen_fbo);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glBindRenderbuffer(GL_RENDERBUFFER, screen_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screen_width, screen_height); //GL_DEPTH_COMPONENT24
	// attach it
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, screen_rbo);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, screen_id, 0);

	// clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void renderScreenEnd()
{
	//unbind
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glDeleteRenderbuffers(1, &screen_rbo);
	glDeleteFramebuffers(1, &screen_fbo);
}

void initScreenQuad()
{
	GLfloat vertices[] = {
		-1.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,

		 -1.0f,  1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f
	};

	glGenVertexArrays(1, &screen_quad_VAO);
	glGenBuffers(1, &screen_quad_VBO);

	glBindVertexArray(screen_quad_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, screen_quad_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Unbind VAO
	glBindVertexArray(0);
}

void drawScreenQuad()
{
	if (pps == GRAY)
		glUseProgram(gray_shader);
	else if (pps == UNIFORM)
		glUseProgram(uniform_shader);
	else if (pps == GAUSSIAN)
		glUseProgram(gaussian_shader);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, screen_id);
	if (pps == GRAY)
		glUniform1i(glGetUniformLocation(gray_shader, "screen"), 0);
	else if (pps == UNIFORM)
		glUniform1i(glGetUniformLocation(uniform_shader, "screen"), 0);
	else if (pps == GAUSSIAN)
	{
		glUniform1i(glGetUniformLocation(gaussian_shader, "screen"), 0);
		glUniform2fv(glGetUniformLocation(gaussian_shader, "img_size"), 1, &vec2(screen_width, screen_height)[0]);
	}
	glBindVertexArray(screen_quad_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
}