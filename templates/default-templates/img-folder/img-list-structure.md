# The structure of img-list.json

This is the way you can send images to the Proto-Test and use it on Sprites, etc. Do not name your images with the same name. All the elements are required. 

```json
{
	"<nameYourImage>": "<imageFileName>.$format$",
}
```

The object send the images files to the Proto-Test, the supported formats are: .png, .bmp

- **"&lt;nameYourImage&gl;"** This the name name you choose to be the id of your image. 
  - **"&lt;imageFileName&gt;".$format$"** This the the file name with one of the formats: .png, .bmp
 
**Example**:

```json
{
	"sprite-megaman": "megaman-spritesheet.png",
	"sprite-bomberman": "bomberman-spritesheet.bmp",
}
```
  
