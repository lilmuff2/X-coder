
# <img src="https://github.com/lilmuff2/X-coder/blob/master/android/res/drawable-xxxhdpi/icon.png?raw=true" width="35" height="35"> X-coder
Инструмкнт для декодирования (sc в png) и кодирования (png в sc) файлов sc которые используются в играх Supercell
![Menu](https://github.com/lilmuff2/X-coder/blob/images/new_menu.png?raw=true)
# Функции:
## Decode File: 
1. Нажмите на кнопку select и выбирите или впишите в поле input file путь к файлу текстуры(_tex.sc, редко _dl.sc)
2. Начиная с 54 версии Brawl Stars появились zktx файлы их надо поместить в ту же папку что и файл текстуры
3. Можно выбрать папку в которую будут сохранены пнг (out folder) она будет удалена! Если не выбирать она создастся автоматически
4. Нажать на кнопку decode и дождатся завершения декодирования

![Decode example](https://github.com/lilmuff2/X-coder/blob/images/new_decode.png?raw=true)
## Encode File:
1. Нажмите на кнопку select и выбирите или впишите в поле input folder путь к папке полученой в результате декодирования 
2. Можно выбрать куда сохранять файл текстуры (out file), если не выбирать он выберится автоматически
3. Нажать на кнопку encode и дождатся завершения декодирования (может занять много времени)   
![Encode example](https://github.com/lilmuff2/X-coder/blob/images/new_encode.png?raw=true)
## Decode folder: 
1. Нажмите на кнопку select и выбирите или впишите в поле input folder путь папке с файлами текстуры
2. Можно выбрать папку в которой будут созданы папки с пнг (out folder) она будет удалена! Если не выбирать она создастся автоматически
3. В этой папке появтяся папки с именами файлов текстур в которых будут лежать пнг

![Decode folder example](https://github.com/lilmuff2/X-coder/blob/images/new_decode_folder.png?raw=true) 
## Encode folder: 
1. Нажмите на кнопку select и выбирите или впишите в поле input folder путь к папке полученой в результате декодирования папки
2. Можно выбрать папку в которую будут сохранены файлы текстуры (out folder) она будет удалена! Если не выбирать она создастся автоматически
3. В этой папке будут сохранены файлы текстуры

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
Может быть khronos (мало весит) и raw (быстро кодируется) при включеном HasZKTX текстуры кодируются в khronos,

Пимер ui_highres_tex с Zstandard сжатием:
Encoding | Время кодирования | Размер файла
---|---|---
khronos | 35 с | 30 MB
raw | 7 с | 41 MB
> Я рекомендую во время разработки мода использовать raw а при релизе khronos
### HasZKTX 
Параметр отвечающий за наличие zktk файлов
### ISDL
Параметр отвечающий за создание dl файла (объеденение sc и tex_sc), обычно используется в Clash Royale требует наличия sc файла в папке
### Compression
Сжатие файла , включеном HasZKTX всешда используеься Zstandard
Пример сжатия на ui_highres_tex 
Compression | Время кодирования файла | Вес файла | Итог
--- | --- | --- | --- 
Lzma | 41 с | 26MB| Дольше но сжимает лучше
Lzham | 73с | 27MB | Оно бесполезно
Zstandard | 35 с | 30 MB| Стандартное, быстрое и хорошо сжимает

# Остались вопросы?
Мой ник в сотсетях lilmuff1 пишите или присойденяйтесь к <a target=”_blank” href="https://t.me/XcoderBS">Telegram</a> или <a href="https://discord.com/invite/yNajwpBe" target=”_blank”>Discord</a>  
