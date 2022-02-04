# Structure of sprite-template.json

This file describes all elements on the sprite-template files, this is not the documentation on how to use them. The full documentation will be released in the future. 


How it looks like:

```json
{
	"type": "$SpriteType$",
	"tags": {
	},
	"image": "<ImageToUse>",
	"speedX": 0.1,
	"speedY": 0.2,
	"srcRect": [0, 0, 64, 64],
	"dstRect": [0.0, 0.0, 64.0, 64.0],
	"animations": [
	],
	"behavior": "<behaviorName>",
	"colliders": [
	],
	
	"misc": {
	}
} 
```

- **"type"** Required - This represents the type of that sprite-template
  - **"$SpriteType$"** Here you can have those values: 
    - **PT_Sprite** Represents the basic sprite
    - **PT_DynamicImage** Represents the sprite for the dynamic image that you can manipulate via a screen. 
  
## tags

Example:

```json
"tags": {
		"collider": "<group-name>",
		"movement": [
			"constant-stop-horizontal",
			"constant-stop-vertical",
		],
	},
```

- **"collider"** This element represents the tag group of colliders to be tested against, if one sprite have a different tag collider group name, the tests for collider will never occur for these sprites.   
  - **value** string.
- **"movement"** This element sends some flags to the movement system.
  - **"constant-stop-horizontal"** Before every call to PT_SpriteMoveLeft or PT_SpriteMoveRight it will set to 0 to prevent the sprite to continue moving through once the input was pressed. 
  - **"constant-stop-vertical"** Before every call to PT_SpriteMoveUp or PT_SpriteMoveDown it will set to 0 to prevent the sprite to continue moving through once the input was pressed. 
  

## miscellaneous 
  
- **"image"** Required - The sprite image
  - **"&lt;ImageToUse&gt;"** Choose a previously loaded image via the img-list.json
- **"speedX"** Not required - The value that represents the final velocity of the sprite on the horizontal axis, this value must have the notation C-like double notation: 0.0, 0.120, 1.020
- **"speedY"** Not required - The value that represents the final velocity of the sprite on the vertical axis, this value must have the notation C-like double notation: 0.0, 0.120, 1.020
- **"srcRect"** Not required- This represents the crop values to the image, that can be the entire image or not.
  - [integer, integer, integer, integer] - That is: [left, top, right, bottom] those values must not have the floating/double point notation, only integer: 10, 20, 100. 
- **"dstRect"** Not required - This represents the sprite position on the screen and the scale of it.
  - [double, double, double, double] - That is: [x-position, y-position, width-scale, height-scale] those values must be in C-like double notation: 0.0, 0.120, 1.021
  
## animations

Example:

```json
"animations": [
	{
		"name": "<animation-name>",
		"frame-delay": 0,
		"frame-width": 64,
		"frame-height": 64,
		"frame-column": 0,
		"frame-column-dir": 0,
		"frame-column-max": 3,
		"frame-row": 1,
		"frame-row-dir": 0,
		"frame-row-max": 1,
	},
],
```
With this element, you can add animation to the sprite sheets. 

- **"name"** The animation name. 
  - **value** need to be a string.
- **"frame-delay"** The time in milliseconds to change from one frame to other. 
  - **value** integer.
- **"frame-width"** The crop width for each frame on the sheet.
  - **value** integer.
- **"frame-height"** The crop height for each frame on the sheet. 
  - **value** integer. 
- **"frame-column"** The start column of the frame.
  - **value** integer. 
- **"frame-column-dir"** The direction to run on the column.
  - **value** integer: 1, -1, 0.
- **"frame-column-max"** The number of frames on the columns.
  - **value** integer.
- **"frame-row"** The start row of the frame.
  - **value** integer.
- **"frame-row-dir"** The direction to run on the row.
  - **value** integer: 1, -1, 0.
- **"frame-row-max"** The number of frames on the rows.
  - **value** integer.

## colliders

With this element you can create colliders to be tested. 

Example: 

```json
"colliders": [
	{
		"visible": true,
		"type": "PT_COLLIDER_TYPE_RECTANGLE",
		"name": "sprite2 top collider",
		
		"fields": {
			"x": 10,
			"y": 5,
			"w": 50,
			"h": 10,
		}
	},
],
```

- **"visible"** A value that allows the collider to be draw.
  - **value** Boolean. 
- **"type"**  This change how the element **"fields"** are filled. 
  - **values** PT_COLLIDER_TYPE_RECTANGLE.
- **"name"** The collider name.
  - **value** string. 
  
### colliders.fields

Depending on the **"type"** the this element can have different values. 

- **"type"** = **"PT_COLLIDER_TYPE_RECTANGLE"**
  - **"x"** The start rectangle x position on the sprite/frame image.
    - **value** integer.
  - **"y"** The start rectangle y position on the sprite/frame image.
    - **value** integer.
  - **"w"** The width of the rectangle on the sprite/frame image.
    - **value** integer.
  - **"h"** The height of the rectangle on the sprite/frame image.
    - **value** integer.

## miscellaneous
  
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
    - **value** string.
  - **"frame-delay"** Integer value in milliseconds. Represents the delay of each frame.
    - **value** integer.
  - **"frame-width"** The width of each crop size from the original image. 
    - **value** integer.
  - **"frame-height"** The height of each crop size from the original image.
    - **value** integer.
  - **"frame-column"** The column that will be start the animation. 
    - **value** integer.
  - **"frame-column-dir"** The animation direction, that can be 1 run from left to right, -1 to run from right to left, 0 to not run.
    - **value** integer.
  - **"framecolumn-max"** This is the max number of frames on columns. 
    - **value** integer.
  - **"frame-row"** The row that will be start the animation.
    - **value** integer.
  - **"frame-row-dir"** The same as column, but to control the row direction.
    - **value** integer.
  - **"frame-row-max"** This is the max number of frames on rows. 
    - **value** integer.
  

  
## misc

This is a special object that holds additional information to others types beside PT_Sprite.


  
  
