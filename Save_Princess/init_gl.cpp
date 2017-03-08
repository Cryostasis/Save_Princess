#include <GL/glew.h>
#include <GL/freeglut.h>

#include <iostream>
#include "init_gl.h"
#include "error_log.h"
#include "file_load.h"
#include "gl_vars.h"
#include "glerrors.h"
#include "save_princess.h"

typedef BOOL(APIENTRY * wglSwapIntervalEXT_Func)(int);

void reg_textures()
{

}

#pragma warning (disable:4996) //sprintf

int check_attrunif(GLint* d, GLchar* name)
{
	char *buf = new char[50];
	if (*d == -1)
	{
		sprintf(buf, "Can't find '%s'\n", name);
		raise_error(buf, false, true);
		delete[] buf;
		return ST_SHADER_ERROR;
	}
	delete[] buf;
	return ST_OK;
}

int get_attrib_loc(GLint* attrib, GLuint prog, GLchar* name)
{
	*attrib = glGetAttribLocation(prog, name);
	return check_attrunif(attrib, name);
}

int get_unif_loc(GLint* unif, GLuint prog, GLchar* name)
{
	*unif = glGetUniformLocation(prog, name);
	return check_attrunif(unif, name);
}

char* read_shader_f(char *file)
{
	int len;
	char *buf;

	len = load_file(file, false, false, &buf);

	if (!len)
		return NULL;

	for (int i = 0; i < len; i++)
		if (buf[i] == '~')
		{
			buf[i] = 0;
			break;
		}

	return buf;
}

int check_shader(GLuint shader, char *name)
{
	int   infologLen = 0;
	int   charsWritten = 0;
	char *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);

	if (infologLen > 1)
	{
		infoLog = new char[infologLen];
		glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog);
		printf("Shader '%s' log: %s\n", name, infoLog);
		delete[] infoLog;
		return ST_SHADER_ERROR;
	}
	return ST_OK;
}

int check_program(GLuint program)
{
	int   infologLen = 0;
	int   charsWritten = 0;
	char *infoLog;

	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infologLen);

	if (infologLen > 1)
	{
		infoLog = new char[infologLen];
		glGetProgramInfoLog(program, infologLen, &charsWritten, infoLog);
		printf("Program log: %s\n", infoLog);
		delete[] infoLog;
		return ST_SHADER_ERROR;
	}
	return ST_OK;
}

int init_shaders(char *p_v, char* p_f, GLuint &prog)
{
	GLuint vs, fs;

	char *vs_code = read_shader_f(p_v);
	char *fs_code = read_shader_f(p_f);

	if (!vs_code || !fs_code)
	{
		raise_error("Error read shaders \n", false, true);
		return ST_SHADER_ERROR;
	}

	vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_code, NULL);
	glCompileShader(vs);
	int err = check_shader(vs, p_v);
	if (err)
		return err;

	fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_code, NULL);
	glCompileShader(fs);
	err = check_shader(fs, p_f);
	if (err)
		return err;

	prog = glCreateProgram();
	glAttachShader(prog, vs);
	glAttachShader(prog, fs);

	glLinkProgram(prog);
	err = check_program(prog);
	if (err)
		return err;

	delete[] vs_code;
	delete[] fs_code;

	if (check_GL_error())
		return ST_SHADER_ERROR;
	return ST_OK;
}

void set_vsync(bool vs)
{
	wglSwapIntervalEXT_Func wglSwapIntervalEXT = wglSwapIntervalEXT_Func(wglGetProcAddress("wglSwapIntervalEXT"));
	if (wglSwapIntervalEXT)
		wglSwapIntervalEXT(vs);
}

int init_glew()
{
	GLenum glew_status = glewInit();
	if (GLEW_OK != glew_status)
	{
		std::cout << "Error: " << glewGetErrorString(glew_status) << std::endl;
		return ST_GLEW_INIT_ERROR;
	}

	if (!GLEW_VERSION_3_3)
	{
		std::cout << "No support for OpenGL 3.3 found\n";
		return ST_GLEW_INIT_ERROR;
	}
	return ST_OK;
}

int init_programs()
{
	int err;
	if (err = init_shaders("shaders/text_v.glsl", "shaders/text_f.glsl", textProgram))
		return err;

	return ST_OK;
}

int main_gl_init(int argc, char *argv[])
{
	int k = 0;
	glutInit(&k, 0);
	//glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WND_RES[0], WND_RES[1]);
	glutInitWindowPosition(WND_POS[0], WND_POS[1]);
	glutCreateWindow("Game");
	glutHideWindow();

	//hwnd = GetActiveWindow();

	if (int err = init_glew())
		return err;
	if (int err = init_programs())
		return err;

	glutDisplayFunc(render_all);
	glutTimerFunc(TIMER_TICKS, timer, 0);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glutKeyboardFunc(on_press_key);
	glutSpecialFunc(on_press_spec_key);
	glutKeyboardUpFunc(on_up_key);
	glutSpecialUpFunc(on_up_spec_key);
	glutMotionFunc(on_mouse_move);
	glutPassiveMotionFunc(on_mouse_move);
	glutMouseFunc(on_mouse_click);

	//glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	cout << (unsigned char*)glGetString(GL_VENDOR) << endl << (unsigned char*)glGetString(GL_RENDERER) << endl;

	return ST_OK;
}