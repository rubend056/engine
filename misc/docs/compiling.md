<h1 style="display:flex; justify-content:center">
	<img src="../icons/gear.png" alt="drawing" width="30" style="margin-right:10px;margin-left:10px"/> 
	Triangle Compilation
</h1>

There's two ways compilation is set up and both are crucial to creating a shippable game/software:

1. **Editor**, the program that allows you to edit, play, set up your assets and preview your game
2. **Player**, the program that will run on your target hardware/software stack and is incredibly optimized for it. This program is the lightweight version of the engine that can only execute the game.

## Editor

- Linux - pacman/apt
  - Run `make` from the project root to compile and launch the engine on project `./games/game0`. This loads all the assets and launches the last open scene.
- Mac (unavailable)
- Windows (unavaliable)

## Player

- This is to be compiled by the engine binary above, you should **not** have to manually compile the player.
- An option to pick target binaries from the editor for compilation to happen. Window with progress bar/etc, the whole shabang... not yet implemented.
