# Inoki's EGL Playground

EGL™ is an interface between Khronos rendering APIs such as OpenGL ES or OpenVG and the underlying native platform window system.

There are some examples for EGL. Notice that this playground just gives an overview of EGL. For further information, you can access [EGL Overview](https://www.khronos.org/egl).

## How to use

The default targets are just for no-window API tests. To build them, use:

```sh
make
```

This will build:

- `egl-query.out` demonstrates a query of EGL capacities in your system;
- `egl-configuration.out` lists available configurations of EGL;
- `egl-surface-n-context.out` creates a PBuffer (offscreen rendering) surface and a context for that surface.

To build the EGL x Wayland example

```sh
make egl-wayland-surface.out
```

This will build a program which creates a black surface by communicating with the wayland compositor.

For X11 user, build:

```sh
make egl-x-surface.out
```

This program is still under development.

## TODO

- X11 EGL example
- Write a tutorial

## References

1. EGL Overview https://www.khronos.org/egl
2. Programming wayland clients https://jan.newmarch.name/Wayland/index.html
