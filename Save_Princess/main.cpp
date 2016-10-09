#define _CRT_SECURE_NO_WARNINGS

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <Windows.h>
#include <shellapi.h>
#include <iostream>

#include "winfuncs.h"
#include "init_gl.h"
#include "gl_vars.h"
#include "fonts.h"
#include "save_princess.h"

using namespace std;

void init(int argc, char *argv[])
{
	setlocale(LC_ALL, "Russian");

	create_console();
	main_gl_init(argc, argv);

	activate_font("textures/font");
	init_scene();

	cout << (unsigned char*)glGetString(GL_VENDOR) << endl << (unsigned char*)glGetString(GL_RENDERER) << endl;

	glutWindow = glutGetWindow();
	glutShowWindow();
	glutMainLoop();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int argc;
	char** argv;
	LPWSTR* lpArgv = CommandLineToArgvW(GetCommandLineW(), &argc);
	argv = (char**)malloc(argc*sizeof(char*));
	int size;
	for (int i = 0; i < argc; ++i)
	{
		size = wcslen(lpArgv[i]) + 1;
		argv[i] = (char *)malloc(size);
		wcstombs(argv[i], lpArgv[i], size);
	}
	LocalFree(lpArgv);

	init(argc, argv);
	return 0;
}
