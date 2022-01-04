# Structure of sprite-template.json

This file describes all elements on a sprite-template, this is not the documentation on how to use them. The full documentation will be released in the future. 


```json
{
	"type": "$SpriteType$",
	"image": "<ImageToUse>",
	"speedX": 0.1,
	"speedY": 0.2,
	"srcRect": [0, 0, 64, 64],
	"dstRect": [0.0, 0.0, 64.0, 64.0],
	"behavior": "<behaviorName>",
	
	"misc": {
	}
} 
```

- **"type"** Required - This represents the type of that sprite-template
  - **"$SpriteType$"** Here you can have those values: 
    - **PT_Sprite** Represents the basic sprite
    - **PT_DynamicImage** Represents the sprite for the dynamic image that you can manipulate via a screen. 
- **"image"** Required - The sprite image
  - **"&lt;ImageToUse&gt;"** Choose a previously loaded image via the img-list.json
- **"speedX"** Not required - The value that represents the final velocity of the sprite on the horizontal axis, this value must have the notation C-like double notation: 0.0, 0.120, 1.020
- **"speedY"** Not required - The value that represents the final velocity of the sprite on the vertical axis, this value must have the notation C-like double notation: 0.0, 0.120, 1.020
- **"srcRect"** Not required- This represents the crop values to the image, that can be the entire image or not.
  - [integer, integer, integer, integer] - That is: [left, top, right, bottom] those values must not have the floating/double point notation, only integer: 10, 20, 100. 
- **"dstRect"** Not required - This represents the sprite position on the screen and the scale of it.
  - [double, double, double, double] - That is: [x-position, y-position, width-scale, height-scale] those values must be in C-like double notation: 0.0, 0.120, 1.021
- **"behavior"** Not required - The behavior-template to use.
  - **"&lt;behaviorName&gt;"** The name of a previously created behavior-template. Do not put the .json it will assumes that is a json file. 
  
```json
"animations": [
	{
		"name": "first-anim",
		"frame-delay": 1000,
		"frame-width": 32,
		"frame-height": 32,
		"frame-column": 0,
		"frame-column-dir": 1,
		"frame-column-max": 2,
		"frame-row": 0,
		"frame-row-dir": 0,
		"frame-row-max": 2,
	},
	{
		"name": "second-anim",
		"frame-delay": 1000,
		"frame-width": 32,
		"frame-height": 32,
		"frame-column": 0,
		"frame-column-dir": 1,
		"frame-column-max": 2,
		"frame-row": 1,
		"frame-row-dir": 0,
		"frame-row-max": 2,
	},
]
```
  
- **"animations"** Optional - This represents the array of animations. All fields below are optional but, will be replaced by the default values from above. 
  - **"name"** The animation name.
  - **"frame-delay"** Integer value in milliseconds. Represents the delay of each frame.
  - **"frame-width"** The width of each crop size from the original image. 
  - **"frame-height"** The height of each crop size from the original image.
  - **"frame-column"** The column that will be start the animation. 
  - **"frame-column-dir"** The animation direction, that can be 1 run from left to right, -1 to run from right to left, 0 to not run.
  - **"framecolumn-max"** This is the max number of frames on columns. 
  - **"frame-row"** The row that will be start the animation.
  - **"frame-row-dir"** The same as column, but to control the row direction.
  - **"frame-row-max"** This is the max number of frames on rows. 
  

  
## misc

This is a special object that holds additional information to others types beside PT_Sprite.


  
  
