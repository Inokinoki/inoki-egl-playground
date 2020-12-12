
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#include <EGL/egl.h>
#include <wayland-client.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* definition in wayland, copied from EGL/eglplatform.h */
//typedef struct wl_display     *EGLNativeDisplayType;
//typedef struct wl_egl_pixmap  *EGLNativePixmapType;
//typedef struct wl_egl_window  *EGLNativeWindowType;

struct wl_display *wlDisplay = NULL;
struct wl_compositor *wlCompositor = NULL;
struct wl_surface *wlSurface;
struct wl_shell *wlShell = NULL;
struct wl_shell_surface *wlShellSurface;

struct wl_egl_window *egl_window;


EGLDisplay display;
EGLSurface surface;
EGLContext context;

int egl();
int clear_egl();

static void
global_registry_handler(void *data, struct wl_registry *registry, uint32_t id,
	       const char *interface, uint32_t version)
{
    // printf("Got a registry event for %s id %d\n", interface, id);
    if (strcmp(interface, "wl_compositor") == 0) {
        wlCompositor = wl_registry_bind(registry, 
				      id, 
				      &wl_compositor_interface, 
				      1);
    } else if (strcmp(interface, "wl_shell") == 0) {
        wlShell = wl_registry_bind(registry, id,
                                 &wl_shell_interface, 1);
    }
}

static void
global_registry_remover(void *data, struct wl_registry *registry, uint32_t id)
{
    printf("Got a registry losing event for %d\n", id);
}

static const struct wl_registry_listener registry_listener = {
    global_registry_handler,
    global_registry_remover
};


int main(int argc, char **argv) {
    /* connect to wayland */
    wlDisplay = wl_display_connect(NULL);
    if (wlDisplay == NULL) {
        printf("Can't connect to display\n");
        return 1;
    }
    printf("connected to display\n");

    struct wl_registry *registry = wl_display_get_registry(wlDisplay);
    wl_registry_add_listener(registry, &registry_listener, NULL);

    wl_display_dispatch(wlDisplay);
    wl_display_roundtrip(wlDisplay);

    if (wlCompositor == NULL) {
        printf("Can't find compositor\n");
        return 1;
    } else {
	    printf("Found compositor\n");
    }

    wlSurface = wl_compositor_create_surface(wlCompositor);
    if (wlSurface == NULL) {
        printf("Can't create surface\n");
        exit(1);
    } else {
	    printf("Created surface\n");
    }

    if (wlShell == NULL) {
        printf("Haven't got a Wayland shell\n");
        exit(1);
    }
    wlShellSurface = wl_shell_get_shell_surface(wlShell, wlSurface);
    if (wlShellSurface == NULL) {
        printf("Can't create shell surface\n");
        exit(1);
    } else {
	    printf("Created shell surface\n");
    }
    wl_shell_surface_set_toplevel(wlShellSurface);

    /* call egl */
    egl();
    while (wl_display_dispatch(wlDisplay) != -1) {
	    ;
    }
    clear_egl();

    wl_display_disconnect(wlDisplay);
    printf("disconnected from display\n");
    
    return 0;
}

int egl()
{
    /* prepare wayland */
    struct wl_region *region = wl_compositor_create_region(wlCompositor);
    wl_region_add(region, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    wl_surface_set_opaque_region(wlSurface, region);

    EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

    // get current display
    display = eglGetDisplay(wlDisplay); // eglGetCurrentDisplay();

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
        return 1;
    }

    /* get a config */
    EGLConfig config;
    EGLint numConfigs;
    EGLint fbAttribs[] =
    {
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_NONE
    };
    if (eglChooseConfig(display, fbAttribs, &config, 1, &numConfigs) != EGL_TRUE)
    {
        printf("EGL no filtered configuration 1\n");
        return 1;
    }
    else if (numConfigs < 1)
    {
        printf("EGL no filtered configuration 2\n");
        return 1;
    }

    /* create context */
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);

    egl_window = wl_egl_window_create(wlSurface, WINDOW_WIDTH, WINDOW_HEIGHT);
    if (egl_window == EGL_NO_SURFACE)
    {
        printf("Can't create egl window\n");
        return 1;
    }
    else
    {
	    printf("Created egl window\n");
    }

    /* create window surface */
    surface = eglCreateWindowSurface(display, config, egl_window, NULL);
    if (surface == EGL_NO_SURFACE)
    {
        printf("EGL surface creation failed %X\n", eglGetError());
        return 1;
    }

    /* make context current */
    if (eglMakeCurrent(display, surface, surface, context) != EGL_TRUE)
    {
        printf("Could not make the current window current !\n");
    }
    else
    {
        printf("Succeed in creating a pbuffer surface&context\n");
    }

    if (eglSwapBuffers(display, surface))
    {
	    printf("Swapped buffers\n");
    }
    else
    {
	    printf("Swapped buffers failed\n");
    }

    return 0;
}

int clear_egl()
{
    /* release resources */
    eglDestroyContext(display, context);
    eglDestroySurface(display, surface);
    eglTerminate(display);
    return 0;
}
