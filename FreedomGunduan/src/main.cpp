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
	//initOpenAL();
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
	glutAddMenuEntry("Yo Battle", 6);
	glutAddMenuEntry("Opening Pose", 7);
	glutAddMenuEntry("Shoot", 8);
	glutAddMenuEntry("All Shoot", 9);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//與右鍵關聯

	ModeMenu = glutCreateMenu(ModeMenuEvents);//建立右鍵菜單
	//加入右鍵物件
	glutAddMenuEntry("Line", 0);
	glutAddMenuEntry("Fill", 1);
	glutAddMenuEntry("Diamond Reflect", 2);
	glutAddMenuEntry("Diamond Refract", 3);
	glutAddMenuEntry("Diamond", 4);
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
	Projection = perspective(80.0f, (float)w / h, 0.1f, 500.0f);
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
	light_pos = vec3(-10, 0, 0);

	shooting = false;

	for (int i = 0; i < PARTSNUM; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			angles[i][j] = 0.0f;
		}
	}
	angles[LEFTARM][Y] = -22.2f;
	angles[LEFTARM][Z] = -26.8f;

	angles[RIGHTARM][Z] = 32.2f;

	angles[LEFTLEGARMOR][Z] = -20.456f;
	angles[RIGHTLEGARMOR][Z] = 21.505f;

	angles[LEFTCONNECTOR][Y] = -25.888f;
	angles[LEFTCONNECTOR][X] = 5.0f;
	angles[LEFTINSIDEBIGWING][Y] = 2.118f;
	angles[LEFTGUN][Y] = 1.657f;
	angles[LEFTOUTSIDEBIGWING][Y] = -1.487f;
	angles[LEFTOUTSIDESMALLWING][Y] = -1.013f;
	angles[LEFTMIDDLESMALLWING][Y] = -0.756f;
	angles[LEFTINSIDESMALLWING][Y] = 2.77f;

	angles[RIGHTCONNECTOR][Y] = 20.888f;
	angles[RIGHTCONNECTOR][X] = 5.0f;
	angles[RIGHTINSIDEBIGWING][Y] = -2.118f;
	angles[RIGHTGUN][Y] = -1.657f;
	angles[RIGHTOUTSIDEBIGWING][Y] = 1.487f;
	angles[RIGHTOUTSIDESMALLWING][Y] = 1.013f;
	angles[RIGHTMIDDLESMALLWING][Y] = 0.756f;
	angles[RIGHTINSIDESMALLWING][Y] = 2.77f;

	positions[BODY][X] = 0.0f;
	positions[BODY][Y] = 0.0f; // 34.0f
	positions[BODY][Z] = 0.0f;

	positions[HEAD][X] = 0;
	positions[HEAD][Y] = 26.0f;
	positions[HEAD][Z] = 0;

	positions[LEFTSHOULDER][X] = -12.0f;
	positions[LEFTSHOULDER][Y] = 19.0f;
	positions[LEFTSHOULDER][Z] = -1.0f;

	positions[LEFTARM][X] = -8.1f;
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

	positions[LEFTLEGARMOR][X] = -6.0f;
	positions[LEFTLEGARMOR][Y] = 8.0f;
	positions[LEFTLEGARMOR][Z] = 2.0f;

	positions[LEFTLEGGUNSTOCK][X] = 0.0f;
	positions[LEFTLEGGUNSTOCK][Y] = -7.0f;
	positions[LEFTLEGGUNSTOCK][Z] = -5.0f;

	positions[LEFTLEGGUNBARREL][X] = 0.0f;
	positions[LEFTLEGGUNBARREL][Y] = -2.778f;
	positions[LEFTLEGGUNBARREL][Z] = 4.025f;

	positions[LEFTLEGGUNPOINT][X] = 2.0f;
	positions[LEFTLEGGUNPOINT][Y] = -43.0f;
	positions[LEFTLEGGUNPOINT][Z] = -11.0f;

	positions[LEFTFOOT][X] = -4.0f;
	positions[LEFTFOOT][Y] = -31.0f;
	positions[LEFTFOOT][Z] = 12.0f;

	positions[RIGHTLEG][X] = 8.0f;
	positions[RIGHTLEG][Y] = -10.0f;
	positions[RIGHTLEG][Z] = 0.0f;

	positions[RIGHTLEGARMOR][X] = 6.0f;
	positions[RIGHTLEGARMOR][Y] = 9.0f;
	positions[RIGHTLEGARMOR][Z] = 3.0f;

	positions[RIGHTLEGGUNSTOCK][X] = 0.0f;
	positions[RIGHTLEGGUNSTOCK][Y] = -7.0f;
	positions[RIGHTLEGGUNSTOCK][Z] = -4.0f;

	positions[RIGHTLEGGUNBARREL][X] = 0.0f;
	positions[RIGHTLEGGUNBARREL][Y] = -2.778f;
	positions[RIGHTLEGGUNBARREL][Z] = 5.0f;

	positions[RIGHTLEGGUNPOINT][X] = 0.0f;
	positions[RIGHTLEGGUNPOINT][Y] = -44.0f;
	positions[RIGHTLEGGUNPOINT][Z] = -10.0f;

	positions[RIGHTFOOT][X] = 5.0f;
	positions[RIGHTFOOT][Y] = -29.0f;
	positions[RIGHTFOOT][Z] = 12.0f;

	positions[LEFTCONNECTOR][X] = -14.0f;
	positions[LEFTCONNECTOR][Y] = 0.0f;
	positions[LEFTCONNECTOR][Z] = -8.0f;

	positions[LEFTINSIDEBIGWING][X] = -17.0f;
	positions[LEFTINSIDEBIGWING][Y] = 12.0f;
	positions[LEFTINSIDEBIGWING][Z] = 3.0f;

	positions[LEFTGUN][X] = -21.0f;
	positions[LEFTGUN][Y] = 25.0f;
	positions[LEFTGUN][Z] = 0.0f;

	positions[LEFTOUTSIDEBIGWING][X] = -17.0f;
	positions[LEFTOUTSIDEBIGWING][Y] = 12.0f;
	positions[LEFTOUTSIDEBIGWING][Z] = -3.0f;

	positions[LEFTOUTSIDESMALLWING][X] = -11.0f;
	positions[LEFTOUTSIDESMALLWING][Y] = 4.0f;
	positions[LEFTOUTSIDESMALLWING][Z] = -3.0f;

	positions[LEFTMIDDLESMALLWING][X] = -11.0f;
	positions[LEFTMIDDLESMALLWING][Y] = 4.0f;
	positions[LEFTMIDDLESMALLWING][Z] = 0.0f;

	positions[LEFTINSIDESMALLWING][X] = -11.0f;
	positions[LEFTINSIDESMALLWING][Y] = 4.0f;
	positions[LEFTINSIDESMALLWING][Z] = 3.0f;

	positions[RIGHTCONNECTOR][X] = 14.0f;
	positions[RIGHTCONNECTOR][Y] = 0.0f;
	positions[RIGHTCONNECTOR][Z] = -8.0f;

	positions[RIGHTINSIDEBIGWING][X] = 17.0f;
	positions[RIGHTINSIDEBIGWING][Y] = 12.0f;
	positions[RIGHTINSIDEBIGWING][Z] = 3.0f;

	positions[RIGHTGUN][X] = 20.0f;
	positions[RIGHTGUN][Y] = 24.0f;
	positions[RIGHTGUN][Z] = -0.2f;

	positions[RIGHTOUTSIDEBIGWING][X] = 17.0f;
	positions[RIGHTOUTSIDEBIGWING][Y] = 12.0f;
	positions[RIGHTOUTSIDEBIGWING][Z] = -3.0f;

	positions[RIGHTOUTSIDESMALLWING][X] = 11.5f;
	positions[RIGHTOUTSIDESMALLWING][Y] = 4.0f;
	positions[RIGHTOUTSIDESMALLWING][Z] = -3.0f;

	positions[RIGHTMIDDLESMALLWING][X] = 11.5f;
	positions[RIGHTMIDDLESMALLWING][Y] = 4.0f;
	positions[RIGHTMIDDLESMALLWING][Z] = 0.0f;

	positions[RIGHTINSIDESMALLWING][X] = 11.5f;
	positions[RIGHTINSIDESMALLWING][Y] = 4.0f;
	positions[RIGHTINSIDESMALLWING][Z] = 3.0f;

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

	JumpingJack_beginpose = true;

	GangnanStyle_rightfoot = false;
	GangnanStyle_footcount = 0;
	GangnanStyle_handdown = false;
	GangnanStyle_handprepare = false;
	GangnanStyle_temple = 0;
	GangnanStyle_beginpose = true;
}

