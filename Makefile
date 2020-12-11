CC=gcc
WAYLAND_LIB=-lwayland-egl -lwayland-client
EGL_LIB=-lEGL
X11_LIB=-lX11

all: egl-query.out egl-configuration.out egl-surface-n-context.out \
	egl-x-surface.out

egl-query.out: egl-query.c
	$(CC) $< $(EGL_LIB) -o $@

egl-configuration.out: egl-configuration.c
	$(CC) $< $(EGL_LIB) -o $@

egl-surface-n-context.out: egl-surface-n-context.c
	$(CC) $< $(EGL_LIB) -o $@

egl-x-surface.out: egl-x-surface.c
	$(CC) $< $(EGL_LIB) $(X11_LIB) -o $@
