# <img src="https://github.com/lilmuff2/X-coder/blob/images/icon.png?raw=true" width="35" height="35"> X-coder
Tool to decode and encode sc files from supercell games (works only on Windows) 
![menu](https://github.com/lilmuff2/X-coder/blob/images/menu.png?raw=true)
## [GUI VERSION](https://github.com/lilmuff2/X-coder/tree/PC-GUI) 
![menu-gui](https://github.com/lilmuff2/X-coder/blob/images/menu-gui.png?raw=true)
If you want more beautiful version with interface download it
## [APK](https://github.com/lilmuff2/X-coder/releases/download/v1.1android/X.coder_1.1.apk) 
If you use android download it and if you dont know how to use it check pc gui version it is same in usage
## How to use:
Put [Xcoder.exe](https://github.com/lilmuff2/X-coder/releases/download/v1.1/Xcoder.exe) and [cv2 dll](https://github.com/lilmuff2/X-coder/releases/download/v1.1/opencv_world470.dll) in one folder and run exe


OR


Just DOWNLOAD [Instaler](https://github.com/lilmuff2/X-coder/releases/download/v1.1/Xcoder_instaler.exe) and run it
### Functions:
#### Decode: 
1. Select _tex.sc file
2. Wait some time while textures are decoding 
3. Select folder where the pngs will be saved or close select dialog to create folder automatically
4. Done! now you can edit pngs
![Decode example](https://github.com/lilmuff2/X-coder/blob/images/decode.png?raw=true)
#### Encode:
1. Select folder with pngs, folder must contain only those png files that will be in the final sc, and should preferably have a data.json file
2. Wait some time while textures are encoding it is quet long, but you can put false in data.json file in "UseKronos" then it'll be a lot faster
3. Select out file path or close select dialog to save file in pngs folder
4. Done! you can put your sc in game files

Below is information about the data.json file

   
![Encode example](https://github.com/lilmuff2/X-coder/blob/images/encode.png?raw=true)
#### Decode folder: 
1. Select folder with tex.sc files
2. Select folder where pngs will be saved or close select dialog to Select same folder as folder with tex.sc files
3. In folder with pngs folders with names of sc will be crated and pngs will be there
![Decode folder example](https://github.com/lilmuff2/X-coder/blob/images/decodefolder.png?raw=true) 
#### Encode folder: 
1. Select folder with folders with pngs
2. Select folder where tex.sc will be saved or close select dialog to Select same folder as with folders with tex.sc files
3. In this folder sc with names of folder wil be sved
![Encode folder example](https://github.com/lilmuff2/X-coder/blob/images/encodefolder.png?raw=true)
#### Setings: 
Just editor of [config.json](#Config.json-file) file
![Encode folder example](https://github.com/lilmuff2/X-coder/blob/images/setings.png?raw=true)
## Data.json file:
This file stores texture information, it is recommended not to change anything except "IsKronos", but note that "IsKronos: true" only works with 50+ version of Brawl Stars.
Example of data.json file:
```javascript
[
 {
  "Downscaling": true,
  "Filter": 0,
  "IsKhronos": true,
  "Linear": true,
  "PixelFormat": 0
 },
 {
  "Downscaling": true,
  "Filter": 0,
  "IsKhronos": true,
  "Linear": true,
  "PixelFormat": 0
 }
]
```
### IsKhronos:
Just a bool that says whether to use the new texture type. If used, the output file weighs much less and has a positive effect on game optimization. But encode will take many times longer.
In fact it's just ktx in sc

Example with ui_highres_tex and ZSTD compresion :
IsKhronos | encode time(seconds) | save time(seconds)  | file size
---|---|---|---
true | 137.5 | 7.798 | 13.8 MB
false | 1.112 | 19.605 | 22,8 MB
> I recomend you not to use it while testing your mod and use it in release version 
## Config.json file:
The file is created when the script is run for the first time and stores the settings.
Default config.json file:
```javascript
{
 "Default Compresion": 3,
 "Skip Folder Select": false,
 "Skip Save Select": false
}
```
### Default Compresion:
Allows you to change the compression used during encode. 
Index | Compression | Description
--- | --- | ---
1 | LZMA | Compresses large files, like ui well enough
2 | LZHAM | IDK Just dont use it
3 | ZSTD | Used by default, best for most of sc, used by supercell
### Skip Folder Select 
If true, when decoding, skip folder select and do same as if you just close it 
### Skip Save Select
Same but for encoding and for out path selection

# Any questions?
My nickname evrywhere is lilmuff1, i mostly check discord and telegram so write there.