void updateObj(int frame)
{
	if (eyeAngley < eyeAngleyGoal - 0.1f)
		eyeAngley += 0.4f;
	else if (eyeAngley > eyeAngleyGoal + 0.1f)
		eyeAngley -= 0.4f;

	if (eyeX < eyeXGoal - 0.1f)
		eyeX += 0.4f;
	else if (eyeX > eyeXGoal + 0.1f)
		eyeX -= 0.4f;

	for (int asteroids_index = 0; asteroids_index < ASTEROIDAMOUNT; asteroids_index++)
	{
		asteroids_pos[asteroids_index].z -= gundam_speed;
		if (asteroids_pos[asteroids_index].z < -500.0f)
			asteroids_pos[asteroids_index].z += 1000.0f;
	}

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

			angles[WING][X] += 0.25f;

			//angles[LEFTLEGARMOR][X] += 5.0f;
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

			angles[WING][X] -= 0.25f;

			//angles[LEFTLEGARMOR][X] -= 5.0f;
		}

		fly_position = 1.0f * sin((float)frame / fps * 3.1415);
	}
	else if (action == JumpingJack)
	{
		if (second_current == 0 && frame < 30)
		{
			angles[RIGHTSHOULDER][Y] += 1.5f;

			angles[LEFTSHOULDER][Y] -= 1.5f;
		}

		if (second_current % 2 == 0)
		{
			angles[LEFTSHOULDER][Z] -= 1.5f;
			angles[LEFTARM][X] -= 1.5f;

			angles[RIGHTSHOULDER][Z] += 1.5f;
			angles[RIGHTARM][X] -= 1.5f;

			angles[LEFTLEG][Z] -= 0.5f;
			angles[LEFTFOOT][Z] += 0.25f;
			angles[LEFTLEGARMOR][Z] -= 0.25f;

			angles[RIGHTLEG][Z] += 0.5f;
			angles[RIGHTFOOT][Z] -= 0.25f;
			angles[RIGHTLEGARMOR][Z] += 0.25f;

			angles[WING][X] += 0.75f;

			angles[LEFTINSIDEBIGWING][Z] -= 2.0f;
			angles[LEFTINSIDESMALLWING][Z] -= 1.5f;
			angles[LEFTMIDDLESMALLWING][Z] -= 1.0f;
			angles[LEFTOUTSIDESMALLWING][Z] -= 0.5f;

			angles[RIGHTINSIDEBIGWING][Z] += 2.0f;
			angles[RIGHTINSIDESMALLWING][Z] += 1.5f;
			angles[RIGHTMIDDLESMALLWING][Z] += 1.0f;
			angles[RIGHTOUTSIDESMALLWING][Z] += 0.5f;
		}
		else
		{
			angles[LEFTSHOULDER][Z] += 1.5f;
			angles[LEFTARM][X] += 1.5f;

			angles[RIGHTSHOULDER][Z] -= 1.5f;
			angles[RIGHTARM][X] += 1.5f;

			angles[LEFTLEG][Z] += 0.5f;
			angles[LEFTFOOT][Z] -= 0.25f;
			angles[LEFTLEGARMOR][Z] += 0.25f;

			angles[RIGHTLEG][Z] -= 0.5f;
			angles[RIGHTFOOT][Z] += 0.25f;
			angles[RIGHTLEGARMOR][Z] -= 0.25f;

			angles[WING][X] -= 0.75f;

			angles[LEFTINSIDEBIGWING][Z] += 2.0f;
			angles[LEFTINSIDESMALLWING][Z] += 1.5f;
			angles[LEFTMIDDLESMALLWING][Z] += 1.0f;
			angles[LEFTOUTSIDESMALLWING][Z] += 0.5f;

			angles[RIGHTINSIDEBIGWING][Z] -= 2.0f;
			angles[RIGHTINSIDESMALLWING][Z] -= 1.5f;
			angles[RIGHTMIDDLESMALLWING][Z] -= 1.0f;
			angles[RIGHTOUTSIDESMALLWING][Z] -= 0.5f;
		}

		fly_position = 3.0f * sin((float)frame / fps * 3.1415);
	}
	else if (action == Squat)
	{
		if (second_current % 2 == 0)
		{
			angles[RIGHTSHOULDER][X] -= 1.5f;

			angles[LEFTSHOULDER][X] -= 1.5f;

			angles[LEFTLEG][X] -= 1.5f;
			angles[LEFTLEGARMOR][X] += 1.2f;
			angles[LEFTLEGARMOR][Z] -= 0.5f;
			angles[LEFTFOOT][X] += 1.5f;

			angles[RIGHTLEG][X] -= 1.5f;
			angles[RIGHTLEGARMOR][X] += 1.2f;
			angles[RIGHTLEGARMOR][Z] += 0.5f;
			angles[RIGHTFOOT][X] += 1.5f;

			positions[BODY][Y] -= 0.075f;

			angles[WING][X] += 0.5f;

			angles[LEFTINSIDEBIGWING][Z] -= 1.0f;
			angles[LEFTINSIDESMALLWING][Z] -= 0.8f;
			angles[LEFTMIDDLESMALLWING][Z] -= 0.6f;
			angles[LEFTOUTSIDESMALLWING][Z] -= 0.4f;

			angles[RIGHTINSIDEBIGWING][Z] += 1.0f;
			angles[RIGHTINSIDESMALLWING][Z] += 0.8f;
			angles[RIGHTMIDDLESMALLWING][Z] += 0.6f;
			angles[RIGHTOUTSIDESMALLWING][Z] += 0.4f;
		}
		else
		{
			angles[RIGHTSHOULDER][X] += 1.5f;

			angles[LEFTSHOULDER][X] += 1.5f;

			angles[LEFTLEG][X] += 1.5f;
			angles[LEFTLEGARMOR][X] -= 1.2f;
			angles[LEFTLEGARMOR][Z] += 0.5;
			angles[LEFTFOOT][X] -= 1.5f;

			angles[RIGHTLEG][X] += 1.5f;
			angles[RIGHTLEGARMOR][X] -= 1.2f;
			angles[RIGHTLEGARMOR][Z] -= 0.5;
			angles[RIGHTFOOT][X] -= 1.5f;

			positions[BODY][Y] += 0.075f;

			angles[WING][X] -= 0.5f;

			angles[LEFTINSIDEBIGWING][Z] += 1.0f;
			angles[LEFTINSIDESMALLWING][Z] += 0.8f;
			angles[LEFTMIDDLESMALLWING][Z] += 0.6f;
			angles[LEFTOUTSIDESMALLWING][Z] += 0.4f;

			angles[RIGHTINSIDEBIGWING][Z] -= 1.0f;
			angles[RIGHTINSIDESMALLWING][Z] -= 0.8f;
			angles[RIGHTMIDDLESMALLWING][Z] -= 0.6f;
			angles[RIGHTOUTSIDESMALLWING][Z] -= 0.4f;
		}
	}
	else if (action == MoonWalk)
	{
		if (second_current == 0 && frame < 45)
		{
			if (frame < 15)
			{
				angles[HEAD][Y] += 2.0f;
				angles[LEFTSHOULDER][Z] += 1.5f;
				angles[RIGHTSHOULDER][Z] -= 1.5f;
			}
			else if (frame < 30)
			{
				angles[HEAD][Y] -= 4.0f;
			}
			else
			{
				angles[HEAD][Y] += 2.0f;
			}
		}
		else if (second_current == 0 || (second_current == 1 && frame < 45))
		{
			angles[BODY][Y] += 6.0f;
		}
		else if (second_current == 1)
		{
			angles[LEFTSHOULDER][Z] -= 1.5f;
			angles[RIGHTSHOULDER][Z] += 1.5f;
		}
		else if ((second_current == 2 && frame < 15))
		{
			angles[BODY][Y] += 6.0f;

			angles[LEFTSHOULDER][X] += 1.5f;
			angles[LEFTARM][X] += 1.0f;

			angles[RIGHTSHOULDER][X] -= 1.5f;
			angles[RIGHTARM][X] -= 1.0f;

			angles[LEFTLEG][X] -= 1.0f;
			angles[LEFTFOOT][X] += 0.25f;

			angles[RIGHTLEG][X] += 1.0f;
			angles[RIGHTFOOT][X] -= 0.25f;
		}
		else
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

				angles[WING][X] += 0.5f;
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

				angles[WING][X] -= 0.5f;
			}
			positions[BODY][X] -= 0.15f;
		}
	}
	else if (action == GangnanStyle)
	{
		if (second_current == 0 && frame < 30)
		{
			angles[RIGHTARM][X] -= 1.5f; // -45
			angles[RIGHTSHOULDER][Y] -= 2.0f; // -60

			//angles[LEFTARM][Z] += 1.0f; // 30
			angles[LEFTSHOULDER][X] -= 4.5f; // -135
			//angles[LEFTSHOULDER][Z] -= 1.0f; // -30
		}

		if (frame % 30 == 0)
		{
			if (GangnanStyle_footcount == 3)
			{
				GangnanStyle_footcount = 0;
				GangnanStyle_temple++;
				if (GangnanStyle_temple == 2)
				{
					GangnanStyle_temple = 0;
					GangnanStyle_handdown = !GangnanStyle_handdown;
					GangnanStyle_handprepare = true;
				}
			}
			else
			{
				GangnanStyle_rightfoot = !GangnanStyle_rightfoot;
				GangnanStyle_footcount++;
			}
		}

		if (GangnanStyle_handdown)
		{
			if (GangnanStyle_handprepare)
			{
				angles[LEFTARM][X] -= 6.0f; // 45 to -45
				angles[LEFTSHOULDER][Y] += 4.0f; // 0 to 60

				//angles[LEFTARM][Z] -= 2.0f; // 60 to 30
				angles[LEFTSHOULDER][X] += 7.0f; // -135 to -30
				//if (GangnanStyle_beginpose)
				//	angles[LEFTSHOULDER][Z] += 2.0f; // -30 to 0
				//else
				//	angles[LEFTSHOULDER][Z] += 4.0f; // -60 to 0
				if (angles[LEFTSHOULDER][X] > -31.0f) // after 15 frames
				{
					GangnanStyle_handprepare = false;
					GangnanStyle_beginpose = false;
				}
			}
			else
			{
				if (frame % 30 < 15)
				{
					angles[LEFTSHOULDER][X] += 2.0f;
					angles[RIGHTSHOULDER][X] += 2.0f;
				}
				else
				{
					angles[LEFTSHOULDER][X] -= 2.0f;
					if (!(GangnanStyle_temple == 1 && GangnanStyle_footcount == 3))
						angles[RIGHTSHOULDER][X] -= 2.0f;
				}
			}
		}
		else
		{
			if (GangnanStyle_handprepare)
			{
				angles[LEFTARM][X] += 6.0f; // -45 to 45
				angles[LEFTSHOULDER][Y] -= 4.0f; // 60 to 0

				//angles[LEFTARM][Z] += 2.62f; // 20.636082 to 60
				angles[LEFTSHOULDER][X] -= 5.0f; // -60 to -135
				//angles[LEFTSHOULDER][Z] -= 2.0f; // 0 to -30

				if (angles[LEFTSHOULDER][X] < -134.0f) // after 15 frames
				{
					GangnanStyle_handprepare = false;
				}
			}
			else
			{
				if (frame % 30 < 15)
				{
					angles[LEFTSHOULDER][Z] += 2.0f;
				}
				else
				{
					angles[LEFTSHOULDER][Z] -= 2.0f;
				}

				angles[LEFTSHOULDER][Z] = 35.0f * sin((frame / 15.0f) * 3.1415);
				angles[LEFTARM][X] = 35.0f * cos((frame / 15.0f) * 3.1415);
			}
		}

		if (GangnanStyle_rightfoot)
		{
			if (frame % 30 < 15)
			{
				angles[RIGHTLEG][X] -= 2.0f;
				angles[RIGHTLEG][Z] += 2.0f;
				angles[RIGHTFOOT][X] += 2.0f;
				angles[RIGHTFOOT][Z] -= 2.0f;

				positions[BODY][Y] += 0.1f;
			}
			else
			{
				angles[RIGHTLEG][X] += 2.0f;
				angles[RIGHTLEG][Z] -= 2.0f;
				angles[RIGHTFOOT][X] -= 2.0f;
				angles[RIGHTFOOT][Z] += 2.0f;

				positions[BODY][Y] -= 0.1f;
			}
		}
		else
		{
			if (frame % 30 < 15)
			{
				angles[LEFTLEG][X] -= 2.0f;
				angles[LEFTLEG][Z] -= 2.0f;
				angles[LEFTFOOT][X] += 2.0f;
				angles[LEFTFOOT][Z] += 2.0f;

				positions[BODY][Y] += 0.1f;
			}
			else
			{
				angles[LEFTLEG][X] += 2.0f;
				angles[LEFTLEG][Z] += 2.0f;
				angles[LEFTFOOT][X] -= 2.0f;
				angles[LEFTFOOT][Z] -= 2.0f;

				positions[BODY][Y] -= 0.1f;
			}
		}
	}
	else if (action == YoBattle)
	{
		if (second_current == 0 && frame < 30)
		{
			angles[LEFTARM][X] -= 2.5f; // -75
			angles[LEFTSHOULDER][Y] += 3.0f; // 90

			angles[RIGHTARM][X] -= 2.5f; // -75
			angles[RIGHTSHOULDER][Y] -= 3.0f; // -90

			angles[BODY][Z] += 0.5f; // 15
			angles[LEFTLEG][Z] -= 0.5f; // -15 resist body
			angles[RIGHTLEG][Z] -= 0.5f; // -15 resist body
		}
		else if (second_current == 0)
		{
			angles[RIGHTSHOULDER][Y] += 2.0f; // -90 to -30
			angles[RIGHTSHOULDER][X] -= 2.0f; // 0 to -60

			angles[BODY][X] -= 0.5f; // -15
			angles[LEFTLEG][X] += 0.5f; // 15 resist body
			angles[RIGHTLEG][X] += 0.5f; // 15 resist body
		}
		else if (second_current == 1 && frame >= 30)
		{
			angles[RIGHTSHOULDER][Y] -= 2.0f; // -30 to -90
			angles[RIGHTSHOULDER][X] += 2.0f; // -60 to 0

			angles[BODY][X] += 0.5f; // -15 to 0
			angles[LEFTLEG][X] -= 0.5f; // 15 to 0 resist body
			angles[RIGHTLEG][X] -= 0.5f; // 15 to 0 resist body
		}
		else if (second_current == 2 && frame < 30)
		{
			angles[LEFTSHOULDER][Y] -= 3.0f; // 90 to 0
			angles[RIGHTSHOULDER][Y] += 3.0f; // -90 to 0
			angles[LEFTARM][X] += 2.5f; // -75 to 0
			angles[RIGHTARM][X] += 2.5f; // -75 to 0
		}

		if (second_current == 2)
		{
			if (frame < 10)
			{
				angles[LEFTLEG][Z] += 1.5f; // -15 to 0 resist body
				angles[RIGHTLEG][Z] += 1.5f; // -15 to 0 resist body
			}
			else
			{
				positions[BODY][Y] -= 0.1f;
				positions[BODY][X] -= 0.2f;
			}

			angles[LEFTSHOULDER][Z] -= 1.0f; // 0 to -60
			angles[RIGHTSHOULDER][Z] += 1.0f; // 0 to 60

			angles[BODY][Z] -= 1.5f; // 15 to -75
		}
	}
	else if (action == Opening)
	{
		if (second_current == 0 && frame == 0)
		{
			earth_pos = earth_pos_begin;
		}

		if (second_current == 0 && frame < 15)
		{
			angles[HEAD][Y] -= 4.0f;
			angles[HEAD][X] -= 1.0f;

			angles[LEFTSHOULDER][X] -= 12.0f;

			angles[BODY][Y] += 1.0f;

			angles[LEFTLEG][Z] -= 1.0f;
			angles[LEFTFOOT][X] += 1.0f;
			angles[RIGHTLEG][Z] += 3.0f;
			angles[RIGHTFOOT][X] += 3.0f;

			positions[BODY][X] += 0.3f;
			positions[BODY][Y] += 0.2f;
			positions[BODY][Z] += 0.3f;

			earth_pos.x -= 2.0f;
			earth_pos.y -= 2.0f;
		}
		else if (second_current == 0 && frame >= 30 && frame < 35)
		{
			angles[LEFTSHOULDER][X] += 27.0f;
			angles[LEFTSHOULDER][Y] -= 6.0f;
			angles[LEFTSHOULDER][Z] -= 9.0f;
			angles[LEFTARM][X] -= 1.0f;

			earth_pos.x -= 8.0f;
			earth_pos.y += 6.0f;
		}
		else if (second_current == 0 && frame == 59)
		{
			light_pos = vec3(positions[BODY][X] - 20, positions[BODY][Y], positions[BODY][Z] - 20);
		}
	}
	else if (action == Shoot)
	{
		if (second_current == 0 && frame < 15)
		{
			angles[LEFTINSIDEBIGWING][Z] -= 8.0f;
			angles[LEFTINSIDESMALLWING][Z] -= 6.0f;
			angles[LEFTMIDDLESMALLWING][Z] -= 4.0f;
			angles[LEFTOUTSIDESMALLWING][Z] -= 2.0f;

			angles[RIGHTINSIDEBIGWING][Z] += 8.0f;
			angles[RIGHTINSIDESMALLWING][Z] += 6.0f;
			angles[RIGHTMIDDLESMALLWING][Z] += 4.0f;
			angles[RIGHTOUTSIDESMALLWING][Z] += 2.0f;
		}
		else if (second_current == 0 && frame < 30)
		{
			gundam_speed = 5.0f;

			positions[BODY][Y] -= 0.05f;

			angles[WING][X] += 1.5f;
			angles[LEFTFOOT][X] += 2.0f;
			angles[RIGHTFOOT][X] += 2.0f;
		}
		else if (second_current == 1 && frame < 30)
		{
			angles[BODY][Z] += 12.0f;
		}
		else if (second_current == 1 && frame >= 45)
		{
			angles[LEFTSHOULDER][X] -= 12.0f;
			angles[LEFTSHOULDER][Z] += 1.5f;
		}
		else if (second_current == 2 && frame < 5)
		{
			angles[LEFTSHOULDER][X] += 22.0f;
			angles[LEFTARM][Y] += 10.0f;
		}

		// shoot first beam
		if (second_current == 2 && frame == 10)
		{
			shooting = true;
			beam_pos = vec3(Models[LEFTARM] * vec4(-2.161, -62.734, 25.61, 1));
			beam_scale = vec3(0.25, 0.25, 0.25);
		}
		if (second_current == 2 && frame > 10 && frame < 20)
		{
			beam_scale = vec3(0.25, 0.25, 0.25 * (frame - 10) * beam_speed * 8);
		}
		if (second_current == 2 && frame > 10)
		{
			beam_offset = vec3(0, 0, (frame - 10) * beam_speed);
		}

		// shoot second beam
		if (second_current == 2 && frame == 40)
		{
			beam_scale = vec3(0.25, 0.25, 0.25);
		}
		if (second_current == 2 && frame > 40 && frame < 50)
		{
			beam_scale = vec3(0.25, 0.25, 0.25 * (frame - 40) * beam_speed * 8);
		}
		if (second_current == 2 && frame > 40)
		{
			beam_offset = vec3(0, 0, (frame - 40) * beam_speed);
		}

		// shoot third beam
		if (second_current == 3 && frame == 10)
		{
			beam_scale = vec3(0.25, 0.25, 0.25);
		}
		if (second_current == 3 && frame > 10 && frame < 20)
		{
			beam_scale = vec3(0.25, 0.25, 0.25 * (frame - 10) * beam_speed * 8);
		}
		if (second_current == 3 && frame > 10)
		{
			beam_offset = vec3(0, 0, (frame - 10) * beam_speed);
		}
	}
	else if (action == AllShoot)
	{
		if (second_current == 0 && frame < 15)
		{
			angles[LEFTGUN][Z] -= 12.0f;
			angles[RIGHTGUN][Z] += 12.0f;

			angles[LEFTLEGGUNBARREL][X] -= 7.0f;
			angles[RIGHTLEGGUNBARREL][X] -= 7.0f;
			angles[LEFTLEGGUNSTOCK][X] += 4.0f;
			angles[RIGHTLEGGUNSTOCK][X] += 4.0f;

			angles[LEFTSHOULDER][X] -= 12.0f;
			angles[LEFTSHOULDER][Z] += 1.5f;
		}
		else if (second_current == 0 && frame >= 20 && frame < 30)
		{
			angles[LEFTGUN][X] += 8.5f;
			angles[RIGHTGUN][X] += 8.5f;

			angles[LEFTSHOULDER][X] += 11.0f;
			angles[LEFTARM][Y] += 5.0f;

			angles[LEFTLEGGUNPOINT][X] -= 18.0f;
			angles[RIGHTLEGGUNPOINT][X] -= 18.0f;

			positions[LEFTLEGGUNPOINT][Z] += 0.3f;
			positions[RIGHTLEGGUNPOINT][Z] += 0.3f;
		}

		// shoot first beam
		if (second_current == 0 && frame == 40)
		{
			shooting = true;
			beam_pos = vec3(Models[LEFTARM] * vec4(-2.161, -62.734, 25.61, 1));
			beam_scale = vec3(0.25, 0.25, 0.25);
		}
		if (second_current == 0 && frame > 40)
		{
			beam_scale = vec3(0.5, 0.5, 0.5 * (frame - 40) * beam_speed * 4);
		}
		if (second_current == 0 && frame > 40)
		{
			beam_offset = vec3(0, 0, (frame - 40) * beam_speed);
		}
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
	 for (int asteroids_index = 0; asteroids_index < ASTEROIDAMOUNT; asteroids_index++)
	 {
		 asteroids_species[asteroids_index] = rand() % ASTEROIDNUM;
		 do
		 {
			 asteroids_pos[asteroids_index] = vec3(rand() % 200 - 100, rand() % 200 - 100, rand() % 1000 - 500);
		 } while (distance(vec2(asteroids_pos[asteroids_index].x, asteroids_pos[asteroids_index].y), vec2(positions[BODY][X], positions[BODY][Y])) < 50.0f);
		 asteroids_scale[asteroids_index] = vec3(rand() % 8);
	 }

	 //VAO
	 glGenVertexArrays(1, &VAO);

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

	 ShaderInfo basic_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/simple.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/simple.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 basic_shader = LoadShaders(basic_shaders);//讀取shader

	 ShaderInfo diamond_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/diamond_surface.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/diamond_surface.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 diamond_shader = LoadShaders(diamond_shaders);//讀取shader

	 ShaderInfo texture_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/texture.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/texture.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 texture_shader = LoadShaders(texture_shaders);//讀取shader

	 glUseProgram(gundaun_shader);//uniform參數數值前必須先use shader

	 MatricesIdx = glGetUniformBlockIndex(gundaun_shader, "MatVP");
	 ModelID = glGetUniformLocation(gundaun_shader, "u_model");
	 M_KaID = glGetUniformLocation(gundaun_shader, "Material.Ka");
	 M_KdID = glGetUniformLocation(gundaun_shader, "Material.Kd");
	 M_KsID = glGetUniformLocation(gundaun_shader, "Material.Ks");
	 //or
	 M_KdID = M_KaID + 1;
	 M_KsID = M_KaID + 2;

	 Projection = glm::perspective(80.0f, 4.0f / 3.0f, 0.1f, 500.0f);
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

	 //UBO
	 glGenBuffers(1, &UBO);
	 glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	 glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 2, NULL, GL_STATIC_DRAW);
	 glBindBufferRange(GL_UNIFORM_BUFFER, /*binding point*/0, UBO, 0, sizeof(mat4) * 2);
	 glBindBuffer(GL_UNIFORM_BUFFER, 0);

	 sphere_vao = sphereGenerator(4);
	 earth_texture = loadTexture("../FreedomGunduan/images/earth.bmp");
	 beam_texture = loadTexture("../FreedomGunduan/images/beam.bmp");
	 asteroids_textures[0] = loadTexture("../FreedomGunduan/images/asteroids/Aster_Small_1_Color.bmp");
	 asteroids_textures[1] = loadTexture("../FreedomGunduan/images/asteroids/Aster_Small_2_Color.bmp");
	 asteroids_textures[2] = loadTexture("../FreedomGunduan/images/asteroids/Aster_Small_3_Color.bmp");
	 asteroids_textures[3] = loadTexture("../FreedomGunduan/images/asteroids/Aster_Small_4_Color.bmp");
	 asteroids_textures[4] = loadTexture("../FreedomGunduan/images/asteroids/Aster_Small_5_Color.bmp");
	 asteroids_textures[5] = loadTexture("../FreedomGunduan/images/asteroids/Aster_Small_6_Color.bmp");

	 glGenVertexArrays(1, &asteroids_VAO);

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
		vec3(eyedistance * sin(eyey) + eyeX, 2 + eyeheight, eyedistance * cos(eyey)), // Camera is at (0,0,20), in World Space
		vec3(eyeX, eyeheight, 0), // and looks at the origin
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

	if (mode == DIAMONDREFLECT || mode == DIAMONDREFRACT || mode == DIAMOND)
	{
		glUseProgram(diamond_shader);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
		glUniform1i(glGetUniformLocation(diamond_shader, "skybox"), 0);
		glUniform1i(glGetUniformLocation(diamond_shader, "mode"), mode - DIAMONDREFLECT);
	}
	else
		glUseProgram(gundaun_shader); //uniform參數數值前必須先use shader

	GLuint offset[3] = { 0,0,0 };//offset for vertices , uvs , normals
	for (int i = 0; i < PARTSNUM; i++) {
		glBindVertexArray(VAO);
		if (!(mode == DIAMONDREFLECT || mode == DIAMONDREFRACT || mode == DIAMOND))
		{
			glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Models[i][0][0]);
			glUniform3fv(glGetUniformLocation(gundaun_shader, "vLightPosition"), 1, &light_pos[0]);
		}
		else
		{
			glUniformMatrix4fv(glGetUniformLocation(diamond_shader, "u_model"), 1, GL_FALSE, &Models[i][0][0]);
		}

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
			if (!(mode == DIAMONDREFLECT || mode == DIAMONDREFRACT || mode == DIAMOND))
			{
				glUniform3fv(M_KdID, 1, &KDs[mtlname][0]);
				glUniform3fv(M_KsID, 1, &Ks[0]);
			}
			glDrawArrays(GL_TRIANGLES, vertexIDoffset, faces[i][j + 1] * 3);
			//we draw triangles by giving the glVertexID base and vertex count is face count*3
			vertexIDoffset += faces[i][j + 1] * 3;//glVertexID's base offset is face count*3
		}//end for loop for draw one part of the robot	

	//unbind VAO
		glBindVertexArray(0);
	}//end for loop for updating and drawing model

	glUseProgram(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (action == Opening)
	{
		drawEarth();
	}

	if (shooting)
	{
		drawBeam();
	}

	if (action != Opening)
	{
		drawAsteroids();
	}

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

	load2Buffer("../FreedomGunduan/objs/wingBase.obj", WING);

	load2Buffer("../FreedomGunduan/objs/leftLeg.obj", LEFTLEG);
	load2Buffer("../FreedomGunduan/objs/leftLegArmor.obj", LEFTLEGARMOR);
	load2Buffer("../FreedomGunduan/objs/leftLegGunStock.obj", LEFTLEGGUNSTOCK);
	load2Buffer("../FreedomGunduan/objs/leftLegGunBarrel.obj", LEFTLEGGUNBARREL);
	load2Buffer("../FreedomGunduan/objs/leftLegGunPoint.obj", LEFTLEGGUNPOINT);
	load2Buffer("../FreedomGunduan/objs/leftLegSaber.obj", LEFTLEGSABER);
	load2Buffer("../FreedomGunduan/objs/left_foot.obj", LEFTFOOT);

	load2Buffer("../FreedomGunduan/objs/rightLeg.obj", RIGHTLEG);
	load2Buffer("../FreedomGunduan/objs/rightLegArmor.obj", RIGHTLEGARMOR);
	load2Buffer("../FreedomGunduan/objs/rightLegGunStock.obj", RIGHTLEGGUNSTOCK);
	load2Buffer("../FreedomGunduan/objs/rightLegGunBarrel.obj", RIGHTLEGGUNBARREL);
	load2Buffer("../FreedomGunduan/objs/rightLegGunPoint.obj", RIGHTLEGGUNPOINT);
	load2Buffer("../FreedomGunduan/objs/rightLegSaber.obj", RIGHTLEGSABER);
	load2Buffer("../FreedomGunduan/objs/right_foot.obj", RIGHTFOOT);

	load2Buffer("../FreedomGunduan/objs/leftConnector.obj", LEFTCONNECTOR);
	load2Buffer("../FreedomGunduan/objs/leftInsideBigWing.obj", LEFTINSIDEBIGWING);
	load2Buffer("../FreedomGunduan/objs/leftGun.obj", LEFTGUN);
	load2Buffer("../FreedomGunduan/objs/leftOutsideBigWing.obj", LEFTOUTSIDEBIGWING);
	load2Buffer("../FreedomGunduan/objs/leftOutsideSmallWing.obj", LEFTOUTSIDESMALLWING);
	load2Buffer("../FreedomGunduan/objs/leftMiddleSmallWing.obj", LEFTMIDDLESMALLWING);
	load2Buffer("../FreedomGunduan/objs/leftInsideSmallWing.obj", LEFTINSIDESMALLWING);

	load2Buffer("../FreedomGunduan/objs/rightConnector.obj", RIGHTCONNECTOR);
	load2Buffer("../FreedomGunduan/objs/rightInsideBigWing.obj", RIGHTINSIDEBIGWING);
	load2Buffer("../FreedomGunduan/objs/rightGun.obj", RIGHTGUN);
	load2Buffer("../FreedomGunduan/objs/rightOutsideBigWing.obj", RIGHTOUTSIDEBIGWING);
	load2Buffer("../FreedomGunduan/objs/rightOutsideSmallWing.obj", RIGHTOUTSIDESMALLWING);
	load2Buffer("../FreedomGunduan/objs/rightMiddleSmallWing.obj", RIGHTMIDDLESMALLWING);
	load2Buffer("../FreedomGunduan/objs/rightInsideSmallWing.obj", RIGHTINSIDESMALLWING);

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

	std::vector<vec3> asteroids_Kds;
	std::vector<vec3> asteroids_Kas;
	std::vector<vec3> asteroids_Kss;
	std::vector<std::string> asteroids_Materials;//mtl-name
	std::string asteroids_texture;
	loadMTL("../FreedomGunduan/objs/asteroids/Asteroid_Small_6X.mtl", asteroids_Kds, asteroids_Kas, asteroids_Kss, asteroids_Materials, asteroids_texture);

	for (int i = 0; i < asteroids_Materials.size(); i++) {
		string mtlname = asteroids_Materials[i];
		//  name            vec3
		asteroids_KDs[mtlname] = asteroids_Kds[i];
	}

	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals; // Won't be used at the moment.
	std::vector<unsigned int> materialIndices;

	load2AsteroidBuffer("../FreedomGunduan/objs/asteroids/Asteroid_Small_1.obj", 0);
	load2AsteroidBuffer("../FreedomGunduan/objs/asteroids/Asteroid_Small_2.obj", 1);
	load2AsteroidBuffer("../FreedomGunduan/objs/asteroids/Asteroid_Small_3.obj", 2);
	load2AsteroidBuffer("../FreedomGunduan/objs/asteroids/Asteroid_Small_4.obj", 3);
	load2AsteroidBuffer("../FreedomGunduan/objs/asteroids/Asteroid_Small_5.obj", 4);
	load2AsteroidBuffer("../FreedomGunduan/objs/asteroids/Asteroid_Small_6.obj", 5);
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
	Rotatation[BODY] = rotate(angles[BODY][X], 1, 0, 0) * rotate(angles[BODY][Y], 0, 1, 0) * rotate(angles[BODY][Z], 0, 0, 1);
	Translation[BODY] = translate(positions[BODY][X], positions[BODY][Y] + fly_position, positions[BODY][Z]);
	Models[BODY] = Translation[BODY] * Rotatation[BODY] * Scaling[BODY];
	//============================================================

	//左肩膀
	Rotatation[LEFTSHOULDER] = rotate(angles[LEFTSHOULDER][X], 1, 0, 0) * rotate(angles[LEFTSHOULDER][Z], 0, 0, 1) * rotate(angles[LEFTSHOULDER][Y], 0, 1, 0);
	Translation[LEFTSHOULDER] = translate(positions[LEFTSHOULDER][X], positions[LEFTSHOULDER][Y], positions[LEFTSHOULDER][Z]);//位移到左上手臂處
	Models[LEFTSHOULDER] = Models[BODY] * Translation[LEFTSHOULDER] * Rotatation[LEFTSHOULDER];
	//============================================================
	
	//左下手臂
	Rotatation[LEFTARM] = rotate(angles[LEFTARM][Z], 0, 0, 1) * rotate(angles[LEFTARM][Y], 0, 1, 0) * rotate(angles[LEFTARM][X], 1, 0, 0);
	Translation[LEFTARM] = translate(positions[LEFTARM][X], positions[LEFTARM][Y], positions[LEFTARM][Z]); // 0 -3 0
	Models[LEFTARM] = Models[LEFTSHOULDER] * Translation[LEFTARM] * Rotatation[LEFTARM];
	//============================================================
	
	//頭
	Rotatation[HEAD] = rotate(angles[HEAD][X], 1, 0, 0) * rotate(angles[HEAD][Y], 0, 1, 0);
	Translation[HEAD] = translate(positions[HEAD][X], positions[HEAD][Y], positions[HEAD][Z]);
	Models[HEAD] = Models[BODY] * Translation[HEAD] * Rotatation[HEAD];
	//============================================================

	//右肩膀(面對畫面右邊)
	Rotatation[RIGHTSHOULDER] = rotate(angles[RIGHTSHOULDER][X], 1, 0, 0) * rotate(angles[RIGHTSHOULDER][Z], 0, 0, 1) * rotate(angles[RIGHTSHOULDER][Y], 0, 1, 0);
	Translation[RIGHTSHOULDER] = translate(positions[RIGHTSHOULDER][X], positions[RIGHTSHOULDER][Y], positions[RIGHTSHOULDER][Z]);
	Models[RIGHTSHOULDER] = Models[BODY] * Translation[RIGHTSHOULDER] * Rotatation[RIGHTSHOULDER];
	//============================================================
	
	//右手(面對畫面右邊)
	Rotatation[RIGHTARM] = rotate(angles[RIGHTARM][Z], 0, 0, 1) * rotate(angles[RIGHTARM][Y], 0, 1, 0) * rotate(angles[RIGHTARM][X], 1, 0, 0);
	Translation[RIGHTARM] = translate(positions[RIGHTARM][X], positions[RIGHTARM][Y], positions[RIGHTARM][Z]);
	Models[RIGHTARM] = Models[RIGHTSHOULDER] * Translation[RIGHTARM] * Rotatation[RIGHTARM];
	//=============================================================
	
	//wing
	Rotatation[WING] = rotate(angles[WING][X], 1, 0, 0);
	Translation[WING] = translate(positions[WING][X], positions[WING][Y], positions[WING][Z]);
	Models[WING] = Models[BODY] * Translation[WING] * Rotatation[WING];
	//=============================================================
	
	//左腿(面對畫面左邊)
	Rotatation[LEFTLEG] = rotate(angles[LEFTLEG][X], 1, 0, 0) * rotate(angles[LEFTLEG][Z], 0, 0, 1);
	Translation[LEFTLEG] = translate(positions[LEFTLEG][X], positions[LEFTLEG][Y], positions[LEFTLEG][Z]);
	Models[LEFTLEG] = Models[BODY] * Translation[LEFTLEG] * Rotatation[LEFTLEG];
	//=============================================================

	//左腿鎧甲
	Rotatation[LEFTLEGARMOR] = rotate(angles[LEFTLEGARMOR][X], 1, 0, 0) * rotate(angles[LEFTLEGARMOR][Z], 0, 0, 1) * rotate(angles[LEFTLEGARMOR][Y], 0, 1, 0);
	Translation[LEFTLEGARMOR] = translate(positions[LEFTLEGARMOR][X], positions[LEFTLEGARMOR][Y], positions[LEFTLEGARMOR][Z]);
	Models[LEFTLEGARMOR] = Models[LEFTLEG] * Translation[LEFTLEGARMOR] * Rotatation[LEFTLEGARMOR];
	//=============================================================

	//左腿槍托
	Rotatation[LEFTLEGGUNSTOCK] = rotate(angles[LEFTLEGGUNSTOCK][X], 1, 0, 0) * rotate(angles[LEFTLEGGUNSTOCK][Z], 0, 0, 1) * rotate(angles[LEFTLEGGUNSTOCK][Y], 0, 1, 0);
	Translation[LEFTLEGGUNSTOCK] = translate(positions[LEFTLEGGUNSTOCK][X], positions[LEFTLEGGUNSTOCK][Y], positions[LEFTLEGGUNSTOCK][Z]);
	Models[LEFTLEGGUNSTOCK] = Models[LEFTLEGARMOR] * Translation[LEFTLEGGUNSTOCK] * Rotatation[LEFTLEGGUNSTOCK];
	//=============================================================

	//左腿槍管
	Rotatation[LEFTLEGGUNBARREL] = rotate(angles[LEFTLEGGUNBARREL][X], 1, 0, 0) * rotate(angles[LEFTLEGGUNBARREL][Z], 0, 0, 1) * rotate(angles[LEFTLEGGUNBARREL][Y], 0, 1, 0);
	Translation[LEFTLEGGUNBARREL] = translate(positions[LEFTLEGGUNBARREL][X], positions[LEFTLEGGUNBARREL][Y], positions[LEFTLEGGUNBARREL][Z]);
	Models[LEFTLEGGUNBARREL] = Models[LEFTLEGARMOR] * Translation[LEFTLEGGUNBARREL] * Rotatation[LEFTLEGGUNBARREL];
	//=============================================================

	//左腿槍口
	Rotatation[LEFTLEGGUNPOINT] = rotate(angles[LEFTLEGGUNPOINT][X], 1, 0, 0) * rotate(angles[LEFTLEGGUNPOINT][Z], 0, 0, 1) * rotate(angles[LEFTLEGGUNPOINT][Y], 0, 1, 0);
	Translation[LEFTLEGGUNPOINT] = translate(positions[LEFTLEGGUNPOINT][X], positions[LEFTLEGGUNPOINT][Y], positions[LEFTLEGGUNPOINT][Z]);
	Models[LEFTLEGGUNPOINT] = Models[LEFTLEGGUNBARREL] * Translation[LEFTLEGGUNPOINT] * Rotatation[LEFTLEGGUNPOINT];
	//=============================================================

	//左腿光劍
	Rotatation[LEFTLEGSABER] = rotate(angles[LEFTLEGSABER][X], 1, 0, 0) * rotate(angles[LEFTLEGSABER][Z], 0, 0, 1) * rotate(angles[LEFTLEGSABER][Y], 0, 1, 0);
	Translation[LEFTLEGSABER] = translate(positions[LEFTLEGSABER][X], positions[LEFTLEGSABER][Y], positions[LEFTLEGSABER][Z]);
	Models[LEFTLEGSABER] = Models[LEFTLEGARMOR] * Translation[LEFTLEGSABER] * Rotatation[LEFTLEGSABER];
	//=============================================================

	//左腳(面對畫面左邊)
	Rotatation[LEFTFOOT] = rotate(angles[LEFTFOOT][X], 1, 0, 0) * rotate(angles[LEFTFOOT][Z], 0, 0, 1);
	Translation[LEFTFOOT] = translate(positions[LEFTFOOT][X], positions[LEFTFOOT][Y], positions[LEFTFOOT][Z]);
	Models[LEFTFOOT] = Models[LEFTLEG] * Translation[LEFTFOOT] * Rotatation[LEFTFOOT];
	//=============================================================
	
	//右腿
	Rotatation[RIGHTLEG] = rotate(angles[RIGHTLEG][X], 1, 0, 0) * rotate(angles[RIGHTLEG][Z], 0, 0, 1);
	Translation[RIGHTLEG] = translate(positions[RIGHTLEG][X], positions[RIGHTLEG][Y], positions[RIGHTLEG][Z]);
	Models[RIGHTLEG] = Models[BODY] * Translation[RIGHTLEG] * Rotatation[RIGHTLEG];
	//=============================================================

	//右腿鎧甲
	Rotatation[RIGHTLEGARMOR] = rotate(angles[RIGHTLEGARMOR][X], 1, 0, 0) * rotate(angles[RIGHTLEGARMOR][Z], 0, 0, 1) * rotate(angles[RIGHTLEGARMOR][Y], 0, 1, 0);
	Translation[RIGHTLEGARMOR] = translate(positions[RIGHTLEGARMOR][X], positions[RIGHTLEGARMOR][Y], positions[RIGHTLEGARMOR][Z]);
	Models[RIGHTLEGARMOR] = Models[RIGHTLEG] * Translation[RIGHTLEGARMOR] * Rotatation[RIGHTLEGARMOR];
	//=============================================================

	//右腿槍托
	Rotatation[RIGHTLEGGUNSTOCK] = rotate(angles[RIGHTLEGGUNSTOCK][X], 1, 0, 0) * rotate(angles[RIGHTLEGGUNSTOCK][Z], 0, 0, 1) * rotate(angles[RIGHTLEGGUNSTOCK][Y], 0, 1, 0);
	Translation[RIGHTLEGGUNSTOCK] = translate(positions[RIGHTLEGGUNSTOCK][X], positions[RIGHTLEGGUNSTOCK][Y], positions[RIGHTLEGGUNSTOCK][Z]);
	Models[RIGHTLEGGUNSTOCK] = Models[RIGHTLEGARMOR] * Translation[RIGHTLEGGUNSTOCK] * Rotatation[RIGHTLEGGUNSTOCK];
	//=============================================================

	//右腿槍管
	Rotatation[RIGHTLEGGUNBARREL] = rotate(angles[RIGHTLEGGUNBARREL][X], 1, 0, 0) * rotate(angles[RIGHTLEGGUNBARREL][Z], 0, 0, 1) * rotate(angles[RIGHTLEGGUNBARREL][Y], 0, 1, 0);
	Translation[RIGHTLEGGUNBARREL] = translate(positions[RIGHTLEGGUNBARREL][X], positions[RIGHTLEGGUNBARREL][Y], positions[RIGHTLEGGUNBARREL][Z]);
	Models[RIGHTLEGGUNBARREL] = Models[RIGHTLEGARMOR] * Translation[RIGHTLEGGUNBARREL] * Rotatation[RIGHTLEGGUNBARREL];
	//=============================================================

	//右腿槍口
	Rotatation[RIGHTLEGGUNPOINT] = rotate(angles[RIGHTLEGGUNPOINT][X], 1, 0, 0) * rotate(angles[RIGHTLEGGUNPOINT][Z], 0, 0, 1) * rotate(angles[RIGHTLEGGUNPOINT][Y], 0, 1, 0);
	Translation[RIGHTLEGGUNPOINT] = translate(positions[RIGHTLEGGUNPOINT][X], positions[RIGHTLEGGUNPOINT][Y], positions[RIGHTLEGGUNPOINT][Z]);
	Models[RIGHTLEGGUNPOINT] = Models[RIGHTLEGGUNBARREL] * Translation[RIGHTLEGGUNPOINT] * Rotatation[RIGHTLEGGUNPOINT];
	//=============================================================

	//右腿光劍
	Rotatation[RIGHTLEGSABER] = rotate(angles[RIGHTLEGSABER][X], 1, 0, 0) * rotate(angles[RIGHTLEGSABER][Z], 0, 0, 1) * rotate(angles[RIGHTLEGSABER][Y], 0, 1, 0);
	Translation[RIGHTLEGSABER] = translate(positions[RIGHTLEGSABER][X], positions[RIGHTLEGSABER][Y], positions[RIGHTLEGSABER][Z]);
	Models[RIGHTLEGSABER] = Models[RIGHTLEGARMOR] * Translation[RIGHTLEGSABER] * Rotatation[RIGHTLEGSABER];
	//=============================================================

	//右腳
	Rotatation[RIGHTFOOT] = rotate(angles[RIGHTFOOT][X], 1, 0, 0) * rotate(angles[RIGHTFOOT][Z], 0, 0, 1);
	Translation[RIGHTFOOT] = translate(positions[RIGHTFOOT][X], positions[RIGHTFOOT][Y], positions[RIGHTFOOT][Z]);
	Models[RIGHTFOOT] = Models[RIGHTLEG] * Translation[RIGHTFOOT] * Rotatation[RIGHTFOOT];
	//=============================================================

	//翅膀左連接器(面對畫面左邊)
	Rotatation[LEFTCONNECTOR] = rotate(angles[LEFTCONNECTOR][Y], 0, 1, 0) * rotate(angles[LEFTCONNECTOR][X], 1, 0, 0) * rotate(angles[LEFTCONNECTOR][Z], 0, 0, 1);
	Translation[LEFTCONNECTOR] = translate(positions[LEFTCONNECTOR][X], positions[LEFTCONNECTOR][Y], positions[LEFTCONNECTOR][Z]);
	Models[LEFTCONNECTOR] = Models[WING] * Translation[LEFTCONNECTOR] * Rotatation[LEFTCONNECTOR];
	//=============================================================

	//左內大翅(靠近鋼彈)
	Rotatation[LEFTINSIDEBIGWING] = rotate(angles[LEFTINSIDEBIGWING][Y], 0, 1, 0) * rotate(angles[LEFTINSIDEBIGWING][X], 1, 0, 0) * rotate(angles[LEFTINSIDEBIGWING][Z], 0, 0, 1);
	Translation[LEFTINSIDEBIGWING] = translate(positions[LEFTINSIDEBIGWING][X], positions[LEFTINSIDEBIGWING][Y], positions[LEFTINSIDEBIGWING][Z]);
	Models[LEFTINSIDEBIGWING] = Models[LEFTCONNECTOR] * Translation[LEFTINSIDEBIGWING] * Rotatation[LEFTINSIDEBIGWING];
	//=============================================================

	//左翅膀槍(面對畫面左邊)
	Rotatation[LEFTGUN] = rotate(angles[LEFTGUN][Y], 0, 1, 0) * rotate(angles[LEFTGUN][X], 1, 0, 0) * rotate(angles[LEFTGUN][Z], 0, 0, 1);
	Translation[LEFTGUN] = translate(positions[LEFTGUN][X], positions[LEFTGUN][Y], positions[LEFTGUN][Z]);
	Models[LEFTGUN] = Models[LEFTCONNECTOR] * Translation[LEFTGUN] * Rotatation[LEFTGUN];
	//=============================================================

	//左外大翅(遠離鋼彈)
	Rotatation[LEFTOUTSIDEBIGWING] = rotate(angles[LEFTOUTSIDEBIGWING][Y], 0, 1, 0) * rotate(angles[LEFTOUTSIDEBIGWING][X], 1, 0, 0) * rotate(angles[LEFTOUTSIDEBIGWING][Z], 0, 0, 1);
	Translation[LEFTOUTSIDEBIGWING] = translate(positions[LEFTOUTSIDEBIGWING][X], positions[LEFTOUTSIDEBIGWING][Y], positions[LEFTOUTSIDEBIGWING][Z]);
	Models[LEFTOUTSIDEBIGWING] = Models[LEFTCONNECTOR] * Translation[LEFTOUTSIDEBIGWING] * Rotatation[LEFTOUTSIDEBIGWING];
	//=============================================================

	//左外小翅(遠離鋼彈)
	Rotatation[LEFTOUTSIDESMALLWING] = rotate(angles[LEFTOUTSIDESMALLWING][Y], 0, 1, 0) * rotate(angles[LEFTOUTSIDESMALLWING][X], 1, 0, 0) * rotate(angles[LEFTOUTSIDESMALLWING][Z], 0, 0, 1);
	Translation[LEFTOUTSIDESMALLWING] = translate(positions[LEFTOUTSIDESMALLWING][X], positions[LEFTOUTSIDESMALLWING][Y], positions[LEFTOUTSIDESMALLWING][Z]);
	Models[LEFTOUTSIDESMALLWING] = Models[LEFTCONNECTOR] * Translation[LEFTOUTSIDESMALLWING] * Rotatation[LEFTOUTSIDESMALLWING];
	//=============================================================

	//左中小翅
	Rotatation[LEFTMIDDLESMALLWING] = rotate(angles[LEFTMIDDLESMALLWING][Y], 0, 1, 0) * rotate(angles[LEFTMIDDLESMALLWING][X], 1, 0, 0) * rotate(angles[LEFTMIDDLESMALLWING][Z], 0, 0, 1);
	Translation[LEFTMIDDLESMALLWING] = translate(positions[LEFTMIDDLESMALLWING][X], positions[LEFTMIDDLESMALLWING][Y], positions[LEFTMIDDLESMALLWING][Z]);
	Models[LEFTMIDDLESMALLWING] = Models[LEFTCONNECTOR] * Translation[LEFTMIDDLESMALLWING] * Rotatation[LEFTMIDDLESMALLWING];
	//=============================================================

	//左內小翅(靠近鋼彈)
	Rotatation[LEFTINSIDESMALLWING] = rotate(angles[LEFTINSIDESMALLWING][Y], 0, 1, 0) * rotate(angles[LEFTINSIDESMALLWING][X], 1, 0, 0) * rotate(angles[LEFTINSIDESMALLWING][Z], 0, 0, 1);
	Translation[LEFTINSIDESMALLWING] = translate(positions[LEFTINSIDESMALLWING][X], positions[LEFTINSIDESMALLWING][Y], positions[LEFTINSIDESMALLWING][Z]);
	Models[LEFTINSIDESMALLWING] = Models[LEFTCONNECTOR] * Translation[LEFTINSIDESMALLWING] * Rotatation[LEFTINSIDESMALLWING];
	//=============================================================

	//翅膀右連接器(面對畫面右邊)
	Rotatation[RIGHTCONNECTOR] = rotate(angles[RIGHTCONNECTOR][Y], 0, 1, 0) * rotate(angles[RIGHTCONNECTOR][X], 1, 0, 0) * rotate(angles[RIGHTCONNECTOR][Z], 0, 0, 1);
	Translation[RIGHTCONNECTOR] = translate(positions[RIGHTCONNECTOR][X], positions[RIGHTCONNECTOR][Y], positions[RIGHTCONNECTOR][Z]);
	Models[RIGHTCONNECTOR] = Models[WING] * Translation[RIGHTCONNECTOR] * Rotatation[RIGHTCONNECTOR];
	//=============================================================

	//右內大翅(靠近鋼彈)
	Rotatation[RIGHTINSIDEBIGWING] = rotate(angles[RIGHTINSIDEBIGWING][Y], 0, 1, 0) * rotate(angles[RIGHTINSIDEBIGWING][X], 1, 0, 0) * rotate(angles[RIGHTINSIDEBIGWING][Z], 0, 0, 1);
	Translation[RIGHTINSIDEBIGWING] = translate(positions[RIGHTINSIDEBIGWING][X], positions[RIGHTINSIDEBIGWING][Y], positions[RIGHTINSIDEBIGWING][Z]);
	Models[RIGHTINSIDEBIGWING] = Models[RIGHTCONNECTOR] * Translation[RIGHTINSIDEBIGWING] * Rotatation[RIGHTINSIDEBIGWING];
	//=============================================================

	//右翅膀槍(面對畫面右邊)
	Rotatation[RIGHTGUN] = rotate(angles[RIGHTGUN][Y], 0, 1, 0) * rotate(angles[RIGHTGUN][X], 1, 0, 0) * rotate(angles[RIGHTGUN][Z], 0, 0, 1);
	Translation[RIGHTGUN] = translate(positions[RIGHTGUN][X], positions[RIGHTGUN][Y], positions[RIGHTGUN][Z]);
	Models[RIGHTGUN] = Models[RIGHTCONNECTOR] * Translation[RIGHTGUN] * Rotatation[RIGHTGUN];
	//=============================================================

	//右外大膀(遠離鋼彈)
	Rotatation[RIGHTOUTSIDEBIGWING] = rotate(angles[RIGHTOUTSIDEBIGWING][Y], 0, 1, 0) * rotate(angles[RIGHTOUTSIDEBIGWING][X], 1, 0, 0) * rotate(angles[RIGHTOUTSIDEBIGWING][Z], 0, 0, 1);
	Translation[RIGHTOUTSIDEBIGWING] = translate(positions[RIGHTOUTSIDEBIGWING][X], positions[RIGHTOUTSIDEBIGWING][Y], positions[RIGHTOUTSIDEBIGWING][Z]);
	Models[RIGHTOUTSIDEBIGWING] = Models[RIGHTCONNECTOR] * Translation[RIGHTOUTSIDEBIGWING] * Rotatation[RIGHTOUTSIDEBIGWING];
	//=============================================================

	//右外小膀(遠離鋼彈)
	Rotatation[RIGHTOUTSIDESMALLWING] = rotate(angles[RIGHTOUTSIDESMALLWING][Y], 0, 1, 0) * rotate(angles[RIGHTOUTSIDESMALLWING][X], 1, 0, 0) * rotate(angles[RIGHTOUTSIDESMALLWING][Z], 0, 0, 1);
	Translation[RIGHTOUTSIDESMALLWING] = translate(positions[RIGHTOUTSIDESMALLWING][X], positions[RIGHTOUTSIDESMALLWING][Y], positions[RIGHTOUTSIDESMALLWING][Z]);
	Models[RIGHTOUTSIDESMALLWING] = Models[RIGHTCONNECTOR] * Translation[RIGHTOUTSIDESMALLWING] * Rotatation[RIGHTOUTSIDESMALLWING];
	//=============================================================

	//右中小膀
	Rotatation[RIGHTMIDDLESMALLWING] = rotate(angles[RIGHTMIDDLESMALLWING][Y], 0, 1, 0) * rotate(angles[RIGHTMIDDLESMALLWING][X], 1, 0, 0) * rotate(angles[RIGHTMIDDLESMALLWING][Z], 0, 0, 1);
	Translation[RIGHTMIDDLESMALLWING] = translate(positions[RIGHTMIDDLESMALLWING][X], positions[RIGHTMIDDLESMALLWING][Y], positions[RIGHTMIDDLESMALLWING][Z]);
	Models[RIGHTMIDDLESMALLWING] = Models[RIGHTCONNECTOR] * Translation[RIGHTMIDDLESMALLWING] * Rotatation[RIGHTMIDDLESMALLWING];
	//=============================================================

	//右內小膀
	Rotatation[RIGHTINSIDESMALLWING] = rotate(angles[RIGHTINSIDESMALLWING][Y], 0, 1, 0) * rotate(angles[RIGHTINSIDESMALLWING][X], 1, 0, 0) * rotate(angles[RIGHTINSIDESMALLWING][Z], 0, 0, 1);
	Translation[RIGHTINSIDESMALLWING] = translate(positions[RIGHTINSIDESMALLWING][X], positions[RIGHTINSIDESMALLWING][Y], positions[RIGHTINSIDESMALLWING][Z]);
	Models[RIGHTINSIDESMALLWING] = Models[RIGHTCONNECTOR] * Translation[RIGHTINSIDESMALLWING] * Rotatation[RIGHTINSIDESMALLWING];
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
		/*angles[BODY][Y] += 5;
		if (angles[BODY][Y] >= 360) angles[BODY][Y] = 0;*/
		break;
	case 'o':
	case 'O':
		/*angles[BODY][Y] -= 5;
		if (angles[BODY][Y] <= 0) angles[BODY][Y] = 360;*/
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
		if (eyeXGoal > eyeX - 4)
			eyeXGoal -= 4;
		break;
	case 'd':
	case 'D':
		if (eyeXGoal < eyeX + 4)
			eyeXGoal += 4;
		break;
	case 'q':
	case 'Q':
		if (eyeAngleyGoal > eyeAngley - 10)
			eyeAngleyGoal -= 10;
		break;
	case 'e':
	case 'E':
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
	}
	glutPostRedisplay();
}

