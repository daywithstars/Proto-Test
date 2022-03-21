# Structure of global-settings-template.json

The file is the global settings for the games. All fields below are required. 
This file must be in the same folder of the executable, this file must have the name: global-settings.json

```json
{
	"root-folder": "<relativePath$/$>",
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

- **"root-folder"** Is the relative path starting from the executable location to the folder contain the games folders. 
  - **/** This final character is required on the place of *$/$*. 
- **"games"** Are the array of games that you have in the directory, pointed by "root-folder" element.
  - **"name"** This element is inside an object that represents the name of your game:
    - **"&lt;nameOfYourGame&gt;"** Strings that represents the game name.


