# <img src="https://github.com/lilmuff2/X-coder/blob/master/android/res/drawable-xxxhdpi/icon.png?raw=true" width="35" height="35"> X-coder
[ДЛЯ РУССКИХ](https://github.com/lilmuff2/X-coder/blob/master/Readme_ru.md)

A tool for decoding (sc to png) and encoding (png to sc) sc files used in Supercell games.
![Menu](https://github.com/lilmuff2/X-coder/blob/images/new_menu.png?raw=true)
# How to use
1. Download the version you need from [releases](https://github.com/lilmuff2/X-coder/releases/tag/v2.2 ) (apk - for android, setup.exe - installer for Windows, zip archive with exe for Windows)
3. Select the desired option (each one is described below)
5. Select or type the input file or folder
7. Optionally select either type the out folder or file, or it will be selected automatically <b>!IT WILL BE DELETED!</b>
8. Press the button at the bottom (not close) and wait


# DECODE FILE:
Converts sc to png<br>
Input file - <b>_tex.sc , not .sc!</b> or _dl.sc in rare cases<br>
Starting with the 54th version of brawl stars, zktx files appeared. They need to be placed in the same folder as the input file<br>
Out folder - the folder in which the apg will be saved<br>

![Decode example](https://github.com/lilmuff2/X-coder/blob/images/new_decode.png?raw=true)

# ENCODE FOLDER:
Converts png to sc<br>
Input folder - folder with png and [json file](#json-file)(optionally)<br>
The name of the json file must match the name of the folder<br>
Out file - where to save sc<br>
If you need sc for the old version, change the compression in the json file from Zstandard to Lzma<br>

![Encode example](https://github.com/lilmuff2/X-coder/blob/images/new_encode.png?raw=true)


# DECODE FILES:
Converts a lot of sc files to png, it is usfull to simply copy the sc folder from the apk and decode them all at once
Input folder - folder with <b>_tex.sc , not .sc!</b> or _dl.sc files<br>
Outfolder - the folder where the folders will be created in which the pngs will be saved<br>

![Decode folder example](https://github.com/lilmuff2/X-coder/blob/images/new_decode_folder.png?raw=true) 


# ENCODE FOLDERS:
Converts many png folders to sc files<br>
Input folder - a folder with folders containing png<br>
Output folder - the folder where the sc files will be saved<br>

![Encode folder example](https://github.com/lilmuff2/X-coder/blob/images/new_encode_folder.png?raw=true)
# .json file:
This file generated after decoding example:
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

## Encoding
Can be khronos (low weight) and raw (fast encoding) if HasZKTX enabled textures are encoded in khronos

Example ui_highres_tex with Zstandard compression:
Encoding | Encoding time | File size
---|---|---
khronos | 35 s | 30 MB
raw | 7 s | 41 MB
> I recommend using raw during mod development and khronos on release.
## HasZKTX 
Parameter responsible for the existence of zktk files
## ISDL
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
My social networking nickname is lilmuff1 message me or join [Telegram](https://t.me/XcoderBS) or [Discord](https://discord.com/invite/yNajwpBe)