void menuEvents(int option){}

void ActionMenuEvents(int option)
{
	action = option;
	
	reset_action = true;

	earth_pos = earth_pos_begin;

	gundam_speed = 0.5f;
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

void drawEarth()
{
	glUseProgram(texture_shader);
	mat4 model_matrix = mat4();
	model_matrix = translate(model_matrix, earth_pos);
	model_matrix = scale(model_matrix, vec3(60, 60, 60));
	glUniformMatrix4fv(glGetUniformLocation(basic_shader, "u_model"), 1, GL_FALSE, &model_matrix[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, earth_texture);
	glUniform1i(glGetUniformLocation(basic_shader, "u_texture"), 0);
	drawShpere(sphere_vao, sphere_indices_size);
	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
}

GLint loadTexture(string path)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	int width, height;
	unsigned char* image;
	image = stbi_load(path.c_str(), &width, &height, 0, 0);
	if (image)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		stbi_image_free(image);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(image);
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

GLuint sphereGenerator(int subdivision_level)
{
	vector<vec3> indexed_vertices;
	vector<vec3> indexed_normals;
	vector<vec2> indexed_uvs;
	std::vector<unsigned short> indices;

	int d_angle = 90.0 / pow(2.0f, subdivision_level);
	unsigned short indices_index = 0;
	float r = 0.5f;
	float sphere_x, sphere_y, sphere_z;
	for (int phi = -90; phi < 90; phi += d_angle)
	{
		for (int theta = 0; theta < 360; theta += d_angle)
		{
			sphere_x = r * glm::cos(glm::radians((float)phi)) * glm::cos(glm::radians((float)theta));
			sphere_y = r * glm::sin(glm::radians((float)phi));
			sphere_z = r * glm::cos(glm::radians((float)phi)) * glm::sin(glm::radians((float)theta));
			indexed_vertices.push_back(glm::vec3(sphere_x, sphere_y, sphere_z));
			indexed_normals.push_back(glm::normalize(glm::vec3(sphere_x, sphere_y, sphere_z)));
			indexed_uvs.push_back(glm::vec2(1.0 - theta / 360.0f, phi / 180.0f + 0.5f));

			sphere_x = r * glm::cos(glm::radians((float)phi)) * glm::cos(glm::radians((float)(theta + d_angle)));
			sphere_y = r * glm::sin(glm::radians((float)phi));
			sphere_z = r * glm::cos(glm::radians((float)phi)) * glm::sin(glm::radians((float)(theta + d_angle)));
			indexed_vertices.push_back(glm::vec3(sphere_x, sphere_y, sphere_z));
			indexed_normals.push_back(glm::normalize(glm::vec3(sphere_x, sphere_y, sphere_z)));
			indexed_uvs.push_back(glm::vec2(1.0 - (theta + d_angle) / 360.0f, phi / 180.0f + 0.5f));

			sphere_x = r * glm::cos(glm::radians((float)(phi + d_angle))) * glm::cos(glm::radians((float)(theta + d_angle)));
			sphere_y = r * glm::sin(glm::radians((float)(phi + d_angle)));
			sphere_z = r * glm::cos(glm::radians((float)(phi + d_angle))) * glm::sin(glm::radians((float)(theta + d_angle)));
			indexed_vertices.push_back(glm::vec3(sphere_x, sphere_y, sphere_z));
			indexed_normals.push_back(glm::normalize(glm::vec3(sphere_x, sphere_y, sphere_z)));
			indexed_uvs.push_back(glm::vec2(1.0 - (theta + d_angle) / 360.0f, (phi + d_angle) / 180.0f + 0.5f));

			sphere_x = r * glm::cos(glm::radians((float)(phi + d_angle))) * glm::cos(glm::radians((float)theta));
			sphere_y = r * glm::sin(glm::radians((float)(phi + d_angle)));
			sphere_z = r * glm::cos(glm::radians((float)(phi + d_angle))) * glm::sin(glm::radians((float)theta));
			indexed_vertices.push_back(glm::vec3(sphere_x, sphere_y, sphere_z));
			indexed_normals.push_back(glm::normalize(glm::vec3(sphere_x, sphere_y, sphere_z)));
			indexed_uvs.push_back(glm::vec2(1.0 - theta / 360.0f, (phi + d_angle) / 180.0f + 0.5f));

			indices.push_back(indices_index);
			indices.push_back(indices_index + 3);
			indices.push_back(indices_index + 2);
			indices.push_back(indices_index);
			indices.push_back(indices_index + 2);
			indices.push_back(indices_index + 1);
			indices_index += 4;
		}
	}

	GLuint VertexArrayID;
	VAOProcess(VertexArrayID, indexed_vertices, indexed_normals, indexed_uvs, indices, sphere_indices_size);

	return VertexArrayID;
}

void VAOProcess(GLuint& VertexArrayID, vector<vec3> indexed_vertices, vector<vec3> indexed_normals, vector<vec2> indexed_uvs, std::vector<unsigned short> indices, size_t& indices_size)
{
	GLuint vertexbuffer;
	GLuint uvbuffer;
	GLuint normalbuffer;
	GLuint elementbuffer;

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	// Load it into a VBO
	glGenBuffers(1, &vertexbuffer);
	glGenBuffers(1, &normalbuffer);
	glGenBuffers(1, &uvbuffer);
	glGenBuffers(1, &elementbuffer); // Generate a buffer for the indices as well

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,                  // attribute
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(GLfloat),                  // stride
		(GLvoid*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,                                // attribute
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		3 * sizeof(GLfloat),                                // stride
		(GLvoid*)0                          // array buffer offset
	);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		2,                                // attribute
		2,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		2 * sizeof(GLfloat),                                // stride
		(GLvoid*)0                          // array buffer offset
	);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
	indices_size = indices.size();

	// Unbind VAO
	glBindVertexArray(0);
}

void drawShpere(GLuint VertexArrayID, int indices_size)
{
	//bind VAO
	glBindVertexArray(VertexArrayID);

	// Draw the triangles !
	glDrawElements(
		GL_TRIANGLES,      // mode
		indices_size,    // count
		GL_UNSIGNED_SHORT,   // type
		(GLvoid*)0           // element array buffer offset
	);

	//unbind VAO
	glBindVertexArray(0);
}

void drawBeam()
{
	glUseProgram(texture_shader);
	mat4 model_matrix = mat4();
	model_matrix = translate(model_matrix, beam_pos);
	vec3 beam_rotate = vec3(Models[LEFTARM] * vec4(24.380f, 23.491f, 1.979f, 1));
	model_matrix = rotate(model_matrix, beam_rotate.x, vec3(1, 0, 0));
	model_matrix = rotate(model_matrix, beam_rotate.y, vec3(0, 1, 0));
	model_matrix = rotate(model_matrix, beam_rotate.z, vec3(0, 0, 1));
	model_matrix = translate(model_matrix, beam_offset);
	model_matrix = scale(model_matrix, beam_scale);
	glUniformMatrix4fv(glGetUniformLocation(basic_shader, "u_model"), 1, GL_FALSE, &model_matrix[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, beam_texture);
	glUniform1i(glGetUniformLocation(basic_shader, "u_texture"), 0);
	drawShpere(sphere_vao, sphere_indices_size);
	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
}

void load2AsteroidBuffer(char* obj, int i)
{
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals; // Won't be used at the moment.

	bool res = loadOBJ(obj, vertices, uvs, normals, asteroids_faces[i], asteroids_mtls[i]);
	if (!res) printf("load failed\n");

	glGenBuffers(1, &asteroids_VBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, asteroids_VBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec3), &vertices[0], GL_STATIC_DRAW);
	asteroids_vertices_size[i] = vertices.size();

	glGenBuffers(1, &asteroids_uVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, asteroids_uVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);
	asteroids_uvs_size[i] = uvs.size();

	glGenBuffers(1, &asteroids_nVBOs[i]);
	glBindBuffer(GL_ARRAY_BUFFER, asteroids_nVBOs[i]);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);
	asteroids_normals_size[i] = normals.size();
}

