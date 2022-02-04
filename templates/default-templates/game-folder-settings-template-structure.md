# Structure of game-folder-settings-template.json

The file is the basic settings for the game in the folder game/your-game. All fields below are required. 

```json
{ 
	"graphics": {
		"screenWidth": 640, 
		"screenHeight": 480, 
		"renderDrawColor": [255, 0, 0, 255] 
	},
	
	"camera": {
		"width": 640,
		"height": 480,
	},
}
```

- **"graphics"** Is the main object here, it make the setup for the graphics of the game.
  - **"screenWidth"** Means the width of the screen in pixels.
  - **"screenHeight"** Means the height of the screen in pixels.
  - **"renderDrawColor"** Is the background clear color for the game.
    - [unsigned integer 8-bit, unsigned integer 8-bit, unsigned integer 8-bit, unsigned integer 8-bit,] This is the RGBA value, red, green, blue, alpha.
    
- **"camera"** Is the main camera, viewport size. 
  - **"width"** 
    - **value** integer.
  - **"height"**
    - **value** integer. 
