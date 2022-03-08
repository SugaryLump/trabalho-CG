with (import <nixpkgs> {});
mkShell {
  name = "cg";
  buildInputs = [ clang-tools cmake gnumake libGL libGLU freeglut ];
}
