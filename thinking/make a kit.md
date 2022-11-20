# mdmE Kits (.mdk)

Kits in mdmE are zip files containing at least one file called `kit.json` .
The structure of this file is organized under x properties:

- **"name"** : *string* - the name of the kit.
- **"version"** : *string* - the version of the kit.
- **"spChannels"** : *array* - array defining specials channels containing other data than notes.
- **"notes"** : *dictionnary* - dictionnary of notes descriptions, the index being a string corresponding to it's id.

## channels types

0 normal channel
1 normal channel but events
2 normal channel but values (hex numbers)
3 one number (as float)

## notes types

0 normal note
1 double (gemini)
2 slider (hold or masher)

## Special Channels

Special channels are channels who can contain one value per measure.
The value can be of any type, int, float, string...
There is two properties:

- name : string - name of the channel
- type : int - value type of the channel

## Notes descriptions

Notes descriptions describe all the notes in a pack and their properties.
The properties are:

- **"name"** : *string* - name of the note. the sprite and/or the sound files must have the same.
- **"type"** : *string* - type of the note, can be Beat, Slider, or Event.
- **"sound"** : *bool* - does the note have a sound.
- **"sprite"** : *bool* - does the note have a sprite. not available for events.
- **"channels"** : *array of strings* - list of channels were the note can be placed. Can't be a special channel
- **"kitSkin"** : *string* - available for skin pack kits, specify the kit to modify.

If the kit is a skin pack, only **"name"**, **"sound"**, **"sprite"** and **"kitSkin"** are read.
