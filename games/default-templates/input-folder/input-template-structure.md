# The structure of input-template.json

The way you delegate each input to a name you want, this will be sent to the Proto-Test and can be used in behavior-template. All the elements are required. 

```json
{
	"file": "<thisInputTemplateFileName>.json",

	"settings": {
		"name": "<nameYourInputTemplateFile>"
	},
	
	"buttons-map": [
		{ "<nameYourButton>": "<see inputCommands.md>" },
		{ "<nameYourButton>": "<see inputCommands.md>" },
	]
}
```

- **"file"** is a label that will be used internally. 
  - **"&lt;thisInputTemplateFileName&gt;.json"** the file name in your disk.
- **"settings"** The object that holds information's to make the basic setup.
  - **"name"** This element holds the name of your input-template into Proto-Test. 
    - **"&lt;nameYourInputTemplateFile&gt;"** Choose the name you want. 
- **"buttons-map"** The array of buttons to map.
  - **"&lt;nameYourButton&gt;"** is the map name you want to the **inputCommand** between {} the json object.
    - **"&lt;see inputCommands.md&gt;"** That is the list of all keys.  
  

