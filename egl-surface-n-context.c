
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#include <EGL/egl.h>
#include <stdio.h>

int main()
{
    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

    // get current display
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY); // eglGetCurrentDisplay();

    // init egl
    EGLint major = 1, minor = 5;
    if (eglInitialize(display, &major, &minor) != EGL_TRUE)
    {
        printf("EGL initialization failed\n");
        return 1;
    }

    /* bind API */
    if (eglBindAPI(EGL_OPENGL_ES_API) != EGL_TRUE)
    {
        printf("EGL API bind failed\n");
        eglTerminate(display);
        return 1;
    }

    /* get a config */
    EGLConfig config;
    EGLint numConfigs;
    EGLint fbAttribs[] =
    {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE,        8,
        EGL_GREEN_SIZE,      8,
        EGL_BLUE_SIZE,       8,
        EGL_NONE
    };
    if (eglChooseConfig(display, fbAttribs, &config, 1, &numConfigs) != EGL_TRUE)
    {
        printf("EGL no filtered configuration 1\n");
        eglTerminate(display);
        return 1;
    }
    else if (numConfigs < 1)
    {
        printf("EGL no filtered configuration 2\n");
        eglTerminate(display);
        return 1;
    }

    /* create pbuffer surface */
    EGLint surfaceAttribs[] =
    {
        EGL_WIDTH, WINDOW_WIDTH,    // set width to WINDOW_WIDTH
        EGL_HEIGHT, WINDOW_HEIGHT,  // set height to WINDOW_HEIGHT
        EGL_NONE
    };
    EGLSurface surface = eglCreatePbufferSurface(display, config, surfaceAttribs);
    if (surface == EGL_NO_SURFACE)
    {
        printf("EGL surface creation failed %X\n", eglGetError());
        eglDestroySurface(display, surface);
        eglTerminate(display);
        return 1;
    }

    /* create context */
    EGLContext context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);

    /* make context current */
    if (eglMakeCurrent(display, surface, surface, context) != EGL_TRUE)
    {
        printf("Could not make the current window current !\n");
    }
    else
    {
        printf("Succeed in creating a pbuffer surface&context\n");
    }

    /* release resources */
    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);

    return 0;
}
