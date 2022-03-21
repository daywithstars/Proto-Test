const char* defaultSettingsTemplate =  
"{\n\
	\"graphics\": {\n\
		\"screenWidth\": 640, \n\
		\"screenHeight\": 480, \n\
		\"renderDrawColor\": [240, 240, 240, 255] \n\
	},\n\
	\n\
	\"camera\": {\n\
		\"width\": 640,\n\
		\"height\": 480,\n\
		\n\
		\"colliders\": [\n\
			{\n\
				\"visible\": true,\n\
				\"type\": \"PT_COLLIDER_TYPE_RECTANGLE\",\n\
				\"name\": \"camera-left-screen-collider\",\n\
				\"color\": {\n\
					\"red\": 200,\n\
					\"green\": 0,\n\
					\"blue\": 0,\n\
					\"alpha\": 140,\n\
				},\n\
				\n\
				\"fields\": {\n\
					\"x\": 0,\n\
					\"y\": 0,\n\
					\"w\": 10,\n\
					\"h\": 480,\n\
				}\n\
			},\n\
			{\n\
				\"visible\": true,\n\
				\"type\": \"PT_COLLIDER_TYPE_RECTANGLE\",\n\
				\"name\": \"camera-right-screen-collider\",\n\
				\"color\": {\n\
					\"red\": 0,\n\
					\"green\": 0,\n\
					\"blue\": 200,\n\
					\"alpha\": 140,\n\
				},\n\
				\n\
				\"fields\": {\n\
					\"x\": 630,\n\
					\"y\": 0,\n\
					\"w\": 10,\n\
					\"h\": 480,\n\
				}\n\
			},\n\
			{\n\
				\"visible\": true,\n\
				\"type\": \"PT_COLLIDER_TYPE_RECTANGLE\",\n\
				\"name\": \"camera-top-screen-collider\",\n\
				\"color\": {\n\
					\"red\": 0,\n\
					\"green\": 200,\n\
					\"blue\": 0,\n\
					\"alpha\": 140,\n\
				},\n\
				\n\
				\"fields\": {\n\
					\"x\": 10,\n\
					\"y\": 0,\n\
					\"w\": 620,\n\
					\"h\": 10,\n\
				}\n\
			},\n\
			{\n\
				\"visible\": true,\n\
				\"type\": \"PT_COLLIDER_TYPE_RECTANGLE\",\n\
				\"name\": \"camera-bottom-screen-collider\",\n\
				\"color\": {\n\
					\"red\": 200,\n\
					\"green\": 200,\n\
					\"blue\": 200,\n\
					\"alpha\": 190,\n\
				},\n\
				\n\
				\"fields\": {\n\
					\"x\": 10,\n\
					\"y\": 470,\n\
					\"w\": 620,\n\
					\"h\": 10,\n\
				}\n\
			},\n\
		],\n\
	},\n\
	\n\
	\"input\": {\n\
  		\"mouse\": {\n\
  			\"rect\": [0, 0, 4, 4],\n\
  		},\n\
  	},\n\
}";

