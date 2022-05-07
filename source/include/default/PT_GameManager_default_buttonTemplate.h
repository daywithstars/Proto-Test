const char* defaultButtonTemplate = 
"{\n\
	\"type\": \"PT_InputButton\",\n\
	\"tags\": {\n\
		\"collider\": {\n\
			\"group\": \"GameManager Buttons\",\n\
		},\n\
	},\n\
	\"image\": \"button-load\",\n\
	\"speedX\": 0.0,\n\
	\"speedY\": 0.0,\n\
	\"srcRect\": [0, 0, 96, 64],\n\
	\"dstRect\": [100.0, 50.0, 96.0, 64.0],\n\
	\"animations\": [\n\
		{\n\
			\"name\": \"idle\",\n\
			\"frame-delay\": 0,\n\
			\"frame-width\": 96,\n\
			\"frame-height\": 64,\n\
			\"frame-column\": 0,\n\
			\"frame-column-dir\": 1,\n\
			\"frame-column-max\": 1,\n\
			\"frame-row\": 0,\n\
			\"frame-row-dir\": 0,\n\
			\"frame-row-max\": 0,\n\
		},\n\
		{\n\
			\"name\": \"active\",\n\
			\"frame-delay\": 0,\n\
			\"frame-width\": 96,\n\
			\"frame-height\": 64,\n\
			\"frame-column\": 1,\n\
			\"frame-column-dir\": 0,\n\
			\"frame-column-max\": 2,\n\
			\"frame-row\": 0,\n\
			\"frame-row-dir\": 0,\n\
			\"frame-row-max\": 0,\n\
		},\n\
	],\n\
	\"colliders\": [\n\
		{\n\
			\"visible\": false,\n\
			\"type\": \"PT_COLLIDER_TYPE_RECTANGLE\",\n\
			\"name\": \"collider\",\n\
			\n\
			\"color\": {\n\
				\"red\": 120,\n\
				\"green\": 220,\n\
				\"blue\": 60,\n\
				\"alpha\": 200,\n\
			},\n\
			\"fields\": {\n\
				\"x\": 0,\n\
				\"y\": 0,\n\
				\"w\": 96,\n\
				\"h\": 64,\n\
			}\n\
		},\n\
	],\n\
	\"behavior\": \"none\",\n\
	\n\
	\"misc\": {\n\
		\"button-name\": \"Load the game\",\n\
		\n\
		\"actions\": {\n\
			\"mouse-over\": {\n\
				\"animation\": \"active\",\n\
			},\n\
			\"mouse-out\": {\n\
				\"animation\": \"idle\",\n\
			},\n\
			\"mouse-click\": {\n\
				\"button\": \"left\",\n\
			},\n\
		},\n\
	}\n\
}";



