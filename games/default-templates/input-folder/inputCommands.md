# Input - List of keys

These values are used into: input-template.buttons-map.element.**&lt;value&gt;**, Three elements are required in order to build the **value**, the first is the type of input, that can be: kyd for keyboard, mse for mouse. The second is the state from that input, that can be: h for holding keys, p for pressing keys, r for released keys. The third one is the key that can be one of from the list. 

Example: for keyboard hold space = "kyd-h-space"

Special values, those values have different notation:

- "kyd-none" means no keys from keyboard are pressed. 

**Key List:**

- 



## ISSUES

### Hardware Limitation 

Some hardware limitation like on "cheap" keyboards, can only handler with an range of keys in the 
same circuit. If you try to create an game with more than the number of simultaneously key pressed on the same keyboard circuit it will fail to grab those key press into the Proto-Test. 

Trying avoid that, you can use less key hold and use more key pressed or released. 
