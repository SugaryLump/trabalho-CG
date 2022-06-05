{
  description = "Trabalho CG";

  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let pkgs = nixpkgs.legacyPackages.${system}; in
      with pkgs;{
        devShell = mkShell
          {
            name = "cg";
            buildInputs = [ git clang-tools cmake gnumake libGL libGLU freeglut glew pkg-config libdevil gdb ];
            shellHook = ''
              export GLU_DIRECTORY=${libGLU.dev}/include
              export GLUT_DIRECTORY=${freeglut.dev}/include
            '';
          };
      }
    );
}
