# trabalho-CG
Trabalho de CG realizado em 2022

## Instruções de compilação
Este projeto utiliza _submodules_, por isso é importante garantir que eles estão instalados.

```bash
$ git clone https://github.com/SugaryLump/trabalho-CG --recursive
$ cd trabalho-CG
$ cmake . -B build
$ cd build
$ make
```

As seguintes bibliotecas precisam de estar instaladas no sistema:
 - OpenGL
 - GLU
 - freeglut

Para além dessas, ao clonar recursivamente são instaladas as seguintes bibliotecas:
 - fmtlib
 - pugixml
 - argparse
 
 ## Instruções de uso
 Para obter as instruções de uso do comando do gerador e do _engine_ utiliza-se a opção ``--help``.
 
 ## Controlos do engine
 
  - F: Mudar entre modo explorador e FPS
  - Setas: Mover câmara em modo explorador
  - F1 e F2: Alterar raio da câmara em modo explorador
  - WASD: Deslocar câmara em modo FPS
  - Espaço: Subir câmara em modo FPS
  - C: Descer câmara em modo FPS
  - Rato: Rodar câmara em modo FPS
  - .: Ligar/Desligar renderização de eixos
  - ,: Mudar modo de renderização de VBOs
