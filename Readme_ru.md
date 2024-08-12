# <img src="https://github.com/lilmuff2/X-coder/blob/master/android/res/drawable-xxxhdpi/icon.png?raw=true" width="35" height="35"> X-coder
#X-coder
Инструмент для декодирования (sc в png) и кодирования (png в sc) файлов sc которые используются в играх Supercell
# Как пользоваться
1. Скачиваем нужную нам версию с [релизов](https://github.com/lilmuff2/X-coder/releases/tag/v2.2) (apk - на андроид, setup.exe - установщик на винду, zip - архив с exe для винды)
3. Выбрать нужную опцию (о каждой расписано ниже)
5. Выбрать или ввести входной файл или папку
7. По желанию выбрать или ввести выходную папку или файл, или она будет выбрана автоматически <b>!ОНА БУДЕТ УДАЛЕНА!</b>
8. Нажать на кнопку снизу (не закрыть) и ждать


![Menu](https://github.com/lilmuff2/X-coder/blob/images/ru_menu.png?raw=true)

# ДЕКОД ФАЙЛ:
Конвертирует sc в пнг<br>
Входной файл - <b>_tex.sc, не .sc!</b> или _dl.sc в редких случаях<br>
Начиная с 54 версии бравл старса появились zktx файлы их нужно поместить в ту же папку, что и входной файл<br>
Выходная папка - папка в которую будут сохранены пнг<br>

![Decode example](https://github.com/lilmuff2/X-coder/blob/images/ru_decode.png?raw=true)


# ЕНКОД ПАПКУ:
Конвертирует пнг в sc<br>
Входная папка - папка с пнг и [json файлом](#json-%D1%84%D0%B0%D0%B9%D0%BB) (желательно)<br>
Имя json файла должно совпадать с названием папки<br>
Выходной файл - куда сохранять sc<br>
Если вам нужен sc для старой версии измените сжатие в json файле с Zstandard на Lzma<br>

![Encode example](https://github.com/lilmuff2/X-coder/blob/images/ru_encode.png?raw=true)


# ДЕКОД ФАЙЛЫ:
Конвертирует много sc файлов в пнг, удобно просто скопировать папку sc из апк и декодировать их все разом<br>
Входная папка - папка с <b>_tex.sc, не .sc!</b> или _dl.sc файлами<br>
Выходная папка - папка где будут созданы папки в которых будут пнг<br>

![Decode folder example](https://github.com/lilmuff2/X-coder/blob/images/ru_decodefolder.png?raw=true)


# ЕНКОД ПАПКИ:
Конвертирует много папок с пнг в sc файлы<br>
Входная папка - папка с папками в которых лежат пнг<br>
Выходная папка - папка куда будут сохранены sc файлы<br>

![Encode folder example](https://github.com/lilmuff2/X-coder/blob/images/ru_encodefolder.png?raw=true)


# .json файл:
Этот файл генерируется при декодировании
Пример:
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
Может быть khronos (мало весит) и raw (быстро кодируется), при включённом HasZKTX текстуры кодируются в khronos<br>
Пример ui_highres_tex с Zstandard сжатием:
Encoding | Время кодирования | Размер файла
---|---|---
khronos | 35 с | 30 MB
raw | 7 с | 41 MB
> Я рекомендую во время разработки мода использовать raw а при релизе khronos
## HasZKTX
Параметр отвечающий за наличие zktk файлов
### ISDL
Параметр отвечающий за создание dl файла (объединение sc и tex_sc), обычно используется в Clash Royale требует наличия sc файла в папке
## Compression
Сжатие файла, при включённом HasZKTX всегда используется Zstandard.
Пример сжатия на ui_highres_tex:
Compression | Время кодирования файла | Вес файла | Итог
--- | --- | --- | ---
Lzma | 41 с | 26MB| Дольше, но сжимает лучше
Lzham | 73с | 27MB | Оно бесполезно
Zstandard | 35 с | 30 MB| Стандартное, быстрое и хорошо сжимает

# Остались вопросы?
Мой ник в соцсетях lilmuff1 пишите или присоединяйтесь к [Telegram](https://t.me/XcoderBS) or [Discord](https://discord.com/invite/yNajwpBe)
