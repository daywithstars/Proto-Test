# Structure of music-list-template.json

This is the list of soundtrack put into the folder: sound/music, that will be loaded into the Proto-Test. 
Proto-Test supports: .mp3, .ogg, .wav formats. 

```json
{
	"musics": [
		"<musicFileName>.$format$",
		"<musicFileName>.$format$",
	]
}
```

- **"musics"** The array of music files.
  - **"&lt;musicFileName&gt;.$format$"** The complete file name with extension.
    - **$format$** Can be one of those: .mp3, .ogg or .wav
    
