* [Back](../00-introduction.md)

# Create the game folder

In order to edit/play your "proto-game" you need an *template-folder* that can be create 
from the beginner or use an already created. The term *template-folder* means any folder 
that goes inside your game folder or your own game folder, that is your *proto-game*. 

Now you know that is possible to build your game like pieces of templates let's create 
the first folder, the game folder and give it a name. 

## Creating the game folder manually 

You're on the root folder that is: 
* doc/
* games/
* SDL/
* source/
* curl/
* build/
* Release/
* CMakeLists.txt/
* LICENSE.md/
* setup.sh/

Or depending the version you are using, some folders/files not exits but anyways, our work is 
on the *games* folder. 

- In the *games* folder you will simple create an folder with the name of your game.
  - It's recommended that you do not put special characters on the folder name.
- Now let's tell to the Proto Test the game we want to load. 
  - Open the `settings.json` file and on section: `"game": "your game name"` you will put the folder name 
on `"your-game-name"`.
  - Save the file end exit.
- Now you have your first game, just run the Proto Test. 

Well, now we have our game folder, you can edit any configuration file(.json files) and 
customize the game, before edit any json file, look at [configuration files](../configuration-files.md),
but for now i recommend to continue to the next section: 
* [ 01 - Adding Images to the game ](../01-images/images.md).



