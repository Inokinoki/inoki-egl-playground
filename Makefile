CC=gcc
WAYLAND_LIB=-lwayland-egl -lwayland-client
EGL_LIB=-lEGL


all: egl-query.out egl-configuration.out egl-surface-n-context.out

egl-query.out: egl-query.c
	$(CC) $< $(EGL_LIB) $(WAYLAND_LIB) -o $@

egl-configuration.out: egl-configuration.c
	$(CC) $< $(EGL_LIB) $(WAYLAND_LIB) -o $@

egl-surface-n-context.out: egl-surface-n-context.c
	$(CC) $< $(EGL_LIB) $(WAYLAND_LIB) -o $@
