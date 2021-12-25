# Behavior Event Trigger and fields

Once you choose the **&lt;TriggerType&gt;**, **&lt;ValueType&gt;** and the **&lt;fields&gt;** will be filled of data in order to work. 


## &lt;TriggerType&gt;

You need to put one of those: 

- **"none"**
  - Will ignore the rest of event, and will not execute. 
- **"aways"**
  - It will be happens on every circle. 
  - Do not require any data on *"fields"*. 
- **"times"**
  - It will be run only a number of times.
  - require: **"times": integer** on fileds
- **"condition"**
  - It will need an certain condition to execute. 
  - In construction. 
- **"timer"**
  - It will execute only one time, once the reach the delay.
  - require: **"timer": integer(in milliseconds)** on fields
- **"input"**
  - It will execute once an keyMap is pressed.
  - require: **"key-map": "&lt;input-template.buttons-map.name&gt;"**
- **"collision"**
  - It will execute once the collision happens.
  - In construction. 

## &lt;ValueType&gt;

- **"play-sample"**
  - Play an sample when the **&lt;TriggerType&gt;** occurs. 
  - require **"sample-name": "&lt;SampleName&gt;** on fields.
  - require **"loop": integer** on fields.
- **"play-music"**
  - Play an music when the **&lt;TriggerType&gt;** occurs. 
  - require **"music-name": "&lt;MusicName&gt;** on fields.
  - require **"loop": integer** on fields.
- **"change-screen"**
  - Change the screen.
  - In construction.


 
## Examples

### Play an sample when press some keyMap

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



