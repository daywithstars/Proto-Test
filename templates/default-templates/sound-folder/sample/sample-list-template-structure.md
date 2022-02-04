# Structure of sample-list-template.json

This is the list of sfx/samples put into the folder: sound/sample, that will be loaded into the Proto-Test. 
Proto-Test support .wav format only. So do not put long and heavy music files here or the memory ram will be wasted.  

```json
{
	"samples": [
		"<sampleFileName>.wav",
		"<sampleFileName>.wav",
	]
}
```

- **"samples"** The array of sample files.
  - **"&lt;sampleFileName&gt;.wav"** The complete file name with extension.
    
