const char* defaultButtonTemplate = 
"{
	"type": "PT_ScreenButton",
	"image": "none",
	"speedX": 0.0,
	"speedY": 0.0,
	"srcRect": [0, 0, 96, 64],
	"dstRect": [100.0, 20.0, 96.0, 64.0],
	"animations": [
		{
			"name": "idle",
			"frame-delay": 0,
			"frame-width": 96,
			"frame-height": 64,
			"frame-column": 0,
			"frame-column-dir": 1,
			"frame-column-max": 2,
			"frame-row": 0,
			"frame-row-dir": 0,
			"frame-row-max": 0,
		},
		{
			"name": "active",
			"frame-delay": 0,
			"frame-width": 96,
			"frame-height": 64,
			"frame-column": 1,
			"frame-column-dir": 0,
			"frame-column-max": 2,
			"frame-row": 0,
			"frame-row-dir": 0,
			"frame-row-max": 0,
		},
	],
	"colliders": [
		{
			"visible": true,
			"type": "PT_COLLIDER_TYPE_RECTANGLE",
			"name": "collider",
			
			"color": {
				"red": 120,
				"green": 220,
				"blue": 60,
				"alpha": 100,
			},
			"fields": {
				"x": 0,
				"y": 0,
				"w": 96,
				"h": 64,
			}
		},
	],
	"behavior": "none",
	
	"misc": {
		"button-name": "Load the game",
		
		"actions": {
			"mouse-over": {
				"change-animation": "active",
			},
			"mouse-click": {
				"button": "left",
			},
		},
	}
}";

