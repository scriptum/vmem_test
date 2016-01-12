#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>

#include "render.h"
#include "SOIL/SOIL.h"

typedef struct FBO {
	GLuint tex, fbo;
} FBO;

FBO newFBO(GLuint width, GLuint height)
{
	FBO fbo;
	// generate texture save target
	glGenTextures(1, &fbo.tex);
	glBindTexture(GL_TEXTURE_2D, fbo.tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,0);
	glBindTexture(GL_TEXTURE_2D, 0);
	// create framebuffer
	glGenFramebuffers_(1, &fbo.fbo);
	glBindFramebuffer_(GL_FRAMEBUFFER_EXT, fbo.fbo);
	glFramebufferTexture2D_(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT,
			GL_TEXTURE_2D, fbo.tex, 0);
	if(!checkFramebufferStatus()) exit(0);
	return fbo;
}

int is_mem_zero(void* mem, size_t size)
{
	size_t *w, i;
	#define DO if(w[i] != (size_t)0) return 0; i++;
	for(i = 0, w = (size_t*)mem; i < size / sizeof(size_t);)
	{
		DO DO DO DO
	}
	#undef DO
	return 1;
}

int main(int argc, const char* argv[]){
	Display *dpy;
	Window root;
	GLint att[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	XVisualInfo *vi;
	GLXContext glc;
	clock_t start_time;
	size_t i, j;
	const size_t VRAM = 1024 * 1024 * 1024;
	const size_t width = 1920, height = 1080;

	dpy = XOpenDisplay(NULL);

	if ( !dpy ) {
		printf("Cannot connect to X server\n\n");
		exit(0);
	}

	root = DefaultRootWindow(dpy);
	vi = glXChooseVisual(dpy, 0, att);

	if (!vi) {
		printf("No appropriate visual found\n\n");
		exit(0);
	}

	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, root, glc);

	initRenderer();
	start_time = clock();

	size_t len = sizeof(char) * 3 * width * height * 100;
	unsigned char* img = (unsigned char*)malloc(len);
	char fname[] = "xxxxxxxxxxx.bmp";
	for(i = 0; i < VRAM / (width * height * 3); i++)
	{
		FBO fbo = newFBO(width, height);
		glBindFramebuffer_(GL_FRAMEBUFFER_EXT, fbo.fbo);
		glBindTexture(GL_TEXTURE_2D, fbo.tex);
		glReadBuffer(GL_COLOR_ATTACHMENT0_EXT);
		glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, img);
		if(!is_mem_zero(img, len))
		{
			snprintf(fname, sizeof(fname), "%d.bmp", i + 1);
			SOIL_save_image(fname, SOIL_SAVE_TYPE_BMP, width, height, 3, img);
		}
	}
	printf("Time: %f sec\n", (float)(clock() - start_time)/(float)CLOCKS_PER_SEC);

	return 0;
}
