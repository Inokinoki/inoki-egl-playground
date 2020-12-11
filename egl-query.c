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

    // get query string
    const char *client_apis = eglQueryString(display, EGL_CLIENT_APIS);
    if (client_apis)
    {
        printf("Client APIS: %s\n", client_apis);
    }
    else
    {
        printf("Client APIS: NULL %X\n", eglGetError());
    }

    const char *egl_version = eglQueryString(display, EGL_VERSION);
    if (egl_version)
    {
        printf("EGL Version: %s\n", egl_version);
    }
    else
    {
        printf("EGL Version: NULL %X\n", eglGetError());
    }

    const char *egl_vendor = eglQueryString(display, EGL_VENDOR);
    if (egl_vendor)
    {
        printf("EGL Vendor: %s\n", egl_vendor);
    }
    else
    {
        printf("EGL Vendor: NULL %X\n", eglGetError());
    }

    /* describes the supported extensions by EGL */
    const char *no_display_extensions = eglQueryString(EGL_NO_DISPLAY, EGL_EXTENSIONS);
    if (no_display_extensions)
    {
        printf("Client extension:\n %s\n", no_display_extensions);
    }
    else
    {
        printf("Client extension:\n NULL %X\n", eglGetError());
    }

    /* describes the supported extensions by display */
    const char *extensions = eglQueryString(display, EGL_EXTENSIONS);
    if (extensions)
    {
        printf("Display extensions:\n %s\n", extensions);
    }
    else
    {
        printf("Display extensions:\n NULL %X\n", eglGetError());
    }

    return 0;
}
