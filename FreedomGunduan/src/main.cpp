#include "main.h"

static unsigned int SHADOW_WIDTH = 9102;
static unsigned int SHADOW_HEIGHT = 9102;
vec3 camera = vec3(0, 0, 20);
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitContextVersion(4, 3);//�HOpenGL version4.3���������
	glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);//�O�_�V�U�ۮe,GLUT_FORWARD_COMPATIBLE���䴩(?
	glutInitContextProfile(GLUT_CORE_PROFILE);

	//multisample for golygons smooth
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL 4.3 - Freedom Gunduan");

	glewExperimental = GL_TRUE; //�m��glewInit()���e
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
	ActionMenu = glutCreateMenu(ActionMenuEvents);//�إߥk����
	//�[�J�k�䪫��
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
	glutAddMenuEntry("Draw Saber", 10);
	glutAddMenuEntry("Power On", 11);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//�P�k�����p

	ModeMenu = glutCreateMenu(ModeMenuEvents);//�إߥk����
	//�[�J�k�䪫��
	glutAddMenuEntry("Line", 0);
	glutAddMenuEntry("Fill", 1);
	glutAddMenuEntry("Diamond Reflect", 2);
	glutAddMenuEntry("Diamond Refract", 3);
	glutAddMenuEntry("Diamond", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//�P�k�����p

	PPMenu = glutCreateMenu(PPSMenuEvents);//�إߥk����
	//�[�J�k�䪫��
	glutAddMenuEntry("Origin", 0);
	glutAddMenuEntry("Gray", 1);
	glutAddMenuEntry("Uniform", 2);
	glutAddMenuEntry("Gaussian", 3);
	glutAddMenuEntry("Shadow Debug", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//�P�k�����p

	glutCreateMenu(menuEvents);//�إߥk����
	//�[�J�k�䪫��
	glutAddSubMenu("Action", ActionMenu);
	glutAddSubMenu("Mode", ModeMenu);
	glutAddSubMenu("Post-Processing", PPMenu);
	glutAttachMenu(GLUT_RIGHT_BUTTON);	//�P�k�����p

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
	light_pos = vec3(0, 10, 0);

	fly_position = 0.0;

	rifle_shooting = false;
	cannon_shooting = false;
	railgun_shooting = false;

	drawRifle = true;
	drawBlade = false;
	exchangeBladeParent = false;
	drawDissolveGray = false;
	recordLastBladeModels = false;
	openRadialBlur = false;
	drawToonShading = false;
	drawParticleSystem = true;

	for (int i = 0; i < PARTSNUM; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			angles[i][j] = 0.0f;
		}
	}
	angles[LEFTARM][Y] = -22.2f;
	angles[LEFTARM][Z] = -26.8f;

	angles[LEFTARMGUN][X] = 1.102f;
	angles[LEFTARMGUN][Y] = -24.925f;
	angles[LEFTARMGUN][Z] = 0.0f;

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

	rifle_beam_angles[X] = -22.134f;
	rifle_beam_angles[Y] = 0.584f;
	rifle_beam_angles[Z] = 0.0f;

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

	positions[LEFTARMGUN][X] = 2.5f;
	positions[LEFTARMGUN][Y] = -7.0f;
	positions[LEFTARMGUN][Z] = 4.5f;

	positions[LEFTFIST][X] = 0.55f;
	positions[LEFTFIST][Y] = -19.0f;
	positions[LEFTFIST][Z] = 0.0f;

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

	positions[RIGHTLEGBLADE][X] = -0.5f;
	positions[RIGHTLEGBLADE][Y] = 0.0f;
	positions[RIGHTLEGBLADE][Z] = -4.0f;

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

	rifle_beam_positions[X] = 0.389f;
	rifle_beam_positions[Y] = -37.872f;
	rifle_beam_positions[Z] = 22.017f;

	positions[RIGHTLEGSABER][X] = 0.0f;
	positions[RIGHTLEGSABER][Y] = 0.0f;
	positions[RIGHTLEGSABER][Z] = 0.0f;

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

	angles[RIGHTLEGSABER][X] = 0.0f;
	angles[RIGHTLEGSABER][Y] = 0.0f;
	angles[RIGHTLEGSABER][Z] = 0.0f;

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
	if (eyeAngleY < eyeAngleYGoal - 0.1f)
		eyeAngleY += 0.8f;
	else if (eyeAngleY > eyeAngleYGoal + 0.1f)
		eyeAngleY -= 0.8f;

	if (eyeAngleX < eyeAngleXGoal - 0.1f)
		eyeAngleX += 0.8f;
	else if (eyeAngleX > eyeAngleXGoal + 0.1f)
		eyeAngleX -= 0.8f;

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

			angles[LEFTSHOULDER][X] -= 4.5f; // -135
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
		drawToonShading = true;

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
		drawToonShading = true;

		if (second_current == 0 && frame < 15)
		{
			angles[LEFTINSIDEBIGWING][Z] -= 8.0f;
			angles[LEFTINSIDESMALLWING][Z] -= 6.0f;
			angles[LEFTMIDDLESMALLWING][Z] -= 4.0f;
			angles[LEFTOUTSIDESMALLWING][Z] -= 2.0f;
			angles[LEFTOUTSIDEBIGWING][Z] -= 4.0f;

			angles[RIGHTINSIDEBIGWING][Z] += 8.0f;
			angles[RIGHTINSIDESMALLWING][Z] += 6.0f;
			angles[RIGHTMIDDLESMALLWING][Z] += 4.0f;
			angles[RIGHTOUTSIDESMALLWING][Z] += 2.0f;
			angles[RIGHTOUTSIDEBIGWING][Z] += 4.0f;
		}
		else if (second_current == 0 && frame < 30)
		{
			gundam_speed = 5.0f;
			openRadialBlur = true;
			drawParticleSystem = true;

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
			rifle_shooting = true;
			rifle_beam_scale = vec3(2.0, 2.0, 2.0);
		}
		if (second_current == 2 && frame > 10 && frame < 20)
		{
			rifle_beam_scale = vec3(2.0, 2.0 * (frame - 10) * beam_speed, 2.0);
		}
		if (second_current == 2 && frame > 10)
		{
			rifle_beam_offset = vec3(0, -(frame - 10) * beam_speed, 0);
		}

		// shoot second beam
		if (second_current == 2 && frame == 40)
		{
			rifle_beam_scale = vec3(2.0, 2.0, 2.0);
		}
		if (second_current == 2 && frame > 40 && frame < 50)
		{
			rifle_beam_scale = vec3(2.0, 2.0 * (frame - 40) * beam_speed, 2.0);
		}
		if (second_current == 2 && frame > 40)
		{
			rifle_beam_offset = vec3(0, -(frame - 40) * beam_speed, 0);
		}

		// shoot third beam
		if (second_current == 3 && frame == 10)
		{
			rifle_beam_scale = vec3(2.0, 2.0, 2.0);
		}
		if (second_current == 3 && frame > 10 && frame < 20)
		{
			rifle_beam_scale = vec3(2.0, 2.0 * (frame - 10) * beam_speed, 2.0);
		}
		if (second_current == 3 && frame > 10)
		{
			rifle_beam_offset = vec3(0, -(frame - 10) * beam_speed, 0);
		}

		if (second_current == 4)
		{
			rifle_shooting = false;
		}
	}
	else if (action == AllShoot)
	{
		drawToonShading = true;

		if (second_current == 0 && frame < 15)
		{
			angles[LEFTGUN][Z] -= 12.0f;
			angles[RIGHTGUN][Z] += 12.0f;

			angles[LEFTLEGGUNBARREL][X] -= 7.0f;
			angles[RIGHTLEGGUNBARREL][X] -= 7.0f;
			angles[LEFTLEGGUNSTOCK][X] += 4.5f;
			angles[RIGHTLEGGUNSTOCK][X] += 4.5f;

			angles[LEFTSHOULDER][X] -= 12.0f;
			angles[LEFTSHOULDER][Z] += 1.5f;
		}
		else if (second_current == 0 && frame >= 20 && frame < 30)
		{
			angles[LEFTGUN][X] += 8.5f;
			angles[RIGHTGUN][X] += 8.5f;

			angles[LEFTSHOULDER][X] += 11.0f;
			angles[LEFTARM][Y] += 5.0f;

			angles[LEFTLEGGUNPOINT][X] += 18.0f;
			angles[RIGHTLEGGUNPOINT][X] += 18.0f;

			positions[LEFTLEGGUNPOINT][Z] += 0.3f;
			positions[RIGHTLEGGUNPOINT][Z] += 0.3f;
		}

		// shoot Beam Rifle beam
		if (second_current == 0 && frame == 50)
		{
			rifle_shooting = true;
			rifle_beam_scale = vec3(0.5, 0.5, 0.5);

			cannon_shooting = true;
			lcannon_beam_pos = vec3(Models[LEFTGUN] * vec4(-40.441, -102.426, -0.431, 1));
			lcannon_beam_scale = vec3(0.5, 0.5, 0.5);
			rcannon_beam_pos = vec3(Models[RIGHTGUN] * vec4(40.410, -101.951, 0.335, 1));
			rcannon_beam_scale = vec3(0.5, 0.5, 0.5);

			railgun_shooting = true;
			lrailgun_beam_pos = vec3(Models[LEFTLEGGUNPOINT] * vec4(-1.873, 41.98, 18.142, 1));
			lrailgun_beam_scale = vec3(0.5, 0.5, 0.5);
			rrailgun_beam_pos = vec3(Models[RIGHTLEGGUNPOINT] * vec4(-0.07, 42.5, 17.622, 1));
			rrailgun_beam_scale = vec3(0.5, 0.5, 0.5);
		}
		if (second_current == 1)
		{
			positions[BODY][Z] -= 0.05f;
			lcannon_beam_pos = vec3(Models[LEFTGUN] * vec4(-40.441, -102.426, -0.431, 1));
			rcannon_beam_pos = vec3(Models[RIGHTGUN] * vec4(40.410, -101.951, 0.335, 1));
			lrailgun_beam_pos = vec3(Models[LEFTLEGGUNPOINT] * vec4(-1.873, 41.98, 18.142, 1));
			rrailgun_beam_pos = vec3(Models[RIGHTLEGGUNPOINT] * vec4(-0.07, 42.5, 17.622, 1));

			rifle_beam_scale = vec3(2.0, 2.0 * frame * beam_speed, 2.0);
			lcannon_beam_scale = vec3(8.0, 8.0, 0.5 * frame * beam_speed * 4);
			rcannon_beam_scale = vec3(8.0, 8.0, 0.5 * frame * beam_speed * 4);
			lrailgun_beam_scale = vec3(4.0, 4.0, 0.5 * frame * beam_speed * 4);
			rrailgun_beam_scale = vec3(4.0, 4.0, 0.5 * frame * beam_speed * 4);
		}
		if (second_current == 1)
		{
			rifle_beam_offset = vec3(0, -frame * beam_speed, 0);
			lcannon_beam_offset = vec3(0, 0, frame * beam_speed);
			rcannon_beam_offset = vec3(0, 0, frame * beam_speed);
			lrailgun_beam_offset = vec3(0, 0, frame * beam_speed);
			rrailgun_beam_offset = vec3(0, 0, frame * beam_speed);
		}

		if (second_current == 2)
		{
			rifle_beam_offset = vec3(0, -(frame + 60) * beam_speed, 0);
			lcannon_beam_offset = vec3(0, 0, (frame + 60) * beam_speed);
			rcannon_beam_offset = vec3(0, 0, (frame + 60) * beam_speed);
			lrailgun_beam_offset = vec3(0, 0, (frame + 60) * beam_speed);
			rrailgun_beam_offset = vec3(0, 0, (frame + 60) * beam_speed);
		}

		if (second_current == 3)
		{
			rifle_shooting = false;
			cannon_shooting = false;
			railgun_shooting = false;
		}
	}
	else if (action == DrawSaber)
	{
		drawToonShading = true;

		if (second_current == 0 && frame > 40)
		{
			recordLastBladeModels = true;
		}
		else
		{
			recordLastBladeModels = false;
		}

		if (second_current == 0 && frame < 15)
		{
			angles[LEFTINSIDEBIGWING][Z] -= 8.0f;
			angles[LEFTINSIDESMALLWING][Z] -= 6.0f;
			angles[LEFTMIDDLESMALLWING][Z] -= 4.0f;
			angles[LEFTOUTSIDESMALLWING][Z] -= 2.0f;
			angles[LEFTOUTSIDEBIGWING][Z] -= 4.0f;

			angles[RIGHTINSIDEBIGWING][Z] += 8.0f;
			angles[RIGHTINSIDESMALLWING][Z] += 6.0f;
			angles[RIGHTMIDDLESMALLWING][Z] += 4.0f;
			angles[RIGHTOUTSIDESMALLWING][Z] += 2.0f;
			angles[RIGHTOUTSIDEBIGWING][Z] += 4.0f;
		}
		else if (second_current == 0 && frame < 30)
		{
			drawRifle = false;

			positions[BODY][Y] -= 0.05f;

			angles[BODY][X] += 2.0f;
			angles[BODY][Y] += 1.0f;

			angles[LEFTSHOULDER][Y] += 6.0f;
			angles[LEFTARM][X] -= 5.0f;
			angles[LEFTFIST][Y] += 8.0f;
			angles[LEFTFIST][X] += 3.0f;
			angles[LEFTFIST][Z] += 3.0f;

			angles[RIGHTSHOULDER][Z] += 2.0f; // Avoid saber cutting the shield

			angles[WING][X] += 1.5f;
			angles[LEFTFOOT][X] += 2.0f;
			angles[RIGHTFOOT][X] += 2.0f;
		}
		else if (second_current == 0 && frame >= 40 && frame < 50)
		{
			drawBlade = true;
			gundam_speed = 5.0f;
			openRadialBlur = true;
			drawParticleSystem = true;

			angles[BODY][Y] -= 1.5f;

			angles[LEFTSHOULDER][Y] -= 11.0f;
			angles[LEFTARM][X] += 8.0f;
			angles[LEFTSHOULDER][Z] -= 2.5f;
			angles[LEFTFIST][X] -= 3.0f;
			angles[LEFTFIST][Y] -= 8.0f;
			angles[LEFTFIST][Z] -= 4.5f;
		}
		
		if (recordLastBladeModels)
		{
			int afterImageLength = 10 - abs(frame - 50); // record 40 ~ 60 frames
			for (int recordIndex = 0; recordIndex < recordLastBladeLength; recordIndex++)
			{
				for (int axisIndex = 0; axisIndex < 3; axisIndex++) //record all value
				{
					lastBodyAngles[recordIndex][axisIndex] = angles[BODY][axisIndex];
					lastLeftFistAngles[recordIndex][axisIndex] = angles[LEFTFIST][axisIndex];
					lastLeftArmAngles[recordIndex][axisIndex] = angles[LEFTARM][axisIndex];
					lastLeftShoulderAngles[recordIndex][axisIndex] = angles[LEFTSHOULDER][axisIndex];
				}

				lastBodyAngles[recordIndex][Y] += 1.5f * afterImageLength;

				lastLeftShoulderAngles[recordIndex][Y] += 11.0f * afterImageLength;
				lastLeftArmAngles[recordIndex][X] -= 8.0f * afterImageLength;
				lastLeftShoulderAngles[recordIndex][Z] += 2.5f * afterImageLength;
				lastLeftFistAngles[recordIndex][X] += 3.0f * afterImageLength;
				lastLeftFistAngles[recordIndex][Y] += 8.0f * afterImageLength;
				lastLeftFistAngles[recordIndex][Z] += 4.5f * afterImageLength;

				lastBodyAngles[recordIndex][Y] -= 1.5f * afterImageLength / recordLastBladeLength * recordIndex;

				lastLeftShoulderAngles[recordIndex][Y] -= 11.0f * afterImageLength / recordLastBladeLength * recordIndex;
				lastLeftArmAngles[recordIndex][X] += 8.0f * afterImageLength / recordLastBladeLength * recordIndex;
				lastLeftShoulderAngles[recordIndex][Z] -= 2.5f * afterImageLength / recordLastBladeLength * recordIndex;
				lastLeftFistAngles[recordIndex][X] -= 3.0f * afterImageLength / recordLastBladeLength * recordIndex;
				lastLeftFistAngles[recordIndex][Y] -= 8.0f * afterImageLength / recordLastBladeLength * recordIndex;
				lastLeftFistAngles[recordIndex][Z] -= 4.5f * afterImageLength / recordLastBladeLength * recordIndex;
			}
		}

		if (second_current == 0 && frame == 30)
		{
			exchangeBladeParent = true;
			positions[RIGHTLEGSABER][X] = -1.193f + -0.3;
			positions[RIGHTLEGSABER][Y] = -6.32f + 0.3;
			positions[RIGHTLEGSABER][Z] = 4.714f + 6.0;
			angles[RIGHTLEGSABER][X] = -32.832f;
			angles[RIGHTLEGSABER][Y] = 144.938f;
			angles[RIGHTLEGSABER][Z] = 0.0f;
		}
	}
	else if (action == PowerOn)
	{
		if (second_current == 0 && frame == 0)
		{
			drawDissolveGray = true;
			drawToonShading = false;
		}

		if (second_current == 0)
		{
			t_drawDissolveGray = frame;
		}
		else if (second_current == 1 && frame < 40)
		{
			t_drawDissolveGray = 60 + frame;
		}
		else
		{
			drawDissolveGray = false;
		}

		if (second_current == 1 && frame < 15)
		{
			angles[HEAD][Y] -= 1.0f;

			angles[LEFTSHOULDER][X] -= 12.0f;

			angles[BODY][Y] += 1.0f;
			angles[BODY][Z] -= 1.0f;

			angles[LEFTINSIDEBIGWING][Z] -= 8.0f;
			angles[LEFTINSIDESMALLWING][Z] -= 6.0f;
			angles[LEFTMIDDLESMALLWING][Z] -= 4.0f;
			angles[LEFTOUTSIDESMALLWING][Z] -= 2.0f;
			angles[LEFTOUTSIDEBIGWING][Z] -= 4.0f;

			angles[RIGHTINSIDEBIGWING][Z] += 8.0f;
			angles[RIGHTINSIDESMALLWING][Z] += 6.0f;
			angles[RIGHTMIDDLESMALLWING][Z] += 4.0f;
			angles[RIGHTOUTSIDESMALLWING][Z] += 2.0f;
			angles[RIGHTOUTSIDEBIGWING][Z] += 4.0f;

			angles[LEFTLEG][Z] -= 1.0f;
			angles[LEFTFOOT][X] += 1.0f;
			angles[RIGHTLEG][Z] += 2.0f;
			angles[RIGHTFOOT][X] += 3.0f;
		}
		else if (second_current == 1 && frame >= 30 && frame < 35)
		{
			angles[LEFTSHOULDER][X] += 21.0f;
			angles[LEFTARM][Y] += 8.0f;
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
	 generatingDepthMap();

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
	 gundaun_shader = LoadShaders(shaders);//Ū��shader

	 ShaderInfo skybox_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/skybox.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/skybox.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 skybox_shader = LoadShaders(skybox_shaders);//Ū��shader

	 ShaderInfo gray_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/gray.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/gray.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 gray_shader = LoadShaders(gray_shaders);//Ū��shader

	 ShaderInfo uniform_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/uniform.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/uniform.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 uniform_shader = LoadShaders(uniform_shaders);//Ū��shader

	 ShaderInfo gaussian_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/gaussian.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/gaussian.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 gaussian_shader = LoadShaders(gaussian_shaders);//Ū��shader

	 ShaderInfo basic_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/simple.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/simple.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 basic_shader = LoadShaders(basic_shaders);//Ū��shader

	 ShaderInfo diamond_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/diamond_surface.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/diamond_surface.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 diamond_shader = LoadShaders(diamond_shaders);//Ū��shader

	 ShaderInfo texture_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/texture.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/texture.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 texture_shader = LoadShaders(texture_shaders);//Ū��shader

	 ShaderInfo radialBlur_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/radialBlur.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/radialBlur.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 radialBlur_shader = LoadShaders(radialBlur_shaders);//Ū��shader

	 ShaderInfo shadow_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/shadow.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/shadow.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 shadow_shader = LoadShaders(shadow_shaders);//Ū��shader

	 ShaderInfo shadowDebug_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/shadowDebug.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/shadowDebug.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 shadowDebug_shader = LoadShaders(shadowDebug_shaders);//Ū��shader

	 ShaderInfo pointSprite_shaders[] = {
		 { GL_VERTEX_SHADER, "../FreedomGunduan/src/shaders/pointSprite.vert" },//vertex shader
		 { GL_FRAGMENT_SHADER, "../FreedomGunduan/src/shaders/pointSprite.frag" },//fragment shader
		 { GL_NONE, NULL } };
	 pointSprite_shader = LoadShaders(pointSprite_shaders);//Ū��shader

	 glUseProgram(gundaun_shader);//uniform�ѼƼƭȫe������use shader

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

	 glUseProgram(0);

	 //UBO
	 glGenBuffers(1, &UBO);
	 glBindBuffer(GL_UNIFORM_BUFFER, UBO);
	 glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 2, NULL, GL_STATIC_DRAW);
	 glBindBufferRange(GL_UNIFORM_BUFFER, /*binding point*/0, UBO, 0, sizeof(mat4) * 2);
	 glBindBuffer(GL_UNIFORM_BUFFER, 0);

	 sphere_vao = sphereGenerator(4);
	 earth_texture = loadTexture("../FreedomGunduan/images/earth.bmp");
	 rifle_beam_texture = loadTexture("../FreedomGunduan/images/rifle_beam.bmp");
	 cannon_beam_texture = loadTexture("../FreedomGunduan/images/cannon_beam.bmp");
	 railgun_beam_texture = loadTexture("../FreedomGunduan/images/railgun_beam.bmp");
	 asteroids_textures[0] = loadTexture("../FreedomGunduan/images/asteroids/Aster_Small_1_Color.bmp");
	 asteroids_textures[1] = loadTexture("../FreedomGunduan/images/asteroids/Aster_Small_2_Color.bmp");
	 asteroids_textures[2] = loadTexture("../FreedomGunduan/images/asteroids/Aster_Small_3_Color.bmp");
	 asteroids_textures[3] = loadTexture("../FreedomGunduan/images/asteroids/Aster_Small_4_Color.bmp");
	 asteroids_textures[4] = loadTexture("../FreedomGunduan/images/asteroids/Aster_Small_5_Color.bmp");
	 asteroids_textures[5] = loadTexture("../FreedomGunduan/images/asteroids/Aster_Small_6_Color.bmp");
	 dissolveTex = loadTexture("../FreedomGunduan/images/febucci-dissolve-texture.jpg");
	 particle_texture = loadTexture("../FreedomGunduan/images/particle.png", true);

	 glGenVertexArrays(1, &asteroids_VAO);

	 initSkybox();

	 //skybox_matrices_ubo
	 glGenBuffers(1, &skybox_matrices_ubo);
	 glBindBuffer(GL_UNIFORM_BUFFER, skybox_matrices_ubo);
	 glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 2, NULL, GL_STATIC_DRAW);
	 glBindBufferRange(GL_UNIFORM_BUFFER, /*binding point*/1, skybox_matrices_ubo, 0, sizeof(mat4) * 2);
	 glBindBuffer(GL_UNIFORM_BUFFER, 0);
	 glClearColor(0.0, 0.0, 0.0, 1);//black screen

	 initPointSprite();

	 initScreenRender();

	 initScreenQuad();
 }

