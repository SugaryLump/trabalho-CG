with (import <nixpkgs> {});
mkShell {
  name = "cg";
  buildInputs = [ cmake gnumake libGL libGLU freeglut ];
}
