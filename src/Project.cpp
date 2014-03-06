#include "GameManager.h"

#include <iostream>

#define CAPTION "PSJ Assignment 1"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;


/////////////////////////////////////////////////////////////////////// CALLBACKS
void cleanup() {
	GameManager::getInstance()->destroyBufferObjects();
}


void display() {
	++FrameCount;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearStencil(0);

	GameManager::getInstance()->update();
	GameManager::getInstance()->draw();
	GameManager::getInstance()->postProcessing();
	Input::getInstance()->reset();

	glutSwapBuffers();
}


void idle() {
	glutPostRedisplay();
}


void reshape(int w, int h) {
	WinX = w;
	WinY = h;

	glViewport(0, 0, WinX, WinY);
}


void keyPressed(unsigned char key, int x, int y){
	Input::getInstance()->keyPressed(key);
}


void keyUp(unsigned char key, int x, int y){
	Input::getInstance()->keyUp(key);
}


void specialPressed(int key, int x, int y){
	Input::getInstance()->specialPressed(key);
}


void specialUp(int key, int x, int y) {
	Input::getInstance()->specialUp(key);

}


void mouse(int button, int state, int x, int y)  {
	Input::getInstance()->mouse(button, state);
}

void mouseClickMotion(int x, int y) {
	Input::getInstance()->mouseClickMotion(x, y);
}


void mousePassiveMotion(int x, int y) {
	Input::getInstance()->mousePassiveMotion(x, y);
}


void mouseWheel(int wheel, int direction, int x, int y) {
	Input::getInstance()->mouseWheel(direction);
}


void timer(int value) {
	std::ostringstream oss;
	oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
	std::string s = oss.str();
	glutSetWindow(WindowHandle);
	glutSetWindowTitle(s.c_str());
    FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks()  {
	glutCloseFunc(cleanup);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);
	glutSpecialFunc(specialPressed);
	glutSpecialUpFunc(specialUp);

	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mousePassiveMotion);
	glutMotionFunc(mouseClickMotion);
	glutMouseWheelFunc(mouseWheel);

	glutTimerFunc(0,timer,0);
}

void setupOpenGL() {
	std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glDepthRange(0.0, 1.0);
	glClearDepth(1.0);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
}

void setupGLEW() {
	glewExperimental = GL_TRUE;
	GLenum result = glewInit() ; 
	if (result != GLEW_OK) { 
		std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
		exit(EXIT_FAILURE);
	} 
	GLenum err_code = glGetError();
}

void setupGLUT(int argc, char* argv[]) {
	glutInit(&argc, argv);
	
	//glutInitContextVersion(3, 1);
	//glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
	//glutInitContextProfile(GLUT_CORE_PROFILE);

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	
	glutInitWindowSize(WinX, WinY);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	WindowHandle = glutCreateWindow(CAPTION);
	if(WindowHandle < 1) {
		std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
		exit(EXIT_FAILURE);
	}
}

void init(int argc, char* argv[]) {
	setupGLUT(argc, argv);
	setupGLEW();
	setupOpenGL();
	GameManager::getInstance()->init();
	setupCallbacks();
}

int main(int argc, char* argv[]) {	
	init(argc, argv);
	glutMainLoop();	
	exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////