void drawAsteroids()
{
	glUseProgram(basic_shader);
	for (int asteroids_index = 0; asteroids_index < ASTEROIDAMOUNT; asteroids_index++)
	{
		glBindVertexArray(asteroids_VAO);

		mat4 model_matrix = mat4();
		model_matrix = translate(model_matrix, asteroids_pos[asteroids_index]);
		model_matrix = scale(model_matrix, asteroids_scale[asteroids_index]);
		glUniformMatrix4fv(glGetUniformLocation(basic_shader, "u_model"), 1, GL_FALSE, &model_matrix[0][0]);
		glUniform3fv(glGetUniformLocation(basic_shader, "vLightPosition"), 1, &light_pos[0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, asteroids_textures[asteroids_species[asteroids_index]]);
		glUniform1i(glGetUniformLocation(basic_shader, "u_texture"), 0);

		glBindBuffer(GL_ARRAY_BUFFER, asteroids_VBOs[asteroids_species[asteroids_index]]);
		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0,				//location
			3,				//vec3
			GL_FLOAT,			//type
			GL_FALSE,			//not normalized
			0,				//strip
			(void*)0);//buffer offset

		// 2nd attribute buffer : normals
		glEnableVertexAttribArray(1);//location 2 :vec3 Normal
		glBindBuffer(GL_ARRAY_BUFFER, asteroids_nVBOs[asteroids_species[asteroids_index]]);
		glVertexAttribPointer(1,
			3,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0);

		// 3rd attribute buffer : UVs
		glEnableVertexAttribArray(2);//location 3 :vec2 UV
		glBindBuffer(GL_ARRAY_BUFFER, asteroids_uVBOs[asteroids_species[asteroids_index]]);
		glVertexAttribPointer(2,
			2,
			GL_FLOAT,
			GL_FALSE,
			0,
			(void*)0);

		int vertexIDoffset = 0;//glVertexID's offset 
		string mtlname;//material name
		for (int j = 0; j < asteroids_mtls[asteroids_species[asteroids_index]].size(); j++) {//
			mtlname = asteroids_mtls[asteroids_species[asteroids_index]][j];
			//find the material diffuse color in map:KDs by material name.

			glDrawArrays(GL_TRIANGLES, vertexIDoffset, asteroids_faces[asteroids_species[asteroids_index]][j + 1] * 3);
			//we draw triangles by giving the glVertexID base and vertex count is face count*3
			vertexIDoffset += asteroids_faces[asteroids_species[asteroids_index]][j + 1] * 3;//glVertexID's base offset is face count*3
		}//end for loop for draw one part of the robot	
	//unbind VAO
		glBindVertexArray(0);
	}
	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
}

