Заметка:
Пусть блоков для файла доступно 15. (в inode есть массив адресов блоков и размер этого массива 15).
Тогда размер файла - 15 * 4096 = 61440 байт.
Быть может, это временное решение.
Итак, адрес - 16 бит. 
Максимальный размер файловой системы - 2^16 * 4096 байт = 256мб.
Всего файлов не более 2^16 = 65536.
В самом начале файла (файловой системы) находится информация о свободных блоках и свободных inode.
После этого будет список inode, записанных подряд.
Всего inode максимум столько же, сколько и файлов - 2^16 = 65536.
То есть inode будет всего размера 64 байт.
8 байт - временная метка.
2 * 20 = 40ды
 байт - адреса блоков.
1 байт - тип файла.
Они будут занимать статический размер - 2^16 * 32 байт = 2^21 байт = 2^11 кбайт = 2мб.
В нвчале надо будет указать размер от 0 до 256 мб в килобайтах, который обрежется по размеру блока
(округлится вниз).
UPD: свободные блоки будем поддерживать след. образом: в самом начале файла будем хранить 2 числа: первый блок,
после которого все свободны, и указатель на первый блок, который будет "Освободившимся". 
"Освободившиеся" будут составлять стек.