#define DOR(body_angle) (body_angle*3.1415/180);
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float eyey = DOR(eyeAngleY);
	cameraPos = vec3(eyeX, 2 + eyeheight, eyedistance);
	mat4 cameraModel = rotate(mat4(1.0), eyeAngleY, vec3(0, 1, 0));
	cameraModel = rotate(cameraModel, eyeAngleX, vec3(1, 0, 0));
	cameraPos = vec3(cameraModel * vec4(cameraPos, 1.0));
	vec3 cameraCenter = vec3(cameraModel * vec4(eyeX, eyeheight, 0, 1));
	mat4 cameraUpRotation = rotate(mat4(1.0), eyeAngleX, vec3(1, 0, 0));
	vec3 cameraUp = vec3(cameraUpRotation * vec4(0, 1, 0, 1));
	View = lookAt(
		cameraPos, // Camera is at (0,0,20), in World Space
		cameraCenter, // and looks at the origin
		cameraUp  // Head is up (set to 0,-1,0 to look upside-down)
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

	renderDepthMapBegin();
	drawGunduan(true);
	renderDepthMapEnd();

	if (pps != ORIGIN || openRadialBlur)
		renderScreenBegin();

	drawGunduan();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (action == Opening)
	{
		drawEarth();
	}

	if (rifle_shooting)
	{
		drawRifleBeam();
	}

	if (cannon_shooting)
	{
		drawCannonBeam();
	}

	if (railgun_shooting)
	{
		drawRailgunBeam();
	}

	if (action != Opening)
	{
		drawAsteroids();
	}

	drawSkybox();

	// draw after image for blade
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	if (drawParticleSystem)
	{
		for (int particle_index = 0; particle_index < NUM_PARTICLE_SYSTEM; particle_index++)
		{
			drawPointSprite(particle_index);
		}
	}
	

	if (recordLastBladeModels) // debug using drawBlade
	{
		int vertexIDoffset = 0;
		glUseProgram(gundaun_shader); //uniform�ѼƼƭȫe������use shader
		glUniform3fv(glGetUniformLocation(gundaun_shader, "vLightPosition"), 1, &light_pos[0]);
		glUniform1i(glGetUniformLocation(gundaun_shader, "dissolveGray"), drawDissolveGray);
		glUniform1i(glGetUniformLocation(gundaun_shader, "dissolveTex"), 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, dissolveTex);
		glUniform1f(glGetUniformLocation(gundaun_shader, "dissolveThreshold"), t_drawDissolveGray / 100.0f);
		glUniform1f(glGetUniformLocation(gundaun_shader, "alpha"), 0.5f);
		glUniform1i(glGetUniformLocation(gundaun_shader, "useLighting"), 0);
		glBindVertexArray(VAO);
		for (int j = 0; j < mtls[RIGHTLEGBLADE].size(); j++)
		{
			for (int recordIndex = 0; recordIndex < recordLastBladeLength; recordIndex++)
			{
				glUniformMatrix4fv(ModelID, 1, GL_FALSE, &lastBladeModels[recordIndex][0][0]);
				glDrawArrays(GL_TRIANGLES, vertexIDoffset, faces[RIGHTLEGBLADE][j + 1] * 3);
			}
			vertexIDoffset += faces[RIGHTLEGBLADE][j + 1] * 3;//glVertexID's base offset is face count*3
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}
	glDisable(GL_BLEND);

	if (pps != ORIGIN || openRadialBlur)
	{
		renderScreenEnd();

		drawScreenQuad();
	}

	glFlush();//�j�����W����OpenGL commands
	glutSwapBuffers();//�մ��e�x�M��xbuffer ,���Obuffer�e���M�e�xbuffer�洫�ϧڭ̬ݨ���
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
	load2Buffer("../FreedomGunduan/objs/leftHandGun.obj", LEFTARMGUN);
	load2Buffer("../FreedomGunduan/objs/leftFist.obj", LEFTFIST);
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
	load2Buffer("../FreedomGunduan/objs/rightLegBlade.obj", RIGHTLEGBLADE);
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
	//bind vbo ,�Ĥ@��bind�]�P���� create vbo 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);//VBO��target�OGL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, totalSize[0], NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uVBO);//VBO��target�OGL_ARRAY_BUFFER
	glBufferData(GL_ARRAY_BUFFER, totalSize[1], NULL, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, nVBO);//VBO��target�OGL_ARRAY_BUFFER
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
	//if (recordLastBladeModels)
	{
		for (int recordIndex = 0; recordIndex < recordLastBladeLength; recordIndex++)
		{
			Rotatation[BODY] = rotate(lastBodyAngles[recordIndex][X], 1, 0, 0) * rotate(lastBodyAngles[recordIndex][Y], 0, 1, 0) * rotate(lastBodyAngles[recordIndex][Z], 0, 0, 1);
			Models[BODY] = Translation[BODY] * Rotatation[BODY] * Scaling[BODY];
			Rotatation[LEFTSHOULDER] = rotate(lastLeftShoulderAngles[recordIndex][X], 1, 0, 0) * rotate(lastLeftShoulderAngles[recordIndex][Z], 0, 0, 1) * rotate(lastLeftShoulderAngles[recordIndex][Y], 0, 1, 0);
			Models[LEFTSHOULDER] = Models[BODY] * Translation[LEFTSHOULDER] * Rotatation[LEFTSHOULDER];
			Rotatation[LEFTARM] = rotate(lastLeftArmAngles[recordIndex][Z], 0, 0, 1) * rotate(lastLeftArmAngles[recordIndex][Y], 0, 1, 0) * rotate(lastLeftArmAngles[recordIndex][X], 1, 0, 0);
			Models[LEFTARM] = Models[LEFTSHOULDER] * Translation[LEFTARM] * Rotatation[LEFTARM];
			Rotatation[LEFTFIST] = rotate(lastLeftFistAngles[recordIndex][Z], 0, 0, 1) * rotate(lastLeftFistAngles[recordIndex][Y], 0, 1, 0) * rotate(lastLeftFistAngles[recordIndex][X], 1, 0, 0);
			Models[LEFTFIST] = Models[LEFTARM] * Translation[LEFTFIST] * Rotatation[LEFTFIST];
			Models[RIGHTLEGSABER] = Models[LEFTFIST] * Translation[RIGHTLEGSABER] * Rotatation[RIGHTLEGSABER];
			lastBladeModels[recordIndex] = Models[RIGHTLEGSABER] * Translation[RIGHTLEGBLADE] * Rotatation[RIGHTLEGBLADE];
		}
	}

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

	//���ӻH
	Rotatation[LEFTSHOULDER] = rotate(angles[LEFTSHOULDER][X], 1, 0, 0) * rotate(angles[LEFTSHOULDER][Z], 0, 0, 1) * rotate(angles[LEFTSHOULDER][Y], 0, 1, 0);
	Translation[LEFTSHOULDER] = translate(positions[LEFTSHOULDER][X], positions[LEFTSHOULDER][Y], positions[LEFTSHOULDER][Z]);//�첾�쥪�W���u�B
	Models[LEFTSHOULDER] = Models[BODY] * Translation[LEFTSHOULDER] * Rotatation[LEFTSHOULDER];
	//============================================================
	
	//����
	Rotatation[LEFTARM] = rotate(angles[LEFTARM][Z], 0, 0, 1) * rotate(angles[LEFTARM][Y], 0, 1, 0) * rotate(angles[LEFTARM][X], 1, 0, 0);
	Translation[LEFTARM] = translate(positions[LEFTARM][X], positions[LEFTARM][Y], positions[LEFTARM][Z]); // 0 -3 0
	Models[LEFTARM] = Models[LEFTSHOULDER] * Translation[LEFTARM] * Rotatation[LEFTARM];
	//============================================================

	//�����Y
	Rotatation[LEFTFIST] = rotate(angles[LEFTFIST][Z], 0, 0, 1) * rotate(angles[LEFTFIST][Y], 0, 1, 0) * rotate(angles[LEFTFIST][X], 1, 0, 0);
	Translation[LEFTFIST] = translate(positions[LEFTFIST][X], positions[LEFTFIST][Y], positions[LEFTFIST][Z]); // 0 -3 0
	Models[LEFTFIST] = Models[LEFTARM] * Translation[LEFTFIST] * Rotatation[LEFTFIST];
	//============================================================

	//����j
	Rotatation[LEFTARMGUN] = rotate(angles[LEFTARMGUN][Z], 0, 0, 1) * rotate(angles[LEFTARMGUN][Y], 0, 1, 0) * rotate(angles[LEFTARMGUN][X], 1, 0, 0);
	Translation[LEFTARMGUN] = translate(positions[LEFTARMGUN][X], positions[LEFTARMGUN][Y], positions[LEFTARMGUN][Z]); // 0 -3 0
	Models[LEFTARMGUN] = Models[LEFTFIST] * Translation[LEFTARMGUN] * Rotatation[LEFTARMGUN];
	//============================================================
	
	//�Y
	Rotatation[HEAD] = rotate(angles[HEAD][X], 1, 0, 0) * rotate(angles[HEAD][Y], 0, 1, 0);
	Translation[HEAD] = translate(positions[HEAD][X], positions[HEAD][Y], positions[HEAD][Z]);
	Models[HEAD] = Models[BODY] * Translation[HEAD] * Rotatation[HEAD];
	//============================================================

	//�k�ӻH(����e���k��)
	Rotatation[RIGHTSHOULDER] = rotate(angles[RIGHTSHOULDER][X], 1, 0, 0) * rotate(angles[RIGHTSHOULDER][Z], 0, 0, 1) * rotate(angles[RIGHTSHOULDER][Y], 0, 1, 0);
	Translation[RIGHTSHOULDER] = translate(positions[RIGHTSHOULDER][X], positions[RIGHTSHOULDER][Y], positions[RIGHTSHOULDER][Z]);
	Models[RIGHTSHOULDER] = Models[BODY] * Translation[RIGHTSHOULDER] * Rotatation[RIGHTSHOULDER];
	//============================================================
	
	//�k��(����e���k��)
	Rotatation[RIGHTARM] = rotate(angles[RIGHTARM][Z], 0, 0, 1) * rotate(angles[RIGHTARM][Y], 0, 1, 0) * rotate(angles[RIGHTARM][X], 1, 0, 0);
	Translation[RIGHTARM] = translate(positions[RIGHTARM][X], positions[RIGHTARM][Y], positions[RIGHTARM][Z]);
	Models[RIGHTARM] = Models[RIGHTSHOULDER] * Translation[RIGHTARM] * Rotatation[RIGHTARM];
	//=============================================================
	
	//wing
	Rotatation[WING] = rotate(angles[WING][X], 1, 0, 0);
	Translation[WING] = translate(positions[WING][X], positions[WING][Y], positions[WING][Z]);
	Models[WING] = Models[BODY] * Translation[WING] * Rotatation[WING];
	//=============================================================
	
	//���L(����e������)
	Rotatation[LEFTLEG] = rotate(angles[LEFTLEG][X], 1, 0, 0) * rotate(angles[LEFTLEG][Z], 0, 0, 1);
	Translation[LEFTLEG] = translate(positions[LEFTLEG][X], positions[LEFTLEG][Y], positions[LEFTLEG][Z]);
	Models[LEFTLEG] = Models[BODY] * Translation[LEFTLEG] * Rotatation[LEFTLEG];
	//=============================================================

	//���L�Z��
	Rotatation[LEFTLEGARMOR] = rotate(angles[LEFTLEGARMOR][X], 1, 0, 0) * rotate(angles[LEFTLEGARMOR][Z], 0, 0, 1) * rotate(angles[LEFTLEGARMOR][Y], 0, 1, 0);
	Translation[LEFTLEGARMOR] = translate(positions[LEFTLEGARMOR][X], positions[LEFTLEGARMOR][Y], positions[LEFTLEGARMOR][Z]);
	Models[LEFTLEGARMOR] = Models[LEFTLEG] * Translation[LEFTLEGARMOR] * Rotatation[LEFTLEGARMOR];
	//=============================================================

	//���L�j��
	Rotatation[LEFTLEGGUNSTOCK] = rotate(angles[LEFTLEGGUNSTOCK][X], 1, 0, 0) * rotate(angles[LEFTLEGGUNSTOCK][Z], 0, 0, 1) * rotate(angles[LEFTLEGGUNSTOCK][Y], 0, 1, 0);
	Translation[LEFTLEGGUNSTOCK] = translate(positions[LEFTLEGGUNSTOCK][X], positions[LEFTLEGGUNSTOCK][Y], positions[LEFTLEGGUNSTOCK][Z]);
	Models[LEFTLEGGUNSTOCK] = Models[LEFTLEGARMOR] * Translation[LEFTLEGGUNSTOCK] * Rotatation[LEFTLEGGUNSTOCK];
	//=============================================================

	//���L�j��
	Rotatation[LEFTLEGGUNBARREL] = rotate(angles[LEFTLEGGUNBARREL][X], 1, 0, 0) * rotate(angles[LEFTLEGGUNBARREL][Z], 0, 0, 1) * rotate(angles[LEFTLEGGUNBARREL][Y], 0, 1, 0);
	Translation[LEFTLEGGUNBARREL] = translate(positions[LEFTLEGGUNBARREL][X], positions[LEFTLEGGUNBARREL][Y], positions[LEFTLEGGUNBARREL][Z]);
	Models[LEFTLEGGUNBARREL] = Models[LEFTLEGARMOR] * Translation[LEFTLEGGUNBARREL] * Rotatation[LEFTLEGGUNBARREL];
	//=============================================================

	//���L�j�f
	Rotatation[LEFTLEGGUNPOINT] = rotate(angles[LEFTLEGGUNPOINT][X], 1, 0, 0) * rotate(angles[LEFTLEGGUNPOINT][Z], 0, 0, 1) * rotate(angles[LEFTLEGGUNPOINT][Y], 0, 1, 0);
	Translation[LEFTLEGGUNPOINT] = translate(positions[LEFTLEGGUNPOINT][X], positions[LEFTLEGGUNPOINT][Y], positions[LEFTLEGGUNPOINT][Z]);
	Models[LEFTLEGGUNPOINT] = Models[LEFTLEGGUNBARREL] * Translation[LEFTLEGGUNPOINT] * Rotatation[LEFTLEGGUNPOINT];
	//=============================================================

	//���L���C
	Rotatation[LEFTLEGSABER] = rotate(angles[LEFTLEGSABER][X], 1, 0, 0) * rotate(angles[LEFTLEGSABER][Z], 0, 0, 1) * rotate(angles[LEFTLEGSABER][Y], 0, 1, 0);
	Translation[LEFTLEGSABER] = translate(positions[LEFTLEGSABER][X], positions[LEFTLEGSABER][Y], positions[LEFTLEGSABER][Z]);
	Models[LEFTLEGSABER] = Models[LEFTLEGARMOR] * Translation[LEFTLEGSABER] * Rotatation[LEFTLEGSABER];
	//=============================================================

	//���}(����e������)
	Rotatation[LEFTFOOT] = rotate(angles[LEFTFOOT][X], 1, 0, 0) * rotate(angles[LEFTFOOT][Z], 0, 0, 1);
	Translation[LEFTFOOT] = translate(positions[LEFTFOOT][X], positions[LEFTFOOT][Y], positions[LEFTFOOT][Z]);
	Models[LEFTFOOT] = Models[LEFTLEG] * Translation[LEFTFOOT] * Rotatation[LEFTFOOT];
	//=============================================================
	
	//�k�L
	Rotatation[RIGHTLEG] = rotate(angles[RIGHTLEG][X], 1, 0, 0) * rotate(angles[RIGHTLEG][Z], 0, 0, 1);
	Translation[RIGHTLEG] = translate(positions[RIGHTLEG][X], positions[RIGHTLEG][Y], positions[RIGHTLEG][Z]);
	Models[RIGHTLEG] = Models[BODY] * Translation[RIGHTLEG] * Rotatation[RIGHTLEG];
	//=============================================================

	//�k�L�Z��
	Rotatation[RIGHTLEGARMOR] = rotate(angles[RIGHTLEGARMOR][X], 1, 0, 0) * rotate(angles[RIGHTLEGARMOR][Z], 0, 0, 1) * rotate(angles[RIGHTLEGARMOR][Y], 0, 1, 0);
	Translation[RIGHTLEGARMOR] = translate(positions[RIGHTLEGARMOR][X], positions[RIGHTLEGARMOR][Y], positions[RIGHTLEGARMOR][Z]);
	Models[RIGHTLEGARMOR] = Models[RIGHTLEG] * Translation[RIGHTLEGARMOR] * Rotatation[RIGHTLEGARMOR];
	//=============================================================

	//�k�L�j��
	Rotatation[RIGHTLEGGUNSTOCK] = rotate(angles[RIGHTLEGGUNSTOCK][X], 1, 0, 0) * rotate(angles[RIGHTLEGGUNSTOCK][Z], 0, 0, 1) * rotate(angles[RIGHTLEGGUNSTOCK][Y], 0, 1, 0);
	Translation[RIGHTLEGGUNSTOCK] = translate(positions[RIGHTLEGGUNSTOCK][X], positions[RIGHTLEGGUNSTOCK][Y], positions[RIGHTLEGGUNSTOCK][Z]);
	Models[RIGHTLEGGUNSTOCK] = Models[RIGHTLEGARMOR] * Translation[RIGHTLEGGUNSTOCK] * Rotatation[RIGHTLEGGUNSTOCK];
	//=============================================================

	//�k�L�j��
	Rotatation[RIGHTLEGGUNBARREL] = rotate(angles[RIGHTLEGGUNBARREL][X], 1, 0, 0) * rotate(angles[RIGHTLEGGUNBARREL][Z], 0, 0, 1) * rotate(angles[RIGHTLEGGUNBARREL][Y], 0, 1, 0);
	Translation[RIGHTLEGGUNBARREL] = translate(positions[RIGHTLEGGUNBARREL][X], positions[RIGHTLEGGUNBARREL][Y], positions[RIGHTLEGGUNBARREL][Z]);
	Models[RIGHTLEGGUNBARREL] = Models[RIGHTLEGARMOR] * Translation[RIGHTLEGGUNBARREL] * Rotatation[RIGHTLEGGUNBARREL];
	//=============================================================

	//�k�L�j�f
	Rotatation[RIGHTLEGGUNPOINT] = rotate(angles[RIGHTLEGGUNPOINT][X], 1, 0, 0) * rotate(angles[RIGHTLEGGUNPOINT][Z], 0, 0, 1) * rotate(angles[RIGHTLEGGUNPOINT][Y], 0, 1, 0);
	Translation[RIGHTLEGGUNPOINT] = translate(positions[RIGHTLEGGUNPOINT][X], positions[RIGHTLEGGUNPOINT][Y], positions[RIGHTLEGGUNPOINT][Z]);
	Models[RIGHTLEGGUNPOINT] = Models[RIGHTLEGGUNBARREL] * Translation[RIGHTLEGGUNPOINT] * Rotatation[RIGHTLEGGUNPOINT];
	//=============================================================

	//�k�L���C�`
	Rotatation[RIGHTLEGSABER] = rotate(angles[RIGHTLEGSABER][X], 1, 0, 0) * rotate(angles[RIGHTLEGSABER][Z], 0, 0, 1) * rotate(angles[RIGHTLEGSABER][Y], 0, 1, 0);
	Translation[RIGHTLEGSABER] = translate(positions[RIGHTLEGSABER][X], positions[RIGHTLEGSABER][Y], positions[RIGHTLEGSABER][Z]);
	if (exchangeBladeParent)
	{
		Models[RIGHTLEGSABER] = Models[LEFTFIST] * Translation[RIGHTLEGSABER] * Rotatation[RIGHTLEGSABER];
	}
	else
	{
		Models[RIGHTLEGSABER] = Models[RIGHTLEGARMOR] * Translation[RIGHTLEGSABER] * Rotatation[RIGHTLEGSABER];
	}
	//=============================================================

	//�k�L���C�b
	Rotatation[RIGHTLEGBLADE] = rotate(angles[RIGHTLEGBLADE][X], 1, 0, 0) * rotate(angles[RIGHTLEGBLADE][Z], 0, 0, 1) * rotate(angles[RIGHTLEGBLADE][Y], 0, 1, 0);
	Translation[RIGHTLEGBLADE] = translate(positions[RIGHTLEGBLADE][X], positions[RIGHTLEGBLADE][Y], positions[RIGHTLEGBLADE][Z]);
	Models[RIGHTLEGBLADE] = Models[RIGHTLEGSABER] * Translation[RIGHTLEGBLADE] * Rotatation[RIGHTLEGBLADE];
	//=============================================================

	//�k�}
	Rotatation[RIGHTFOOT] = rotate(angles[RIGHTFOOT][X], 1, 0, 0) * rotate(angles[RIGHTFOOT][Z], 0, 0, 1);
	Translation[RIGHTFOOT] = translate(positions[RIGHTFOOT][X], positions[RIGHTFOOT][Y], positions[RIGHTFOOT][Z]);
	Models[RIGHTFOOT] = Models[RIGHTLEG] * Translation[RIGHTFOOT] * Rotatation[RIGHTFOOT];
	//=============================================================

	//�ͻH���s����(����e������)
	Rotatation[LEFTCONNECTOR] = rotate(angles[LEFTCONNECTOR][Y], 0, 1, 0) * rotate(angles[LEFTCONNECTOR][X], 1, 0, 0) * rotate(angles[LEFTCONNECTOR][Z], 0, 0, 1);
	Translation[LEFTCONNECTOR] = translate(positions[LEFTCONNECTOR][X], positions[LEFTCONNECTOR][Y], positions[LEFTCONNECTOR][Z]);
	Models[LEFTCONNECTOR] = Models[WING] * Translation[LEFTCONNECTOR] * Rotatation[LEFTCONNECTOR];
	//=============================================================

	//�����j��(�a����u)
	Rotatation[LEFTINSIDEBIGWING] = rotate(angles[LEFTINSIDEBIGWING][Y], 0, 1, 0) * rotate(angles[LEFTINSIDEBIGWING][X], 1, 0, 0) * rotate(angles[LEFTINSIDEBIGWING][Z], 0, 0, 1);
	Translation[LEFTINSIDEBIGWING] = translate(positions[LEFTINSIDEBIGWING][X], positions[LEFTINSIDEBIGWING][Y], positions[LEFTINSIDEBIGWING][Z]);
	Models[LEFTINSIDEBIGWING] = Models[LEFTCONNECTOR] * Translation[LEFTINSIDEBIGWING] * Rotatation[LEFTINSIDEBIGWING];
	//=============================================================

	//���ͻH�j(����e������)
	Rotatation[LEFTGUN] = rotate(angles[LEFTGUN][Y], 0, 1, 0) * rotate(angles[LEFTGUN][X], 1, 0, 0) * rotate(angles[LEFTGUN][Z], 0, 0, 1);
	Translation[LEFTGUN] = translate(positions[LEFTGUN][X], positions[LEFTGUN][Y], positions[LEFTGUN][Z]);
	Models[LEFTGUN] = Models[LEFTCONNECTOR] * Translation[LEFTGUN] * Rotatation[LEFTGUN];
	//=============================================================

	//���~�j��(�������u)
	Rotatation[LEFTOUTSIDEBIGWING] = rotate(angles[LEFTOUTSIDEBIGWING][Y], 0, 1, 0) * rotate(angles[LEFTOUTSIDEBIGWING][X], 1, 0, 0) * rotate(angles[LEFTOUTSIDEBIGWING][Z], 0, 0, 1);
	Translation[LEFTOUTSIDEBIGWING] = translate(positions[LEFTOUTSIDEBIGWING][X], positions[LEFTOUTSIDEBIGWING][Y], positions[LEFTOUTSIDEBIGWING][Z]);
	Models[LEFTOUTSIDEBIGWING] = Models[LEFTCONNECTOR] * Translation[LEFTOUTSIDEBIGWING] * Rotatation[LEFTOUTSIDEBIGWING];
	//=============================================================

	//���~�p��(�������u)
	Rotatation[LEFTOUTSIDESMALLWING] = rotate(angles[LEFTOUTSIDESMALLWING][Y], 0, 1, 0) * rotate(angles[LEFTOUTSIDESMALLWING][X], 1, 0, 0) * rotate(angles[LEFTOUTSIDESMALLWING][Z], 0, 0, 1);
	Translation[LEFTOUTSIDESMALLWING] = translate(positions[LEFTOUTSIDESMALLWING][X], positions[LEFTOUTSIDESMALLWING][Y], positions[LEFTOUTSIDESMALLWING][Z]);
	Models[LEFTOUTSIDESMALLWING] = Models[LEFTCONNECTOR] * Translation[LEFTOUTSIDESMALLWING] * Rotatation[LEFTOUTSIDESMALLWING];
	//=============================================================

	//�����p��
	Rotatation[LEFTMIDDLESMALLWING] = rotate(angles[LEFTMIDDLESMALLWING][Y], 0, 1, 0) * rotate(angles[LEFTMIDDLESMALLWING][X], 1, 0, 0) * rotate(angles[LEFTMIDDLESMALLWING][Z], 0, 0, 1);
	Translation[LEFTMIDDLESMALLWING] = translate(positions[LEFTMIDDLESMALLWING][X], positions[LEFTMIDDLESMALLWING][Y], positions[LEFTMIDDLESMALLWING][Z]);
	Models[LEFTMIDDLESMALLWING] = Models[LEFTCONNECTOR] * Translation[LEFTMIDDLESMALLWING] * Rotatation[LEFTMIDDLESMALLWING];
	//=============================================================

	//�����p��(�a����u)
	Rotatation[LEFTINSIDESMALLWING] = rotate(angles[LEFTINSIDESMALLWING][Y], 0, 1, 0) * rotate(angles[LEFTINSIDESMALLWING][X], 1, 0, 0) * rotate(angles[LEFTINSIDESMALLWING][Z], 0, 0, 1);
	Translation[LEFTINSIDESMALLWING] = translate(positions[LEFTINSIDESMALLWING][X], positions[LEFTINSIDESMALLWING][Y], positions[LEFTINSIDESMALLWING][Z]);
	Models[LEFTINSIDESMALLWING] = Models[LEFTCONNECTOR] * Translation[LEFTINSIDESMALLWING] * Rotatation[LEFTINSIDESMALLWING];
	//=============================================================

	//�ͻH�k�s����(����e���k��)
	Rotatation[RIGHTCONNECTOR] = rotate(angles[RIGHTCONNECTOR][Y], 0, 1, 0) * rotate(angles[RIGHTCONNECTOR][X], 1, 0, 0) * rotate(angles[RIGHTCONNECTOR][Z], 0, 0, 1);
	Translation[RIGHTCONNECTOR] = translate(positions[RIGHTCONNECTOR][X], positions[RIGHTCONNECTOR][Y], positions[RIGHTCONNECTOR][Z]);
	Models[RIGHTCONNECTOR] = Models[WING] * Translation[RIGHTCONNECTOR] * Rotatation[RIGHTCONNECTOR];
	//=============================================================

	//�k���j��(�a����u)
	Rotatation[RIGHTINSIDEBIGWING] = rotate(angles[RIGHTINSIDEBIGWING][Y], 0, 1, 0) * rotate(angles[RIGHTINSIDEBIGWING][X], 1, 0, 0) * rotate(angles[RIGHTINSIDEBIGWING][Z], 0, 0, 1);
	Translation[RIGHTINSIDEBIGWING] = translate(positions[RIGHTINSIDEBIGWING][X], positions[RIGHTINSIDEBIGWING][Y], positions[RIGHTINSIDEBIGWING][Z]);
	Models[RIGHTINSIDEBIGWING] = Models[RIGHTCONNECTOR] * Translation[RIGHTINSIDEBIGWING] * Rotatation[RIGHTINSIDEBIGWING];
	//=============================================================

	//�k�ͻH�j(����e���k��)
	Rotatation[RIGHTGUN] = rotate(angles[RIGHTGUN][Y], 0, 1, 0) * rotate(angles[RIGHTGUN][X], 1, 0, 0) * rotate(angles[RIGHTGUN][Z], 0, 0, 1);
	Translation[RIGHTGUN] = translate(positions[RIGHTGUN][X], positions[RIGHTGUN][Y], positions[RIGHTGUN][Z]);
	Models[RIGHTGUN] = Models[RIGHTCONNECTOR] * Translation[RIGHTGUN] * Rotatation[RIGHTGUN];
	//=============================================================

	//�k�~�j�H(�������u)
	Rotatation[RIGHTOUTSIDEBIGWING] = rotate(angles[RIGHTOUTSIDEBIGWING][Y], 0, 1, 0) * rotate(angles[RIGHTOUTSIDEBIGWING][X], 1, 0, 0) * rotate(angles[RIGHTOUTSIDEBIGWING][Z], 0, 0, 1);
	Translation[RIGHTOUTSIDEBIGWING] = translate(positions[RIGHTOUTSIDEBIGWING][X], positions[RIGHTOUTSIDEBIGWING][Y], positions[RIGHTOUTSIDEBIGWING][Z]);
	Models[RIGHTOUTSIDEBIGWING] = Models[RIGHTCONNECTOR] * Translation[RIGHTOUTSIDEBIGWING] * Rotatation[RIGHTOUTSIDEBIGWING];
	//=============================================================

	//�k�~�p�H(�������u)
	Rotatation[RIGHTOUTSIDESMALLWING] = rotate(angles[RIGHTOUTSIDESMALLWING][Y], 0, 1, 0) * rotate(angles[RIGHTOUTSIDESMALLWING][X], 1, 0, 0) * rotate(angles[RIGHTOUTSIDESMALLWING][Z], 0, 0, 1);
	Translation[RIGHTOUTSIDESMALLWING] = translate(positions[RIGHTOUTSIDESMALLWING][X], positions[RIGHTOUTSIDESMALLWING][Y], positions[RIGHTOUTSIDESMALLWING][Z]);
	Models[RIGHTOUTSIDESMALLWING] = Models[RIGHTCONNECTOR] * Translation[RIGHTOUTSIDESMALLWING] * Rotatation[RIGHTOUTSIDESMALLWING];
	//=============================================================

	//�k���p�H
	Rotatation[RIGHTMIDDLESMALLWING] = rotate(angles[RIGHTMIDDLESMALLWING][Y], 0, 1, 0) * rotate(angles[RIGHTMIDDLESMALLWING][X], 1, 0, 0) * rotate(angles[RIGHTMIDDLESMALLWING][Z], 0, 0, 1);
	Translation[RIGHTMIDDLESMALLWING] = translate(positions[RIGHTMIDDLESMALLWING][X], positions[RIGHTMIDDLESMALLWING][Y], positions[RIGHTMIDDLESMALLWING][Z]);
	Models[RIGHTMIDDLESMALLWING] = Models[RIGHTCONNECTOR] * Translation[RIGHTMIDDLESMALLWING] * Rotatation[RIGHTMIDDLESMALLWING];
	//=============================================================

	//�k���p�H
	Rotatation[RIGHTINSIDESMALLWING] = rotate(angles[RIGHTINSIDESMALLWING][Y], 0, 1, 0) * rotate(angles[RIGHTINSIDESMALLWING][X], 1, 0, 0) * rotate(angles[RIGHTINSIDESMALLWING][Z], 0, 0, 1);
	Translation[RIGHTINSIDESMALLWING] = translate(positions[RIGHTINSIDESMALLWING][X], positions[RIGHTINSIDESMALLWING][Y], positions[RIGHTINSIDESMALLWING][Z]);
	Models[RIGHTINSIDESMALLWING] = Models[RIGHTCONNECTOR] * Translation[RIGHTINSIDESMALLWING] * Rotatation[RIGHTINSIDESMALLWING];
	//=============================================================
	
	mat4 rifle_beam_Rotatation = rotate(rifle_beam_angles[X], 1, 0, 0) * rotate(rifle_beam_angles[Y], 0, 1, 0) * rotate(rifle_beam_angles[Z], 0, 0, 1);
	mat4 rifle_beam_Translation = translate(rifle_beam_positions[X], rifle_beam_positions[Y], rifle_beam_positions[Z]);
	mat4 rifle_beam_Baias = translate(rifle_beam_offset.x, rifle_beam_offset.y, rifle_beam_offset.z);
	mat4 rifle_beam_Scaling = scale(rifle_beam_scale.x, rifle_beam_scale.y, rifle_beam_scale.z);
	rifle_beam_model = Models[LEFTARMGUN] * rifle_beam_Translation * rifle_beam_Rotatation * rifle_beam_Baias * rifle_beam_Scaling;

	mat4 particle_translation;
	mat4 particle_rotation;
	particle_parent_models[PARTICLEWING] = Models[WING];
	particle_parent_models[PARTICLEWING2] = Models[WING];
	particle_parent_models[PARTICLELEFTWING] = Models[LEFTINSIDEBIGWING];
	particle_parent_models[PARTICLERIGHTWING] = Models[RIGHTINSIDEBIGWING];
	for (int particle_index = 0; particle_index < NUM_PARTICLE_SYSTEM; particle_index++)
	{
		particle_translation = translate(particle_center[particle_index].x, particle_center[particle_index].y, particle_center[particle_index].z);
		particle_rotation = rotate(particle_angle[particle_index].x, 1, 0, 0) * rotate(particle_angle[particle_index].y, 0, 1, 0) * rotate(particle_angle[particle_index].z, 0, 0, 1);
		particle_models[particle_index] = particle_parent_models[particle_index] * particle_translation * particle_rotation;
	}
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
	vec4 t = vec4(x, y, z, 1);//w = 1 ,�hx,y,z=0�ɤ]��translate
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
		//zz -= 0.1f;
		break;
	case 'o':
	case 'O':
		/*angles[BODY][Y] -= 5;
		if (angles[BODY][Y] <= 0) angles[BODY][Y] = 360;*/
		//zz += 0.1f;
		break;
	case 'i':
	case 'I':
		eyeheight -= 2;
		//yy += 0.1f;
		break;
	case 'k':
	case 'K':
		eyeheight += 2;
		//yy -= 0.1f;
		break;
	case 'j':
	case 'J':
		positions[BODY][X] += 1;
		//xx -= 0.1f;
		break;
	case 'l':
	case 'L':
		positions[BODY][X] -= 1;
		//xx += 0.1f;
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
		if (eyeAngleYGoal > eyeAngleY - 12)
			eyeAngleYGoal -= 12;
		break;
	case 'e':
	case 'E':
		if (eyeAngleYGoal < eyeAngleY + 12)
			eyeAngleYGoal += 12;
		break;
	case 'r':
	case 'R':
		if (eyeAngleXGoal > eyeAngleX - 12)
			eyeAngleXGoal -= 12;
		break;
	case 'f':
	case 'F':
		if (eyeAngleXGoal < eyeAngleX + 12)
			eyeAngleXGoal += 12;
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
	else if (openRadialBlur)
		glUseProgram(radialBlur_shader);
	else if (pps == DRAWSHADOWDEBUG)
		glUseProgram(shadowDebug_shader);

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
	else if (openRadialBlur)
		glUniform1i(glGetUniformLocation(radialBlur_shader, "screen"), 0);
	else if (pps == DRAWSHADOWDEBUG)
	{
		glBindTexture(GL_TEXTURE_2D, depthMapID);
		glUniform1i(glGetUniformLocation(shadowDebug_shader, "depthMap"), 0);
		glUniform1f(glGetUniformLocation(shadowDebug_shader, "near_plane"), 0.1f);
		glUniform1f(glGetUniformLocation(shadowDebug_shader, "far_plane"), 1000.0f);
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

GLint loadTexture(string path, bool transparent)
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
		if (transparent)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		else
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

void drawRifleBeam()
{
	glUseProgram(texture_shader);
	glUniformMatrix4fv(glGetUniformLocation(texture_shader, "u_model"), 1, GL_FALSE, &rifle_beam_model[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, rifle_beam_texture);
	glUniform1i(glGetUniformLocation(texture_shader, "u_texture"), 0);
	drawShpere(sphere_vao, sphere_indices_size);
	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
}

void drawCannonBeam()
{
	glUseProgram(texture_shader);
	mat4 model_matrix = mat4();
	model_matrix = translate(model_matrix, lcannon_beam_pos);
	vec3 beam_rotate = vec3(Models[LEFTGUN] * vec4(-10.0f, 0.0f, 100.0f, 1));
	model_matrix = rotate(model_matrix, beam_rotate.x, vec3(1, 0, 0));
	model_matrix = rotate(model_matrix, beam_rotate.y, vec3(0, 1, 0));
	model_matrix = rotate(model_matrix, beam_rotate.z, vec3(0, 0, 1));
	model_matrix = translate(model_matrix, lcannon_beam_offset);
	model_matrix = scale(model_matrix, lcannon_beam_scale);
	glUniformMatrix4fv(glGetUniformLocation(texture_shader, "u_model"), 1, GL_FALSE, &model_matrix[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cannon_beam_texture);
	glUniform1i(glGetUniformLocation(texture_shader, "u_texture"), 0);
	drawShpere(sphere_vao, sphere_indices_size);

	model_matrix = mat4();
	model_matrix = translate(model_matrix, rcannon_beam_pos);
	beam_rotate = vec3(Models[RIGHTGUN] * vec4(60.0f, 0.0f, 50.0f, 1));
	model_matrix = rotate(model_matrix, beam_rotate.x, vec3(1, 0, 0));
	model_matrix = rotate(model_matrix, beam_rotate.y, vec3(0, 1, 0));
	model_matrix = rotate(model_matrix, beam_rotate.z + 180.0f, vec3(0, 0, 1));
	model_matrix = translate(model_matrix, rcannon_beam_offset);
	model_matrix = scale(model_matrix, rcannon_beam_scale);
	glUniformMatrix4fv(glGetUniformLocation(texture_shader, "u_model"), 1, GL_FALSE, &model_matrix[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, cannon_beam_texture);
	glUniform1i(glGetUniformLocation(texture_shader, "u_texture"), 0);
	drawShpere(sphere_vao, sphere_indices_size);

	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
}

void drawRailgunBeam()
{
	glUseProgram(texture_shader);
	mat4 model_matrix = mat4();
	model_matrix = translate(model_matrix, lrailgun_beam_pos);
	vec3 beam_rotate = vec3(Models[LEFTLEGGUNPOINT] * vec4(15.0f, 0.0f, 0.0f, 1));
	model_matrix = rotate(model_matrix, beam_rotate.x, vec3(1, 0, 0));
	model_matrix = rotate(model_matrix, beam_rotate.y, vec3(0, 1, 0));
	model_matrix = rotate(model_matrix, beam_rotate.z, vec3(0, 0, 1));
	model_matrix = translate(model_matrix, lrailgun_beam_offset);
	model_matrix = scale(model_matrix, lrailgun_beam_scale);
	glUniformMatrix4fv(glGetUniformLocation(texture_shader, "u_model"), 1, GL_FALSE, &model_matrix[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, railgun_beam_texture);
	glUniform1i(glGetUniformLocation(texture_shader, "u_texture"), 0);
	drawShpere(sphere_vao, sphere_indices_size);

	model_matrix = mat4();
	model_matrix = translate(model_matrix, rrailgun_beam_pos);
	beam_rotate = vec3(Models[RIGHTLEGGUNPOINT] * vec4(-10.0f, 0.0f, 0.0f, 1));
	model_matrix = rotate(model_matrix, beam_rotate.x, vec3(1, 0, 0));
	model_matrix = rotate(model_matrix, beam_rotate.y, vec3(0, 1, 0));
	model_matrix = rotate(model_matrix, beam_rotate.z, vec3(0, 0, 1));
	model_matrix = translate(model_matrix, rrailgun_beam_offset);
	model_matrix = scale(model_matrix, rrailgun_beam_scale);
	glUniformMatrix4fv(glGetUniformLocation(texture_shader, "u_model"), 1, GL_FALSE, &model_matrix[0][0]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, railgun_beam_texture);
	glUniform1i(glGetUniformLocation(texture_shader, "u_texture"), 0);
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

void generatingDepthMap()
{
	//************************************************************************
	//
	// * generate a new texture
	//
	//========================================================================
	glGenFramebuffers(1, &depthMapFBO);
	glGenTextures(1, &depthMapID);
	glBindTexture(GL_TEXTURE_2D, depthMapID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapID, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void setLightSpaceMatrix()
{
	float ortho_size = 32.0f;
	float near_plane = 0.1f, far_plane = ortho_size * 0.95f;
	mat4 lightProjection = glm::ortho(-ortho_size, ortho_size, -ortho_size, ortho_size, near_plane, far_plane);
	mat4 lightView = glm::lookAt(vec3(light_pos), vec3(0.0), vec3(0.0, 0.0, -1.0));
	lightSpaceMatrix = lightProjection * lightView;
}

void renderDepthMapBegin()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
	setLightSpaceMatrix();

	// render scene from light's point of view
	glUseProgram(shadow_shader);
	glUniformMatrix4fv(glGetUniformLocation(shadow_shader, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void renderDepthMapEnd()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, screen_width, screen_height); // reset viewport

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//unbind shader(switch to fixed pipeline)
	glUseProgram(0);
}

void drawGunduan(bool drawShadow)
{
	if (!drawShadow)
	{
		if (mode == DIAMONDREFLECT || mode == DIAMONDREFRACT || mode == DIAMOND)
		{
			glUseProgram(diamond_shader);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_texture);
			glUniform1i(glGetUniformLocation(diamond_shader, "skybox"), 0);
			glUniform1i(glGetUniformLocation(diamond_shader, "mode"), mode - DIAMONDREFLECT);
		}
		else
			glUseProgram(gundaun_shader); //uniform�ѼƼƭȫe������use shader
	}

	if (!drawShadow)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, depthMapID);
		glUniform1i(glGetUniformLocation(gundaun_shader, "shadowMap"), 2);
	}

	GLuint offset[3] = { 0,0,0 };//offset for vertices , uvs , normals
	for (int i = 0; i < PARTSNUM; i++) {
		glBindVertexArray(VAO);
		if (drawShadow)
		{
			glUniformMatrix4fv(glGetUniformLocation(shadow_shader, "model"), 1, GL_FALSE, &Models[i][0][0]);
		}
		else
		{
			if (!(mode == DIAMONDREFLECT || mode == DIAMONDREFRACT || mode == DIAMOND)) // normal case
			{
				glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Models[i][0][0]);
				glUniform3fv(glGetUniformLocation(gundaun_shader, "vLightPosition"), 1, &light_pos[0]);
				glUniform1i(glGetUniformLocation(gundaun_shader, "dissolveGray"), drawDissolveGray);
				glUniform1i(glGetUniformLocation(gundaun_shader, "dissolveTex"), 1);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, dissolveTex);
				glUniform1f(glGetUniformLocation(gundaun_shader, "dissolveThreshold"), t_drawDissolveGray / 100.0f);
				glUniform1i(glGetUniformLocation(gundaun_shader, "toonShading"), drawToonShading);
				glUniform1f(glGetUniformLocation(gundaun_shader, "alpha"), 1.0f);
				glUniform1i(glGetUniformLocation(gundaun_shader, "useLighting"), 1);
				glUniformMatrix4fv(glGetUniformLocation(gundaun_shader, "lightSpaceMatrix"), 1, GL_FALSE, &lightSpaceMatrix[0][0]);
			}
			else // diamond
			{
				glUniformMatrix4fv(glGetUniformLocation(diamond_shader, "u_model"), 1, GL_FALSE, &Models[i][0][0]);
			}
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
//(location,vec3,type,�T�w�I,�s���I�������q,buffer point)
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
		//(location,vec2,type,�T�w�I,�s���I�������q,point)
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
		//(location,vec3,type,�T�w�I,�s���I�������q,point)
		offset[2] += normals_size[i] * sizeof(vec3);

		int vertexIDoffset = 0;//glVertexID's offset 
		string mtlname;//material name
		vec3 Ks = vec3(1, 1, 1);//because .mtl excluding specular , so give it here.
		for (int j = 0; j < mtls[i].size(); j++) {//
			if (drawShadow)
			{
				glDrawArrays(GL_TRIANGLES, vertexIDoffset, faces[i][j + 1] * 3);
			}
			else
			{
				mtlname = mtls[i][j];
				//find the material diffuse color in map:KDs by material name.
				if (!(mode == DIAMONDREFLECT || mode == DIAMONDREFRACT || mode == DIAMOND))
				{
					glUniform3fv(M_KdID, 1, &KDs[mtlname][0]);
					glUniform3fv(M_KsID, 1, &Ks[0]);
				}
				if (!((i == LEFTARMGUN && !drawRifle) || (i == RIGHTLEGBLADE && !drawBlade)))
				{
					if (i == RIGHTLEGBLADE)
						glUniform1i(glGetUniformLocation(gundaun_shader, "useLighting"), 0); // The blade has own lighting
					glDrawArrays(GL_TRIANGLES, vertexIDoffset, faces[i][j + 1] * 3);
				}
			}
			//we draw triangles by giving the glVertexID base and vertex count is face count*3
			vertexIDoffset += faces[i][j + 1] * 3;//glVertexID's base offset is face count*3
		}//end for loop for draw one part of the robot	

		//unbind VAO
		glBindVertexArray(0);
	}//end for loop for updating and drawing model
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void initPointSprite()
{
	struct particle_t
	{
		vec3 direction;
		int begin_t;
	};
	
	particle_range[PARTICLEWING] = vec3(0.1, 0.5, 0.1);
	particle_limit[PARTICLEWING] = vec3(1, 2, 1);
	particle_range[PARTICLEWING2] = vec3(0.1, 0.5, 0.5);
	particle_limit[PARTICLEWING2] = vec3(1, 2, 2);
	particle_range[PARTICLELEFTWING] = vec3(0.5, 0.8, 0.1);
	particle_limit[PARTICLELEFTWING] = vec3(2, 1, 1);
	particle_range[PARTICLERIGHTWING] = vec3(0.5, 0.8, 0.1);
	particle_limit[PARTICLERIGHTWING] = vec3(0, 1, 1);

	particle_center[PARTICLEWING] = vec3(0.0, -12.367, -4.127);
	particle_angle[PARTICLEWING] = vec3(0.0);
	particle_center[PARTICLEWING2] = vec3(0.0, -9.94, -11.358);
	particle_angle[PARTICLEWING2] = vec3(10, 0.0, 0.0);
	particle_center[PARTICLELEFTWING] = vec3(0.0);
	particle_angle[PARTICLELEFTWING] = vec3(0.0, 0.0, 90);
	particle_center[PARTICLERIGHTWING] = vec3(0.0);
	particle_angle[PARTICLERIGHTWING] = vec3(0.0, 0.0, -90);

	for (int particle_index = 0; particle_index < NUM_PARTICLE_SYSTEM; particle_index++)
	{
		glGenVertexArrays(1, &particleVAO[particle_index]);
		glBindVertexArray(particleVAO[particle_index]);

		glGenBuffers(1, &particleBuffer[particle_index]);
		glBindBuffer(GL_ARRAY_BUFFER, particleBuffer[particle_index]);
		glBufferData(GL_ARRAY_BUFFER, NUM_PARTICLE[particle_index] * sizeof(particle_t), NULL, GL_STATIC_DRAW);

		particle_t* particle = (particle_t*)glMapBufferRange(GL_ARRAY_BUFFER, 0, NUM_PARTICLE[particle_index] * sizeof(particle_t), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);

		for (int i = 0; i < NUM_PARTICLE[particle_index]; i++)
		{
			vec3 randomDir(normalize(vec3((rand() % 200 / 100.0f - particle_limit[particle_index].x) * particle_range[particle_index].x, (rand() % 200 / 100.0f - particle_limit[particle_index].y) * particle_range[particle_index].y, (rand() % 200 / 100.0f - particle_limit[particle_index].z) * particle_range[particle_index].z)));
			particle[i].direction[0] = randomDir.x;
			particle[i].direction[1] = randomDir.y;
			particle[i].direction[2] = randomDir.z;
			particle[i].begin_t = rand() % particle_size;
		}

		glUnmapBuffer(GL_ARRAY_BUFFER);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(particle_t), NULL);
		glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(particle_t), (void*)sizeof(glm::vec3));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		// Unbind VAO
		glBindVertexArray(0);
	}
}

void drawPointSprite(int particle_index)
{
	glUseProgram(pointSprite_shader);
	glUniform1i(glGetUniformLocation(pointSprite_shader, "tex_particle"), 1);
	glUniform1i(glGetUniformLocation(pointSprite_shader, "time"), particle_time);
	glUniformMatrix4fv(glGetUniformLocation(pointSprite_shader, "m_matrix"), 1, GL_FALSE, &particle_models[particle_index][0][0]);
	glUniformMatrix4fv(glGetUniformLocation(pointSprite_shader, "v_matrix"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(pointSprite_shader, "p_matrix"), 1, GL_FALSE, &Projection[0][0]);
	glUniform1i(glGetUniformLocation(pointSprite_shader, "speed"), gundam_speed * 10);
	glUniform1i(glGetUniformLocation(pointSprite_shader, "particle_size"), particle_size);
	glUniform3fv(glGetUniformLocation(pointSprite_shader, "cameraPos"), 1, &cameraPos[0]);

	glEnable(GL_POINT_SPRITE);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, particle_texture);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glBindVertexArray(particleVAO[particle_index]);
	glDrawArrays(GL_POINTS, 0, NUM_PARTICLE[particle_index]);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	particle_time++;
	if (particle_time > 2147483646) particle_time = 0;
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