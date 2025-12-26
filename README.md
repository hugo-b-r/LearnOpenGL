# Mon repo qui me permet d'apprendre à coder en OpenGL

## Semaine 1

![Le premier cube de la vie](./Assets/Week1.png)

## Semaine 3
![Les premiers 648 000 cubes de la vie](./Assets/Week3.png)

## To-do 

- [ ] dessiner des textures
- [ ] des textures
- [ ] des chunks, un renderer de chunk etc...
- [ ] configuration : des inputs, des gamemode...
- [ ] implémenter une classe d'entrée pour faire des entrées propres qui appellent correctement la translation de la camera
- [ ] un générateur de chunks type bruit de perlin
- [ ] la gravité et la détection des collisions
- [ ] une interface
- [ ] des beaux shaders pour l'eau, le ciel etc.
- [ ] des arbres, des structures aléatoires...


## Set-up for dev

Cmake is set to automatically expose `compile_commands.json` in the build directory. You then just configure clangdlike that :
```
CompileFlags:
  CompilationDatabase: "build"
```
