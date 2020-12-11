#include <EGL/egl.h>
#include <stdio.h>

int main()
{
    // get current display
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY); // eglGetCurrentDisplay();

    // init egl
    EGLint major = 1, minor = 5;
    if (eglInitialize(display, &major, &minor) != EGL_TRUE)
    {
        printf("EGL initialization failed\n");
        return 1;
    }

    EGLConfig configs[1024];
    EGLint configCounter;

    if (eglGetConfigs(display, configs, 1024, &configCounter) != EGL_TRUE)
    {
        printf("EGL configuration getting failed\n");
        return 1;
    }

    printf("Retrieved %d configurations\n", configCounter);
    for (int i = 0; i < configCounter; i++)
    {
        /* get config value for each item */
        EGLint bufferSize, configId;
        EGLBoolean configOk;
        eglGetConfigAttrib(display, configs[i], EGL_CONFIG_ID, &configId);
        configOk = eglGetConfigAttrib(display, configs[i], EGL_BUFFER_SIZE, &bufferSize);
        
        if (configOk)
        {
            printf("EGL_BUFFER_SIZE of config %d: %X\n", configId, bufferSize);
        }
        else
        {
            printf("EGL_BUFFER_SIZE of config %d: NULL\n", i);
        }
    }

    /* release resources */
    eglTerminate(display);

    return 0;
}
