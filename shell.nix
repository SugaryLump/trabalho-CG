with (import <nixpkgs> {});
mkShell {
  name = "cg";
  buildInputs = [ git clang-tools cmake gnumake libGL libGLU freeglut ];
  shellHook = ''
    export GLU_DIRECTORY=${libGLU.dev}/include
  '';
}
