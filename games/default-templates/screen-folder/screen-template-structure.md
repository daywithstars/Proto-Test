# The structure of screen-template.json

This template is the screen container, that will have everything you want in that screen. 

```json
{
	"file": "<thisScreenFileName>.json",

	"settings": {
		"name": "<nameYourScreen>",
		"tag": "<tagType>",
	},
	
	"sprites": [
		"<sprite-template>",
		"<sprite-template>",
	]
}

```

- **"file"** Is a label that will be used internally. 
  - Required.
  - **"&lt;thisScreenFileName&gt;.json"** the file name in your disk. 
  
- **"settings"** The object that holds information's to make the basic setup. 
  - Required.
  - **"name"** This element holds the name of your screen-template into Proto-Test. 
    - **"&lt;nameYourScreen&gt;"** Choose the name you want. 
  - **"tag"** This is the special element that sends some flags. 
    - **"$tagType$"** Replace that string with some of those values:
      - **first-screen** This defines the first screen that will be loaded, then set to be the first screen. 
      - If multiple screens are tagged with "first-screen" then, All screens with the Proto-Test will be loaded and set the last screen from the screen-list.json to be the first screen.  
      
- **"sprites"** The array of sprites from that screen.
  - Not required
  - **"&lt;sprite-template&gt;"** sprite-template file name. 


