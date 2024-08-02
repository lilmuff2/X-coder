#  X-coder
[Скачать](https://github.com/lilmuff2/X-coder/releases/tag/v2.0)
Инструмент для декодирования (sc в png) и кодирования (png в sc) файлов sc которые используются в играх Supercell
![Menu](https://github.com/lilmuff2/X-coder/blob/images/new_menu.png?raw=true)
# Функции:
## Decode File:
1. Нажмите на кнопку select и выберите или впишите в поле input file путь к файлу текстуры(_tex.sc, редко _dl.sc)
2. Начиная с 54 версии Brawl Stars появились zktx файлы их надо поместить в ту же папку, что и файл текстуры
3. Можно выбрать папку в которую будут сохранены пнг (out folder) она будет удалена! Если не выбирать она создастся автоматически
4. Нажать на кнопку decode и дождаться завершения декодирования

![Decode example](https://github.com/lilmuff2/X-coder/blob/images/new_decode.png?raw=true)
## Encode File:
1. Нажмите на кнопку select и выберите или впишите в поле input folder путь к папке полученной в результате декодирования
2. Можно выбрать куда сохранять файл текстуры (out file), если не выбирать он выберется автоматически
3. Нажать на кнопку encode и дождаться завершения декодирования (может занять много времени)
![Encode example](https://github.com/lilmuff2/X-coder/blob/images/new_encode.png?raw=true)
## Decode folder:
1. Нажмите на кнопку select и выберите или впишите в поле input folder путь папке с файлами текстуры
2. Можно выбрать папку в которой будут созданы папки с пнг (out folder) она будет удалена! Если не выбирать она создастся автоматически
3. В этой папке появятся папки с именами файлов текстур в которых будут лежать пнг

![Decode folder example](https://github.com/lilmuff2/X-coder/blob/images/new_decode_folder.png?raw=true)
## Encode folder:
1. Нажмите на кнопку select и выберите или впишите в поле input folder путь к папке подученной в результате декодирования папки
2. Можно выбрать папку в которую будут сохранены файлы текстуры (out folder) она будет удалена! Если не выбирать она создастся автоматически
3. В этой папке будут сохранены файлы текстуры

![Encode folder example](https://github.com/lilmuff2/X-coder/blob/images/new_encode_folder.png?raw=true)
## .json file:
Этот файл генерируется при декодировании Пример:
javascript
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


### Encoding
Может быть khronos (мало весит) и raw (быстро кодируется) при включённом HasZKTX текстуры кодируются в khronos,

Пример ui_highres_tex с Zstandard сжатием:
Encoding | Время кодирования | Размер файла
---|---|---
khronos | 35 с | 30 MB
raw | 7 с | 41 MB
> Я рекомендую во время разработки мода использовать raw а при релизе khronos
### HasZKTX
Параметр отвечающий за наличие zktk файлов
### ISDL
Параметр отвечающий за создание dl файла (объединение sc и tex_sc), обычно используется в Clash Royale требует наличия sc файла в папке
### Compression
Сжатие файла, при включённом HasZKTX всегда используется Zstandard
Пример сжатия на ui_highres_tex
Compression | Время кодирования файла | Вес файла | Итог
--- | --- | --- | ---
Lzma | 41 с | 26MB| Дольше, но сжимает лучше
Lzham | 73с | 27MB | Оно бесполезно
Zstandard | 35 с | 30 MB| Стандартное, быстрое и хорошо сжимает

# Остались вопросы?
Мой ник в соцсетях lilmuff1 пишите или присоединяйтесь к [Telegram](https://t.me/XcoderBS) or [Discord](https://discord.com/invite/yNajwpBe)
