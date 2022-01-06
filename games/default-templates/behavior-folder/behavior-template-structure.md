# The structure of behavior-template.json

## Settings

```json
"settings": {
	"input-template": "<input-template.settings.name>",
},
```

This *json element* is where the behavior will get information to make its basic setup.

- **"input-template"** The template from input folder to use. 
  - **&lt;input-template.settings.name&gt;** Is where you can find the name of your input-template.json 

## States (optional)

The biggest part from behavior-template.json, this is where the logic happens. Each state has its own different name, and it can change from one to another depending of each state logic. The first state on the list, is the default state. 

```json
"states": [
]
```

The first element is the array that will hold every state that you create. That is, an object:

```json
{
	"name": "<nameOfState>",
},
```

The first part of this object is the name element, that represents the state name.

- **"name"** The state name element.
  - **&lt;nameOfState&gt;** string that represent the name of that state. 

The object holds three main features: **input**, **event**, **always**. These features have its exclusive implementation and meaning:

### input (optional)

```json
"input": {
	"<input-template.buttons-map.name>": [
		"$callback-function$",
		"change-state-<states.name>",
		"change-animation-<sprite-template.animations[].name>",
	],

	"<input-template.buttons.map.name>": "$callback-function$",
	"<input-template.buttons.map.name>": "change-state-<states.name>",
	"<input-template.buttons-map.name>": "change-animation-<sprite-template.animations[].name>",
}
```

This is the main way to implement input to the state, it's use the settings.input-template previously described, as "key mapping" for their action into the Proto-Test objects. You can use one map name to task several commands using the array type. 

- **"&lt;input-template.buttons-map.name&gt;"** from file input-template.md this is the string place for your mapped buttons, created in that template. If this element is from JSON array type, you can give any command you want for this key map. 
  - With this element you can assign an **$callback-function$** or **"change-state-&lt;states.name&gt;"**(required)

**$callback-function$**(required)

Depends on the sprite-template.type:

- for type: PT_Sprite
  - PT_SpriteMoveLeft move the sprite to the left of the screen based on sprite-template.speedX
  - PT_SpriteMoveRight move the sprite to the right of the screen based on sprite-template.speedX
  - PT_SpriteMoveUp move the sprite to the top of the screen based on sprite-template.speedY
  - PT_SpriteMoveDown move the sprite to the bottom of the screen based on sprite-template.speedY
  - PT_SpriteStopMoveHorizontal stop the sprite on x-axis
  - PT_SpriteStopMoveVertical stop the sprite on y-axis
  - PT_SpriteGrab works only with mouse grab from input-template-structure.json

**"change-state-&lt;states.name&gt;"**(required)

If the input occurs, the state will change to what you want, for example: "change-state-jumping". 


**"change-animation-&lt;sprite-template.animations[].name&gt;"**(required)

When the button map is pressed, it will change the current animation to another if exists. 



### event (optional)

```json
"event": [
	{
	},
]
```

The event element is where you have more access and control to things inside the Proto-Test. Here you can have different triggers that allows different events happens. Once you decide to put the above structure the rest is **required**. 

```json
"trigger": {
}
```

The object inside the array of objects from "event", that represent the trigger and event values. 

```json
"type": "$TriggerType$",
```

- **"type"** Is the trigger type element. 
  - **"$TriggerType$"** the element value. That can be:
    - **none**, Will ignore the rest of event, and will not execute. 
    - **always**, It will happens every time. 
    - **times**, It will run only a number of times.
      - This requires in **"fields"**, **"times": integer**.
    - **condition** It will need an certain condition to allow the event. 
      - In construction.
    - **timer** It will execute only one time, once they reach the delay value.
      - This requires in **"fields"**, **"timer": integer(in milliseconds)**. 
    - **input** It will execute once a **"&lt;input-template.buttons-map.name&gt;"** happens.
      - This requires in **"fields"**, **"key-map": "&lt;input-template.buttons-map.name&gt;"**
    - **collision** It will allows once the collision happens.
      - In construction. 

```json
"value": {
	"type": "$ValueType$",
	"fields": {
	}
}
```

This object is the final part.

- **"type"** Is the element value type.
  - **"$ValueType$"** it can be:
    - **"play-sample"** Play a sample when the **"$TriggerType$"** occurs. 
      - Require **"sample-name": "&lt;SampleName&gt;** on **"fields"**.
      - Require **"loop": integer** on **"fields"**.
    - **"play-music"** Play a music when the **"$TriggerType$"** occurs.
      - Require **"music-name": "&lt;MusicName&gt;** on **"fields"**.
      - Require **"loop": integer** on **"fields"**.
    - **"change-screen"** Change the screen.
      - In construction

#### Example

```json
"trigger": {

	"type": "input",
	
	"value": {
		"type": "play-sample",
		"fields": {
			"keyMap": "input-shot",
			"sample-name": "jump",
			"loop": 0
		}
	}
},
```


### always (optional)

```json
"always": [
]
```

This element represents an array of strings, that will always be executed while the state is running. 


```json
"$callback-function$",
"$callback-function$",
```

**$callback-function$**(required) the same as the **input**. 



