# Structure of global-settings-template.json

The file is the global settings for the games. All fields below are required. 

```json
{
	"games": [
		{ 
			"name": "<nameOfYourGame>", 
			"folder-name": "<gameFolderName>" 
		},
		{ 
			"name": "<nameOfYourGame>", 
			"folder-name": "<gameFolderName>" 
		},
  	],
}
```

- **"games"** Are the array of games that you have in the directory: root/games
  - **"name"** This element is inside an object that represents the name of your game:
    - **"&lt;nameOfYourGame&gt;"** Strings that represents the game name.
    
    
for now, the Proto-Test will only loads the first object from the array. 


