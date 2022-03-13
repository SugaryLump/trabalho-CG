with (import <nixpkgs> {});
mkShell {
  name = "cg";
  buildInputs = [ git clang-tools cmake gnumake libGL libGLU freeglut glew ];
  shellHook = ''
    export GLU_DIRECTORY=${libGLU.dev}/include
    export GLUT_DIRECTORY=${freeglut.dev}/include
  '';
}
