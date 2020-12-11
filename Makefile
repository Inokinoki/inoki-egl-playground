CC=gcc
WAYLAND_LIB=-lwayland-egl -lwayland-client
EGL_LIB=-lEGL


all: egl-query.out

egl-query.out: egl-query.c
	$(CC) $< $(EGL_LIB) $(WAYLAND_LIB) -o $@
