#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "render.h"
#define SDL_GL_GetProcAddress glXGetProcAddress

// GL_EXT_framebuffer_object
PFNGLBINDRENDERBUFFEREXTPROC        glBindRenderbuffer_        = NULL;
PFNGLDELETERENDERBUFFERSEXTPROC     glDeleteRenderbuffers_     = NULL;
PFNGLGENFRAMEBUFFERSEXTPROC         glGenRenderbuffers_        = NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC     glRenderbufferStorage_     = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC  glCheckFramebufferStatus_  = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC         glBindFramebuffer_         = NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC      glDeleteFramebuffers_      = NULL;
PFNGLGENFRAMEBUFFERSEXTPROC         glGenFramebuffers_         = NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC    glFramebufferTexture2D_    = NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbuffer_ = NULL;
PFNGLGENERATEMIPMAPEXTPROC          glGenerateMipmap_          = NULL;

// GL_ARB_multitexture
PFNGLACTIVETEXTUREARBPROC       glActiveTexture_       = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTexture_ = NULL;
PFNGLMULTITEXCOORD2FARBPROC     glMultiTexCoord2f_     = NULL;
PFNGLMULTITEXCOORD3FARBPROC     glMultiTexCoord3f_     = NULL;
PFNGLMULTITEXCOORD4FARBPROC     glMultiTexCoord4f_     = NULL;

// GL_EXT_framebuffer_blit
PFNGLBLITFRAMEBUFFEREXTPROC         glBlitFramebuffer_         = NULL;


void initRenderer()
{
	const char *vendor = (const char *)glGetString(GL_VENDOR);
	const char *exts = (const char *)glGetString(GL_EXTENSIONS);
	const char *renderer = (const char *)glGetString(GL_RENDERER);
	const char *version = (const char *)glGetString(GL_VERSION);
	printf("Renderer: %s (%s)\n", renderer, vendor);
	printf("Driver: %s\n", version);
	supported.GLSL = 0;

	supported.BE = 0;

	supported.FBO = 0;
	if(strstr(exts, "GL_EXT_framebuffer_object"))
	{
		glBindRenderbuffer_        = (PFNGLBINDRENDERBUFFEREXTPROC)       SDL_GL_GetProcAddress("glBindRenderbufferEXT");
		glDeleteRenderbuffers_     = (PFNGLDELETERENDERBUFFERSEXTPROC)    SDL_GL_GetProcAddress("glDeleteRenderbuffersEXT");
		glGenRenderbuffers_        = (PFNGLGENFRAMEBUFFERSEXTPROC)        SDL_GL_GetProcAddress("glGenRenderbuffersEXT");
		glRenderbufferStorage_     = (PFNGLRENDERBUFFERSTORAGEEXTPROC)    SDL_GL_GetProcAddress("glRenderbufferStorageEXT");
		glCheckFramebufferStatus_  = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC) SDL_GL_GetProcAddress("glCheckFramebufferStatusEXT");
		glBindFramebuffer_         = (PFNGLBINDFRAMEBUFFEREXTPROC)        SDL_GL_GetProcAddress("glBindFramebufferEXT");
		glDeleteFramebuffers_      = (PFNGLDELETEFRAMEBUFFERSEXTPROC)     SDL_GL_GetProcAddress("glDeleteFramebuffersEXT");
		glGenFramebuffers_         = (PFNGLGENFRAMEBUFFERSEXTPROC)        SDL_GL_GetProcAddress("glGenFramebuffersEXT");
		glFramebufferTexture2D_    = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)   SDL_GL_GetProcAddress("glFramebufferTexture2DEXT");
		glFramebufferRenderbuffer_ = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)SDL_GL_GetProcAddress("glFramebufferRenderbufferEXT");
		glGenerateMipmap_          = (PFNGLGENERATEMIPMAPEXTPROC)         SDL_GL_GetProcAddress("glGenerateMipmapEXT");
		supported.FBO = 1;
	}
	else
	{
		printf("Framebuffers isn't supported by your hardware.\n");
		exit(0);
	}
}

int checkFramebufferStatus() {
	GLenum status;
	status = (GLenum) glCheckFramebufferStatus_(GL_FRAMEBUFFER_EXT);
	switch(status) {
			case GL_FRAMEBUFFER_COMPLETE_EXT:
					return 1;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
    printf("Framebuffer incomplete, incomplete attachment\n");
					return 0;
			case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
    printf("Unsupported framebuffer format\n");
					return 0;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
    printf("Framebuffer incomplete, missing attachment\n");
					return 0;
			case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
    printf("Framebuffer incomplete, attached images must have same dimensions\n");
					return 0;
			case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
    printf("Framebuffer incomplete, attached images must have same format\n");
					return 0;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
    printf("Framebuffer incomplete, missing draw buffer\n");
					return 0;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
    printf("Framebuffer incomplete, missing read buffer\n");
					return 0;
	}
	return 0;
}