//void initOpenAL()
//{
//	if (!device) {
//		//Tutorial: https://ffainelli.github.io/openal-example/
//		device = alcOpenDevice(NULL);
//		if (!device)
//			puts("ERROR::NO_AUDIO_DEVICE");
//
//		ALboolean enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
//		if (enumeration == AL_FALSE)
//			puts("Enumeration not supported");
//		else
//			puts("Enumeration supported");
//
//		context = alcCreateContext(device, NULL);
//		if (!alcMakeContextCurrent(context))
//			puts("Failed to make context current");
//
//		ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
//		alListener3f(AL_POSITION, positions[BODY][X], positions[BODY][Y], positions[BODY][Z]);
//		alListener3f(AL_VELOCITY, 0, 0, 0);
//		alListenerfv(AL_ORIENTATION, listenerOri);
//
//		alGenSources((ALuint)1, &source);
//		alSourcef(source, AL_PITCH, 1);
//		alSourcef(source, AL_GAIN, 0.5f);
//		alSource3f(source, AL_POSITION, positions[BODY][X], positions[BODY][Y], positions[BODY][Z]);
//		alSource3f(source, AL_VELOCITY, 0, 0, 0);
//		alSourcei(source, AL_LOOPING, AL_TRUE);
//
//		alGenBuffers((ALuint)1, &buffer);
//
//		ALsizei size, freq;
//		ALenum format;
//		ALvoid* data;
//		ALboolean loop = AL_TRUE;
//
//		//Material from: ThinMatrix
//		alutLoadWAVFile((ALbyte*)"../FreedomGunduan/audios/rush.wav", &format, &data, &size, &freq, &loop); //music from bensound.com
//		alBufferData(buffer, format, data, size, freq);
//		alSourcei(source, AL_BUFFER, buffer);
//
//		if (format == AL_FORMAT_STEREO16 || format == AL_FORMAT_STEREO8)
//			puts("TYPE::STEREO");
//		else if (format == AL_FORMAT_MONO16 || format == AL_FORMAT_MONO8)
//			puts("TYPE::MONO");
//
//		alSourcePlay(source);
//	}
//}