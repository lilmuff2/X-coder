# <img src="https://github.com/lilmuff2/X-coder/blob/master/android/res/drawable-xxxhdpi/icon.png?raw=true" width="35" height="35"> X-coder
[ДЛЯ РУССКИХ](https://github.com/lilmuff2/X-coder/blob/master/Readme_ru.md)

[INSTALL](https://github.com/lilmuff2/X-coder/releases/tag/v2.0)

A tool for decoding (sc to png) and encoding (png to sc) sc files used in Supercell games.
![Menu](https://github.com/lilmuff2/X-coder/blob/images/new_menu.png?raw=true)
## Functions:
## Decode File: 
1. Click the select button and choose or type in the input file field the path to the texture file (_tex.sc, rarely _dl.sc).
2. Starting with version 54 of Brawl Stars there are zktx files they must be placed in the same folder as the texture file
3. you can select the folder where the pngs will be saved (out folder) it will be deleted! If you do not choose it will be created automatically
4. Click the decode button and wait for decoding to complete.

![Decode example](https://github.com/lilmuff2/X-coder/blob/images/new_decode.png?raw=true)
## Encode File:
1. Click the select button and choose or type in the input folder field the path to the decoded folder. 
2. You can select where to save the texture file (out file), if you don't select it, it will be selected automatically.
3. click the encode button and wait for decoding to complete (may take a long time).   
![Encode example](https://github.com/lilmuff2/X-coder/blob/images/new_encode.png?raw=true)
## Decode folder: 
1. Click the select button and choose or type in the input folder field the path of the folder with the texture files.
2. You can select the folder where the png folders will be created (out folder) it will be deleted! If you do not select it will be created automatically
3. In this folder will appear folders with the names of texture files in which will lie png.


![Decode folder example](https://github.com/lilmuff2/X-coder/blob/images/new_decode_folder.png?raw=true) 
## Encode folder: 
1. Click the select button and choose or type in the input folder field the folder path to the decoded folder.
2. You can select the folder where the texture files will be saved (out folder) it will be deleted! If you do not select it will be created automatically
3. Texture files will be saved in this folder

![Encode folder example](https://github.com/lilmuff2/X-coder/blob/images/new_encode_folder.png?raw=true)
## .json file:
Этот файл генерируется при декодировании Пример:
```javascript
{
    "Textures": [ 
        {
            "Encoding": "khronos",
            "PixelFormat": "RGBA8",
            "Filtering": "LINEAR_NEAREST",
            "Linear": true
        },
        {
            "Encoding": "khronos",
            "PixelFormat": "RGBA8",
            "Filtering": "LINEAR_NEAREST",
            "Linear": true
        }
    ],
    "IsDL": false,
    "HasZKTX": true,
    "Compression": "Zstandard"
}
```

### Encoding
Can be khronos (low weight) and raw (fast encoding) with HasZKTX enabled textures are encoded in khronos,

Example ui_highres_tex with Zstandard compression:
Encoding | Encoding time | File size
---|---|---
khronos | 35 s | 30 MB
raw | 7 s | 41 MB
> I recommend using raw during mod development and khronos on release.
### HasZKTX 
Parameter responsible for the existence of zktk files
### ISDL
Parameter responsible for creating dl file (combining sc and tex_sc), usually used in Clash Royale requires sc file in the folder.
### Compression
File compression, with HasZKTX enabled, Zstandard is always used.
Example of compression on ui_highres_tex 
Compression | File encoding time | File weight | Total
--- | --- | --- | --- 
Lzma | 41 s | 26MB| Longer but compresses better.
Lzham | 73sec | 27MB | It's useless.
Zstandard | 35secs | 30MB | Standard, fast and compresses well.

# Any questions?
My social networking nickname is lilmuff1 message me or join <a target=”_blank” href="https://t.me/XcoderBS">Telegram</a> or <a href="https://discord.com/invite/yNajwpBe" target=”_blank”>Discord</a>  
